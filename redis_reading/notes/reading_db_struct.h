/*
redis数据库结构

struct redisServer {
    redisDb *db; //数据库数组
    int dbnum;

    // RDB用于保存RDB执行条件
    struct saveparam *saveparams; 
    int saveparamslen;
};

struct redisDb {
    dict *dict; //保存着所有数据库的键
    dict *expires; //键时限 
};

struct redisClient {
    redisDb *db; //正在操作的数据库的指针
}

struct saveparam {
    time_t seconds;
    int changes;
};
*/