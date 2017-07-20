#include "redis_reader.h"

RedisReader::RedisReader(QString host, int port, QObject *parent) : QObject(parent)
{
    this->host = host;
    this->port = port;

    socket = new QTcpSocket;
    reading = false;

    hasMoreData = false;
    replyData.clear();

    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketConnectionClosed()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
}

RedisReader::~RedisReader()
{
    delete socket;
}

void RedisReader::connectHost(const QString &host, quint16 port)
{

    if (reading)
        return;

    socket->connectToHost(host, port);

}

bool RedisReader::isConnected() const
{
    return reading;
}

void RedisReader::disconnectHost()
{
    socket->disconnectFromHost();
}

void RedisReader::socketConnected()
{
    reading = true;
    emit connected();
}

void RedisReader::socketConnectionClosed()
{
    reading = false;
    emit disconnected();
}

void RedisReader::socketReadyRead()
{
    QString reply("");

    reply.append(socket->readAll());

    readLine(reply);
}

void RedisReader::socketError(QAbstractSocket::SocketError e)
{
    emit error(e);
}

void RedisReader::sendData(const QString &data)
{
    if (!reading)
        return;

    //initial variable
    hasMoreData = false;
    replyData.clear();

    QTextStream stream(socket);

    QString test(data);
    int k;

    QChar c,next;
    QStringList parts;
    QString buffer="";
    bool open=false;
    for(k=0;k<test.length();k++)
    {
        c=test.at(k);
        if(open)
        {
            next=k<test.length()-1?test
                                   .at(k+1):' ';
            if(c=='\\'&&next=='"')
            {
                buffer+='"';
                k++;
            }
            else if(c=='"')
                open=false;
            else
                buffer+=c;
        }
        else
        {
            if(!c.isSpace())
            {
                if(c=='"')
                    open=true;
                else
                    buffer+=c;
            }
            else if(!buffer.isEmpty())
            {
                parts<<buffer;
                buffer="";
            }
        }
    }

    if(!buffer.isEmpty())
    {
        parts<<buffer;
    }

    QString bin="";
    bin.append(QString("*%1\r\n")
               .arg(parts.length()));
    int i;
    for(i=0;i<parts.length();i++)
    {
        bin.append(QString("$%1\r\n")
                   .arg(parts.at(i)
                        .length()));

        bin.append(QString("%1\r\n")
                   .arg(parts.at(i)));
    }

    stream<<bin;
    stream.flush();
}

void RedisReader::readLine(const QString &reply)
{
    if(hasMoreData)
    {
        replyData.append(reply);
    }
    else
    {
        replyData = reply;
    }

    QChar first=replyData.at(0);
    QString value;
    QStringList result;

    if(first=='+')
    {
        value=replyData.mid(1);
        value.chop(2);

        result << "string" << value;
    }
    else if(first=='-')
    {
        value=replyData.mid(1);
        value.chop(2);

        result << "error" << value;
    }
    else if(first==':')
    {
        value=replyData.mid(1);
        value.chop(2);

        result << "integer" << value;

    } else if(first=='$')
    {
        int index=replyData.indexOf("\r\n");
        if(index == -1)
        {
            qDebug()<<QString("String need more data! Cannot find $ Num end!");
            hasMoreData = true;
            return;
        }
        int len=replyData.mid(1,index-1)
                .toInt();
        if(len<=0)
            value="NULL";
        else
        {
            value=replyData.mid(index+2,len+2);
            // deal with response not full
            //qDebug()<<"Need len:"<<len<<", value len is "<<value.size();
            if(value.size() < len+2)
            {
                qDebug()<<QString("String need more data! Need len %1, only %2 reply!")
                          .arg(len).arg(value.size());
                hasMoreData = true;
                return;
            }
            value.chop(2);
        }

        result<< "bulk" << value;

    } else if(first=='*')
    {
        int index=replyData.indexOf("\r\n");
        int count=replyData.mid(1,index-1)
                .toInt();
        if(index==-1)
        {
            qDebug()<<QString("StringList need more data! Cannot find $ Num end!");
            hasMoreData = true;
            return;
        }
        int i;
        int len;
        int pos=index+2;
        result << "list";

        if(count<1)
        {
            result <<"";
        }

        for(i=0;i<count;i++)
        {
            index=replyData.indexOf("\r\n",pos);
            if(index == -1)
            {
                qDebug()<<QString("List need more data! Current count %1 cannot find \r\n!")
                          .arg(count);
                hasMoreData = true;
                return;
            }
            len=replyData.mid(pos+1,index-pos-1)
                    .toInt();
            QString value;
            if(len <= 0)
            {
                value = "NULL";
            }
            else
            {
                value = replyData.mid(index+2,len+2);

                if(value.size() < len+2)
                {
                    qDebug()<<QString("List need more data! Current count %1 need len %2, only %3 reply!")
                              .arg(count).arg(len).arg(value.size());
                    hasMoreData = true;
                    return;
                }
                value.chop(2);
            }
            result<<value;
            pos=index+2+len+2;
        }
    }

    emit response(result);
}
