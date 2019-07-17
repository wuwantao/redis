#include<stdio.h>  
#include <time.h> 
#include<hircluster.h>

int main()  
{  
	redisClusterContext *cc = redisClusterConnect("192.168.188.131:7000,192.168.188.131:7001,192.168.188.131:7002,192.168.188.132:7003,192.168.188.132:7004,192.168.188.132:7005",HIRCLUSTER_FLAG_ROUTE_USE_SLOTS );  
	if(cc == NULL || cc->err)  
	{  
		printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);  
		return -1;  
	}  

	long i = 0;  
	long j = 0;
	redisReply* reply = NULL;  
	clock_t start, finish;
	double duration; 
	start = clock(); 
	int count = 10000 * 2;

	for(i = 0; i<count; i++)
	{
		for(j = 0;j < 50; j++)
		{
			//redisClusterAppendCommand(cc, "set key%d%d value%d%d",i,j);
			redisClusterAppendCommand(cc, "get key%d%d",i,j);
		}
		for(j = 0;j < 50; j++)
		{
			redisClusterGetReply(cc, (void **)&reply);
			freeReplyObject(reply);;
		}
		redisClusterReset(cc);
	}

	finish = clock();  
	duration =count * 50 /((double)(finish - start) / CLOCKS_PER_SEC);  
	printf( "%f seconds\n", duration );  
	redisClusterFree(cc);  
	return 0;  

}  



