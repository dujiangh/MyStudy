#ifndef __TS_H_
#define __TS_H_

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <stdbool.h>
#include <linux/input.h>//触摸屏头文件

enum Direction{
	Left,
	Right,
	Up,
	Down,
	Click,//单击
};

extern int ts_func(int *x,int *y);

#endif 

