#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#define UNUSED __attribute__ ((unused))

#ifdef DEBUG

#define ASSERT(condition,msg) \
	do{\
		if(!condition){\
			fprintf(stderr,"ASSERT Failed!File \"%s\",line %d ,in function %s:%s",\
				__FILE__,__LINE__,__func__,msg);\
			abort();\
		}\
	}while(false)

#else

#define ASSERT(condition,msg) (0)

#endif // DEBUG

#define NOTREACHED()\
	do{\
		fprintf(stderr,"NOT Reached!%s:%d in function %s",\
			__FILE__,__LINE__,__func__);\
		while(true);\
	}while(false)