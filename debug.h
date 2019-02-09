// Created by Tau on 2019/2/7.

#ifndef HG_DEBUG_H
#define HG_DEBUG_H

#include <stdlib.h>
#define LOG_ON
#ifdef LOG_ON
#include <stdio.h>
#define log(x, y) printf("%s: ",__FUNCTION__),printf(x, y),printf("\n")
#else
#define log(x, y)
#endif
#define DEBUG_ON
#ifdef DEBUG_ON
#define panic(x, y) printf("%s: ",__FUNCTION__),printf(x, y),printf("\n"),exit(2)
#else

#endif


#endif //HG_DEBUG_H
