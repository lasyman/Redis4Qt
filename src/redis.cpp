#include "lht_redis.h"
#include <QtCore>
#include <QMutexLocker>
#include <ctkServiceReference.h>
#include <ctkPluginContext.h>
#include <lht_settings_i.h>
#include <lht_log4qt_i.h>
#include <lht_protocol.h>
#include <lht_errors.h>


LHRedis::LHRedis()
    : m_bInit(false),
      m_nInstance(-1)
{
}

LHRedis::~LHRedis()
{
}

int LHRedis::Init(const LHParameters &Parameters)
{
    if (m_bInit)
        return LH_SUCCESS;

    m_PluginContext = static_cast<ctkPluginContext *>(Parameters[LH_KEY_PLUGIN_CONTEXT].value<void *>());
    if (!m_PluginContext)
        return LH_FAILURE;

    //! 配置模块
    ctkServiceReference refSetting= m_PluginContext->getServiceReference("LHSettingsInterface");
    if(!refSetting.getPlugin().isNull()) //! 必须
    {
        m_Settings = (LHSettingsInterface *)(qobject_cast<LHSettingsInterface *>(m_PluginContext->getService(refSetting)));
        if (!m_Settings)
            return LH_FAILURE;

        //!
        m_Settings->Init(Parameters);
        //!
        QVariant varInstance;
        m_Settings->CreateInstance(varInstance, Parameters);
        m_nSettingsInstance = varInstance.toInt();
    }
    else
        return LH_FAILURE;

    //! 日志模块
    ctkServiceReference refLog4Qt = m_PluginContext->getServiceReference("LHLoggerInterface");
    if(!refLog4Qt.getPlugin().isNull()) //! 可选
    {
        m_Log4qt = (qobject_cast<LHLoggerInterface *>(m_PluginContext->getService(refLog4Qt)));
        if (m_Log4qt)
        {
            m_Log4qt->Init(Parameters);
        }
    }

    m_bInit = true;

    return LH_SUCCESS;
}

int LHRedis::Uninit()
{
    DestroyInstance(m_nInstance);
    m_bInit = false;
    return LH_SUCCESS;
}

int LHRedis::CreateInstance(QVariant &varInstance, const LHParameters &Parameters)
{
    if (!m_bInit)
        return LH_NOT_INIT;

    //<1> 读取redis相关配置
    if (m_nInstance == -1)
    {
        QVariant varOption;

        m_Settings->GetOption(varOption, m_nSettingsInstance, LH_CONF_REDISHOST);
        m_redisHost = varOption.toString();

        m_Settings->GetOption(varOption, m_nSettingsInstance, LH_CONF_REDISPORT);
        m_redisPort = varOption.toInt();

        m_Settings->GetOption(varOption, m_nSettingsInstance, LH_CONF_REDISTIMEOUT);
        m_redisCmdTimeout = varOption.toInt();
        if(m_redisCmdTimeout <= 0)
            m_redisCmdTimeout = DEAFULT_REDIS_CMD_TIMEOUT;

        m_nInstance = 1;
    }

    varInstance.setValue(m_nInstance);

    return LH_SUCCESS;
}

int LHRedis::DestroyInstance(const QVariant &varInstance)
{
    if (!m_bInit)
        return LH_NOT_INIT;

    if ((varInstance.toInt() != m_nInstance && m_nInstance != -1) || varInstance.isNull())
        return LH_NOT_EXISTS;

    //释放redis连接
    QList<ThreadRedis> conns = m_redis.values();
    foreach (ThreadRedis t, conns)
    {
        t.core->closeConnection();
        t.core->deleteLater();
        delete t.lock;
    }
    m_redis.clear();

    m_nInstance = -1;

    return LH_SUCCESS;
}

int LHRedis::key_del(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_del", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(KEY_DEL, data, error);
    return ret.toInt();
}

QString LHRedis::key_dump(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_dump", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(KEY_DUMP, data, error);
    return ret.toString();
}

bool LHRedis::key_exists(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_exists", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(KEY_EXISTS, data, error);
    return ret.toBool();
}

bool LHRedis::key_expire(QString key, int seconds, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_expire", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(seconds);

    QVariant ret = dealwithCmd(KEY_EXPIRE, data, error);
    return ret.toBool();
}

bool LHRedis::key_expireat(QString key, qint64 timestamp, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_expireat", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(timestamp);

    QVariant ret = dealwithCmd(KEY_EXPIRE_AT, data, error);
    return ret.toBool();
}

QStringList LHRedis::keys(QString pattern, bool *error)
{
    //<1> 执行redis指令
    QVariantList data;
    data.append(pattern);

    QVariant ret = dealwithCmd(KEYS, data, error);
    return ret.toStringList();
}

int LHRedis::key_pttl(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_pttl", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(KEY_PTTL, data, error);
    return ret.toInt();
}

int LHRedis::key_ttl(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_ttl", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(KEY_TTL, data, error);
    return ret.toInt();
}

bool LHRedis::key_rename(QString key, QString newkey, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_pttl", error) || !validKey(newkey, "key_pttl", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(newkey);

    QVariant ret = dealwithCmd(KEY_RENAME, data, error);
    return ret.toBool();
}

QString LHRedis::key_type(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "key_type", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(KEY_TYPE, data, error);
    return ret.toString();
}

bool LHRedis::string_set(QString key, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_set", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(value);

    QVariant ret = dealwithCmd(STRING_SET, data, error);
    return ret.toBool();
}

QString LHRedis::string_get(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_get", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(STRING_GET, data, error);
    return ret.toString();
}

QString LHRedis::string_getrange(QString key, int start, int stop, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_getrange", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(start);
    data.append(stop);

    QVariant ret = dealwithCmd(STRING_GET_RANGE, data, error);
    return ret.toString();
}

QStringList LHRedis::string_mget(QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKeys(keys, "string_mget", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(keys);

    QVariant ret = dealwithCmd(STRING_MGET, data, error);
    return ret.toStringList();
}

bool LHRedis::string_mset(QMap<QString, QVariant> map, bool *error)
{
    //<1> 执行redis指令
    QVariantList data;
    data.append(map);

    QVariant ret = dealwithCmd(STRING_MSET, data, error);
    return ret.toBool();
}

int LHRedis::string_incr(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_incr", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(STRING_INCR, data, error);
    return ret.toInt();
}

int LHRedis::string_incrby(QString key, int interval, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_incrby", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(interval);

    QVariant ret = dealwithCmd(STRING_INCRBY, data, error);
    return ret.toInt();
}

int LHRedis::string_decr(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_decr", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(STRING_DECR, data, error);
    return ret.toInt();
}

int LHRedis::string_decrby(QString key, int interval, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_decrby", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(interval);

    QVariant ret = dealwithCmd(STRING_DECRBY, data, error);
    return ret.toInt();
}

int LHRedis::string_append(QString key, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "string_append", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(value);

    QVariant ret = dealwithCmd(STRING_APPEND, data, error);
    return ret.toInt();
}

QStringList LHRedis::list_blpop(QString key, int timeout, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_blpop", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(timeout);

    QVariant ret = dealwithCmd(LIST_BLPOP, data, error);
    return ret.toStringList();
}

QStringList LHRedis::list_brpop(QString key, int timeout, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_brpop", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(timeout);

    QVariant ret = dealwithCmd(LIST_BRPOP, data, error);
    return ret.toStringList();
}

QStringList LHRedis::list_brpoplpush(QString source, QString destination, int timeout, bool *error)
{
    //<1> 校验key
    if(!validKey(source, "list_brpoplpush", error) || !validKey(destination, "list_brpoplpush", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(source);
    data.append(destination);
    data.append(timeout);

    QVariant ret = dealwithCmd(LIST_BRPOPLPUSH, data, error);
    return ret.toStringList();
}

QString LHRedis::list_index(QString key, int index, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_index", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(index);

    QVariant ret = dealwithCmd(LIST_INDEX, data, error);
    return ret.toString();
}

int LHRedis::list_insert(QString key, QString position, QString pivot, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_insert", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(position);
    data.append(pivot);
    data.append(value);

    QVariant ret = dealwithCmd(LIST_INSERT, data, error);
    return ret.toInt();
}

int LHRedis::list_len(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_len", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(LIST_LEN, data, error);
    return ret.toInt();
}

QString LHRedis::list_lpop(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_lpop", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(LIST_LPOP, data, error);
    return ret.toString();
}

int LHRedis::list_lpush(QString key, QString value, bool *error)
{
    return list_lpush(key, QStringList()<<value, error);
}

int LHRedis::list_lpush(QString key, QStringList values, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_lpush", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(values);

    QVariant ret = dealwithCmd(LIST_LPUSH, data, error);
    return ret.toInt();
}

int LHRedis::list_lpushx(QString key, QStringList values, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_lpushx", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(values);

    QVariant ret = dealwithCmd(LIST_LPUSHX, data, error);
    return ret.toInt();
}

QStringList LHRedis::list_range(QString key, int start, int end, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_range", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(start);
    data.append(end);

    QVariant ret = dealwithCmd(LIST_RANGE, data, error);
    return ret.toStringList();
}

int LHRedis::list_rem(QString key, int count, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_rem", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(count);
    data.append(value);

    QVariant ret = dealwithCmd(LIST_REMOVE, data, error);
    return ret.toInt();
}

int LHRedis::list_set(QString key, int index, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_set", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(index);
    data.append(value);

    QVariant ret = dealwithCmd(LIST_SET, data, error);
    return ret.toInt();
}

bool LHRedis::list_trim(QString key, int start, int stop, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_trim", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(start);
    data.append(stop);

    QVariant ret = dealwithCmd(LIST_TRIM, data, error);
    return ret.toBool();
}

QString LHRedis::list_rpop(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_rpop", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(LIST_RPOP, data, error);
    return ret.toString();
}

QString LHRedis::list_rpoplpush(QString source, QString destination, bool *error)
{
    //<1> 校验key
    if(!validKey(source, "list_rpoplpush", error) || !validKey(destination, "list_rpoplpush", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(source);
    data.append(destination);

    QVariant ret = dealwithCmd(LIST_RPOPLPUSH, data, error);
    return ret.toString();
}

int LHRedis::list_rpush(QString key, QString value, bool *error)
{
    return list_rpush(key, QStringList()<<value, error);
}

int LHRedis::list_rpush(QString key, QStringList values, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_rpush", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(values);

    QVariant ret = dealwithCmd(LIST_RPUSH, data, error);
    return ret.toInt();
}

int LHRedis::list_rpushx(QString key, QStringList values, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "list_rpushx", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(values);

    QVariant ret = dealwithCmd(LIST_RPUSHX, data, error);
    return ret.toInt();
}

int LHRedis::set_add(QString key, QString member, bool *error)
{
    return set_add(key, QStringList()<<member, error);
}

int LHRedis::set_add(QString key, QStringList members, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_add", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(members);

    QVariant ret = dealwithCmd(SET_ADD, data, error);
    return ret.toInt();
}

int LHRedis::set_card(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_card", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(SET_CARD, data, error);
    return ret.toInt();
}

QStringList LHRedis::set_diff(QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKeys(keys, "set_diff", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(keys);

    QVariant ret = dealwithCmd(SET_DIFF, data, error);
    return ret.toStringList();
}

int LHRedis::set_diffstore(QString destination, QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKey(destination, "set_diffstore", error))
    {
        return INVALID_INTEGER;
    }
    if(!validKeys(keys, "set_diffstore", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(destination);
    data.append(keys);

    QVariant ret = dealwithCmd(SET_DIFFSTORE, data, error);
    return ret.toInt();
}

QStringList LHRedis::set_inter(QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKeys(keys, "set_inter", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(keys);

    QVariant ret = dealwithCmd(SET_INTER, data, error);
    return ret.toStringList();
}

int LHRedis::set_interstore(QString destination, QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKey(destination, "set_interstore", error))
    {
        return INVALID_INTEGER;
    }
    if(!validKeys(keys, "set_interstore", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(destination);
    data.append(keys);

    QVariant ret = dealwithCmd(SET_INTERSTORE, data, error);
    return ret.toInt();
}

bool LHRedis::set_ismember(QString key, QString member, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_ismember", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(member);

    QVariant ret = dealwithCmd(SET_ISMEMBER, data, error);
    return ret.toBool();
}

QStringList LHRedis::set_members(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_members", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(SET_MEMBERS, data, error);
    return ret.toStringList();
}

bool LHRedis::set_move(QString source, QString destination, QString member, bool *error)
{
    //<1> 校验key
    if(!validKey(source, "set_move", error) || !validKey(destination, "set_move", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(source);
    data.append(destination);
    data.append(member);

    QVariant ret = dealwithCmd(SET_MOVE, data, error);
    return ret.toBool();
}

QString LHRedis::set_pop(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_pop", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(SET_POP, data, error);
    return ret.toString();
}

QStringList LHRedis::set_randmember(QString key, bool *error)
{
    return set_randmember(key, 1, error);
}

QStringList LHRedis::set_randmember(QString key, int count, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_randmember", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(count);

    QVariant ret = dealwithCmd(SET_RANDMEMBER, data, error);
    return ret.toStringList();
}

int LHRedis::set_rem(QString key, QString member, bool *error)
{
    return set_rem(key, QStringList()<<member, error);
}

int LHRedis::set_rem(QString key, QStringList members, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "set_rem", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(members);

    QVariant ret = dealwithCmd(SET_REMOVE, data, error);
    return ret.toInt();
}

QStringList LHRedis::set_union(QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKeys(keys, "set_union", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(keys);

    QVariant ret = dealwithCmd(SET_UNION, data, error);
    return ret.toStringList();
}

int LHRedis::set_unionstore(QString destination, QStringList keys, bool *error)
{
    //<1> 校验key
    if(!validKey(destination, "set_unionstore", error))
    {
        return INVALID_INTEGER;
    }
    if(!validKeys(keys, "set_unionstore", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(destination);
    data.append(keys);

    QVariant ret = dealwithCmd(SET_UNIONSTORE, data, error);
    return ret.toInt();
}

int LHRedis::hash_del(QString key, QString field, bool *error)
{
    return hash_del(key, QStringList()<<field, error);
}

int LHRedis::hash_del(QString key, QStringList fields, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_del", error))
    {
        return INVALID_INTEGER;
    }
    if(!validKeys(fields, "hash_del", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(fields);

    QVariant ret = dealwithCmd(HASH_DEL, data, error);
    return ret.toInt();
}

bool LHRedis::hash_exists(QString key, QString field, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_exists", error) || !validKey(field, "hash_exists", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(field);

    QVariant ret = dealwithCmd(HASH_EXISTS, data, error);
    return ret.toBool();
}

QString LHRedis::hash_get(QString key, QString field, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_get", error) || !validKey(field, "hash_get", error))
    {
        return INVALID_STRING;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(field);

    QVariant ret = dealwithCmd(HASH_GET, data, error);
    return ret.toString();
}

QVariantMap LHRedis::hash_getall(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_getall", error))
    {
        return INVALID_VARIANTMAP;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(HASH_GETALL, data, error);
    return ret.toMap();
}

int LHRedis::hash_incrby(QString key, QString field, int increment, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_incrby", error) || !validKey(field, "hash_incrby", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(field);
    data.append(increment);

    QVariant ret = dealwithCmd(HASH_INCRBY, data, error);
    return ret.toInt();
}

float LHRedis::hash_incrbyfloat(QString key, QString field, float increment, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_incrbyfloat", error) || !validKey(field, "hash_incrbyfloat", error))
    {
        return INVALID_FLOAT;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(field);
    data.append(increment);

    QVariant ret = dealwithCmd(HASH_INCRBYFLOAT, data, error);
    return ret.toFloat();
}

QStringList LHRedis::hash_keys(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_keys", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(HASH_KEYS, data, error);
    return ret.toStringList();
}

int LHRedis::hash_len(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_len", error))
    {
        return INVALID_INTEGER;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(HASH_LEN, data, error);
    return ret.toInt();
}

QStringList LHRedis::hash_mget(QString key, QStringList fields, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_mget", error))
    {
        return INVALID_STRINGLIST;
    }
    if(!validKeys(fields, "hash_mget", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(fields);

    QVariant ret = dealwithCmd(HASH_MGET, data, error);
    return ret.toStringList();
}

bool LHRedis::hash_mset(QString key, QMap<QString, QVariant> map, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_mset", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(map);

    QVariant ret = dealwithCmd(HASH_MSET, data, error);
    return ret.toInt();
}

bool LHRedis::hash_set(QString key, QString field, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_set", error) || !validKey(field, "hash_set", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(field);
    data.append(value);

    QVariant ret = dealwithCmd(HASH_SET, data, error);
    return ret.toBool();
}

bool LHRedis::hash_setnx(QString key, QString field, QString value, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_setnx", error) || !validKey(field, "hash_setnx", error))
    {
        return INVALID_BOOL;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);
    data.append(field);
    data.append(value);

    QVariant ret = dealwithCmd(HASH_SETNX, data, error);
    return ret.toBool();
}

QStringList LHRedis::hash_values(QString key, bool *error)
{
    //<1> 校验key
    if(!validKey(key, "hash_values", error))
    {
        return INVALID_STRINGLIST;
    }

    //<2> 执行redis指令
    QVariantList data;
    data.append(key);

    QVariant ret = dealwithCmd(HASH_VALUES, data, error);
    return ret.toStringList();
}

void LHRedis::publish(QString channel, QString message, bool *error)
{

}

void LHRedis::subscribe(QString channel, bool *error)
{

}

void LHRedis::unsubscribe(QString channel, bool *error)
{

}

void LHRedis::psubscribe(QString channel, bool *error)
{

}

void LHRedis::punsubscribe(QString channel, bool *error)
{

}

int LHRedis::getLastError()
{
    QMutexLocker locker(&m_Lock);
    if(m_redis.contains((void*)QThread::currentThread()))
    {
        return m_redis[QThread::currentThread()].core->getLastError();
    }
    return -1;
}

LHRedisCore *LHRedis::getRedisInstance()
{
    //<1> 每个线程分配一个redis实例，对应一个tcp连接
    LHRedisCore * core = NULL;
    m_Lock.lock();

    //<2> 判断当前线程对应的redis实例是否存在
    if(m_redis.contains((void*)QThread::currentThread()))
    {
        core =  m_redis[QThread::currentThread()].core;
    }

    //<3> redis实例不存在，创建并使用(暂不限制实例个数)
    else
    {
        core = new LHRedisCore(m_redisHost, m_redisPort);
        core->setTimeout(m_redisCmdTimeout);
        ThreadRedis redis;
        redis.core = core;
        redis.lock = new QMutex;
        redis.thread = QThread::currentThread();
        m_redis.insert(QThread::currentThread(), redis);
    }
    m_Lock.unlock();

    //<4> 判断是否已连接redis服务器
    if(!core->isConnected())
    {
        if(!core->openConnection())
        {
            m_Log4qt->Log(QString("Connect to redis server [%1:%2] failed!").arg(m_redisHost).arg(m_redisPort),
                          LHLoggerInterface::LH_ERROR);
            core = NULL;
        }
    }
    return core;
}

bool LHRedis::validKey(QString &key, QString funcName, bool *error)
{
    //<1> 校验key, 非二进制安全， 不能包含空格和换行
    bool valid = true;
    if(key.contains(" ") || key.contains("\n"))
        valid = false;

    //<2> key无效处理
    if(!valid)
    {
        m_Log4qt->Log(QString("Func [%1], invalid key [%2]!").arg(funcName).arg(key),
                      LHLoggerInterface::LH_WARN);
        if(error)
            *error = true;
        setLastError(LH_REDIS_INVALID_KEY);
    }

    return valid;
}

bool LHRedis::validKeys(QStringList &key, QString funcName, bool *error)
{
    //<1> 校验key, 非二进制安全， 不能包含空格和换行
    bool valid = true;
    foreach (QString k, key)
    {
        if(k.contains(" ") || k.contains("\n"))
        {
            valid = false;
            break;
        }
    }

    //<2> key无效处理
    if(!valid)
    {
        m_Log4qt->Log(QString("Func [%1], invalid key [%2]!").arg(funcName).arg(key.join(" ")),
                      LHLoggerInterface::LH_WARN);
        if(error)
            *error = true;
        setLastError(LH_REDIS_INVALID_KEY);
    }

    return valid;
}

void LHRedis::setLastError(int error)
{
    LHRedisCore * core = NULL;
    QMutexLocker locker(&m_Lock);

    //<2> 判断当前线程对应的redis实例是否存在
    if(m_redis.contains((void*)QThread::currentThread()))
    {
        core =  m_redis[QThread::currentThread()].core;
    }

    //<3> redis实例不存在，创建并使用(暂不限制实例个数)
    else
    {
        core = new LHRedisCore(m_redisHost, m_redisPort);
        core->setTimeout(m_redisCmdTimeout);
        ThreadRedis redis;
        redis.core = core;
        redis.lock = new QMutex;
        redis.thread = QThread::currentThread();
        m_redis.insert(QThread::currentThread(), redis);
    }

    core->setLastError(error);
}

QVariant LHRedis::dealwithCmd(FuncType type, QVariantList &data, bool *error)
{
    //<1> 初始化
    QVariant ret;
    if(error)
        *error = false;

    //<2> 获取redis实例
    LHRedisCore * core = getRedisInstance();
    if(core)
    {
        ret = excuteCmd(type, core, data);
        if(error && core->getLastError() != LH_REDIS_SUCCESS)
            *error = true;
    }
    else if(error)
    {
        *error = true;
    }
    return ret;
}

QVariant LHRedis::excuteCmd(LHRedis::FuncType type, LHRedisCore *redis, QVariantList &data)
{
    QVariant ret;
    if(!m_redis[QThread::currentThread()].lock->tryLock())
    {
        m_redis[QThread::currentThread()].core->setLastError(LH_REDIS_RUNNING);
        return ret;
    }

    switch(type)
    {
    //<1> key操作
    case KEY_DEL:
    {
        ret =  redis->del(data[0].toString());
        break;
    }
    case KEY_DUMP:
    {
        ret = redis->dump(data[0].toString());
        break;
    }
    case KEY_EXISTS:
    {
        ret =  redis->exists(data[0].toString());
        break;
    }
    case KEY_EXPIRE:
    {
        ret =  redis->expire(data[0].toString(), data[1].toInt());
        break;
    }
    case KEY_EXPIRE_AT:
    {
        ret =  redis->expireat(data[0].toString(), data[1].toLongLong());
        break;
    }
    case KEYS:
    {
        ret =  redis->keys(data[0].toString());
        break;
    }
    case KEY_PTTL:
    {
        ret =  redis->pttl(data[0].toString());
        break;
    }
    case KEY_TTL:
    {
        ret =  redis->ttl(data[0].toString());
        break;
    }
    case KEY_RENAME:
    {
        ret =  redis->rename(data[0].toString(), data[1].toString());
        break;
    }
    case KEY_TYPE:
    {
        ret = redis->type(data[0].toString());
        break;
    }

        //<2> string操作
    case STRING_SET:
    {
        ret = redis->set(data[0].toString(), data[1].toString());
        break;
    }
    case STRING_GET:
    {
        ret =  redis->get(data[0].toString());
        break;
    }
    case STRING_GET_RANGE:
    {
        ret =  redis->getrange(data[0].toString(), data[1].toInt(), data[2].toInt());
        break;
    }
    case STRING_MGET:
    {
        ret =  redis->mget(data[0].toStringList());
        break;
    }
    case STRING_MSET:
    {
        ret =  redis->mset(data[0].toMap());
        break;
    }
    case STRING_INCR:
    {
        ret =  redis->incr(data[0].toString());
        break;
    }
    case STRING_INCRBY:
    {
        ret =  redis->incrby(data[0].toString(), data[1].toInt());
        break;
    }
    case STRING_DECR:
    {
        ret =  redis->decr(data[0].toString());
        break;
    }
    case STRING_DECRBY:
    {
        ret = redis->decrby(data[0].toString(), data[1].toInt());
        break;
    }
    case STRING_APPEND:
    {
        ret =  redis->append(data[0].toString(), data[1].toString());
        break;
    }

        //<3> list操作
    case LIST_BLPOP:
    {
        ret =  redis->blpop(data[0].toString(), data[1].toInt());
        break;
    }
    case LIST_BRPOP:
    {
        ret =  redis->brpop(data[0].toString(), data[1].toInt());
        break;
    }
    case LIST_BRPOPLPUSH:
    {
        ret =  redis->brpoplpush(data[0].toString(), data[1].toString(), data[2].toInt());
        break;
    }
    case LIST_INDEX:
    {
        ret =  redis->lindex(data[0].toString(), data[1].toInt());
        break;
    }
    case LIST_INSERT:
    {
        ret =  redis->linsert(data[0].toString(), data[1].toString(), data[2].toString(), data[3].toString());
        break;
    }
    case LIST_LEN:
    {
        ret =  redis->llen(data[0].toString());
        break;
    }
    case LIST_LPOP:
    {
        ret = redis->lpop(data[0].toString());
        break;
    }
    case LIST_LPUSH:
    {
        ret =  redis->lpush(data[0].toString(), data[1].toStringList());
        break;
    }
    case LIST_LPUSHX:
    {
        ret = redis->lpushx(data[0].toString(), data[1].toStringList());
        break;
    }
    case LIST_RANGE:
    {
        ret = redis->lrange(data[0].toString(), data[1].toInt(), data[2].toInt());
        break;
    }
    case LIST_REMOVE:
    {
        ret = redis->lrem(data[0].toString(), data[1].toInt(), data[2].toString());
        break;
    }
    case LIST_SET:
    {
        ret = redis->lset(data[0].toString(), data[1].toInt(), data[2].toString());
        break;
    }
    case LIST_TRIM:
    {
        ret = redis->ltrim(data[0].toString(), data[1].toInt(), data[2].toInt());
        break;
    }
    case LIST_RPOP:
    {
        ret = redis->rpop(data[0].toString());
        break;
    }
    case LIST_RPOPLPUSH:
    {
        ret = redis->rpoplpush(data[0].toString(), data[1].toString());
        break;
    }
    case LIST_RPUSH:
    {
        ret = redis->rpush(data[0].toString(), data[1].toStringList());
        break;
    }
    case LIST_RPUSHX:
    {
        ret = redis->rpushx(data[0].toString(), data[1].toStringList());
        break;
    }

        //<4> set操作
    case SET_ADD:
    {
        ret = redis->sadd(data[0].toString(), data[1].toStringList());
        break;
    }
    case SET_CARD:
    {
        ret = redis->scard(data[0].toString());
        break;
    }
    case SET_DIFF:
    {
        ret = redis->sdiff(data[0].toStringList());
        break;
    }
    case SET_DIFFSTORE:
    {
        ret = redis->sdiffstore(data[0].toString(), data[1].toStringList());
        break;
    }
    case SET_INTER:
    {
        ret = redis->sinter(data[0].toStringList());
        break;
    }
    case SET_INTERSTORE:
    {
        ret = redis->sinterstore(data[0].toString(), data[1].toStringList());
        break;
    }
    case SET_ISMEMBER:
    {
        ret = redis->sismember(data[0].toString(), data[1].toString());
        break;
    }
    case SET_MEMBERS:
    {
        ret = redis->smembers(data[0].toString());
        break;
    }
    case SET_MOVE:
    {
        ret = redis->smove(data[0].toString(), data[1].toString(), data[2].toString());
        break;
    }
    case SET_POP:
    {
        ret = redis->spop(data[0].toString());
        break;
    }
    case SET_RANDMEMBER:
    {
        ret = redis->srandmember(data[0].toString(), data[1].toInt());
        break;
    }
    case SET_REMOVE:
    {
        ret = redis->srem(data[0].toString(), data[1].toStringList());
        break;
    }
    case SET_UNION:
    {
        ret = redis->sunion(data[0].toStringList());
        break;
    }
    case SET_UNIONSTORE:
    {
        ret = redis->sunionstore(data[0].toString(), data[1].toStringList());
        break;
    }

        //<5> hash操作
    case HASH_DEL:
    {
        ret = redis->hdel(data[0].toString(), data[1].toStringList());
        break;
    }
    case HASH_EXISTS:
    {
        ret = redis->hexists(data[0].toString(), data[1].toString());
        break;
    }
    case HASH_GET:
    {
        ret = redis->hget(data[0].toString(), data[1].toString());
        break;
    }
    case HASH_GETALL:
    {
        ret = redis->hgetall(data[0].toString());
        break;
    }
    case HASH_INCRBY:
    {
        ret = redis->hincrby(data[0].toString(), data[1].toString(), data[2].toInt());
        break;
    }
    case HASH_INCRBYFLOAT:
    {
        ret = redis->hincrbyfloat(data[0].toString(), data[1].toString(), data[2].toFloat());
        break;
    }
    case HASH_KEYS:
    {
        ret = redis->hkeys(data[0].toString());
        break;
    }
    case HASH_LEN:
    {
        ret = redis->hlen(data[0].toString());
        break;
    }
    case HASH_MGET:
    {
        ret = redis->hmget(data[0].toString(), data[1].toStringList());
        break;
    }
    case HASH_MSET:
    {
        ret = redis->hmset(data[0].toString(), data[1].toMap());
        break;
    }
    case HASH_SET:
    {
        ret = redis->hset(data[0].toString(), data[1].toString(), data[2].toString());
        break;
    }
    case HASH_SETNX:
    {
        ret = redis->hsetnx(data[0].toString(), data[1].toString(), data[2].toString());
        break;
    }
    case HASH_VALUES:
    {
        ret = redis->hvals(data[0].toString());
        break;
    }
    default:
    {
        m_Log4qt->Log(QString("Not support func [%1]!").arg(type),
                      LHLoggerInterface::LH_WARN);
        ret = QVariant();
        break;
    }
    }
    m_redis[QThread::currentThread()].lock->unlock();
    return ret;
}
