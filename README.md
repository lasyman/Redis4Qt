# Redis for Qt framework

### Redis 简介

http://www.runoob.com/redis/redis-intro.html

### Redis数据类型

http://www.runoob.com/redis/redis-data-types.html

### Redis4Qt 接口说明

针对不同的数据类型，我们提供了对应的接口，如：

- 对key操作的接口都是以key_开头：如key_del、 key_exists
- 对list操作的接口都是以list_开头：如list_lpop、 list_rpush
- 以此类推
具体的接口详见[Redis4Qt.h](https://lasyman.github.io/Redis4Qt/doc/html/class_redis4_qt.html)文件

### 暂不支持

1、ZSet有序集合

2、订阅发布功能

3、事务功能

