#ifndef main_H_
#define main_H_

//c library 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <hircluster.h>


//c++ lirary
#include <string>

//third library
#include <gtest/gtest.h>

#include "CL_Config.h"
#include "TestRedis.h"


#define LOG_DEBUG(TYPE, FORMAT, ...) printf("[%s:%d] "FORMAT, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_DEBUG_RATELIMIT(TYPE, FORMAT, ...) printf("[%s:%d] "FORMAT, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif

