#ifndef REDIS4QT_GLOBAL_H
#define REDIS4QT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(REDIS4QT_LIBRARY)
#  define REDIS4QTAPI Q_DECL_EXPORT
#else
#  define REDIS4QTAPI Q_DECL_IMPORT
#endif

#endif // REDIS4QT_GLOBAL_H
