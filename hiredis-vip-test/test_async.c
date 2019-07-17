#include<stdio.h>
#include<hiredis-vip/hircluster.h>
#include<hiredis-vip/adapters/libevent.h>
#include<time.h>
#include<pthread.h>

 
#define DEBUG(FORMAT, ...) printf("[%s:%d] "FORMAT, __FUNCTION__, __LINE__, ##__VA_ARGS__)
int all_count=0;
typedef struct CmdDetail {
	char cmd[200];
	int i;
} CmdDetail_t;

typedef struct calldata
{
	redisClusterAsyncContext *acc;
	int count;
}calldata;

void getCallback(redisClusterAsyncContext *acc, void *r, void *privdata)
{
	redisReply *reply = r;
	CmdDetail_t* tmp =  (CmdDetail_t*)privdata;

	if(reply == NULL)  
	{  
		DEBUG("reply is NULL");  
		return;  
	}  
	DEBUG("tmp->i:%d\t cmd:%s\n", tmp->i, tmp->cmd);  
	free(tmp);
}

void connectCallback(const redisAsyncContext *c, int status)
{
	if (status != REDIS_OK) {
		DEBUG("Error: %s\n", c->errstr);
		return;
	}
	DEBUG("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status)
{
	if (status != REDIS_OK) {
		DEBUG("Error: %s\n", c->errstr);
		return;
	}
	
	DEBUG("\nDisconnected...\n");
}

void *thr_fn(void *arg)
{
    event_base_dispatch((struct event_base *)arg);
    return NULL;
}

int main(int argc, char **argv)
{
	int status, i;
	struct event_base *base = event_base_new();
	redisClusterAsyncContext *acc = redisClusterAsyncConnect("127.0.0.1:7000,127.0.0.1:7001,127.0.0.1:7002", 
		HIRCLUSTER_FLAG_ROUTE_USE_SLOTS);
	if (acc->err)
	{
		DEBUG("Error: %s\n", acc->errstr);
		return 1;
	}
	redisClusterLibeventAttach(acc,base);
	redisClusterAsyncSetConnectCallback(acc,connectCallback);
	redisClusterAsyncSetDisconnectCallback(acc,disconnectCallback);
	
	clock_t start, finish;
	double duration; 
	start = clock(); 
	
	for(i = 0; i < 10; i++)
	{
		CmdDetail_t *tmp = malloc(sizeof(CmdDetail_t));
		tmp->i = i;
		strcpy(tmp->cmd,"set");
		status = redisClusterAsyncCommand(acc, getCallback, tmp, "set xxx%d", i);
		if(status != REDIS_OK)
		{
			DEBUG("error: %d %s\n", acc->err, acc->errstr);
		}
	}
	pthread_t ntid;
	int err = 0;
	err = pthread_create(&ntid, NULL, thr_fn, base);
	if (err != 0)
		printf("can't create thread: %s\n", strerror(err));
	
	sleep(5);
	DEBUG("begin add\n");
	for(i = 20; i < 30; i ++)
	{
		//DEBUG("add command\n");
		CmdDetail_t *tmp = malloc(sizeof(CmdDetail_t));
		tmp->i = i;
		strcpy(tmp->cmd,"get");
		status = redisClusterAsyncCommand(acc, getCallback, tmp, "get xxx%d", i);
		if(status != REDIS_OK)
		{
			DEBUG("error: %d %s\n", acc->err, acc->errstr);
		}
	}

	DEBUG("add complete\n");
	system("read");
	pthread_join(ntid,NULL);
	/*
	finish = clock();  
	duration =count /((double)(finish - start) / CLOCKS_PER_SEC);  
	printf( "%f seconds\n", duration );  
	*/
	return 0;
}
