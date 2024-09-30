#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h> //线程头文件

#ifndef __DEV_PATH__
#define __DEV_PATH__
/* 宏设备 */
// 显示屏设备
#define LCD_PATH "/dev/fb0"
// 触摸屏设备
#define TOUCH_PATH "/dev/input/event0"
#endif
/* 宏函数 */

/* 宏数据 */

#ifndef __DEV_INFO__
#define __DEV_INFO__
// 显示屏宽度
#define WIDTH_LCD 800
// 显示屏高度
#define HEIGHT_LCD 480
// 触摸屏宽度
#define WIDTH_TOUCH 1024
// 触摸屏高度
#define HEIGHT_TOUCH 600

#endif

#ifndef __PLOT_H__
#define __PLOT_H__

// 立方体的边长
#define SIDE_LENGTH 100
// 立方体的线粗
#define THICKNESS 2

#define EPS 1e-7 // 判断浮点数是否位于0的一个很小的邻域内[-EPS,EPS]内
typedef struct stPoint stPoint;
// 根据xyz旋转角度返回旋转后的点
stPoint xyzRang(stPoint point, stPoint *rel);
// 画出图像
void drowCube(char *mp);

// 将3维坐标映射到二维上
stPoint xyzPoint(stPoint a, stPoint *b);
// 根据屏幕上的两个点连接线条
void connectLines(char *mp, int x1, int y1, int x2, int y2, int color);
// 根据屏幕上的一个点画周围的点
void drowLinesPoint(char *mp, int x, int y, int n, int color);
// 多线程点击
void *ptTouchPlot(void *arg);
//运行
int runPlot();
#endif  