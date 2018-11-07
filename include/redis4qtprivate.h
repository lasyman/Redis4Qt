#ifndef REDIS4QTPRIVATE_H
#define REDIS4QTPRIVATE_H

#include <QObject>

#include "redis_types.h"
#include "redis_core.h"
#include <QObject>
#include <QMutex>
#include <QSettings>

using namespace Redis4Qt_Types;

class RedisCore;
class QTimer;
struct ThreadRedis
{
    QMutex*      lock;
    QThread*     thread;
    RedisCore* core;
};

class Redis4Qt;
class Redis4QtPrivate : public QObject
{
    Q_OBJECT
public:
    Redis4QtPrivate(Redis4Qt *parent);
    void setRedisCof(QString strCfg);
    int del(QString key, bool *error = NULL);
    QString dump(QString key, bool *error = NULL);
    bool exists(QString key, bool *error = NULL);
    bool expire(QString key,int seconds, bool *error = NULL);
    bool expireat(QString key, qint64 timestamp, bool *error = NULL);
    QStringList keys(QString pattern, bool *error = NULL);
    int pttl(QString key, bool *error = NULL);
    int ttl(QString key, bool *error = NULL);
    bool rename(QString key, QString newkey, bool *error = NULL);
    QString type(QString key, bool *error = NULL);
    bool set(QString key, QString value, bool *error = NULL);
    QString get(QString key, bool *error = NULL);
    QString getrange(QString key, int start, int stop, bool *error = NULL);
    QStringList mget(QStringList keys, bool *error = NULL);
    bool mset(QMap<QString, QVariant> map, bool *error = NULL);
    int incr(QString key, bool *error = NULL);
    int incrby(QString key,int interval, bool *error = NULL);
    int decr(QString key, bool *error = NULL);
    int decrby(QString key,int interval, bool *error = NULL);
    int append(QString key, QString value, bool *error = NULL);
    QStringList blpop(QString key, int timeout, bool *error = NULL);
    QStringList brpop(QString key, int timeout, bool *error = NULL);
    QStringList brpoplpush(QString source, QString destination, int timeout, bool *error = NULL);
    QString lindex(QString key, int index, bool *error = NULL);
    int linsert(QString key, QString position, QString pivot, QString value, bool *error = NULL);
    int llen(QString key, bool *error = NULL);
    QString lpop(QString key, bool *error = NULL);
    int lpush(QString key, QString value, bool *error = NULL);
    int lpush(QString key, QStringList values, bool *error = NULL);
    int lpushx(QString key, QStringList values, bool *error = NULL);
    QStringList lrange(QString key, int start, int end, bool *error = NULL);
    int lrem(QString key, int count, QString value, bool *error = NULL);
    int lset(QString key, int index, QString value, bool *error = NULL);
    bool ltrim(QString key, int start, int stop, bool *error = NULL);
    QString rpop(QString key, bool *error = NULL);
    QString rpoplpush(QString source, QString destination, bool *error = NULL);
    int rpush(QString key, QString value, bool *error = NULL);
    int rpush(QString key, QStringList values, bool *error = NULL);
    int rpushx(QString key, QStringList values, bool *error = NULL);
    int sadd(QString key, QString member, bool *error = NULL);
    int sadd(QString key, QStringList members, bool *error = NULL);
    int scard(QString key, bool *error = NULL);
    QStringList sdiff(QStringList keys, bool *error = NULL);
    int sdiffstore(QString destination, QStringList keys, bool *error = NULL);
    QStringList sinter(QStringList keys, bool *error = NULL);
    int sinterstore(QString destination, QStringList keys, bool *error = NULL);
    bool sismember(QString key, QString member, bool *error = NULL);
    QStringList smembers(QString key, bool *error = NULL);
    bool smove(QString source, QString destination, QString member, bool *error = NULL);
    QString spop(QString key, bool *error = NULL);
    QStringList srandmember(QString key, bool *error = NULL);
    QStringList srandmember(QString key, int count, bool *error = NULL);
    int srem(QString key, QString member, bool *error = NULL);
    int srem(QString key, QStringList members, bool *error = NULL);
    QStringList sunion(QStringList keys, bool *error = NULL);
    int sunionstore(QString destination, QStringList keys, bool *error = NULL);
    int hdel(QString key, QString field, bool *error = NULL);
    int hdel(QString key, QStringList fields, bool *error = NULL);
    bool hexists(QString key, QString field, bool *error = NULL);
    QString hget(QString key, QString field, bool *error = NULL);
    QVariantMap hgetall(QString key, bool *error = NULL);
    int hincrby(QString key, QString field, int increment, bool *error = NULL);
    float hincrbyfloat(QString key, QString field, float increment, bool *error = NULL);
    QStringList hkeys(QString key, bool *error = NULL);
    int hlen(QString key, bool *error = NULL);
    QStringList hmget(QString key, QStringList fields, bool *error = NULL);
    bool hmset(QString key, QMap<QString,QVariant> map, bool *error = NULL);
    bool hset(QString key, QString field, QString value, bool *error = NULL);
    bool hsetnx(QString key, QString field, QString value, bool *error = NULL);
    QStringList hvals(QString key, bool *error = NULL);
    int publish(QString channel, QString message, bool *error = NULL);
    int subscribe(QString channel, bool *error = NULL);
    int unsubscribe(QString channel, bool *error = NULL);
    int psubscribe(QString channel, bool *error = NULL);
    int punsubscribe(QString channel, bool *error = NULL);
    int getLastError();

private:
    RedisCore *getRedisInstance();
    bool validKey(QString &key, QString funcName, bool *error);
    bool validKeys(QStringList &key, QString funcName, bool *error);
    void setLastError(int error);
    QVariant dealwithCmd(FuncType type, QVariantList &data, bool *error);
    QVariant excuteCmd(FuncType type, RedisCore* redis, QVariantList &data);

signals:
    void sigReturnMsg(const QString &channel, const QString &msg);

protected slots:
    void onReturnData(Reply reply);

private:
    friend class Redis4Qt;
    Redis4Qt *q_ptr;
    QString                 m_redisHost;
    qint16                  m_redisPort;
    int                     m_redisCmdTimeout;
    QMap<void *, ThreadRedis>  m_redis;
    QSettings    *m_Settings;
    QMutex                  m_Lock;
    Q_DECLARE_PUBLIC(Redis4Qt)
};

#endif // REDIS4QTPRIVATE_H
