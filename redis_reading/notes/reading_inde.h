/*
独立功能
1、发布订阅
SUBSCRIBE 订阅
UNSUBSCRIBE 取消订阅
PSUBSCRIBE 模式订阅
PUNSUBSCRIBE 模式退订
PUBLISH 发送消息

PUBSUB CHANNELS [pattern] 查看服务端被订阅的频道
PUBSUB NUMSUB <channel_name> 查看有多少client订阅了这个频道
PUBSUB NUMPAT 查看订阅模式的数量

redisServer
    pubsub_channels dict
    pubsub_pattern list


2、事务trasaction
感觉这章关于ACID的描述有点水。
MUTI
commands
EXEC

WATCH 乐观锁
EXEC执行前观察键，到执行EXEC时，如果观察键被修改了，那么EXEC不执行。

redisDB
    watched_keys dict

multiCmd
    argv
    argc
    cmd 命令指针

multiState
    commands
    count

redisClient
    mstate

3、慢查询日志
命令
config set slowlog-log-slower-than x
config set slowlog-max-len x
slowlog get

void slowlogPushEntryIfNeeded(robj **argv, int argc, long long duration);

slowlogEntry
    id
    time
    duration
    argv
    argc

redisServer
    slowlog_entry_id
    slowlog
    slowlog_log_lower_than
    slowlog_max_len

4、监视器
命令
monitor

redisServer
    monitors

void replicationFeedMonitors(redisClient *c, list *monitors, int dictid, robj **argv, int argc);

5、排序
redisSortObject

sort <key> [alpha] [asc] [desc] 

BY
SADD fruits "apple" "banana" "cherry"
MSET apple-price 8 banana-price 5.5 cherry-price 7
SORT fruits BY *-price // * 代入fruit里面的三个元素，然后拼成 *-price 在redis键中找对应的值，然后根据值来排序；加入alpha选项也有效

limit <offset> <count>


*/