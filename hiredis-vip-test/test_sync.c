#include<stdio.h>
#include<hircluster.h>
#define LOG_DEBUG(FORMAT, ...) printf("[%s:%d] "FORMAT, __FUNCTION__, __LINE__, ##__VA_ARGS__)
int main()
{
    char *key="key-a";
    char *field="field-1";
    char *key1="key1";
    char *value1="value-1";
    char *key2="key1";
    char *value2="value-1";
    redisClusterContext *cc;

    cc = redisClusterContextInit();
    redisClusterSetOptionAddNodes(cc, "127.0.0.1:7000,127.0.0.1:7001,127.0.0.1:7002,192.168.188.132:7003");
    redisClusterConnect2(cc);
    if(cc == NULL || cc->err)
    {
        LOG_DEBUG("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
        return -1;
    }

    redisReply *reply = redisClusterCommand(cc, "hmget %s %s", key, field);
    if(reply == NULL)
    {
        LOG_DEBUG("reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    LOG_DEBUG("reply->type:%d", reply->type);

    freeReplyObject(reply);

    reply = redisClusterCommand(cc, "mset %s %s %s %s", key1, value1, key2, value2);
    if(reply == NULL)
    {
        LOG_DEBUG("reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    LOG_DEBUG("reply->str:%s", reply->str);

    freeReplyObject(reply);
    redisClusterFree(cc);
    return 0;
}
