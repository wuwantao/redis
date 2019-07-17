#ifndef TestRedis_H_
#define TestRedis_H_

class TestRedis : public testing::Test
{
protected:
	virtual void SetUp();

private:
	CL_Config m_Config;
	
	std::string m_Consistency_Servers;
	int m_Consistency_WriteCount;
	
	std::string m_Availability_Servers;
	int m_Availability_WriteCount;
	
	std::string m_Capability_Servers;
	int m_Capability_WriteCount;
public:
	TestRedis(void);
	virtual ~TestRedis();

	bool TestConsistency();
	bool TestAvailability();
	bool TestCapability();
};

#endif

