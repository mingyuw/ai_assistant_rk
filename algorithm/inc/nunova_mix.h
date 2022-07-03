/*
* Copyright (c) 2022 NUNOVA All Rights Reserved. 
*
* NOTICE: All information contained herein is, and remains the property of 
* NUNOVA, if any. The intellectual and technical concepts 
* contained herein are proprietary to NUNOVA and may be 
* covered by U.S. and Foreign Patents, patents in process, and are protecte
* by trade secret or copyright law. Dissemination of this information or 
* reproduction of this material is strictly forbidden unless prior written 
* permission is obtained from NUNOVA 
*/


#ifndef __NUNOVA_MIX_H__
#define __NUNOVA_MIX_H__

#include <stdio.h> 
#include <stdarg.h>
#include <stdlib.h>//itoa()......
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <fstream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>


#define COLOR_GRAY "\033[37m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_DARKGRAY "\033[30m"
#define COLOR_BLACK "\033[30m"
#define COLOR_NOCOLOR "\033[0m"
#define COLOR_DBLUE "\033[34m"
#define COLOR_RED "\033[31m"

#define COLOR_NORMAL1 "\033[0m"

#define NUNOVA_LOG_DBG(format,...) printf(COLOR_DBLUE"[NUNOVA DEBUG]: " format COLOR_NOCOLOR, ##__VA_ARGS__)
#define NUNOVA_LOG_INFO(format,...) printf(COLOR_GREEN"[NUNOVA INFO]: " format COLOR_NOCOLOR, ##__VA_ARGS__)
#define NUNOVA_LOG_WARN(format,...) printf(COLOR_YELLOW"[NUNOVA WARN]: " format COLOR_NOCOLOR, ##__VA_ARGS__)
#define NUNOVA_LOG_ERROR(format,...) printf(COLOR_RED"[NUNOVA ERROR]: " format COLOR_NOCOLOR, ##__VA_ARGS__)

#define MY_PRINTF(format...)   \
do{\
    printf(format);  \
}while(0);

#define MY_PRINTF1(...)   \
do{\
    printf(COLOR_RED __VA_ARGS__);  \
}while(0);

#define MY_PRINTF2(format, ...)   \
do{\
    printf(format,##__VA_ARGS__);  \
}while(0);

#define DBG_Sys_Out(level,format...)   \
do{\
    syslog(level,##format);  \
}while(0);

typedef enum
{
    LL_NULL = 0,
    LL_ERR = 1,
    LL_WARN = 2,
    LL_INFO = 3,
    LL_DBG = 4

} eLogLevel;

#define		MAXBUF		15 
#define		DEFAULT_PRECI		3	//符点型默认精度

void WriteCSV(std::string filename, cv::Mat m);
void PrintNORMAL(const char *format, ...) __attribute__((format(printf,1,2)));
void PrintDBG(const char *format, ...) __attribute__((format(printf,1,2)));
void PrintINFO(const char *format, ...) __attribute__((format(printf,1,2)));
void PrintWARN(const char *format, ...) __attribute__((format(printf,1,2)));
void PrintERR(const char *format, ...) __attribute__((format(printf,1,2)));
int GetIsSave();


#endif