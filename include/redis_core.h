#ifndef RedisCore_H
#define RedisCore_H

#include <QObject>
#include <QStringList>
#include <QEventLoop>

#include "redis_reader.h"
#include "redis_errors.h"
#include "redis_types.h"

using namespace Redis4Qt_Types;

class RedisCore : public QObject
{
    Q_OBJECT
public:
    explicit RedisCore(QString host, int port, QObject *parent = 0);
    ~RedisCore();

    bool openConnection();
    bool closeConnection();
    bool isConnected();
    int  getLastError();
    void setLastError(int error);
    void setTimeout(int timeout);

    void publish(QString, QString);
    void subscribe(QString);
    void unsubscribe(QString);
    void psubscribe(QString);
    void punsubscribe(QString);

    Reply command(QString);

    int append(QString key, QString value);
    bool auth(QString password);
    QStringList blpop(QString key, int timeout);
    QStringList brpop(QString key, int timeout);
    QStringList brpoplpush(QString source, QString destination, int timeout);
    int decr(QString key);
    int decrby(QString key,int interval);
    int del(QString key);
    QString dump(QString key);
    bool exists(QString key);
    bool expire(QString key,int seconds);
    bool expireat(QString key, qint64 timestamp);
    QVariant eval(QString script, int numkeys, QString keys, QString arguments);
    QVariant evalsha(QString sha1, int numkeys, QString keys, QString arguments);
    QString get(QString key);
    QString getrange(QString key, int start, int stop);

    int hdel(QString key, QStringList fields);
    bool hexists(QString key, QString field);
    QString hget(QString key, QString field);
    QMap<QString,QVariant> hgetall(QString key);

    int hincrby(QString key, QString field, int increment);
    float hincrbyfloat(QString key, QString field, float increment);
    QStringList hkeys(QString key);
    int hlen(QString key);
    QStringList hmget(QString key, QStringList fields);
    bool hmset(QString key, QMap<QString,QVariant> map);
    bool hset(QString key, QString field, QString value);
    bool hsetnx(QString key, QString field, QString value);
    QStringList hvals(QString key);


    int incr(QString key);
    int incrby(QString key,int interval);
    QStringList keys(QString pattern);
    QString lindex(QString key, int index);
    int linsert(QString key, QString position,
                QString pivot, QString value);
    int llen(QString key);
    QString lpop(QString key);
    int lpush(QString key, QStringList values);
    int lpushx(QString key, QStringList values);
    QStringList lrange(QString key, int start, int stop);
    int lrem(QString key, int count, QString value);
    int lset(QString key, int index, QString value);
    bool ltrim(QString key, int start, int stop);
    QStringList mget(QStringList keys);
    bool migrate(QString host, int port, QString key, int database, int timeout);
    bool move(QString key, int database);
    bool mset(QMap<QString, QVariant> map);
    QVariant object(QString subcommand, QString arguments);
    bool persist(QString key);
    bool pexpire(QString key, qint64 mseconds);
    bool pexpireat(QString key, qint64 mstimestamp);
    int pttl(QString key);
    bool rename(QString key, QString newkey);
    bool renamex(QString key, QString newkey);
    bool restore(QString key, int ttl, QString value);
    QString rpop(QString key);
    QString rpoplpush(QString source, QString destination);
    int rpush(QString key, QStringList values);
    int rpushx(QString key, QStringList values);
    int sadd(QString key, QStringList members);
    int scard(QString key);
    QStringList scriptexists(QString script);
    QString scriptflush();
    QString scriptkill();
    QString scriptload(QString script);
    QStringList sdiff(QStringList keys);
    int sdiffstore(QString destination, QStringList keys);
    QStringList sinter(QStringList keys);
    int sinterstore(QString destination, QStringList keys);
    bool sismember(QString key, QString member);
    QStringList smembers(QString key);
    bool smove(QString source, QString destination, QString member);
    QString spop(QString key);
    QStringList srandmember(QString key, int count);
    int srem(QString key, QStringList members);
    QStringList sunion(QStringList keys);
    int sunionstore(QString destination, QStringList keys);
    bool set(QString key, QString value);
    int setrange(QString key, int offset, QString value);
    QStringList sort(QString key, QString conditions);
    int ttl(QString key);
    QString type(QString key);
    void connectHost(const QString &host, const quint16 port = 6379);
    void disconnectHost();


signals:
    void connected();
    void disconnected();
    void returnData(Reply);
    void cmdSuccess();

private slots:
    void slotConnected();
    void slotDisconnected();
    void response(QVariant);

private:
    int      m_errorCode;              //错误码
    volatile bool    m_hasReply;       //redis是否有返回值
    RedisReader *reader;
    QString host;                      //redis服务器地址
    int port;                          //redis服务器端口
    int cmdTimeout;                    //redis命令超时时间
    QStringList responseData;
};

#endif // RedisCore_H
