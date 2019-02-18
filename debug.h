// Created by Tau on 2019/2/7.

#ifndef HG_DEBUG_H
#define HG_DEBUG_H
#include <stdio.h>

#include <stdlib.h>
//#define LOG_ON
#ifdef LOG_ON
#define log(x) printf("%s: ",__FUNCTION__),(printf x ),printf("\n")
#else
#define log(x)
#endif

#define DEBUG_ON
#ifdef DEBUG_ON
#define panic(x) printf("%s: ",__FUNCTION__),printf x,printf("\n"),exit(2)
#else
#define panic(x)
#endif


#endif //HG_DEBUG_H
