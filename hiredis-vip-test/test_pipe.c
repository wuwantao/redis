#include<stdio.h>
#include<hircluster.h>
int main()
{
	redisReply *reply;

	redisClusterContext *cc = redisClusterConnect("192.168.188.131:7000,192.168.188.131:7001,192.168.188.131:7002,192.168.188.132:7003,192.168.188.132:7004,192.168.188.132:7005",HIRCLUSTER_FLAG_ROUTE_USE_SLOTS );  
	if(cc == NULL || cc->err)
	{
		printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
		return -1;
	}

	redisClusterAppendCommand(cc, "set key1 value1");
	redisClusterAppendCommand(cc, "set key2 value2");
	redisClusterAppendCommand(cc, "set key3 value3");

	redisClusterGetReply(cc, (void **)&reply);  //for "set key1 value1"
	freeReplyObject(reply);
	redisClusterGetReply(cc, (void **)&reply);  //for "get key1"
	freeReplyObject(reply);
	redisClusterGetReply(cc, (void **)&reply);  //for "mset key2 value2 key3 value3"
	freeReplyObject(reply);

	redisClusterReset(cc);


	redisClusterFree(cc);
	return 0;
}
