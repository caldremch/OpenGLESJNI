//
// Created by Caldremch on 2018/1/26.
// 跟print用法一致
//
#include <android/log.h>
#ifndef JNIDEMO_MYLOG_H
#define JNIDEMO_MYLOG_H


#if _LP64 == 1
#define __NDK64BIT
#define INTPTR u_int64_t
#else
#define INTPTR u_int32_t
#endif


#ifndef CCORERELEASE
#define log_info(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "caldremch",fmt,__VA_ARGS__)
#define log_debug(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, "caldremch",fmt,__VA_ARGS__)
#define log_error(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "caldremch",fmt,__VA_ARGS__)


#else

#define log_info(fmt, ...) NULL
#define log_debug(fmt, ...) NULL
#define log_error(fmt, ...) NULL

#endif


#endif //JNIDEMO_MYLOG_H
