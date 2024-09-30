#ifndef __DEBUG_H_
#define __DEBUG_H_

#include<stdio.h>
#include <strings.h>

#define LOG_FILE	"./log.txt"

#define DEBUG //是否开启调试功能  调试的总开关
//#define DEBUG_TEXT //如果定义了该宏   将调试信息输出文本中  

extern int XPRINTF(char *text);

extern int initLog();

#endif