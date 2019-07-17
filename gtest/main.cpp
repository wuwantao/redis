#include "main.h"

//regester test case
#define MY_TEST(_CLASS_, _FUNC_) \
	TEST_F(_CLASS_,	_CLASS_##_##_FUNC_) \
	{\
		EXPECT_EQ(true, _FUNC_()); \
	}


MY_TEST(TestRedis, TestConsistency)
MY_TEST(TestRedis, TestCapability)
MY_TEST(TestRedis, TestAvailability)



int main(int argc, char* argv[])
{
	LOG_DEBUG(CL_LOG_TYPE_NORMAL, "start record log !");
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}
