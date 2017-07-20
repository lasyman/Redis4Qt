#ifndef RedisReader_H
#define RedisReader_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QTcpSocket>
#include <QSocketNotifier>
#include <QByteArray>
#include <QDebug>

class RedisReader : public QObject
{
    Q_OBJECT
public:
    explicit RedisReader(QString host, int port, QObject *parent = 0);
    ~RedisReader();

    void connectHost(const QString &host, quint16 port);
    bool isConnected() const;
    void disconnectHost();

signals:
    void response(QVariant);
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);

public slots:
    void socketConnected();
    void socketConnectionClosed();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void sendData(const QString &);
    void readLine(const QString &);

private:
    QTcpSocket *socket;
    bool reading;
    QSocketNotifier *notifier;
    QString host;
    int port;
    bool hasMoreData;       //current reply data not full, wait for more data
    QString replyData;      //data reply from redis server
};

#endif // RedisReader_H
