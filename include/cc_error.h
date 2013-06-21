#ifndef CC_ERROR_H
#define CC_ERROR_H 1

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>
#include <limits.h>
#include <float.h>
#include <inttypes.h>
#include <stdarg.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "openflow.h"


enum error_code{
	CC_E_ERR = -1,
    CC_E_NONE,
    CC_E_NOMEM,
    CC_E_TIMEOUT,
    CC_E_UNTIMEOUT,
    CC_E_INVAL,
    CC_E_EMPTY,
    CC_E_ERR_RECV,
};
typedef enum error_code error_code_e;

#define CC_PERROR(str) printf("func %s, line %d: %s\n", __func__, __LINE__, (str))



#define CC_ERROR_RETURN(ret)            \
    do{                                 \
           if((ret) == CC_E_ERR)   		\
                return CC_E_ERR;    	\
           else							\
            	return CC_E_NONE;		\
    }while(0)

#define CC_ERROR_NULL(ret) 				\
     do{                                \
           if((ret) == NULL)   		    \
                 return CC_E_ERR;       \
           else                         \
                 return CC_E_NONE;      \
       }while(0)


#define CC_NOT_ZERO_CHECK(a)            \
    do{                                 \
            if( (a) == 0 )              \
                return CC_E_NONE;       \
            else						\
                return CC_E_ERR;		\
    }while(0)


#define CC_MAX_VALUE_CHECK(num, max)   	\
    do{                                 \
        if( (num) >= (max) )            \
            return CC_E_ERR;            \
        else                            \
            return CC_E_NONE;           \
    }while(0)

#define CC_MIN_VALUE_CHECK(num, min)   	\
    do{                                 \
        if( (num) <= (min) )            \
           return CC_E_ERR;             \
        else                            \
           return CC_E_NONE;            \
     }while(0)

#define DEBUG_DISP(str)									\
                printf("%s, %s: %d\n", (str), __func__, __LINE__)
	


#endif

