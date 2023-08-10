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
#include "plot.h"

#ifndef __PLOT_C__
#define __PLOT_C__

// 定义坐标点结构体
typedef struct stPoint
{
    double x;
    double y;
    double z;
} stPoint;

// 立方体的8个点 (0 1 2 3 )(4 5 6 7 )顺序连接  0 4,1 5,2 6,3 7相连
stPoint cube[8] = {
    {SIDE_LENGTH / 2, SIDE_LENGTH / 2, SIDE_LENGTH / 2},
    {SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2, SIDE_LENGTH / 2},
    {SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2},
    {SIDE_LENGTH / 2, SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2},
    {-1 * SIDE_LENGTH / 2, SIDE_LENGTH / 2, SIDE_LENGTH / 2},
    {-1 * SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2, SIDE_LENGTH / 2},
    {-1 * SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2},
    {-1 * SIDE_LENGTH / 2, SIDE_LENGTH / 2, -1 * SIDE_LENGTH / 2}};

// 分别对应转动x轴,y轴  z轴的角度
double angle[3] = {0}; //
//退出循环
int re_black=0;
// 颜色
unsigned int colorPlot[250] = {
    0xFF131313, // 0 黑色背景颜色
    0xFFA61808  // 4 红 红方颜色
};
#endif
//运行
int runPlot()
{
// 打开读写LCD屏幕
    int lcd_fd = open(LCD_PATH, O_RDWR);
    if (-1 == lcd_fd)
    {
        perror("lcd_fd文件打开失败");
        return -1;
    }
    // 触摸屏
    int touch_fd = open(TOUCH_PATH, O_RDONLY);
    if (-1 == touch_fd)
    {
        perror("touch_fd文件打开失败");
        return -1;
    }
    // 显示屏映射
    char *mp = (char *)mmap(0, WIDTH_LCD * HEIGHT_LCD * 4, PROT_WRITE | PROT_READ, MAP_SHARED, lcd_fd, 0);
    if (MAP_FAILED == mp)
    {
        perror("屏幕设备mp映射失败");
        return -2;
    }
    for (int i = 0; i < HEIGHT_LCD; i++)
    {
        for (int j = 0; j < WIDTH_LCD; j++)
        {
            memcpy(mp + (i * WIDTH_LCD + j) * 4, &colorPlot[0], 4);
        }
    }
    for (int i = 0; i < 245; i++)
    {
        colorPlot[i] = 0xFF131313;
    }
    colorPlot[244] = 0xFFA61808;
    double cache_angle[3] = {0};
    // 点击事件的进程id
    pthread_t tid_touch;
    pthread_create(&tid_touch, NULL, ptTouchPlot, &touch_fd);
    while (1)
    {
        if (fabs(angle[0] - cache_angle[0]) > EPS || fabs(angle[1] - cache_angle[1]) > EPS || fabs(angle[2] - cache_angle[2]) > EPS)
        {
            drowCube(mp);
            cache_angle[0] = angle[0];
            cache_angle[1] = angle[1];
            cache_angle[2] = angle[2];
        }
        else
        {
            angle[0] += 0.05;
            angle[1] += 0.03 ;
            angle[2] += 0.01 ;
            usleep(10000);
        }
        if(0!=re_black)
        {
            re_black=0;
            break;
        }
    }

    pthread_cancel(tid_touch);
    close(touch_fd);
    munmap(mp, WIDTH_LCD * HEIGHT_LCD * 4);
    close(lcd_fd);
}
// 多线程点击
void *ptTouchPlot(void *arg)
{

    // 触摸屏每滑动375像素就转动90度  每25像素转6度
    double angle1 = 0.01745;
    int x = 0, y = 0, cache_x = 0, cache_y = 0,flage=1;
    int touch_fd = *((int *)arg);
    struct input_event ie;
    while (1)
    {
        read(touch_fd, &ie, sizeof(ie));
        if (EV_KEY == ie.type && BTN_TOUCH == ie.code && 1 == ie.value)
        {
            // printf("手指按下!\n");
            flage=0;
        }
        else if (EV_KEY == ie.type && BTN_TOUCH == ie.code && 0 == ie.value)
        {
            // printf("手指松开!\n");
            if(0==flage)
            {
                re_black++;
            }
        }
        if (EV_ABS == ie.type)
        {
            flage++;
            if (ABS_X == ie.code)
            {
                x = ie.value;
            }
            if (ABS_Y == ie.code)
            {
                y = ie.value;
            }
            // 左右滑动绕到y轴
            if (cache_x != x)
            {
                angle1=fabs(x-cache_x)/300*90*0.01745;
                // 求解到了弧度值
                // 向右滑动
                if (x > cache_x)
                {
                    angle[1] += angle1;
                }
                // 向左滑动
                else if (x < cache_x)
                {
                    angle[1] -= angle1;
                }
                cache_x = x;
            }
            // 上下滑动绕到x轴旋转
            if (cache_y != y)
            {
                // 求解到了旋转的弧度值
                // 绕到x轴的旋转
                
                angle1=fabs(y-cache_y)/300*90*0.01745;
                // 向下滑动
                if (y > cache_y)
                {
                    angle[0] += angle1;
                }
                // 向上滑动
                else if (y < cache_y)
                {
                    angle[0] -= angle1;
                }
                cache_y = y;
            }
        }
    }
}

// 根据xyz旋转角度返回旋转后的点
stPoint xyzRang(stPoint point, stPoint *rel)
{
    stPoint p = point;
    // 旋转x
    p.x = point.x;
    p.y = point.y * cos(angle[0]) - point.z * sin(angle[0]);
    p.z = point.y * sin(angle[0]) + point.z * cos(angle[0]);

    // 旋转y
    point.x = p.x * cos(angle[1]) + p.z * sin(angle[1]);
    point.y = p.y;
    point.z = -1 * p.x * sin(angle[1]) + p.z * cos(angle[1]);

    // 旋转z
    rel->x = point.x * cos(angle[2]) - point.y * sin(angle[2]);
    rel->y = point.x * sin(angle[2]) + point.y * cos(angle[2]);
    rel->z = point.z;
}

// 画出图像
void drowCube(char *mp)
{

    // 计算变换之后的坐标
    stPoint po[8];
    for (int i = 0; i < 8; i++)
    {
        xyzRang(cube[i], &po[i]);
        // 将3维坐标映射到二维上
        xyzPoint(po[i], &po[i]);
    }
    // 先覆盖原来的图像
    for (int i = (HEIGHT_LCD / 2) - SIDE_LENGTH; i < (HEIGHT_LCD / 2) + SIDE_LENGTH; i++)
        for (int j = (WIDTH_LCD / 2) - SIDE_LENGTH; j < (WIDTH_LCD / 2) + SIDE_LENGTH; j+=10)
        {
            memcpy(mp + (i * WIDTH_LCD + j) * 4, colorPlot, 4*10);
        }
    // 先覆盖原来的图像
    // for (int i = (HEIGHT_LCD / 2) - SIDE_LENGTH / 2; i < (HEIGHT_LCD / 2) + SIDE_LENGTH / 2; i++)
    //     memcpy(mp + (i * WIDTH_LCD + (WIDTH_LCD / 2) - SIDE_LENGTH / 2) * 4, color, 4 * SIDE_LENGTH);

    // 再画线
    for (int i = 0; i < 4; i++)
    {
        connectLines(mp, po[i % 4].x, po[i % 4].y, po[(i + 1) % 4].x, po[(i + 1) % 4].y, colorPlot[244]);
        connectLines(mp, po[i + 4].x, po[i + 4].y, po[(i + 5) % 8 + (3 == i ? 4 : 0)].x, po[(i + 5) % 8 + (3 == i ? 4 : 0)].y, colorPlot[244]);
        connectLines(mp, po[i % 4].x, po[i % 4].y, po[i + 4].x, po[i + 4].y, colorPlot[244]);
    }
}
/*
平面 y=SIDE_LENGTH*3
点(0,SIDE_LENGTH*5,0) 点(x1,y1,z1)
x-x1/-x1 = y-y1/(SIDE_LENGTH*5-y1) = z-z1/-z1 =t

x=t*-x1+x1
y=t*(SIDE_LENGTH*5-y1)+y1
z=t*-z1+z1

带入y=SIDE_LENGTH*3
得到t = (SIDE_LENGTH*3-y1)/(SIDE_LENGTH*5-y1)
x=t*-x1+x1
 */

// 将3维坐标映射到二维上
stPoint xyzPoint(stPoint a, stPoint *b)
{
    double t = (SIDE_LENGTH * 3 - a.y) / (SIDE_LENGTH * 5 - a.y);
    b->x = t*-1+a.x ;
    b->y = t*-1+a.z ;
    b->x = WIDTH_LCD / 2 + b->x;
    b->y = HEIGHT_LCD / 2 + b->y;

    // a.x,a.z

    // double t=(SIDE_LENGTH * 5-a.y)/(SIDE_LENGTH * 3);
    // b->x = a.x*t;
    // b->y =a.z*t;
    // b->x = WIDTH_LCD / 2 + (1.0 - t) * a.x;
    // b->y = HEIGHT_LCD / 2 + (1.0 - t) * t * a.z;
}

// 根据屏幕上的两个点连接线条
void connectLines(char *mp, int x1, int y1, int x2, int y2, int color)
{

    // 定义斜率K
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps;                     // 步数
    int direction;                 // 方向 0--x方向  1--y方向
    double xIncrement, yIncrement; // 每一步增加的数值
    double x = x1, y = y1;         // 求出的中间像素点
    if (abs(dx) > abs(dy))
    {
        steps = abs(dx); // 步数
        direction = 0;   // 确认X步进主方向
    }
    else
    {
        steps = abs(dy);
        direction = 1; // 确认Y步进主方向
    }
    xIncrement = ((double)dx) / ((double)steps); // x每步骤增量
    yIncrement = ((double)dy) / ((double)steps); // y的每步增量
    for (int k = 0; k <= steps; ++k)
    {
        // x主步进方向
        if (direction == 0)
        {
            drowLinesPoint(mp, x, y + 0.5, THICKNESS, color);
        }
        else // y主步进方向
        {
            drowLinesPoint(mp, x + 0.5, y, THICKNESS, color);
        }
        x += xIncrement; // x点+增量
        y += yIncrement; // y点+增量
    }
}
// 根据屏幕上的一个点画周围的点
void drowLinesPoint(char *mp, int x, int y, int n, int color)
{
    for (int j = y - n; j < y + n; j++)
        for (int i = x - n; i < x + n; i++)
            memcpy(mp + 4 * (j * WIDTH_LCD + i), &color, sizeof(color));
}