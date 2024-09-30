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

// 声音文件所在文件夹
#define VOICE_PATH "./res"

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


#ifndef __CHESS_H__
#define __CHESS_H__
// 单个空白格子的高度
#define CHESS_WIDTH 50
// 线宽 真正的线宽要乘以2
#define LINE_WIDTH 1
// 单个棋子大小 半径
#define CHESS_MAN 23

/* 函数定义 */
// 一起来快乐的下象棋吧!
int runChess(void);
// 初始化棋盘
void initChessBlock(char *mp);
// 画直线
void drawLine(char *mp, int x1, int y1, int x2, int y2, int color);
// 画斜线
void drawSlash(char *mp, int x1, int y1, int x2, int y2, int color);
// 画圆圈
void drawCircle(char *mp, int x, int y, int r1, int r2, int color);
// 画标记点
void drawMark(char *mp, int x, int y, int i);
// 根据某个点的坐标画出线条和棋子
void drawBlock(char *mp, int i, int j);
// 打印文字在屏幕上,字体的起始位地址,中心点  1右边 2上边  3左边  4下边
void drawFont(char *mp, int k, int x, int y, char direction, int color);
// 棋盘规则
int chessRule(int i1, int j1, int i2, int j2);
//检测是否将军
int isAdmiral(void);
// 多线程点击
void *ptTouch(void *arg);
// 多线程播放音乐
void *ptPlayMusic(void *arg);

#endif