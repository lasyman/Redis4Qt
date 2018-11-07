INCLUDEPATH += \
        $$PWD/include

HEADERS += \
        $$PWD/include/redis4qt_global.h \
        $$PWD/include/redis4qt.h \
        $$PWD/include/redis_core.h \
        $$PWD/include/redis_reader.h \
        $$PWD/include/redis_errors.h \
        $$PWD/include/redis_types.h \
        $$PWD/include/redis4qtprivate.h
	
SOURCES += \
        $$PWD/src/redis4qt.cpp \
        $$PWD/src/redis_core.cpp \
        $$PWD/src/redis_reader.cpp \
        $$PWD/src/redis_types.cpp \
        $$PWD/src/redis4qtprivate.cpp

	
