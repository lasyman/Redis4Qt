#include "example.h"
#include "redis4qt.h"
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

Example::Example(QObject *parent)
    : QObject(parent)
{
    m_strCfgFile = QApplication::applicationDirPath() + "/redis.ini";
#if 0
    QtConcurrent::run(this, &Example::threadAddStringValue);
    QtConcurrent::run(this, &Example::threadReadStringValues);
#else
    funcTest();
#endif
}

Example::~Example()
{

}

void Example::funcTest()
{
    m_redis = new Redis4Qt();
    m_redis->setRedisCof(m_strCfgFile);

    qDebug() << m_redis->key_exists("lht:render:update:R-119");
    m_redis->set_add("RTest", "test");
    qDebug() << m_redis->set_members("RTest");
    m_redis->string_set("string", "string");
    QVariantMap data;
    data.insert("a", 1);
    data.insert("b", true);
    data.insert("c", "string");
    data.insert("d", 0.01);
    bool error;
    qDebug() << m_redis->hash_mset("test", data, &error);
    qDebug() << error;
    qDebug() << m_redis->hash_mget("test", QStringList() << "a" << "d");
}

void Example::threadAddStringValue()
{
    qDebug() << "threadAddStringValue id: " << QThread::currentThreadId();
    Redis4Qt *redis = new Redis4Qt();
    redis->setRedisCof(m_strCfgFile);
    int i = 0;
    while (true)
    {
        redis->string_append("string", QString::number(i));
        qDebug() << "append: " << i;
        i++;
        QThread::msleep(1000);
    }
}

void Example::threadReadStringValues()
{
    qDebug() << "threadReadStringValues id: " << QThread::currentThreadId();
    Redis4Qt *redis = new Redis4Qt();
    QString strCfgFile = QApplication::applicationDirPath() + "/redis.ini";
    redis->setRedisCof(strCfgFile);
    while (true)
    {
        qDebug() << "get: " << redis->string_get("string");
        QThread::msleep(1000);
    }
}
