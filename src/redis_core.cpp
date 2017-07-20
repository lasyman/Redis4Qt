#include "redis_core.h"
#include <QTimer>

RedisCore::RedisCore(QString host, int port, QObject *parent) : QObject(parent)
{
    this->host = host;
    this->port = port;
    m_errorCode = REDIS_SUCCESS;
    cmdTimeout = DEAFULT_REDIS_CMD_TIMEOUT;

    reader = new RedisReader(host, port);
    connect(reader, SIGNAL(response(QVariant)), this, SLOT(response(QVariant)));
    connect(reader, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(reader, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

RedisCore::~RedisCore()
{
    disconnect(this);
    delete reader;
}

bool RedisCore::openConnection()
{
    connectHost(host,port);

    QEventLoop loop;
    connect(reader, SIGNAL(connected()), &loop, SLOT(quit()));
    QTimer::singleShot(cmdTimeout*1000, &loop, SLOT(quit()));
    loop.exec();

    if(!reader->isConnected())
    {
        m_errorCode = REDIS_CONNECT_FAILED;
        return false;
    }
    else
    {
        m_errorCode = REDIS_SUCCESS;
    }

    return true;
}

bool RedisCore::closeConnection()
{
    reader->disconnectHost();
    return true;
}

bool RedisCore::isConnected()
{
    return reader->isConnected();
}

int RedisCore::getLastError()
{
    return m_errorCode;
}

void RedisCore::setLastError(int error)
{
    m_errorCode = error;
}

void RedisCore::setTimeout(int timeout)
{
    cmdTimeout = timeout;
}

void RedisCore::connectHost(const QString &host, const quint16 port)
{
    reader->connectHost(host,port);
}

void RedisCore::disconnectHost()
{
    reader->disconnectHost();
}

void RedisCore::slotConnected()
{
    m_errorCode = REDIS_SUCCESS;
    emit connected();
}

void RedisCore::slotDisconnected()
{
    emit disconnected();
}

/* Redis Public API */
void RedisCore::publish(QString channel, QString message)
{
    QString cmd;
    cmd.append("PUBLISH ");
    cmd.append(channel);
    cmd.append(" ");
    cmd.append(message);

    reader->sendData(cmd);
}

void RedisCore::subscribe(QString channel)
{
    QString cmd;
    cmd.append("SUBSCRIBE ");
    cmd.append(channel);

    reader->sendData(cmd);
}

void RedisCore::unsubscribe(QString channel)
{
    QString cmd;
    cmd.append("UNSUBSCRIBE ");
    cmd.append(channel);

    reader->sendData(cmd);
}

void RedisCore::psubscribe(QString channel)
{
    QString cmd;
    cmd.append("PSUBSCRIBE ");
    cmd.append(channel);

    reader->sendData(cmd);
}

void RedisCore::punsubscribe(QString channel)
{
    QString cmd;
    cmd.append("PUNSUBSCRIBE ");
    cmd.append(channel);

    reader->sendData(cmd);
}

void RedisCore::response(QVariant response)
{
    m_hasReply = true;
    QStringList res = response.toStringList();

    Reply reply;

    //订阅消息
    if (res[1] == "message")
    {
        reply.type = res[0];
        reply.message = res[1];
        reply.channel = res[2];
        reply.value = res[3];

        emit returnData(reply);
    }
    //模式订阅消息
    else if (res[1] == "pmessage")
    {
        reply.type = res[0];
        reply.message = res[1];
        reply.pattern = res[2];
        reply.channel = res[3];
        reply.value = res[4];

        emit returnData(reply);
    }
    //命令返回
    else
    {
        responseData = res;
        emit cmdSuccess();
    }
}

RedisCore::Reply RedisCore::command(QString cmd)
{
    m_hasReply = false;
    Reply reply;

    reader->sendData(cmd);

    QEventLoop loop;
    connect(this, SIGNAL(cmdSuccess()), &loop, SLOT(quit()));
    QTimer::singleShot(cmdTimeout*1000, &loop, SLOT(quit()));
    loop.exec();

    if(m_hasReply)
    {
        if(responseData[0] == "integer")
        {
            reply.type = responseData[0];
            reply.value = responseData[1].toInt();
        }
        else if(responseData[0] == "list")
        {
            reply.type = responseData[0];

            QStringList list;
            for(int i=1; i < responseData.length(); i++)
            {
                if(!responseData[i].isEmpty())
                    list << responseData[i];
            }

            reply.value = list;
        }
        else
        {
            reply.type = responseData[0];
            reply.value = responseData[1];
        }
        m_errorCode = REDIS_SUCCESS;
    }
    else
    {
        m_errorCode = REDIS_TIMEOUT;
    }

    return reply;
}

int RedisCore::append(QString key, QString value)
{
    QString cmd("APPEND ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toInt();
}

bool RedisCore::auth(QString password)
{
    QString cmd("AUTH ");
    cmd.append(password);

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

QStringList RedisCore::blpop(QString key, int timeout)
{
    QString cmd("BLPOP ");

    cmd.append(key);
    cmd.append(" ");

    cmd.append(QString::number(timeout));

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

QStringList RedisCore::brpop(QString key, int timeout)
{
    QString cmd("BRPOP ");

    cmd.append(key);
    cmd.append(" ");

    cmd.append(QString::number(timeout));

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

QStringList RedisCore::brpoplpush(QString source, QString destination, int timeout)
{
    QString cmd("BRPOPLPUSH ");

    cmd.append(source);
    cmd.append(" ");

    cmd.append(destination);
    cmd.append(" ");

    cmd.append(QString::number(timeout));

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::decr(QString key)
{
    QString cmd("DECR ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::decrby(QString key,int interval)
{
    QString cmd("DECRBY ");
    cmd.append(key);

    cmd.append(" ");

    cmd.append(QString::number(interval));

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::del(QString key)
{
    QString cmd("DEL ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QString RedisCore::dump(QString key)
{
    QString cmd("DUMP ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toString();
}

bool RedisCore::exists(QString key)
{
    QString cmd("EXISTS ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toBool();
}

bool RedisCore::expire(QString key,int seconds)
{
    QString cmd("EXPIRE ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(seconds));

    Reply reply = command(cmd);

    return reply.value.toBool();
}

bool RedisCore::expireat(QString key, qint64 timestamp)
{
    QString cmd("EXPIREAT ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(timestamp));

    Reply reply = command(cmd);

    return reply.value.toBool();
}

QVariant RedisCore::eval(QString script, int numkeys, QString keys, QString arguments)
{
    QString cmd("EVAL ");
    cmd.append(script);

    cmd.append(" ");
    cmd.append(QString::number(numkeys));

    cmd.append(" ");
    cmd.append(keys);

    cmd.append(" ");
    cmd.append(arguments);

    Reply reply = command(cmd);

    return reply.value;
}

QVariant RedisCore::evalsha(QString sha1, int numkeys, QString keys, QString arguments)
{
    QString cmd("EVALSHA ");
    cmd.append(sha1);

    cmd.append(" ");
    cmd.append(QString::number(numkeys));

    cmd.append(" ");
    cmd.append(keys);

    cmd.append(" ");
    cmd.append(arguments);

    Reply reply = command(cmd);

    return reply.value;
}

QString RedisCore::get(QString key)
{
    QString cmd("GET ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toString();
}

QString RedisCore::getrange(QString key, int start, int stop)
{
    QString cmd("GETRANGE ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(start));

    cmd.append(" ");
    cmd.append(QString::number(stop));

    Reply reply = command(cmd);

    return reply.value.toString();
}

int RedisCore::hdel(QString key, QStringList fields)
{
    QString cmd("HDEL ");
    cmd.append(key);

    for(int i=0; i<fields.size(); i++)
    {
        cmd.append(" ");
        cmd.append(fields[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

bool RedisCore::hexists(QString key, QString field)
{
    QString cmd("HEXISTS ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(field);

    Reply reply = command(cmd);

    return reply.value.toBool();
}

QString RedisCore::hget(QString key, QString field)
{
    QString cmd("HGET ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(field);

    Reply reply = command(cmd);

    return reply.value.toString();
}

QMap<QString,QVariant> RedisCore::hgetall(QString key)
{
    QString cmd("HGETALL ");
    cmd.append(key);

    Reply reply = command(cmd);

    QMap<QString,QVariant> keypairs;
    QStringList list = reply.value.toStringList();

    for(int i=0; i<list.length(); i++)
    {
        QString k = list[i];
        i++;
        QString v = list[i];

        keypairs[k] = QString(v);
    }

    return keypairs;
}

int RedisCore::hincrby(QString key, QString field, int increment)
{
    QString cmd("HINCRBY ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(field);

    cmd.append(" ");
    cmd.append(QString::number(increment));

    Reply reply = command(cmd);

    return reply.value.toInt();
}

float RedisCore::hincrbyfloat(QString key, QString field, float increment)
{
    QString cmd("HINCRBYFLOAT ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(field);

    cmd.append(" ");
    cmd.append(QString::number(increment));

    Reply reply = command(cmd);

    return reply.value.toFloat();
}

QStringList RedisCore::hkeys(QString key)
{
    QString cmd("HKEYS ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::hlen(QString key)
{
    QString cmd("HLEN ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::hmget(QString key, QStringList fields)
{
    QString cmd("HMGET ");
    cmd.append(key);

    for(int i=0; i<fields.size(); i++)
    {
        cmd.append(" ");
        cmd.append(fields[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

bool RedisCore::hmset(QString key, QMap<QString,QVariant> map)
{
    QString cmd("HMSET ");
    cmd.append(key);

    QMapIterator<QString, QVariant> i(map);

    while (i.hasNext())
    {
        i.next();
        cmd.append(" ");
        cmd.append(i.key());
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(i.value().toString());
        cmd.append("\"");

    }

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

bool RedisCore::hset(QString key, QString field, QString value)
{
    QString cmd("HSET ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(field);

    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toBool();
}

bool RedisCore::hsetnx(QString key, QString field, QString value)
{
    QString cmd("HSETNX ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(field);

    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toBool();
}

QStringList RedisCore::hvals(QString key)
{
    QString cmd("HVALS ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::incr(QString key)
{
    QString cmd("INCR ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::incrby(QString key,int interval)
{
    QString cmd("INCRBY ");
    cmd.append(key);

    cmd.append(" ");

    cmd.append(QString::number(interval));

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::keys(QString pattern)
{
    QString cmd("KEYS ");
    cmd.append(pattern);

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

QString RedisCore::lindex(QString key, int index)
{
    QString cmd("LINDEX ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append(QString::number(index));

    Reply reply = command(cmd);

    return reply.value.toString();
}

int RedisCore::linsert(QString key, QString position,
            QString pivot, QString value)
{
    QString cmd("LINSERT ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append(position);
    cmd.append(" ");
    cmd.append("\"");
    cmd.append(pivot);
    cmd.append("\"");
    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toInt();

}

int RedisCore::llen(QString key)
{
    QString cmd("LLEN ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QString RedisCore::lpop(QString key)
{
    QString cmd("LPOP ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toString();
}

int RedisCore::lpush(QString key, QStringList values)
{
    QString cmd("LPUSH ");
    cmd.append(key);
    for(int i=0; i<values.size(); i++)
    {
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(values[i]);
        cmd.append("\"");
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::lpushx(QString key, QStringList values)
{
    QString cmd("LPUSHX ");
    cmd.append(key);
    for(int i=0; i<values.size(); i++)
    {
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(values[i]);
        cmd.append("\"");
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::lrange(QString key, int start, int stop)
{
    QString cmd("LRANGE ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append(QString::number(start));
    cmd.append(" ");
    cmd.append(QString::number(stop));

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::lrem(QString key, int count, QString value)
{
    QString cmd("LREM ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append(QString::number(count));
    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::lset(QString key, int index, QString value)
{
    QString cmd("LSET ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append(QString::number(index));
    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toInt();
}

bool RedisCore::ltrim(QString key, int start, int stop)
{
    QString cmd("LTRIM ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append(QString::number(start));
    cmd.append(" ");
    cmd.append(QString::number(stop));

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

QStringList RedisCore::mget(QStringList keys)
{
    QString cmd("MGET");
    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

bool RedisCore::migrate(QString host, int port, QString key, int database, int timeout)
{
    QString cmd("MIGRATE ");
    cmd.append(host);

    cmd.append(" ");
    cmd.append(QString::number(port));

    cmd.append(" ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(database));

    cmd.append(" ");
    cmd.append(QString::number(timeout));

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

bool RedisCore::move(QString key, int database)
{
    QString cmd("MOVE ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(database));

    Reply reply = command(cmd);

    return reply.value.toBool();
}

bool RedisCore::mset(QMap<QString, QVariant> map)
{
    QString cmd("MSET");

    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext())
    {
        i.next();
        cmd.append(" ");
        cmd.append(i.key());
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(i.value().toString());
        cmd.append("\"");

    }

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

QVariant RedisCore::object(QString subcommand, QString arguments)
{
    QString cmd("OBJECT ");
    cmd.append(subcommand);

    cmd.append(" ");
    cmd.append(arguments);

    Reply reply = command(cmd);

    return reply.value;
}

bool RedisCore::persist(QString key)
{
    QString cmd("PERSIST ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toBool();
}

bool RedisCore::pexpire(QString key, qint64 mseconds)
{
    QString cmd("PEXPIRE ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(mseconds));

    Reply reply = command(cmd);

    return reply.value.toBool();
}

bool RedisCore::pexpireat(QString key, qint64 mstimestamp)
{
    QString cmd("PEXPIREAT ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(mstimestamp));

    Reply reply = command(cmd);

    return reply.value.toBool();
}

int RedisCore::pttl(QString key)
{
    QString cmd("PTTL ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

bool RedisCore::rename(QString key, QString newkey)
{
    QString cmd("RENAME ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(newkey);

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

bool RedisCore::renamex(QString key, QString newkey)
{
    QString cmd("RENAMEX ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(newkey);

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

bool RedisCore::restore(QString key, int ttl, QString value)
{
    QString cmd("RESTORE ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(ttl));

    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

QString RedisCore::rpop(QString key)
{
    QString cmd("RPOP ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toString();
}

QString RedisCore::rpoplpush(QString source, QString destination)
{
    QString cmd("RPOPLPUSH ");
    cmd.append(source);

    cmd.append(" ");
    cmd.append(destination);

    Reply reply = command(cmd);

    return reply.value.toString();
}

int RedisCore::rpush(QString key, QStringList values)
{
    QString cmd("RPUSH ");
    cmd.append(key);

    for(int i=0; i<values.size(); i++)
    {
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(values[i]);
        cmd.append("\"");
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::rpushx(QString key, QStringList values)
{
    QString cmd("RPUSHX ");
    cmd.append(key);

    for(int i=0; i<values.size(); i++)
    {
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(values[i]);
        cmd.append("\"");
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::sadd(QString key, QStringList members)
{
    QString cmd("SADD ");
    cmd.append(key);

    for(int i=0; i<members.size(); i++)
    {
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(members[i]);
        cmd.append("\"");
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

int RedisCore::scard(QString key)
{
    QString cmd("SCARD ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::scriptexists(QString script)
{
    QString cmd("SCRIPT EXISTS ");
    cmd.append(script);

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

QString RedisCore::scriptflush()
{
    QString cmd("SCRIPT FLUSH");

    Reply reply = command(cmd);

    return reply.value.toString();
}

QString RedisCore::scriptkill()
{
    QString cmd("SCRIPT KILL");

    Reply reply = command(cmd);

    return reply.value.toString();
}

QString RedisCore::scriptload(QString script)
{
    QString cmd("SCRIPT LOAD ");
    cmd.append(script);

    Reply reply = command(cmd);

    return reply.value.toString();
}

QStringList RedisCore::sdiff(QStringList keys)
{
    QString cmd("SDIFF");
    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::sdiffstore(QString destination, QStringList keys)
{
    QString cmd("SDIFFSTORE ");
    cmd.append(destination);

    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::sinter(QStringList keys)
{
    QString cmd("SINTER");
    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::sinterstore(QString destination, QStringList keys)
{
    QString cmd("SINTERSTORE ");
    cmd.append(destination);

    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

bool RedisCore::sismember(QString key, QString member)
{
    QString cmd("SISMEMBER ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append("\"");
    cmd.append(member);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toBool();
}

QStringList RedisCore::smembers(QString key)
{
    QString cmd("SMEMBERS ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

bool RedisCore::smove(QString source, QString destination, QString member)
{
    QString cmd("SMOVE ");
    cmd.append(source);

    cmd.append(" ");
    cmd.append(destination);

    cmd.append(" ");
    cmd.append("\"");
    cmd.append(member);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toBool();
}

QString RedisCore::spop(QString key)
{
    QString cmd("SPOP ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toString();
}

QStringList RedisCore::srandmember(QString key, int count)
{
    QString cmd("SRANDMEMBER ");
    cmd.append(key);

    if(count != 0)
    {
        cmd.append(" ");
        cmd.append(QString::number(count));
    }

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::srem(QString key, QStringList members)
{
    QString cmd("SREM ");
    cmd.append(key);

    for(int i=0; i<members.size(); i++)
    {
        cmd.append(" ");
        cmd.append("\"");
        cmd.append(members[i]);
        cmd.append("\"");
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::sunion(QStringList keys)
{
    QString cmd("SUNION ");
    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::sunionstore(QString destination, QStringList keys)
{
    QString cmd("SUNIONSTORE ");
    cmd.append(destination);

    for(int i=0; i<keys.size(); i++)
    {
        cmd.append(" ");
        cmd.append(keys[i]);
    }

    Reply reply = command(cmd);

    return reply.value.toInt();
}

bool RedisCore::set(QString key, QString value)
{
    QString cmd("SET ");
    cmd.append(key);
    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    if(reply.value.toString() == "OK")
        return true;

    return false;
}

int RedisCore::setrange(QString key, int offset, QString value)
{
    QString cmd("SETRANGE ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(QString::number(offset));

    cmd.append(" ");
    cmd.append("\"");
    cmd.append(value);
    cmd.append("\"");

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QStringList RedisCore::sort(QString key, QString conditions)
{
    QString cmd("SORT ");
    cmd.append(key);

    cmd.append(" ");
    cmd.append(conditions);

    Reply reply = command(cmd);

    return reply.value.toStringList();
}

int RedisCore::ttl(QString key)
{
    QString cmd("TTL ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toInt();
}

QString RedisCore::type(QString key)
{
    QString cmd("TYPE ");
    cmd.append(key);

    Reply reply = command(cmd);

    return reply.value.toString();
}

