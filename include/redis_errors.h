#ifndef REDIS_ERRORS_H
#define REDIS_ERRORS_H

#include <QObject>

#define REDIS_SUCCESS                    401     //成功
#define REDIS_CONNECT_FAILED             402     //连接redis服务器失败
#define REDIS_TIMEOUT                    403     //redis命令超时
#define REDIS_INVALID_KEY                404     //无效的key
#define REDIS_RUNNING                    405     //当前线程正在执行redis命令，稍后再试
#define DEAFULT_REDIS_CMD_TIMEOUT 3

#define INVALID_INTEGER (-1)
#define INVALID_FLOAT   (-1.0)
#define INVALID_STRING  (QString())
#define INVALID_STRINGLIST (QStringList())
#define INVALID_BOOL    (false)
#define INVALID_VARIANTMAP (QVariantMap())

#endif // REDIS_ERRORS_H
