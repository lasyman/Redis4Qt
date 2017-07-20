#ifndef REDIS_TYPES_H
#define REDIS_TYPES_H

#include <QObject>
#include <QSettings>
#include "redis4qt_global.h"

#define DEFAULT_SECTION "redis"
#define KEY_HOST    "host"
#define KEY_PORT    "port"
#define KEY_TIMEOUT "timeout"

namespace Redis4Qt_Types
{
    enum FuncType
    {
        //<1> key操作
        KEY_DEL         = 100,
        KEY_DUMP,
        KEY_EXISTS,
        KEY_EXPIRE,
        KEY_EXPIRE_AT,
        KEYS,
        KEY_PTTL,
        KEY_TTL,
        KEY_RENAME,
        KEY_TYPE,

        //<2> string操作
        STRING_SET      = 200,
        STRING_GET,
        STRING_GET_RANGE,
        STRING_MGET,
        STRING_MSET,
        STRING_INCR,
        STRING_INCRBY,
        STRING_DECR,
        STRING_DECRBY,
        STRING_APPEND,

        //<3> list操作
        LIST_BLPOP      = 300,
        LIST_BRPOP,
        LIST_BRPOPLPUSH,
        LIST_INDEX,
        LIST_INSERT,
        LIST_LEN,
        LIST_LPOP,
        LIST_LPUSH,
        LIST_LPUSHX,
        LIST_RANGE,
        LIST_REMOVE,
        LIST_SET,
        LIST_TRIM,
        LIST_RPOP,
        LIST_RPOPLPUSH,
        LIST_RPUSH,
        LIST_RPUSHX,

        //<4> set操作
        SET_ADD         = 400,
        SET_CARD,
        SET_DIFF,
        SET_DIFFSTORE,
        SET_INTER,
        SET_INTERSTORE,
        SET_ISMEMBER,
        SET_MEMBERS,
        SET_MOVE,
        SET_POP,
        SET_RANDMEMBER,
        SET_REMOVE,
        SET_UNION,
        SET_UNIONSTORE,

        //<5> hash操作
        HASH_DEL        = 500,
        HASH_EXISTS,
        HASH_GET,
        HASH_GETALL,
        HASH_INCRBY,
        HASH_INCRBYFLOAT,
        HASH_KEYS,
        HASH_LEN,
        HASH_MGET,
        HASH_MSET,
        HASH_SET,
        HASH_SETNX,
        HASH_VALUES,

        //<6> 订阅发布
        PUBLISH         = 600,
        SUBSCRIBE,
        UNSUBSCRIBE,
        PSUBSCRIBE,
        PUNSUBSCRIBE
    };
}

class REDIS4QTAPI Redis4QtCommon : public QObject
{
public:
    Redis4QtCommon();
    ~Redis4QtCommon();
    /**
     * @brief getValue 读取配置中某个key的值
     * @param strsection 节点
     * @param strKey     键
     * @param strCfgFile 配置文件
     * @return 值
     */
    static QString getValue(const QString &strsection, const QString &strKey, const QString &strCfgFile);
    /**
     * @brief setValue 设置某个键值
     * @param strsection 节点
     * @param strKey 键
     * @param strValue 值
     * @param strCfgFile 配置文件
     */
    static void setValue(const QString &strsection, const QString &strKey,
                  const QString &strValue, const QString &strCfgFile);
};

#endif // REDIS_TYPES_H
