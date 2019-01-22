#include "redis4qt.h"
#include "redis4qtprivate.h"
#include <QDebug>

Redis4Qt::Redis4Qt()
    : d_ptr(new Redis4QtPrivate(this))
{
}

Redis4Qt::~Redis4Qt()
{
    delete d_ptr;
}

void Redis4Qt::setRedisCof(QString strCfg)
{
    Q_D(Redis4Qt);
    d->setRedisCof(strCfg);
}

int Redis4Qt::key_del(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->del(key, error);
}

QString Redis4Qt::key_dump(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->dump(key, error);
}

bool Redis4Qt::key_exists(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->exists(key, error);
}

bool Redis4Qt::key_expire(QString key, int seconds, bool *error)
{
    Q_D(Redis4Qt);
    return d->expire(key, seconds, error);
}

bool Redis4Qt::key_expireat(QString key, qint64 timestamp, bool *error)
{
    Q_D(Redis4Qt);
    return d->expireat(key, timestamp, error);
}

QStringList Redis4Qt::keys(QString pattern, bool *error)
{
    Q_D(Redis4Qt);
    return d->keys(pattern, error);
}

int Redis4Qt::key_pttl(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->pttl(key, error);
}

int Redis4Qt::key_ttl(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->ttl(key, error);
}

bool Redis4Qt::key_rename(QString key, QString newkey, bool *error)
{
    Q_D(Redis4Qt);
    return d->rename(key, newkey, error);
}

QString Redis4Qt::key_type(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->type(key, error);
}

bool Redis4Qt::string_set(QString key, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->set(key, value, error);
}

QString Redis4Qt::string_get(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->get(key, error);
}

QString Redis4Qt::string_getrange(QString key, int start, int stop, bool *error)
{
    Q_D(Redis4Qt);
    return d->getrange(key, start, stop, error);
}

QStringList Redis4Qt::string_mget(QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->mget(keys, error);
}

bool Redis4Qt::string_mset(QMap<QString, QVariant> map, bool *error)
{
    Q_D(Redis4Qt);
    return d->mset(map, error);
}

int Redis4Qt::string_incr(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->incr(key, error);
}

int Redis4Qt::string_incrby(QString key, int interval, bool *error)
{
    Q_D(Redis4Qt);
    return d->incrby(key, interval, error);
}

int Redis4Qt::string_decr(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->decr(key, error);
}

int Redis4Qt::string_decrby(QString key, int interval, bool *error)
{
    Q_D(Redis4Qt);
    return d->decrby(key, interval, error);
}

int Redis4Qt::string_append(QString key, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->append(key, value, error);
}

QStringList Redis4Qt::list_blpop(QString key, int timeout, bool *error)
{
    Q_D(Redis4Qt);
    return d->blpop(key, timeout, error);
}

QStringList Redis4Qt::list_brpop(QString key, int timeout, bool *error)
{
    Q_D(Redis4Qt);
    return d->brpop(key, timeout, error);
}

QStringList Redis4Qt::list_brpoplpush(QString source, QString destination, int timeout, bool *error)
{
    Q_D(Redis4Qt);
    return d->brpoplpush(source, destination, timeout, error);
}

QString Redis4Qt::list_index(QString key, int index, bool *error)
{
    Q_D(Redis4Qt);
    return d->lindex(key, index, error);
}

int Redis4Qt::list_insert(QString key, QString position, QString pivot, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->linsert(key, position, pivot, value, error);
}

int Redis4Qt::list_len(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->llen(key, error);
}

QString Redis4Qt::list_lpop(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->lpop(key, error);
}

int Redis4Qt::list_lpush(QString key, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->lpush(key, value, error);
}

int Redis4Qt::list_lpush(QString key, QStringList values, bool *error)
{
    Q_D(Redis4Qt);
    return d->lpush(key, values, error);
}

int Redis4Qt::list_lpushx(QString key, QStringList values, bool *error)
{
    Q_D(Redis4Qt);
    return d->lpushx(key, values, error);
}

QStringList Redis4Qt::list_range(QString key, int start, int end, bool *error)
{
    Q_D(Redis4Qt);
    return d->lrange(key, start, end, error);
}

int Redis4Qt::list_rem(QString key, int count, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->lrem(key, count, value, error);
}

int Redis4Qt::list_set(QString key, int index, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->lset(key, index, value, error);
}

bool Redis4Qt::list_trim(QString key, int start, int stop, bool *error)
{
    Q_D(Redis4Qt);
    return d->ltrim(key, start, stop, error);
}

QString Redis4Qt::list_rpop(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->rpop(key, error);
}

QString Redis4Qt::list_rpoplpush(QString source, QString destination, bool *error)
{
    Q_D(Redis4Qt);
    return d->rpoplpush(source, destination, error);
}

int Redis4Qt::list_rpush(QString key, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->rpush(key, value, error);
}

int Redis4Qt::list_rpush(QString key, QStringList values, bool *error)
{
    Q_D(Redis4Qt);
    return d->rpush(key, values, error);
}

int Redis4Qt::list_rpushx(QString key, QStringList values, bool *error)
{
    Q_D(Redis4Qt);
    return d->rpushx(key, values, error);
}

int Redis4Qt::set_add(QString key, QString member, bool *error)
{
    Q_D(Redis4Qt);
    return d->sadd(key, member, error);
}

int Redis4Qt::set_add(QString key, QStringList members, bool *error)
{
    Q_D(Redis4Qt);
    return d->sadd(key, members, error);
}

int Redis4Qt::set_card(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->scard(key, error);
}

QStringList Redis4Qt::set_diff(QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->sdiff(keys, error);
}

int Redis4Qt::set_diffstore(QString destination, QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->sdiffstore(destination, keys, error);
}

QStringList Redis4Qt::set_inter(QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->sinter(keys, error);
}

int Redis4Qt::set_interstore(QString destination, QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->sinterstore(destination, keys, error);
}

bool Redis4Qt::set_ismember(QString key, QString member, bool *error)
{
    Q_D(Redis4Qt);
    return d->sismember(key, member, error);
}

QStringList Redis4Qt::set_members(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->smembers(key, error);
}

bool Redis4Qt::set_move(QString source, QString destination, QString member, bool *error)
{
    Q_D(Redis4Qt);
    return d->smove(source, destination, member, error);
}

QString Redis4Qt::set_pop(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->spop(key, error);
}

QStringList Redis4Qt::set_randmember(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->srandmember(key, error);
}

QStringList Redis4Qt::set_randmember(QString key, int count, bool *error)
{
    Q_D(Redis4Qt);
    return d->srandmember(key, count, error);
}

int Redis4Qt::set_rem(QString key, QString member, bool *error)
{
    Q_D(Redis4Qt);
    return d->srem(key, member, error);
}

int Redis4Qt::set_rem(QString key, QStringList members, bool *error)
{
    Q_D(Redis4Qt);
    return d->srem(key, members, error);
}

QStringList Redis4Qt::set_union(QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->sunion(keys, error);
}

int Redis4Qt::set_unionstore(QString destination, QStringList keys, bool *error)
{
    Q_D(Redis4Qt);
    return d->sunionstore(destination, keys, error);
}

int Redis4Qt::hash_del(QString key, QString field, bool *error)
{
    Q_D(Redis4Qt);
    return d->hdel(key, field, error);
}

int Redis4Qt::hash_del(QString key, QStringList fields, bool *error)
{
    Q_D(Redis4Qt);
    return d->hdel(key, fields, error);
}

bool Redis4Qt::hash_exists(QString key, QString field, bool *error)
{
    Q_D(Redis4Qt);
    return d->hexists(key, field, error);
}

QString Redis4Qt::hash_get(QString key, QString field, bool *error)
{
    Q_D(Redis4Qt);
    return d->hget(key, field, error);
}

QVariantMap Redis4Qt::hash_getall(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->hgetall(key, error);
}

int Redis4Qt::hash_incrby(QString key, QString field, int increment, bool *error)
{
    Q_D(Redis4Qt);
    return d->hincrby(key, field, increment, error);
}

float Redis4Qt::hash_incrbyfloat(QString key, QString field, float increment, bool *error)
{
    Q_D(Redis4Qt);
    return d->hincrbyfloat(key, field, increment, error);
}

QStringList Redis4Qt::hash_keys(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->hkeys(key, error);
}

int Redis4Qt::hash_len(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->hlen(key, error);
}

QStringList Redis4Qt::hash_mget(QString key, QStringList fields, bool *error)
{
    Q_D(Redis4Qt);
    return d->hmget(key, fields, error);
}

bool Redis4Qt::hash_mset(QString key, QMap<QString, QVariant> map, bool *error)
{
    Q_D(Redis4Qt);
    return d->hmset(key, map, error);
}

bool Redis4Qt::hash_set(QString key, QString field, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->hset(key, field, value, error);
}

bool Redis4Qt::hash_setnx(QString key, QString field, QString value, bool *error)
{
    Q_D(Redis4Qt);
    return d->hsetnx(key, field, value, error);
}

QStringList Redis4Qt::hash_values(QString key, bool *error)
{
    Q_D(Redis4Qt);
    return d->hvals(key, error);
}

int Redis4Qt::publish(QString channel, QString message, bool *error)
{
    Q_D(Redis4Qt);
    return d->publish(channel, message, error);
}

int Redis4Qt::subscribe(QString channel, bool *error)
{
    Q_D(Redis4Qt);
    return d->subscribe(channel, error);
}

int Redis4Qt::unsubscribe(QString channel, bool *error)
{
    Q_D(Redis4Qt);
    return d->unsubscribe(channel, error);
}

int Redis4Qt::psubscribe(QString channel, bool *error)
{
    Q_D(Redis4Qt);
    return d->psubscribe(channel, error);
}

int Redis4Qt::punsubscribe(QString channel, bool *error)
{
    Q_D(Redis4Qt);
    return d->punsubscribe(channel, error);
}
