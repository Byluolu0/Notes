/*
集群

redisServer
    clusterState 集群信息
        slots 每个槽分别由哪些节点负责
        slots_to_keys （找某槽的所有键时有用）
        importing_slots_from（从其他节点导入slot）
        migrating_slots_to（slot导出到其他节点）
        clusterNode (dict *nodes) 节点列表
            slots 该节点是否负责该槽
            clusterLink 节点的连接信息
            slaveof 如果这个是从节点，那么指向子节点
            //如果这个节点结构有从节点
            numslaves
            slaves
            fail_reports //疑似下线节点

握手过程
A
CLUSTER MEET

B
clusterCommand
    clusterStartHandshake
        createClusterNode
        clusterAddNode

clusterCron
    anetTcpNonBlockBindConnect 
    createClusterLink
    aeCreateFileEvent AE_READABLE clusterReadHandler
    clusterSendPing CLUSTERMSG_TYPE_MEET


命令以及作用：
1、连接节点
cluster meet <ip> <port> 

2、设置槽点
cluster addslots <slotNums>

3、查看所有节点的状态
cluster nodes

4、从source_id导入槽点
cluster setslot <slot_id> importing <source_id>

5、将节点迁移到target_id
cluster setslot <slot_id> migrating <target_id>

6、设置从节点
cluster replicate <node_id>

7、

错误
1
MOVED错误 转移到其他节点执行

2
ASK错误 正在迁移，暂时转移到其他节点执行（一次性的）

难点

复制
故障检测
故障转移
新主节点选举
raft算法的领头选举
*/