#include "main.h"

#include <boost/thread.hpp>

static void YieldChar(char* vBuf, const int vLen)
{
	srand((unsigned)time( NULL ));
	for (int tI = 0; tI < vLen; tI++)
	{
		vBuf[tI] = rand()%78 + '0';
	}
	vBuf[vLen - 1] = '\0';
}

TestRedis::TestRedis()
{
}

TestRedis::~TestRedis()
{
}

void TestRedis::SetUp()
{
	if(false == m_Config.Load("./config.xml"))
	{
		LOG_DEBUG(CL_LOG_TYPE_ERROR, "Load config.xml failed, tConfigPath=%s !","./config.xml");
		FAIL();
		return;
	}

	m_Config.Get("Redis.Consistency.Servers", &m_Consistency_Servers);
	m_Config.Get("Redis.Consistency.WriteCount", &m_Consistency_WriteCount);

	m_Config.Get("Redis.Capability.Servers", &m_Capability_Servers);
	m_Config.Get("Redis.Capability.WriteCount", &m_Capability_WriteCount);

	m_Config.Get("Redis.Availability.Servers", &m_Availability_Servers);
	m_Config.Get("Redis.Availability.WriteCount", &m_Availability_WriteCount);
	
	if (m_Consistency_Servers.length() == 0 || m_Consistency_WriteCount <= 0)
	{
		LOG_DEBUG(CL_LOG_TYPE_ERROR, "m_Servers|m_WriteCount must great than 0!");
		FAIL();
		return;
	}
}

bool TestRedis::TestConsistency()
{
    long i;  
    redisReply* reply = NULL;  
	char buff[1024];

	redisClusterContext *cc = redisClusterConnect(m_Consistency_Servers.c_str(), HIRCLUSTER_FLAG_ROUTE_USE_SLOTS);  
    if(cc == NULL || cc->err)  
    {  
        LOG_DEBUG(CL_LOG_TYPE_ERROR, "connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);  
        return false;  
    }  
	
	for(i=0; i<m_Consistency_WriteCount; i++)	
	{  
		//set  
		reply = (redisReply*)redisClusterCommand(cc, "set key%d {2netid2:25af3b61f27d7c56702cce2742,2rangid2:25af3b67d27d7c56702cce2af2,2fp_id2:20000000000000000000000002,lease_time:1800,start:1526611062,end:1526611062,domain:22,uid:2xx2,host:2xx2,mac:200:00:01:ad:cf:02,status:262,ip_type:222,mask:8,ip_ver:242,local:1}%d", i, i);  
		if(reply == NULL)  
		{  
			LOG_DEBUG(CL_LOG_TYPE_ERROR, "set key%d, reply is NULL, error info: %s\n", i, cc->errstr);  
			redisClusterFree(cc);  
			return false;	
		}  
		if(i % 10000 == 0)
		{
			printf("Consistency set i:%d\n",i);
		} 
		freeReplyObject(reply);    
	}  
	int same = 1;
	for(i=0; i<m_Consistency_WriteCount; i++)	
	{  
		//get  
		reply = (redisReply*)redisClusterCommand(cc, "get key%d", i);  
		if(reply == NULL)  
		{  
			LOG_DEBUG(CL_LOG_TYPE_ERROR, "get key%d, reply is NULL, error info: %s\n", i, cc->errstr);	
			redisClusterFree(cc);  
			return false;  
		}
		//memset(buff, 0, sizeof(buff));
		sprintf(buff, "{2netid2:25af3b61f27d7c56702cce2742,2rangid2:25af3b67d27d7c56702cce2af2,2fp_id2:20000000000000000000000002,lease_time:1800,start:1526611062,end:1526611062,domain:22,uid:2xx2,host:2xx2,mac:200:00:01:ad:cf:02,status:262,ip_type:222,mask:8,ip_ver:242,local:1}%d",i);
		if(strcmp(buff,reply->str) != 0)
		{
			same = -1;
		}
		if(i % 10000 == 0)
		{
			printf("Consistency get i:%d\n",i);
		}
		freeReplyObject(reply); 
	}  

	return same == -1 ? false : true;
}

bool TestRedis::TestCapability()
{
	clock_t start, finish;
	double duration; 
	int WriteCount = 0;
	int i = 0;
	redisClusterContext *cc = redisClusterConnect(m_Capability_Servers.c_str(), HIRCLUSTER_FLAG_ROUTE_USE_SLOTS);  
    if(cc == NULL || cc->err)  
    {  
        LOG_DEBUG(CL_LOG_TYPE_ERROR, "connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);  
        return false;  
    }  
	
	redisReply* reply = NULL;  
    
  	start = clock(); 
	for(i=0; i<m_Capability_WriteCount; i++)  
	{  
		//set  
		reply = (redisReply*)redisClusterCommand(cc, "set key%d%d {2netid2:25af3b61f27d7c56702cce2742,2rangid2:25af3b67d27d7c56702cce2af2,2fp_id2:20000000000000000000000002,lease_time:1800,start:1526611062,end:1526611062,domain:22,uid:2xx2,host:2xx2,mac:200:00:01:ad:cf:02,status:262,ip_type:222,mask:8,ip_ver:242,local:1}%d", i, i);  
		if(reply == NULL)  
		{  
			LOG_DEBUG(CL_LOG_TYPE_ERROR, "set key%d, reply is NULL, error info: %s\n", i, cc->errstr);  
			redisClusterFree(cc);  
			return false;  
		}  
		freeReplyObject(reply);  
		/*

		//get  
		reply = (redisReply*)redisClusterCommand(cc, "get key%d%d", i);  
		if(reply == NULL)  
		{  
			LOG_DEBUG(CL_LOG_TYPE_ERROR, "get key%d, reply is NULL, error info: %s\n", i, cc->errstr);  
			redisClusterFree(cc);  
			return false;  
		}
		
		if(i % 100000 == 0)
		{
			printf("Capability i:%d\n",i);
		}
		
		freeReplyObject(reply); 
		*/
		
	}  
	finish = clock();  
	duration = m_Capability_WriteCount /((double)(finish - start) / CLOCKS_PER_SEC);  
	printf( "Capability %f lps\n", duration);  
    redisClusterFree(cc); 

	return true;
}

bool TestRedis::TestAvailability()
{
	long i;  
    redisReply* reply = NULL;  
	char buff[1024];

	redisClusterContext *cc = redisClusterConnect(m_Availability_Servers.c_str(), HIRCLUSTER_FLAG_ROUTE_USE_SLOTS);  
    if(cc == NULL || cc->err)  
    {  
        LOG_DEBUG(CL_LOG_TYPE_ERROR, "connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);  
        return false;  
    }  

	for(i=0; i<m_Availability_WriteCount; i++)  
	{  
		//set  
		reply = (redisReply*)redisClusterCommand(cc, "set key%d%d {2netid2:25af3b61f27d7c56702cce2742,2rangid2:25af3b67d27d7c56702cce2af2,2fp_id2:20000000000000000000000002,lease_time:1800,start:1526611062,end:1526611062,domain:22,uid:2xx2,host:2xx2,mac:200:00:01:ad:cf:02,status:262,ip_type:222,mask:8,ip_ver:242,local:1}%d", i, i);  
		if(reply == NULL)  
		{  
			LOG_DEBUG(CL_LOG_TYPE_ERROR, "set key%d, reply is NULL, error info: %s\n", i, cc->errstr);  
			redisClusterFree(cc);  
			return false;  
		}  
		freeReplyObject(reply);  

		//get  
		reply = (redisReply*)redisClusterCommand(cc, "get key%d%d", i);  
		if(reply == NULL)  
		{  
			LOG_DEBUG(CL_LOG_TYPE_ERROR, "get key%d, reply is NULL, error info: %s\n", i, cc->errstr);  
			redisClusterFree(cc);  
			return false;  
		}
		if(i % 10000 == 0)
		{
			printf("Availability i:%d\n",i);
		}
		freeReplyObject(reply); 
		
	}

	return true;
}


