#include "debug.h"

//初始化调试文本
int initLog()
{
	//1、打开文本 并且 清空内容 ，如果不存在，新建
	FILE * fp = fopen(LOG_FILE,"wb+");
	if(fp == NULL)
	{
		printf("fopen %s error\n",LOG_FILE);
		return -1;
	}
	//3、关闭文件
	fclose(fp);
}

#ifdef DEBUG //开启调试功能

#ifndef DEBUG_TEXT
int XPRINTF(char *text) //1、调试功能 定位到屏幕终端
{
	printf("%s",text);
}
#else 
	
int XPRINTF(char *text)//2、调试功能 定位到文本中
{
	
	//1、以可写的方式打开log.txt,如果文件不存在则新建，如果存在了追加
	FILE * fp = fopen(LOG_FILE,"ab");
	if(fp == NULL)
	{
		printf("fopen %s error\n",LOG_FILE);
		return -1;
	}
	//2、将传递进来的信息写入到调试文本中 fprintf
	fprintf(fp,"%s",text);
	//3、关闭文件
	fclose(fp);
}
#endif

#else 
int XPRINTF(char *text)
{
	
}
#endif

