#ifndef REDIS4QT_H
#define REDIS4QT_H

#include "redis4qt_global.h"
#include <QObject>

class Redis4QtPrivate;
class REDIS4QTAPI Redis4Qt : public QObject
{
    Q_OBJECT
public:
    Redis4Qt();
    ~Redis4Qt();
    /**
     * @brief 配置redis服务
     * @param strCfg 配置文件
     */
    void setRedisCof(QString strCfg);

    /**
     * @brief 该命令用于在 key 存在时删除 key
     * @param key 键名
     * @param error 是否发生错误
     * @return 删除成功返回已删除个数
     */
    int key_del(QString key, bool *error = NULL);

    /**
     * @brief 序列化给定 key ，并返回被序列化的值
     * @param key 键名
     * @param error 是否发生错误
     * @return 序列化后的值
     */
    QString key_dump(QString key, bool *error = NULL);

    /**
     * @brief 检查key是否存在
     * @param key 键名
     * @param error 是否发生错误
     * @return 存在返回true， 否则返回false
     */
    bool key_exists(QString key, bool *error = NULL);

    /**
     * @brief 为给定 key 设置过期时间（秒）
     * @param key 键名
     * @param seconds 过期时间秒数
     * @param error 是否发生错误
     * @return 设置成功返回true，否则返回false
     */
    bool key_expire(QString key,int seconds, bool *error = NULL);

    /**
     * @brief 和 EXPIRE 类似, 不同在于 EXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)
     * @param key 键名
     * @param timestamp 到期的时间戳
     * @param error 是否发生错误
     * @return 设置成功返回true，否则返回false
     */
    bool key_expireat(QString key, qint64 timestamp, bool *error = NULL);

    /**
     * @brief 查找所有符合给定模式( pattern)的 key
     * @param pattern 给定的模式(例如*、test*)
     * @param error 是否发生错误
     * @return 符合给定模式的键列表
     */
    QStringList keys(QString pattern, bool *error = NULL);

    /**
     * @brief 以毫秒为单位返回 key 的剩余的过期时间
     * @param key 键名
     * @param error 是否发生错误
     * @return 剩余的过期时间（毫秒）
     */
    int key_pttl(QString key, bool *error = NULL);

    /**
     * @brief 以秒为单位返回 key 的剩余的过期时间
     * @param key 键名
     * @param error 是否发生错误
     * @return 剩余的过期时间（秒）
     */
    int key_ttl(QString key, bool *error = NULL);

    /**
     * @brief 修改 key 的名称
     * @param key 键名
     * @param newkey 新的键名
     * @param error 是否发生错误
     * @return 重命名成功返回true，否则返回false
     */
    bool key_rename(QString key, QString newkey, bool *error = NULL);

    /**
    * @brief 返回 key 所储存的值的类型
    * @param key 键名
    * @param error 是否发生错误
    * @return 键的类型
    */
    QString key_type(QString key, bool *error = NULL);


    /**
     * @brief 设置指定 key 的值
     * @param key 键名
     * @param value 键名对应的值
     * @param error 是否发生错误
     * @return 成功返回true，否则返回false
     */
    bool string_set(QString key, QString value, bool *error = NULL);

    /**
     * @brief 获取指定 key 的值
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回key对应的值，不存在则返回NULL
     */
    QString string_get(QString key, bool *error = NULL);

    /**
     * @brief 返回 key 中字符串值的子字符
     * @param key 键名
     * @param start 键名对应的value中的起始位置
     * @param stop 键名对应的value中的结束位置
     * @param error 是否发生错误
     * @return 返回满足条件的子串，不存在或超出范围返回空串
     */
    QString string_getrange(QString key, int start, int stop, bool *error = NULL);

    /**
     * @brief 获取所有(一个或多个)给定 key 的值
     * @param keys 键名列表
     * @param是否发生错误
     * @return 返回获取到的值列表
     */
    QStringList string_mget(QStringList keys, bool *error = NULL);

    /**
     * @brief 同时设置一个或多个 key-value 对
     * @param map 需要设置的键值对组合
     * @param error 是否发生错误
     * @return 设置成功返回true，否则返回false
     */
    bool string_mset(QMap<QString, QVariant> map, bool *error = NULL);

    /**
     * @brief 将 key 中储存的数字值增一
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回增加后的值
     */
    int string_incr(QString key, bool *error = NULL);

    /**
     * @brief 将 key 所储存的值加上给定的增量值（increment）
     * @param key 键名
     * @param interval 增量值
     * @param error 是否发生错误
     * @return 返回增加后的值
     */
    int string_incrby(QString key,int interval, bool *error = NULL);

    /**
     * @brief 将 key 中储存的数字值减一
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回减少后的值
     */
    int string_decr(QString key, bool *error = NULL);

    /**
     * @brief key 所储存的值减去给定的减量值（decrement）
     * @param key 键名
     * @param interval 减量值
     * @param error 是否发生错误
     * @return 返回减少后的值
     */
    int string_decrby(QString key,int interval, bool *error = NULL);

    /**
     * @brief 如果 key 已经存在并且是一个字符串， APPEND 命令将 value 追加到 key 原来的值的末尾; 不存在则设置key值为value
     * @param key 键名
     * @param value 键名对应的值
     * @param error 是否发生错误
     * @return 返回追加后的值
     */
    int string_append(QString key, QString value, bool *error = NULL);


    /**
     * @brief 移出并获取列表的第一个元素， 如果列表没有元素会阻塞列表直到等待超时或发现可弹出元素为止
     * @param key 键名
     * @param timeout 超时时间（秒）
     * @param error 是否发生错误
     * @return 返回弹出的元素列表
     */
    QStringList list_blpop(QString key, int timeout, bool *error = NULL);

    /**
     * @brief 移出并获取列表的最后一个元素， 如果列表没有元素会阻塞列表直到等待超时或发现可弹出元素为止
     * @param key 键名
     * @param timeout 超时时间（秒）
     * @param error 是否发生错误
     * @return 返回弹出的元素列表
     */
    QStringList list_brpop(QString key, int timeout, bool *error = NULL);

    /**
     * @brief 从列表中弹出一个值，将弹出的元素插入到另外一个列表中并返回它； 如果列表没有元素会阻塞列表直到等待超时或发现可弹出元素为止
     * @param source 弹出元素的键名称
     * @param destination 插入元素的键名称
     * @param timeout 超时时间（秒）
     * @param error 是否发生错误
     * @return 返回弹出的元素列表
     */
    QStringList list_brpoplpush(QString source, QString destination, int timeout, bool *error = NULL);

    /**
     * @brief 通过索引获取列表中的元素。你也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推。
     * @param key 键名
     * @param index 键名对应的列表中元素的索引序号
     * @param error 是否发生错误
     * @return 返回索引对应的元素，不存在返回NULL
     */
    QString list_index(QString key, int index, bool *error = NULL);

    /**
     * @brief 在列表的元素前或者后插入元素。 当指定元素不存在于列表中时，不执行任何操作。
              当列表不存在时，被视为空列表，不执行任何操作。 如果 key 不是列表类型，返回一个错误。
     * @param key 键名
     * @param position 插入元素前后（BEFORE|AFTER）
     * @param pivot 作为参考的元素值
     * @param value 插入的元素值
     * @param error 是否发生错误
     * @return 成功则返回当前列表的元素个数，失败返回-1
     */
    int list_insert(QString key, QString position, QString pivot, QString value, bool *error = NULL);

    /**
     * @brief 返回获取列表长度（列表元素个数）。如果列表 key 不存在，则 key 被解释为一个空列表，返回 0 。 如果 key 不是列表类型，返回一个错误。
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回列表元素个数，不存在返回0
     */
    int list_len(QString key, bool *error = NULL);

    /**
     * @brief 移出并获取列表的第一个元素
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回列表的第一个元素，不存在则返回NULL
     */
    QString list_lpop(QString key, bool *error = NULL);

    /**
     * @brief 将一个值插入到列表头部
     * @param key 键名
     * @param value 需要插入的元素
     * @param error 是否发生错误
     * @return 成功则返回插入后元素个数，失败则返回0
     */
    int list_lpush(QString key, QString value, bool *error = NULL);

    /**
     * @brief 将多个值插入到列表头部
     * @param key 键名
     * @param values 需要插入的元素列表
     * @param error 是否发生错误
     * @return 成功则返回插入后元素个数，失败则返回0
     */
    int list_lpush(QString key, QStringList values, bool *error = NULL);

    /**
     * @brief 将一个或多个值插入到已存在的列表头部，列表不存在时操作无效
     * @param key 键名
     * @param values
     * @param error 是否发生错误
     * @return 成功则返回插入后元素个数，失败则返回0
     */
    int list_lpushx(QString key, QStringList values, bool *error = NULL);

    /**
     * @brief Redis Lrange 返回列表中指定区间内的元素，区间以偏移量 start 和 end 指定。 其中 0 表示列表的第一个元素，
              1表示列表的第二个元素，以此类推。 你也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推
     * @param key 键名
     * @param start 指定的区间起始偏移量
     * @param end 指定的区间起始偏移量
     * @param error 是否发生错误
     * @return 返回当前区间内的元素列表
     */
    QStringList list_range(QString key, int start, int end, bool *error = NULL);

    /**
     * @brief Redis Lrem 根据参数 count 的值，移除列表中与参数 value 相等的元素
              COUNT 的值可以是以下几种：
              count > 0 : 从表头开始向表尾搜索，移除与 VALUE 相等的元素，数量为 COUNT
              count < 0 : 从表尾开始向表头搜索，移除与 VALUE 相等的元素，数量为 COUNT 的绝对值
              count = 0 : 移除表中所有与 VALUE 相等的值
     * @param key 键名
     * @param count 需要移除的元素个数
     * @param value 需要移除的元素值
     * @param error 是否发生错误
     * @return 返回成功移除的元素个数
     */
    int list_rem(QString key, int count, QString value, bool *error = NULL);

    /**
     * @brief 通过索引来设置元素的值。
              当索引参数超出范围，或对一个空列表进行 LSET 时，返回一个错误。
     * @param key 键名
     * @param index 索引号
     * @param value 元素值
     * @param error 是否发生错误
     * @return 成功返回1，失败返回0
     */
    int list_set(QString key, int index, QString value, bool *error = NULL);

    /**
     * @brief 对一个列表进行修剪(trim)，就是说，让列表只保留指定区间内的元素，不在指定区间之内的元素都将被删除。
              下标 0 表示列表的第一个元素，以 1 表示列表的第二个元素，以此类推。
              你也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推
     * @param key 键名
     * @param start 指定的区间起始偏移量
     * @param stop 指定的区间起始偏移量
     * @param error 是否发生错误
     * @return 成功返回true，失败返回false
     */
    bool list_trim(QString key, int start, int stop, bool *error = NULL);

    /**
     * @brief 移除并返回列表的最后一个元素
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回最后一个元素，不存在则返回NULL
     */
    QString list_rpop(QString key, bool *error = NULL);

    /**
     * @brief 移除列表的最后一个元素，并将该元素添加到另一个列表并返回
     * @param source 移除元素的键名称
     * @param destination 插入元素的键名称
     * @param error 是否发生错误
     * @return 返回最后一个元素，不存在则返回NULL
     */
    QString list_rpoplpush(QString source, QString destination, bool *error = NULL);

    /**
     * @brief 将一个值插入到列表的尾部(最右边)。如果列表不存在，一个空列表会被创建并执行 RPUSH 操作。 当列表存在但不是列表类型时，返回一个错误
     * @param key 键名
     * @param values 需要插入的元素
     * @param error 是否发生错误
     * @return 返回插入后的元素个数
     */
    int list_rpush(QString key, QString value, bool *error = NULL);

    /**
     * @brief 将多个值插入到列表的尾部(最右边)。如果列表不存在，一个空列表会被创建并执行 RPUSH 操作。 当列表存在但不是列表类型时，返回一个错误
     * @param key 键名
     * @param values 需要插入的元素列表
     * @param error 是否发生错误
     * @return 返回插入后的元素个数
     */
    int list_rpush(QString key, QStringList values, bool *error = NULL);

    /**
     * @brief 将一个或多个值插入到已存在的列表尾部(最右边)。如果列表不存在，操作无效
     * @param key 键名
     * @param values 需要插入的元素列表
     * @param error 是否发生错误
     * @return 返回插入后的元素个数
     */
    int list_rpushx(QString key, QStringList values, bool *error = NULL);


    /**
     * @brief 将一个成员元素加入到集合中，已经存在于集合的成员元素将被忽略。
              假如集合 key 不存在，则创建一个只包含添加的元素作成员的集合。
              当集合 key 不是集合类型时，返回一个错误。
     * @param key 键名
     * @param member 添加的元素
     * @param error 是否发生错误
     * @return 返回成功添加的元素个数
     */
    int set_add(QString key, QString member, bool *error = NULL);

    /**
     * @brief 将多个成员元素加入到集合中，已经存在于集合的成员元素将被忽略。
              假如集合 key 不存在，则创建一个只包含添加的元素作成员的集合。
              当集合 key 不是集合类型时，返回一个错误。
     * @param key 键名
     * @param members 添加的元素列表
     * @param error 是否发生错误
     * @return 返回成功添加的元素个数
     */
    int set_add(QString key, QStringList members, bool *error = NULL);

    /**
     * @brief 返回集合中元素的数量
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回元素个数
     */
    int set_card(QString key, bool *error = NULL);

    /**
     * @brief 返回给定集合之间的差集。不存在的集合 key 将视为空集
     * @param keys 键名列表
     * @param error 是否发生错误
     * @return 返回差集元素列表
     */
    QStringList set_diff(QStringList keys, bool *error = NULL);

    /**
     * @brief 将给定集合之间的差集存储在指定的集合中。如果指定的集合 key 已存在，则会被覆盖
     * @param destination 存储的指定集合
     * @param keys 键名列表
     * @param error 是否发生错误
     * @return 返回差集元素个数
     */
    int set_diffstore(QString destination, QStringList keys, bool *error = NULL);

    /**
     * @brief 返回给定所有给定集合的交集。不存在的集合 key 被视为空集。 当给定集合当中有一个空集时，结果也为空集(根据集合运算定律)
     * @param keys 键名列表
     * @param error 是否发生错误
     * @return 返回交集元素列表
     */
    QStringList set_inter(QStringList keys, bool *error = NULL);

    /**
     * @brief 将给定集合之间的交集存储在指定的集合中。如果指定的集合已经存在，则将其覆盖
     * @param destination 存储的指定集合
     * @param keys 键名列表
     * @param error 是否发生错误
     * @return 返回交集元素个数
     */
    int set_interstore(QString destination, QStringList keys, bool *error = NULL);

    /**
     * @brief 判断成员元素是否是集合的成员
     * @param key 键名
     * @param member 需要判断的元素
     * @param error 是否发生错误
     * @return 是集合成员则返回true，不是则返回false
     */
    bool set_ismember(QString key, QString member, bool *error = NULL);

    /**
     * @brief 返回集合中的所有的成员。 不存在的集合 key 被视为空集合
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回集合中所有元素
     */
    QStringList set_members(QString key, bool *error = NULL);

    /**
     * @brief 将指定成员 member 元素从 source 集合移动到 destination 集合。
              SMOVE 是原子性操作。
              如果 source 集合不存在或不包含指定的 member 元素，则 SMOVE 命令不执行任何操作，仅返回 0 。否则， member 元素从 source 集合中被移除，并添加到 destination 集合中去。
              当 destination 集合已经包含 member 元素时， SMOVE 命令只是简单地将 source 集合中的 member 元素删除。
              当 source 或 destination 不是集合类型时，返回一个错误。
     * @param source 源集合
     * @param destination 目标集合
     * @param member 需要移动的元素
     * @param error 是否发生错误
     * @return 成功返回true，失败返回false
     */
    bool set_move(QString source, QString destination, QString member, bool *error = NULL);

    /**
     * @brief 移除并返回集合中的一个随机元素
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回随机移除的元素，不存在则返回NULL
     */
    QString set_pop(QString key, bool *error = NULL);

    /**
     * @brief 返回集合中的一个随机元素。
              该操作和 SPOP 相似，但 SPOP 将随机元素从集合中移除并返回，而 Srandmember 则仅仅返回随机元素，而不对集合进行任何改动。
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回随机的元素，不存在则返回NULL
     */
    QStringList set_randmember(QString key, bool *error = NULL);

    /**
     * @brief 返回集合中的一个随机元素。
              从 Redis 2.6 版本开始， Srandmember 命令接受可选的 count 参数：
              如果 count 为正数，且小于集合基数，那么命令返回一个包含 count 个元素的数组，数组中的元素各不相同。如果 count 大于等于集合基数，那么返回整个集合。
              如果 count 为负数，那么命令返回一个数组，数组中的元素可能会重复出现多次，而数组的长度为 count 的绝对值。
              该操作和 SPOP 相似，但 SPOP 将随机元素从集合中移除并返回，而 Srandmember 则仅仅返回随机元素，而不对集合进行任何改动。
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回随机的元素列表
     */
    QStringList set_randmember(QString key, int count, bool *error = NULL);

    /**
     * @brief 移除集合中的一个成员元素，不存在的成员元素会被忽略。当 key 不是集合类型，返回一个错误
     * @param key 键名
     * @param member 移除的元素
     * @param error 是否发生错误
     * @return 成功返回移除的元素个数
     */
    int set_rem(QString key, QString member, bool *error = NULL);

    /**
     * @brief 移除集合中的多个成员元素，不存在的成员元素会被忽略。当 key 不是集合类型，返回一个错误
     * @param key 键名
     * @param members 移除的元素列表
     * @param error 是否发生错误
     * @return 成功返回移除的元素个数
     */
    int set_rem(QString key, QStringList members, bool *error = NULL);

    /**
     * @brief 返回给定集合的并集。不存在的集合 key 被视为空集
     * @param keys 键名列表
     * @param error 是否发生错误
     * @return 返回并集元素列表
     */
    QStringList set_union(QStringList keys, bool *error = NULL);

    /**
     * @brief 将给定集合的并集存储在指定的集合 destination 中。如果 destination 已经存在，则将其覆盖
     * @param destination
     * @param keys 键名列表
     * @param error 是否发生错误
     * @return 返回并集元素个数
     */
    int set_unionstore(QString destination, QStringList keys, bool *error = NULL);


    /**
     * @brief 删除哈希表 key 中的一个指定字段，不存在的字段将被忽略
     * @param key 键名
     * @param field 指定字段
     * @param error 是否发生错误
     * @return 返回成功删除的元素个数
     */
    int hash_del(QString key, QString field, bool *error = NULL);

    /**
     * @brief 删除哈希表 key 中的多个指定字段，不存在的字段将被忽略
     * @param key 键名
     * @param fields 指定字段列表
     * @param error 是否发生错误
     * @return 返回成功删除的元素个数
     */
    int hash_del(QString key, QStringList fields, bool *error = NULL);

    /**
     * @brief 查看哈希表的指定字段是否存在
     * @param key 键名
     * @param field 指定字段
     * @param error 是否发生错误
     * @return 存在则返回true，不存在返回false
     */
    bool hash_exists(QString key, QString field, bool *error = NULL);

    /**
     * @brief 返回哈希表中指定字段的值
     * @param key 键名
     * @param field 指定字段
     * @param error 是否发生错误
     * @return 返回指定字段的元素值，不存在则返回NULL
     */
    QString hash_get(QString key, QString field, bool *error = NULL);

    /**
     * @brief 返回哈希表中，所有的字段和值。在返回值里，紧跟每个字段名(field name)之后是字段的值(value)，所以返回值的长度是哈希表大小的两倍
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回字段和元素的键值对
     */
    QVariantMap hash_getall(QString key, bool *error = NULL);

    /**
     * @brief 为哈希表中的字段值加上指定增量值。
              增量也可以为负数，相当于对指定字段进行减法操作。
              如果哈希表的 key 不存在，一个新的哈希表被创建并执行 HINCRBY 命令。
              如果指定的字段不存在，那么在执行命令前，字段的值被初始化为 0 。
              对一个储存字符串值的字段执行 HINCRBY 命令将造成一个错误。
              本操作的值被限制在 64 位(bit)有符号数字表示之内。
     * @param key 键名
     * @param field 指定字段
     * @param increment 增量（整型）
     * @param error 是否发生错误
     * @return 增加后的当前元素值
     */
    int hash_incrby(QString key, QString field, int increment, bool *error = NULL);

    /**
     * @brief 为哈希表中的字段值加上指定浮点数增量值。如果指定的字段不存在，那么在执行命令前，字段的值被初始化为0
     * @param key 键名
     * @param field 指定字段
     * @param increment 增量（浮点型）
     * @param error 是否发生错误
     * @return 增加后的当前元素值
     */
    float hash_incrbyfloat(QString key, QString field, float increment, bool *error = NULL);

    /**
     * @brief 获取哈希表中的所有字段名
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回所有的字段列表
     */
    QStringList hash_keys(QString key, bool *error = NULL);

    /**
     * @brief 获取哈希表中字段的数量
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回字段个数
     */
    int hash_len(QString key, bool *error = NULL);

    /**
     * @brief 返回哈希表中，一个或多个给定字段的值。如果指定的字段不存在于哈希表，那么返回一个 nil 值
     * @param key 键名
     * @param fields 指定的字段列表
     * @param error 是否发生错误
     * @return 返回字段列表对应的元素列表
     */
    QStringList hash_mget(QString key, QStringList fields, bool *error = NULL);

    /**
     * @brief 同时将多个 field-value (字段-值)对设置到哈希表中。此命令会覆盖哈希表中已存在的字段。如果哈希表不存在，会创建一个空哈希表，并执行 HMSET 操作
     * @param key 键名
     * @param map 字段-元素键值对
     * @param error 是否发生错误
     * @return 成功返回true，失败返回false
     */
    bool hash_mset(QString key, QMap<QString,QVariant> map, bool *error = NULL);

    /**
     * @brief 为哈希表中的字段赋值。如果哈希表不存在，一个新的哈希表被创建并进行 HSET 操作。如果字段已经存在于哈希表中，旧值将被覆盖
     * @param key 键名
     * @param field 字段名
     * @param value 字段对应的元素
     * @param error 是否发生错误
     * @return 新增则返回true，否则返回false
     */
    bool hash_set(QString key, QString field, QString value, bool *error = NULL);

    /**
     * @brief 为哈希表中不存在的的字段赋值。
              如果哈希表不存在，一个新的哈希表被创建并进行 HSET 操作。
              如果字段已经存在于哈希表中，操作无效。
              如果 key 不存在，一个新哈希表被创建并执行 HSETNX 命令。
     * @param key 键名
     * @param field 字段名
     * @param value 字段对应的元素
     * @param error 是否发生错误
     * @return 新增则返回true，否则返回false
     */
    bool hash_setnx(QString key, QString field, QString value, bool *error = NULL);

    /**
     * @brief 返回哈希表所有字段的值
     * @param key 键名
     * @param error 是否发生错误
     * @return 返回所有的元素列表
     */
    QStringList hash_values(QString key, bool *error = NULL);

    /**
     * @brief 将信息发送到指定的频道
     * @param channel 通道名称
     * @param message 需要发布的消息
     * @param error 是否发生错误
     */
    void publish(QString channel, QString message, bool *error = NULL);

    /**
     * @brief 订阅给定的一个或多个频道的信息
     * @param channel 通道名称
     * @param error 是否发生错误
     */
    void subscribe(QString channel, bool *error = NULL);

    /**
     * @brief 退订给定的一个或多个频道的信息
     * @param channel 通道名称
     * @param error 是否发生错误
     */
    void unsubscribe(QString channel, bool *error = NULL);

    /**
     * @brief 订阅一个或多个符合给定模式的频道。每个模式以 * 作为匹配符，比如 it* 匹配所有以 it 开头的频道( it.news 、 it.blog 、 it.tweets 等等)。
              news.* 匹配所有以 news. 开头的频道( news.it 、 news.global.today 等等)，诸如此类。
     * @param channel 通道名称
     * @param error 是否发生错误
     */
    void psubscribe(QString channel, bool *error = NULL);

    /**
     * @brief 退订所有给定模式的频道
     * @param channel 通道名称
     * @param error 是否发生错误
     */
    void punsubscribe(QString channel, bool *error = NULL);

	/**
     * @brief 调用接口发生错误时，获取错误类型
     * @return 错误类型，定义在lht_errors.h文件中
     */
    int getLastError();

private:
    Redis4QtPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Redis4Qt)
    Q_DISABLE_COPY(Redis4Qt)
};

#endif // REDIS4QT_H
