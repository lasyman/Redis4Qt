#include "redis4qtprivate.h"
#include <QTimer>
#include "redis_core.h"
#include "redis_errors.h"
#include "redis_types.h"
#include <QThread>

Redis4QtPrivate::Redis4QtPrivate(Redis4Qt *parent)
    : q_ptr(parent)
{

}

void Redis4QtPrivate::setRedisCof(QString strCfg)
{
    m_redisHost = Redis4QtCommon::getValue(DEFAULT_SECTION, KEY_HOST, strCfg, "127.0.0.1");
    m_redisPort = Redis4QtCommon::getValue(DEFAULT_SECTION, KEY_PORT, strCfg, 6379).toInt();
    m_redisCmdTimeout = Redis4QtCommon::getValue(DEFAULT_SECTION, KEY_TIMEOUT, strCfg, DEAFULT_REDIS_CMD_TIMEOUT).toInt();
}

int Redis4QtPrivate::del(QString key, bool *error)
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

QString Redis4QtPrivate::dump(QString key, bool *error)
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

bool Redis4QtPrivate::exists(QString key, bool *error)
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

bool Redis4QtPrivate::expire(QString key, int seconds, bool *error)
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

bool Redis4QtPrivate::expireat(QString key, qint64 timestamp, bool *error)
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

QStringList Redis4QtPrivate::keys(QString pattern, bool *error)
{
    //<1> 执行redis指令
    QVariantList data;
    data.append(pattern);

    QVariant ret = dealwithCmd(KEYS, data, error);
    return ret.toStringList();
}

int Redis4QtPrivate::pttl(QString key, bool *error)
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

int Redis4QtPrivate::ttl(QString key, bool *error)
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

bool Redis4QtPrivate::rename(QString key, QString newkey, bool *error)
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

QString Redis4QtPrivate::type(QString key, bool *error)
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

bool Redis4QtPrivate::set(QString key, QString value, bool *error)
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

QString Redis4QtPrivate::get(QString key, bool *error)
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

QString Redis4QtPrivate::getrange(QString key, int start, int stop, bool *error)
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

QStringList Redis4QtPrivate::mget(QStringList keys, bool *error)
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

bool Redis4QtPrivate::mset(QMap<QString, QVariant> map, bool *error)
{
    //<1> 执行redis指令
    QVariantList data;
    data.append(map);

    QVariant ret = dealwithCmd(STRING_MSET, data, error);
    return ret.toBool();
}

int Redis4QtPrivate::incr(QString key, bool *error)
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

int Redis4QtPrivate::incrby(QString key, int interval, bool *error)
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

int Redis4QtPrivate::decr(QString key, bool *error)
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

int Redis4QtPrivate::decrby(QString key, int interval, bool *error)
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

int Redis4QtPrivate::append(QString key, QString value, bool *error)
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

QStringList Redis4QtPrivate::blpop(QString key, int timeout, bool *error)
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

QStringList Redis4QtPrivate::brpop(QString key, int timeout, bool *error)
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

QStringList Redis4QtPrivate::brpoplpush(QString source, QString destination, int timeout, bool *error)
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

QString Redis4QtPrivate::lindex(QString key, int index, bool *error)
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

int Redis4QtPrivate::linsert(QString key, QString position, QString pivot, QString value, bool *error)
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

int Redis4QtPrivate::llen(QString key, bool *error)
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

QString Redis4QtPrivate::lpop(QString key, bool *error)
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

int Redis4QtPrivate::lpush(QString key, QString value, bool *error)
{
    return lpush(key, QStringList()<<value, error);
}

int Redis4QtPrivate::lpush(QString key, QStringList values, bool *error)
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

int Redis4QtPrivate::lpushx(QString key, QStringList values, bool *error)
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

QStringList Redis4QtPrivate::lrange(QString key, int start, int end, bool *error)
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

int Redis4QtPrivate::lrem(QString key, int count, QString value, bool *error)
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

int Redis4QtPrivate::lset(QString key, int index, QString value, bool *error)
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

bool Redis4QtPrivate::ltrim(QString key, int start, int stop, bool *error)
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

QString Redis4QtPrivate::rpop(QString key, bool *error)
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

QString Redis4QtPrivate::rpoplpush(QString source, QString destination, bool *error)
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

int Redis4QtPrivate::rpush(QString key, QString value, bool *error)
{
    return rpush(key, QStringList()<<value, error);
}

int Redis4QtPrivate::rpush(QString key, QStringList values, bool *error)
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

int Redis4QtPrivate::rpushx(QString key, QStringList values, bool *error)
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

int Redis4QtPrivate::sadd(QString key, QString member, bool *error)
{
    return sadd(key, QStringList()<<member, error);
}

int Redis4QtPrivate::sadd(QString key, QStringList members, bool *error)
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

int Redis4QtPrivate::scard(QString key, bool *error)
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

QStringList Redis4QtPrivate::sdiff(QStringList keys, bool *error)
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

int Redis4QtPrivate::sdiffstore(QString destination, QStringList keys, bool *error)
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

QStringList Redis4QtPrivate::sinter(QStringList keys, bool *error)
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

int Redis4QtPrivate::sinterstore(QString destination, QStringList keys, bool *error)
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

bool Redis4QtPrivate::sismember(QString key, QString member, bool *error)
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

QStringList Redis4QtPrivate::smembers(QString key, bool *error)
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

bool Redis4QtPrivate::smove(QString source, QString destination, QString member, bool *error)
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

QString Redis4QtPrivate::spop(QString key, bool *error)
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

QStringList Redis4QtPrivate::srandmember(QString key, bool *error)
{
    return srandmember(key, 1, error);
}

QStringList Redis4QtPrivate::srandmember(QString key, int count, bool *error)
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

int Redis4QtPrivate::srem(QString key, QString member, bool *error)
{
    return srem(key, QStringList()<<member, error);
}

int Redis4QtPrivate::srem(QString key, QStringList members, bool *error)
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

QStringList Redis4QtPrivate::sunion(QStringList keys, bool *error)
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

int Redis4QtPrivate::sunionstore(QString destination, QStringList keys, bool *error)
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

int Redis4QtPrivate::hdel(QString key, QString field, bool *error)
{
    return hdel(key, QStringList()<<field, error);
}

int Redis4QtPrivate::hdel(QString key, QStringList fields, bool *error)
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

bool Redis4QtPrivate::hexists(QString key, QString field, bool *error)
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

QString Redis4QtPrivate::hget(QString key, QString field, bool *error)
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

QVariantMap Redis4QtPrivate::hgetall(QString key, bool *error)
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

int Redis4QtPrivate::hincrby(QString key, QString field, int increment, bool *error)
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

float Redis4QtPrivate::hincrbyfloat(QString key, QString field, float increment, bool *error)
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

QStringList Redis4QtPrivate::hkeys(QString key, bool *error)
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

int Redis4QtPrivate::hlen(QString key, bool *error)
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

QStringList Redis4QtPrivate::hmget(QString key, QStringList fields, bool *error)
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

bool Redis4QtPrivate::hmset(QString key, QMap<QString, QVariant> map, bool *error)
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

bool Redis4QtPrivate::hset(QString key, QString field, QString value, bool *error)
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

bool Redis4QtPrivate::hsetnx(QString key, QString field, QString value, bool *error)
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

QStringList Redis4QtPrivate::hvals(QString key, bool *error)
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

int Redis4QtPrivate::publish(QString channel, QString message, bool *error)
{
    // 执行redis指令
    QVariantList data;
    data.append(channel);
    data.append(message);
    QVariant ret = dealwithCmd(PUBLISH, data, error);
    return ret.toInt();
}

int Redis4QtPrivate::subscribe(QString channel, bool *error)
{
    // 执行redis指令
    QVariantList data;
    data.append(channel);
    QVariant ret = dealwithCmd(SUBSCRIBE, data, error);
    return ret.toInt();
}

int Redis4QtPrivate::unsubscribe(QString channel, bool *error)
{
    // 执行redis指令
    QVariantList data;
    data.append(channel);
    QVariant ret = dealwithCmd(UNSUBSCRIBE, data, error);
    return ret.toInt();
}

int Redis4QtPrivate::psubscribe(QString channel, bool *error)
{
    // 执行redis指令
    QVariantList data;
    data.append(channel);
    QVariant ret = dealwithCmd(PSUBSCRIBE, data, error);
    return ret.toInt();
}

int Redis4QtPrivate::punsubscribe(QString channel, bool *error)
{
    // 执行redis指令
    QVariantList data;
    data.append(channel);
    QVariant ret = dealwithCmd(PUNSUBSCRIBE, data, error);
    return ret.toInt();
}

int Redis4QtPrivate::getLastError()
{
    QMutexLocker locker(&m_Lock);
    if(m_redis.contains((void*)QThread::currentThread()))
    {
        return m_redis[QThread::currentThread()].core->getLastError();
    }
    return -1;
}

RedisCore *Redis4QtPrivate::getRedisInstance()
{
    //<1> 每个线程分配一个redis实例，对应一个tcp连接
    RedisCore * core = NULL;
    m_Lock.lock();

    //<2> 判断当前线程对应的redis实例是否存在
    if(m_redis.contains((void*)QThread::currentThread()))
    {
        core =  m_redis[QThread::currentThread()].core;
    }

    //<3> redis实例不存在，创建并使用(暂不限制实例个数)
    else
    {
        core = new RedisCore(m_redisHost, m_redisPort);
        connect(core, SIGNAL(returnData(Reply)), this, SLOT(onReturnData(Reply)));
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
            qDebug() << (QString("Connect to redis server [%1:%2] failed!").arg(m_redisHost).arg(m_redisPort));
            core = NULL;
        }
    }
    return core;
}

bool Redis4QtPrivate::validKey(QString &key, QString funcName, bool *error)
{
    //<1> 校验key, 非二进制安全， 不能包含空格和换行
    bool valid = true;
    if(key.contains(" ") || key.contains("\n"))
        valid = false;

    //<2> key无效处理
    if(!valid)
    {
        qDebug() << (QString("Func [%1], invalid key [%2]!").arg(funcName).arg(key));
        if(error)
            *error = true;
        setLastError(REDIS_INVALID_KEY);
    }

    return valid;
}

bool Redis4QtPrivate::validKeys(QStringList &key, QString funcName, bool *error)
{
    foreach (QString k, key)
    {
        if (!validKey(k, funcName, error))
            return false;
    }
    return true;
}

void Redis4QtPrivate::setLastError(int error)
{
    RedisCore * core = NULL;
    QMutexLocker locker(&m_Lock);

    //<2> 判断当前线程对应的redis实例是否存在
    if(m_redis.contains((void*)QThread::currentThread()))
    {
        core =  m_redis[QThread::currentThread()].core;
    }

    //<3> redis实例不存在，创建并使用(暂不限制实例个数)
    else
    {
        core = new RedisCore(m_redisHost, m_redisPort);
        core->setTimeout(m_redisCmdTimeout);
        ThreadRedis redis;
        redis.core = core;
        redis.lock = new QMutex;
        redis.thread = QThread::currentThread();
        m_redis.insert(QThread::currentThread(), redis);
    }

    core->setLastError(error);
}

QVariant Redis4QtPrivate::dealwithCmd(FuncType type, QVariantList &data, bool *error)
{
    //<1> 初始化
    QVariant ret;
    if(error)
        *error = false;

    //<2> 获取redis实例
    RedisCore * core = getRedisInstance();
    if(core)
    {
        ret = excuteCmd(type, core, data);
        if(error && core->getLastError() != REDIS_SUCCESS)
            *error = true;
    }
    else if(error)
    {
        *error = true;
    }
    return ret;
}

QVariant Redis4QtPrivate::excuteCmd(FuncType type, RedisCore *redis, QVariantList &data)
{
    QVariant ret;
    if(!m_redis[QThread::currentThread()].lock->tryLock())
    {
        m_redis[QThread::currentThread()].core->setLastError(REDIS_RUNNING);
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
    // <6> 订阅发布
    case PUBLISH:
    {
        redis->publish(data[0].toString(), data[1].toString());
        ret = REDIS_SUCCESS;
        break;
    }
    case SUBSCRIBE:
    {
        redis->subscribe(data[0].toString());
        ret = REDIS_SUCCESS;
        break;
    }
    case UNSUBSCRIBE:
    {
        redis->unsubscribe(data[0].toString());
        ret = REDIS_SUCCESS;
        break;
    }
    case PSUBSCRIBE:
    {
        redis->psubscribe(data[0].toString());
        ret = REDIS_SUCCESS;
        break;
    }
    case PUNSUBSCRIBE:
    {
        redis->punsubscribe(data[0].toString());
        ret = REDIS_SUCCESS;
        break;
    }
    default:
    {
        qDebug() << (QString("Not support func [%1]!").arg(type));
        ret = QVariant();
        break;
    }
    }
    m_redis[QThread::currentThread()].lock->unlock();
    return ret;
}

void Redis4QtPrivate::onReturnData(Reply reply)
{
    qDebug() << "channel: " << reply.channel << " pattern: " << reply.pattern;
    qDebug() << reply.value.toString();
    emit sigReturnMsg(reply.channel, reply.value.toString());
}

