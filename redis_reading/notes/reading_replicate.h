/*
复制功能

【开启】
slave的配置
slaveof 改成master的ip port

或者直接向从服务器发命令 slave ip port 

#define REDIS_REPL_NONE 0 // No active replication 
#define REDIS_REPL_CONNECT 1 // Must connect to master 
#define REDIS_REPL_CONNECTING 2 // Connecting to master 
#define REDIS_REPL_RECEIVE_PONG 3 // Wait for PING reply 
#define REDIS_REPL_TRANSFER 4 // Receiving .rdb from master 
#define REDIS_REPL_CONNECTED 5 // Connected to master 


【关键路径】
从服务器收到客户端命令slaveof
[slave]
slaveofCommand
    replicationSetMaster
        server.repl_state = REDIS_REPL_CONNECT; 
replicationCron
    connectWithMaster
        anetTcpNonBlockBindConnect 非阻塞connect，EINPROGRESS
        aeCreateFileEvent syncWithMaster
        server.repl_state = REDIS_REPL_CONNECTING;
syncWithMaster
    getsockopt 判断阻塞connect是否成功
    [if REDIS_REPL_CONNECTING]
        server.repl_state = REDIS_REPL_RECEIVE_PONG;
        syncWrite(fd,"PING\r\n",6,100); //阻塞写ping

[master]
pingCommand
    addReply(c,shared.pong);//发pong回slave

[slave]
syncWithMaster
    [if REDIS_REPL_RECEIVE_PONG]
        aeDeleteFileEvent(server.el,fd,AE_READABLE);
        sendSynchronousCommand(fd,"REPLCONF","listening-port",port,NULL);
            syncWrite //阻塞写

[master]
replconfCommand
    c->slave_listening_port = port;
    addReply(c,shared.ok);

[slave]
回到上面阻塞写
syncReadLine //读ok回来
syncWithMaster
    slaveTryPartialResynchronization
        server.repl_master_initial_offset = -1;
        sendSynchronousCommand(fd,"PSYNC",psync_runid,psync_offset,NULL); //同步写->读

[master]
syncCommand
    masterTryPartialResynchronization //需要fullsync
        "+FULLRESYNC %s %lld\r\n"
    listAddNodeTail(server.slaves,c); //将c加到slaves列表中
    createReplicationBacklog 创建一个partial sync backlog

[slave]
接上面sendSynchronousCommand阻塞读
设置server.repl_master_runid server.repl_master_initial_offset
syncWithMaster
    slaveTryPartialResynchronization返回PSYNC_FULLRESYNC
    server.repl_state = REDIS_REPL_TRANSFER;
    server.repl_transfer_fd = dfd; 创建一个本地文件

[master]
replicationCron
    startBgsaveForReplication
        rdbSaveBackground
            fork
            子进程：
                rdbSave
                    rdbSaveRio
            父进程


rdb子进程状态
rdb_child_pid
rdb_child_type

//fullsync有两种 REDIS_RDB_CHILD_TYPE_DISK、REDIS_RDB_CHILD_TYPE_SOCKET
if 有rdb进程在运行 并且 状态为REDIS_RDB_CHILD_TYPE_DISK
    如果有slave在REDIS_REPL_WAIT_BGSAVE_END状态 -> copy该slave的状态到要同步的slave的状态，并设为 REDIS_REPL_WAIT_BGSAVE_END
    否则 -> 设置状态为 REDIS_REPL_WAIT_BGSAVE_START
elseif 有rdb进程在运行 并且 状态为REDIS_RDB_CHILD_TYPE_SOCKET
    将slave的状态设置为 REDIS_REPL_WAIT_BGSAVE_START
else 
    if repl_diskless_sync REDIS_REPL_WAIT_BGSAVE_START
    else REDIS_REPL_WAIT_BGSAVE_END

【难点】
1、
非阻塞connect
把socket设为非阻塞，然后connnect返回-1，如果错误码为EINPROGRESS，表示连接正在建立。
设置读写事件。在读写时间内调用getsockopt，看是否成功。

2、
从库连主库时，ping用的是阻塞write，并设置超时时间100ms，万一发失败怎么办？

*/