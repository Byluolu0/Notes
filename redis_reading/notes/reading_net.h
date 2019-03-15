/*
AliRedis了解一下，不开源。。。
https://www.oschina.net/p/aliredis

【主要文件】
ae.h
ae.c

anet.h
anet.c

redis.h
redis.c

net_working.c

ae_epoll.c
ae_evport.c
ae_kqueue.c
ae_select.c

【网络事件接口】
static int aeApiCreate(aeEventLoop *eventLoop);
static int aeApiResize(aeEventLoop *eventLoop, int setsize);
static void aeApiFree(aeEventLoop *eventLoop);
static int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask);
static void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int mask);
static int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp);
static char *aeApiName(void);


【主要结构】
redisServer-redis.h

aeEventLoop-ae.h server成员


aeFileEvent-ae.h aeEventLoop成员

aeFiredEvent-ae.h aeEventLoop成员

aeTimeEvent-ae.h aeEventLoop成员

redisClient-redis.h
	
redisDb-redis.h

【gdb相关】
gdb ./src/redis-server
set args redis.conf
b fileName:funcName
r 运行
print varName 
c 继续运行
s 进入
n 下一步 

【关键路径】
main-redis.c
	initServerConfig
		populateCommandTable-redis.c 初始化redisCommand列表，存放在server.commands中
	loadServerConfig 读配置文件
	initServer-redis.c
		aeCreateEventLoop-ae.c
			aeApiCreate-ae_xxx.c 创建poller
		listenToPort-redis.c  监听socket存在server.ipfd
			anetTcp6Server-anet.c
			anetTcpServer-anet.c
				_anetTcpServer-anet.c
					socket
					anetListen-anet.c
						bind
						listen
		aeCreateTimeEvent-ae.c 回调函数为serverCron。把事件放到eventLoop->timeEventHead，这里看是1ms调一次，可能吗？
		aeCreateFileEvent-redis.c 设置accept事件 回调函数保存在eventLoop.events->rfileProc/wfileProc acceptTcpHandler
			aeApiAddEvent-ae_xxx.c
	aeMain-ae.c
		aeProcessEvents-ae.c 循环执行
			aeApiPoll-ae_xxx.c 
			rfileProc
			wfileProc
			processTimeEvents-ae.c 处理定时任务
	loadDataFromDisk



收到连接的处理
acceptTcpHandler-networking.c
	anetTcpAccept-anet.c
		anetGenericAccept-anet.c
			accept
	acceptCommonHandler-networking.c
		createClient-networking.c
			aeCreateFileEvent-ae.c
				aeApiAddEvent-ae_xxx.c


读取请求的处理（这就是我要的收到命令的入口吧）
readQueryFromClient-networking.c
	read
	processInputBuffer-networking.c
		processInlineBuffer-networking.c 将命令分割成一个一个字符串
		processCommand-redis.c 估计就是解析处理命令了
			lookupCommand-redis.c 根据第一个参数在server.commands里面找命令
			call-redis.c 

call 例子
setCommand
	addReply 回复客户端
		_addReplyToBuffer-networking.c redisClient.buf
		_addReplyObjectToList-networking.c redisClient.reply
	setGenericCommand-t_string.c
		setKey-db.c
			lookupKeyWrite-db.c
			dbAdd-db.c
			dbOverwrite-db.c
		addReply-networking.c

回复客户端
addReply
	prepareClientToWrite-networking.c 有点疑惑，每次有内容发送，绑定一个写事件？
		aeCreateFileEvent-ae.c WRITEABLE事件
			aeApiAddEvent-ae_xxx.c
	下面是把发送到客户端的内容放到发送缓存

客户可写回调函数
sendReplyToClient-networking.c 
先写client->buf里面的，再写client->reply里面的。一次写的size有限制。写完所有数据，就删掉可写事件。
	aeDeleteFileEvent-ae.c
		aeApiDelEvent-ae_xxx.c

定时函数
serverCron-redis.c 处理后台时间的定时器函数（这个是回调函数的内容）
	databasesCron-redis.c
		activeExpireCycle-redis.c 删除过期键
			activeExpireCycleTryExpire.c 定期删除一定数量的过期键，而且是随机选取的。问题，如何保证某键在过期后一定被删除了。getCommand时会首先验证expired
	wait3
		backgroundSaveDoneHandler -rdb后台备份完毕回调
		backgroundRewriteDoneHandler -rdb后台备份完毕回调
	rdbSaveBackground 检查rdbsave
	replicationCron

发布订阅
订阅
subscribe命令
	subscribeCommand
		pubsubSubscribeChannel
			把订阅方信息放到server.pubsub_channels

发布
例子
setCommand命令
	setGenericCommand
		notifyKeyspaceEvent
			pubsubPublishMessage
				遍历server.pubsub_channels发布消息

1   2   3   4   5   6   7

【难点】
1、
getaddrinfo这个函数卡了一下，直接看man里面的例子

2、
ae_xxx.c 到底选择哪个，在ae.c有做选择
 #ifdef HAVE_EPOLL
    #include "ae_epoll.c"

而HAVE_EPOLL在config.h有定义
#ifdef __linux__
#define HAVE_EPOLL 1
#endif

__linux__ __APPLE__ 等是平台相关的预编译宏


【省内存的地方】
1、 zipList
2、 tryObjectEncoding
3、 clientsCron 检测客户端输入缓冲区，超过一定值，重新分配默认大小的缓冲区
4、 databasesCron 尝试rehash数据库hash表
5、定时删除过期键

【其他】
redis Makefile分析 配置文件分析 目录分析
http://www.cnblogs.com/ym65536/p/7077216.html

*/

