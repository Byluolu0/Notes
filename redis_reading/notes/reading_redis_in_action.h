/*
重点：回去看各个数据接口的操作的时间、空间复杂度。
目标：不用太纠结于具体业务，主要是了解redis在这些业务中的应用。

redis实战
第四章
数据安全与性能保障
1、快照 和 aof持久化
问题：
a、aof的appendfsync选项：always（总是写到硬盘）、everysec（每秒一次）、no（由操作系统决定）
b、aof开启久了之后，文件会非常大，所以需要重写aof机制。（可手动或自动配置）
2、复制
预留内存做bgsave。复制开始试，需要bgsave，然后同步到从服务器
主从链
检测硬盘写入。往主服务器写一个虚拟值，在从服务器检测到，然后检测info命令返回的aof_pending_bio_fsync是否为0。
aof_pending_bio_fsync: 后台 I/O 队列里面，等待执行的 fsync 调用数量。为0代表全部写入硬盘了。
3、处理系统故障
检验快照文件和aof文件
redis-check-aof
redis-check-dump

4、redis事务
【使用方法：
watch变量，观察他的变化
然后pipeline一次取出所有条件判断所需的数据
做条件判断
然后pipeline一次过更新所有数据。】

5、非事务流水线
一次过提交命令，减少客户端服务器的通讯次数

6、redis附带性能测试程序redis-benchmark

第六章
自动补全
假设要搜索的串只含有a~z
1、用zset保存所有的串
2、对于输入前缀prefix，往zset插入 比前缀可以匹配到的所有串都要小的、但是比其他所有比前缀都小的元素都要大的 串start，end同理。
start和end，返回其中所有的元素。
比如prefix=abc，那么start=abb{，end=abc{

分布式锁
简易锁
acquire_lock
while now < timeout
    setnx lockname

release_lock
    while true
        watch lockname
        multi
        delete lockname
        execute
        unwatch

信号量（用了zset）
版本一
zset保存，成员是id，分值是时间。
获取信号量的时候，先删除过期的，然后以当前时间zadd进去，获取rank，如果小于limit就代表获取成功。
释放信号量，就直接删除自己的项。
问题：如果a比b的时间慢10ms，a先获得最后一个信号量，然后b在10ms内也去获取信号量，这是b会取代a。
版本二
增加一个counter，用counter去做zset的分值，就不会出现之前用时间做分值的情况下，由于时间误差盗取信号量的情况。
版本三
增加刷新信号量持有时间的接口
问题：
a获得counter x，这个b也开始获得counter x+1，并且率先获得最后一个信号量；这时a继续执行，去获得信号量，是会成功的。
版本四
在获取新号量代码执行前，加上上一节实现的分布式锁。（感觉性能很成问题）

带超时的信号量，可以防止客户端发生错误没有释放信号量时一直占用。

任务队列
先进先出任务队列
用redis的list，任务用json格式发送到redis的list，消费者从list里面取。
如果json里面包含callback函数的name，就能实现多种业务。
改进：
如果要实现优先级任务，可以用多个list。消费时先取高优先级的。

延迟任务
zset保存，成员是任务json，分值是执行时间。
消费者死循环从中取，如果第一个元素时间>=当前时间，则获取锁，然后把任务放进先进先出的另外一个队列里面，然后释放锁。

支持离线的发布订阅
chat_client 成员为客户端，分值为拉取了最新一条消息的id 的zset
client_chat 成员为群组id，分值为拉取了该群组最新一条消息的id 的zset
chat_message 成员是消息，分值为递增的消息id
1、加入群组
chat_client 中zadd client 0
client_chat 中zadd chat 0
2、发消息
获取锁，chat_message 中zadd message id
3、拉去消息
遍历client_chat，在chat_message中取消息；更新client_chat chat_client的分值；chat_client取最大id，然后删除chat_message无用消息。
4、退出群组
chat_client中删除对应client
client_chat中删除对应chat
更新chat_message
如果chat_client中没人，删除chat_message，删除chat_client

文件传输
把redis当作一个中继，利用上面群组的接口，发送者传文件内容到redis，接受者主动从redis取，并且处理。

第七章 搜索
搜索的例子
1、找出有意义的词，建立 词->文章 的kv集合
2、对于 关键字 +同义词 -排除的词 格式，对集合进程union insert diff操作，得出结果。
3、对上述查找结果排序，用redis的sort功能，根据文章的更新时间。
4、基于两个以上标准对搜索结果排序，用zset的zinterstore。如文章id的set，文章的更新日期zset，文章的投票zset，三个zinterstore。

广告的例子
1、为广告创建索引。以地区为键名广告id为元素的set、以关键字为键广告id为元素的zset，以"value"为键广告id和价值为元素的zset。
value可按cost per view、cost per click、cost per action。
2、对匹配的地区set进行union，得到一个元素为广告id的set。
3、然后拿这个跟value的zset进行intersect，得到有广告id为成员，分值为价值的zset。
4、假设关键字为键，成员是广告id，分值为附加值的zset
5、对每个匹配的关键字，与步骤3的zset进行intersect，结果为关键字为键，成员为广告id，分值是附加值的zset。
6、对上面每个关键字的zset进行MIN的union 和 MAX的union。然后再对这两个zset 和 步骤3的zset 进行SUM的union，权值分别为0.5 0.5 1。求出每个广告来自与关键字的附加值均值 + 基础价值。

大致业务
1、为广告创建索引。
2、根据请求参数，找出价值最高的广告。参数包括地区、关键字。
3、根据匹配单词，算附加值。（每个单词为键有个有序集合，值是广告id，分值是附加值）

职位搜索例子
1、以技能为键，工作id为成员构建set
2、构建zset，以工作id为键，所需技能个数为分值
3、传入候选人的技能列表，对每个技能的set，进行zunionstore操作，得到一个zset，成员是工作id，分值为候选人满足的技能个数。
（这一步得出候选人满足或不够满足的工作）
4、步骤2 和 步骤3的zset进行zintersect，权值分别是-1，1，得到一个zset，成员是工作id，分值<=0
5、对步骤4的zset进行zrangebyscore，取第一个，就是最佳匹配的工作。

第八章
社交网站
user:<uid> 的hash存玩家基本信息
status:<sid> 的hash存玩家发送的状态信息
home:<uid> 的zset，status的id为成员，分值为发送时间，主页时间线：自己和他人发送的状态信息
profile:<uid> 的zset，status的id为成员，分值为发送时间，个人时间线：自己发送的状态信息
following:<uid> zset，关注的id为成员，分值为关注时间
follower:<uid> zset，关注我的id为成员，分值为关注时间
难点：
发布消息，如果关注者太多(>1000)，不要马上将状态更新到所有人的时间线，而是使用延迟任务。

第九章
降低内存占用
1、短结构
压缩列表
短字符串，如果用list保存，每个结构需要前向、后向指针、val指针、len字段、free字段和一个'\0'共需要额外21个字节。
而用压缩列表，用一个字节表示前一元素长度，一个字节表示当前串长度，只需额外2个字节。节省空间非常明显。
list-max-ziplist-entries 512
list-max-ziplist-value 64

hash-max-ziplist-entries 512
hash-max-ziplist-value 64

zset-max-ziplist-entries 128
zset-max-ziplist-value 64

如果单个数据太大了，压缩节省空间不明显；如果长度太长，增删改操作性能消耗大，编码解码也消耗cpu。

整数集合
如果单个数据太大了，压缩节省空间不明显；如果长度太长，增删改操作性能消耗大。

2、分片结构
将一个大hash分成N个小hash，并不是redis中的集群功能。set同理。

3、打包存储二进制位和字节
例子：保存7亿人的国家地区。每个人两个字节，分多个字符串保存（哪个人属于哪个字符串用分片）。
然后编写求国家地区总人数的接口，还有更新某个人国家地区信息的接口。

第十章
1、扩展读性能
主从，从服务器只读。
注意问题：
同时注册多个从服务器，公用一个快照，但是同时向多个从服务器发快照，会占大量主服务器带宽。
主从链。
带压缩的ssh隧道。
Sentinel自动故障转移。

2、扩展写性能和内存容量
分片
客户端实现的分片。连接多个独立的redis。

3、扩展复杂的查询
分片。拿前几章内容做为例子，重写成分片版本的api。简单的写一个shard函数，给定一个键，返回该键的分片的连接，然后在这个连接上操作。
没有用到redis自带的分片功能。有点粗糙？

【set】
sadd set1 "a"
sadd set1 "b"
sadd set1 "c"

sadd set2 "a"
sadd set2 "b"
sadd set2 "d"

sinter set1 set2
"a"
"b"

sinterstore set3 set1 set2

smembers set3
"a"
"b" 

sunion set1 set2
sunionstore set4 set1 set2

sdiff set1 set2
sdiff set2 set1

sdiffstore

【zset】
ZINCRBY key increment member

zrange key start end [withscores]

zrevrange key start end [withscores]

zunionstore dest numberkeys key [key...] [weights weight [weight ...]] [agregate sum|min|max]
*/