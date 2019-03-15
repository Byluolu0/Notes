/*
todo
看单元测试，看看是怎么测这个sentinel功能的。感觉很难。

server.sentinel_mode

redis.c
sentinelState sentinel

main
    initSentinelConfig
    initSentinel
        sentinelcmds //覆盖原来的命令
    loadServerConfig 读参数
        sentinelHandleConfiguration
            createSentinelRedisInstance 根据参数创建监控服务器的管理对象
    // 最后有一段if (!server.sentinel_mode)

//Sentinel 对 master、slave 建立两个连接，也就是master、slave对sentinel有两个redisClient(分别对应cc pc)
//Sentinel 这边pc发SUBSCRIBE订阅，master slave的频道消息发到这里来
//而Sentinel的cc则是发PUBLISH INFO以及其他的一些信息

定时器
serverCron
    sentinelTimer    
        sentinelHandleDictOfRedisInstances
            sentinelHandleRedisInstance
                sentinelReconnectInstance  
                    redisAsyncConnectBind cc 
                    redisAeAttach 静态函数，绑定redisAeAddRead|redisAeDelRead|redisAeAddWrite|redisAeDelWrite|redisAeCleanup这几个静态函数
                    redisAsyncSetConnectCallback sentinelLinkEstablishedCallback
                    redisAsyncSetDisconnectCallback sentinelDisconnectCallback
                    sentinelSetClientName client命令 setname "cmd" 命令连接
                    sentinelSendPing
                        redisAsyncCommand sentinelPingReplyCallback
                            redisvAsyncCommand 
                                __redisAsyncCommand
                                    __redisAppendCommand 
                                        c->obuf = newbuf;
                                    _EL_ADD_WRITE
                                        redisAeAddWrite
                                            aeCreateFileEvent redisAeWriteEvent(写事件回调)
                    redisAsyncConnectBind pc (if master or slave)
                    redisAeAttach
                    redisAsyncSetConnectCallback
                    redisAsyncSetDisconnectCallback
                    sentinelSetClientName "pubsub"
                    redisAsyncCommand "SUBSCRIBE"

                    sentinelAskMasterStateToOtherSentinels (if master 询问其他sentinel关于master的状态)
            sentinelSendPeriodicCommands
                redisAsyncCommand "INFO" sentinelInfoReplyCallback 回调 每10秒一次
            sentinelSendHello
                redisAsyncCommand "PUBLISH __sentinel__:hello"
                sentinelSendPing sentinelPingReplyCallback 判断是否在线
                sentinelSendHello "PUBLISH"
            sentinelCheckSubjectivelyDown 判断超时，检测断线 SRI_S_DOWN主观下线 SRI_O_DOWN客观下线
                redisAsyncCommand "SENTINEL" is-master-down-by-addr


aeProcessEvents
    aeApiPoll
    wfileProc
    (eg)              
    redisAeWriteEvent 由reactor模式实现的异步IO？
        redisAsyncHandleWrite
            redisBufferWrite
                write
            _EL_ADD_READ
                redisAeAddRead
                    aeCreateFileEvent redisAeReadEvent(读事件回调)


数据流
1、INFO通过master获取slave信息
2、__sentinel__:hello (ˇ?ˇ) 向其他sentinel告知自己的存在
3、PING判断master离线状态
4、sentinelcommand "SENTINEL is-master-down-by-addr" 告知其他sentinel master主观下线，并竞选。回复投票。
*/