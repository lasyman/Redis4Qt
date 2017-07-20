#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>
#include <QThread>

class Redis4Qt;
class Example : public QObject
{
    Q_OBJECT

public:
    Example(QObject *parent = 0);
    ~Example();
    void funcTest();

    void threadAddStringValue();
    void threadReadStringValues();

private:
    Redis4Qt *m_redis;
    QString m_strCfgFile;
};

#endif // EXAMPLE_H
