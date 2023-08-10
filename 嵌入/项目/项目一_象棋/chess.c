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
#include "chess.h"

#ifndef __CHESS_c__
#define __CHESS_c__
/* 全局变量 */

// 画出棋盘的线条 还有棋盘上的点
int chessBlock[9][10][2];
/*
    0x 0000 0000 0000 0001 右      1
    0x 0000 0000 0000 0010 上      2
    0x 0000 0000 0000 0100 左      4
    0x 0000 0000 0000 1000 下      8
    0x 0000 0000 0001 0000 右上      16
    0x 0000 0000 0010 0000 左上        32
    0x 0000 0000 0100 0000 左下        64
    0x 0000 0000 1000 0000 右下        128
    0x 0000 0001 0000 0000 标记点      256

    0x 0000 0000 0000 0001 红      1    上面
    0x 0000 0000 0000 0010 黑      2    下面
    0x 0000 0000 0000 0100 将      4
    0x 0000 0000 0000 1000 士      8
    0x 0000 0000 0001 0000 象      16
    0x 0000 0000 0010 0000 马        32
    0x 0000 0000 0100 0000 车        64
    0x 0000 0000 1000 0000 炮        128
    0x 0000 0001 0000 0000 兵      256

*/
// 颜色
const unsigned int colorChess[] = {
    0xFF342411, // 0 背景颜色
    0xFFCFB793, // 1 木 棋盘背景色
    0xFF8A6C47, // 2 黑 棋盘线条颜色
    0xFFFFC55A, // 3 棋子背景颜色
    0xFFA61808, // 4 红 红方颜色
    0xFF110D0A, // 5 绿 绿方颜色
    0xFFA61808, // 红 棋子选中颜色
    0xFFA61808};
// 声音定义
int voice = 0;
/*
0000 0001    001 获胜的声音
0000 0010    002 男 将军
0000 0100    003 女 将军
0000 1000    004 吃棋
0001 0000    005 走棋
0010 0000    006 走棋过不去
 */

// 汉字定义
const char FONT_CHECK[][5] =
    {
        "将",
        "士",
        "象",
        "马",
        "车",
        "炮",
        "卒",
        "帅",
        "仕",
        "相",
        "马",
        "车",
        "炮",
        "兵",
        "退",
        "出",
        "棋",
        "盘",
        "重",
        "新",
        "开",
        "局",
        "撤",
        "回",
        "一",
        "步",
        "你",
        "赢",
        "了"};
// 字库数据定义
const unsigned char FONT_DATA[][16] = {
    {0x0C, 0xC0, 0x0C, 0xFC, 0x0D, 0x8C, 0x6F, 0x18, 0x3C, 0xF0, 0x3C, 0x60, 0x0C, 0xD8, 0x0F, 0x98},
    {0x1C, 0x18, 0x3F, 0xFF, 0xEC, 0x18, 0x0D, 0x98, 0x0C, 0xD8, 0x0C, 0x18, 0x0C, 0x78, 0x0C, 0x30}, /*"将",0*/
    {0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF, 0x01, 0x80},
    {0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x3F, 0xFC, 0x00, 0x00}, /*"士",1*/
    {0x0C, 0x00, 0x1F, 0xF0, 0x30, 0x30, 0x7F, 0xFC, 0xF1, 0x8C, 0x33, 0x0C, 0x3F, 0xFC, 0x07, 0x00},
    {0x1D, 0x98, 0x73, 0xF0, 0x0E, 0xE0, 0x79, 0xF0, 0x07, 0xD8, 0x1C, 0xCC, 0xF3, 0xC7, 0x01, 0x80}, /*"象",2*/
    {0x00, 0x00, 0x7F, 0xF0, 0x00, 0x30, 0x00, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x1F, 0xFE},
    {0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0xFF, 0xF6, 0x00, 0x06, 0x00, 0x06, 0x00, 0x3C, 0x00, 0x18}, /*"马",3*/
    {0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x7F, 0xFE, 0x06, 0x00, 0x0D, 0x80, 0x19, 0x80, 0x31, 0x80},
    {0x3F, 0xFC, 0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80}, /*"车",4*/
    {0x18, 0xC0, 0x18, 0xC0, 0x19, 0xFE, 0x1F, 0x86, 0x7F, 0x06, 0x7F, 0xFE, 0x79, 0x9E, 0xD9, 0x9E},
    {0x19, 0x9E, 0x19, 0xFE, 0x19, 0x86, 0x3D, 0xBC, 0x37, 0x9B, 0x67, 0x83, 0x60, 0xFF, 0xC0, 0x00}, /*"炮",5*/
    {0x03, 0x00, 0x01, 0x80, 0x7F, 0xFE, 0x0C, 0x30, 0x0C, 0x30, 0x0C, 0x30, 0x1E, 0x78, 0x33, 0xCC},
    {0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80}, /*"卒",6*/
    {0x0C, 0x30, 0x0C, 0x30, 0x6C, 0x30, 0x6C, 0x30, 0x6D, 0xFE, 0x6D, 0xB6, 0x6D, 0xB6, 0x6D, 0xB6},
    {0x6D, 0xB6, 0x6D, 0xB6, 0x6D, 0xB6, 0x0D, 0xBE, 0x19, 0xBC, 0x18, 0x30, 0x30, 0x30, 0x60, 0x30}, /*"帅",7*/
    {0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x18, 0x60, 0x18, 0x60, 0x3F, 0xFF, 0x38, 0x60, 0x78, 0x60},
    {0xD8, 0x60, 0x18, 0x60, 0x18, 0x60, 0x18, 0x60, 0x18, 0x60, 0x1F, 0xFE, 0x18, 0x00, 0x18, 0x00}, /*"仕",8*/
    {0x0C, 0x00, 0x0C, 0xFE, 0x0C, 0xC6, 0x0C, 0xC6, 0xFF, 0xC6, 0x0C, 0xFE, 0x1C, 0xC6, 0x1E, 0xC6},
    {0x3F, 0xC6, 0x3F, 0xFE, 0x6C, 0xC6, 0xCC, 0xC6, 0x0C, 0xC6, 0x0C, 0xC6, 0x0C, 0xFE, 0x0C, 0xC6}, /*"相",9*/
    {0x00, 0x00, 0x7F, 0xF0, 0x00, 0x30, 0x00, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x1F, 0xFE},
    {0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0xFF, 0xF6, 0x00, 0x06, 0x00, 0x06, 0x00, 0x3C, 0x00, 0x18}, /*"马",10*/
    {0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x7F, 0xFE, 0x06, 0x00, 0x0D, 0x80, 0x19, 0x80, 0x31, 0x80},
    {0x3F, 0xFC, 0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80}, /*"车",11*/
    {0x18, 0xC0, 0x18, 0xC0, 0x19, 0xFE, 0x1F, 0x86, 0x7F, 0x06, 0x7F, 0xFE, 0x79, 0x9E, 0xD9, 0x9E},
    {0x19, 0x9E, 0x19, 0xFE, 0x19, 0x86, 0x3D, 0xBC, 0x37, 0x9B, 0x67, 0x83, 0x60, 0xFF, 0xC0, 0x00}, /*"炮",12*/
    {0x00, 0x30, 0x00, 0xF8, 0x1F, 0x80, 0x18, 0x00, 0x18, 0x00, 0x1F, 0xFC, 0x18, 0xC0, 0x18, 0xC0},
    {0x18, 0xC0, 0x18, 0xC0, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x60, 0x18, 0x30, 0x30, 0x18, 0x60, 0x0C}, /*"兵",13*/
    {0x03, 0xF8, 0x22, 0x08, 0x12, 0x08, 0x13, 0xF8, 0x02, 0x08, 0x02, 0x08, 0xF3, 0xF8, 0x12, 0x44},
    {0x12, 0x28, 0x12, 0x10, 0x12, 0x88, 0x13, 0x04, 0x12, 0x04, 0x28, 0x00, 0x47, 0xFE, 0x00, 0x00}, /*"退",14*/
    {0x01, 0x00, 0x01, 0x00, 0x21, 0x08, 0x21, 0x08, 0x21, 0x08, 0x21, 0x08, 0x3F, 0xF8, 0x01, 0x08},
    {0x01, 0x00, 0x01, 0x00, 0x41, 0x04, 0x41, 0x04, 0x41, 0x04, 0x41, 0x04, 0x7F, 0xFC, 0x00, 0x04}, /*"出",15*/
    {0x11, 0x08, 0x11, 0x08, 0x13, 0xFC, 0x11, 0x08, 0xFD, 0x08, 0x11, 0xF8, 0x31, 0x08, 0x39, 0x08},
    {0x55, 0xF8, 0x51, 0x08, 0x91, 0x08, 0x17, 0xFE, 0x10, 0x00, 0x10, 0x90, 0x11, 0x08, 0x12, 0x04}, /*"棋",16*/
    {0x02, 0x00, 0x04, 0x00, 0x1F, 0xF0, 0x11, 0x10, 0x10, 0x90, 0xFF, 0xFE, 0x10, 0x10, 0x12, 0x10},
    {0x21, 0x50, 0x40, 0x20, 0x3F, 0xF8, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0xFF, 0xFE, 0x00, 0x00}, /*"盘",17*/
    {0x00, 0x10, 0x00, 0xF8, 0x3F, 0x00, 0x01, 0x00, 0xFF, 0xFE, 0x01, 0x00, 0x1F, 0xF0, 0x11, 0x10},
    {0x1F, 0xF0, 0x11, 0x10, 0x1F, 0xF0, 0x01, 0x00, 0x3F, 0xF8, 0x01, 0x00, 0xFF, 0xFE, 0x00, 0x00}, /*"重",18*/
    {0x10, 0x00, 0x08, 0x04, 0x7F, 0x78, 0x00, 0x40, 0x22, 0x40, 0x14, 0x40, 0xFF, 0x7E, 0x08, 0x48},
    {0x08, 0x48, 0x7F, 0x48, 0x08, 0x48, 0x2A, 0x48, 0x49, 0x48, 0x88, 0x88, 0x28, 0x88, 0x11, 0x08}, /*"新",19*/
    {0x00, 0x00, 0x7F, 0xFC, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0xFF, 0xFE},
    {0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x10, 0x20, 0x10, 0x20, 0x20, 0x20, 0x40, 0x20}, /*"开",20*/
    {0x00, 0x00, 0x3F, 0xF8, 0x20, 0x08, 0x20, 0x08, 0x3F, 0xF8, 0x20, 0x00, 0x20, 0x00, 0x3F, 0xFC},
    {0x20, 0x04, 0x27, 0xE4, 0x24, 0x24, 0x24, 0x24, 0x47, 0xE4, 0x40, 0x04, 0x80, 0x28, 0x00, 0x10}, /*"局",21*/
    {0x22, 0x08, 0x21, 0x08, 0x27, 0xC8, 0x22, 0x10, 0xF4, 0x9E, 0x2F, 0xD4, 0x20, 0x64, 0x27, 0x94},
    {0x34, 0x94, 0xE7, 0x94, 0x24, 0x94, 0x27, 0x88, 0x24, 0x88, 0x24, 0x94, 0xA4, 0xA4, 0x45, 0xC2}, /*"撤",22*/
    {0x00, 0x00, 0x3F, 0xF8, 0x20, 0x08, 0x20, 0x08, 0x27, 0xC8, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48},
    {0x24, 0x48, 0x24, 0x48, 0x27, 0xC8, 0x20, 0x08, 0x20, 0x08, 0x3F, 0xF8, 0x20, 0x08, 0x00, 0x00}, /*"回",23*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"一",24*/
    {0x01, 0x00, 0x01, 0x00, 0x11, 0xF8, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0xFF, 0xFE, 0x01, 0x00},
    {0x01, 0x00, 0x11, 0x08, 0x11, 0x10, 0x21, 0x20, 0x40, 0xC0, 0x03, 0x00, 0x1C, 0x00, 0xE0, 0x00}, /*"步",25*/
    {0x08, 0x80, 0x08, 0x80, 0x08, 0x80, 0x11, 0xFE, 0x11, 0x02, 0x32, 0x04, 0x34, 0x20, 0x50, 0x20},
    {0x91, 0x28, 0x11, 0x24, 0x12, 0x24, 0x12, 0x22, 0x14, 0x22, 0x10, 0x20, 0x10, 0xA0, 0x10, 0x40}, /*"你",26*/
    {0x01, 0x00, 0xFF, 0xFE, 0x20, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x3F, 0xF8, 0x20, 0x08, 0x3F, 0xF8},
    {0x00, 0x00, 0x77, 0xDC, 0x54, 0x54, 0x75, 0x54, 0x55, 0x5C, 0x75, 0x56, 0x52, 0x96, 0xB4, 0x62}, /*"赢",27*/
    {0x00, 0x00, 0x7F, 0xF8, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x01, 0x80, 0x01, 0x00, 0x01, 0x00},
    {0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x05, 0x00, 0x02, 0x00}, /*"了",28*/
};

// 选中的棋子 10退出棋盘   11 重新开局  12 撤回上一步
int check_chess[2] = {-1, -1};
// 先行棋子   0  黑  1  另外一方
int first_chess = 0;
/*
    棋盘的每片空白都是 50*50
    中间的边界线 线宽2
    外围的边界线 线宽4
 */
// 将军的位置
int admiral[2][2] = {{4, 0}, {4, 9}};
// 历史记录结构体
struct
{
    char i1; // 上一次的坐标
    char j1; // 上一次的坐标
    char i2; // 下一次的坐标
    char j2; // 下一次的坐标
    int i;   // 上一次棋子的类型
    int j;   // 下一次棋子的类型
} history;
#endif
int runChess(void)
{

    // 设置文件目录在 VOICE_PATH
    chdir(VOICE_PATH);
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
    // 历史记录的文件
    int data_fd = open("./data", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXG);
    if (-1 == data_fd)
    {
        perror("data_fd文件打开失败");
        return -1;
    }
    lseek(data_fd, 0, SEEK_SET);
    /* 历史记录文件结构
    两个char 保存初始地址
    一个int 保存棋子类型
    两个char 保存末尾地址
    一个int 保存棋子类型


     */
    // 点击事件的进程id
    pthread_t tid_touch, tid_voice;
    // 初始化棋盘
    initChessBlock(mp);
    // 上一次点击事件的缓存值
    int cache_click[2] = {-1, -1};
    pthread_create(&tid_touch, NULL, ptTouch, &touch_fd);
    pthread_create(&tid_voice, NULL, ptPlayMusic, NULL);

    while (1)
    {
        // 检查是否有点击事件
        if (check_chess[0] >= 0 && check_chess[1] >= 0)
        {
            if (check_chess[0] >= 10 && check_chess[1] >= 10)
            {
                // 重新开局
                if (check_chess[0] == 11 && check_chess[1] == 11)
                {
                    // printf("重新开局\n");
                    // 初始化棋盘
                    initChessBlock(mp);
                    // 历史记录文件指针回到顶部
                    lseek(data_fd, 0, SEEK_SET);
                    first_chess = 0;
                }
                // 撤回上一步
                else if (check_chess[0] == 12 && check_chess[1] == 12)
                {
                    // printf("撤回上一步\n");
                    // 初始化棋盘
                    if (!(lseek(data_fd, 0, SEEK_CUR) <= 0))
                    {
                        lseek(data_fd, -1 * sizeof(history), SEEK_CUR);
                        read(data_fd, &history, sizeof(history));
                        lseek(data_fd, -1 * sizeof(history), SEEK_CUR);

                        chessBlock[history.i1][history.j1][1] = history.i;
                        chessBlock[history.i2][history.j2][1] = history.j;
                        drawBlock(mp, history.i1, history.j1);
                        drawBlock(mp, history.i2, history.j2);
                        first_chess++;
                        first_chess %= 2;
                        // 如果撤回的是将,要更新将的位置
                        if ((chessBlock[history.i1][history.j1][1] & 4) == 4)
                        {
                            admiral[(chessBlock[history.i1][history.j1][1] & 3) == 2 ? 0 : 1][0] = history.i1;
                            admiral[(chessBlock[history.i1][history.j1][1] & 3) == 2 ? 0 : 1][1] = history.j1;
                        }
                    }
                }
                // 退出
                else if (check_chess[0] == 10 && check_chess[1] == 10)
                {
                    check_chess[0] = 11;
                    check_chess[1] = 11;
                    // printf("退出\n");
                    break;
                }
            }
            // 上一次有点击事件完成
            else if (cache_click[0] >= 0 && cache_click[1] >= 0)
            {
                // 行为规则满足条件
                if (1 == chessRule(cache_click[0], cache_click[1], check_chess[0], check_chess[1]))
                {

                    // 声音事件
                    if (4 == (chessBlock[check_chess[0]][check_chess[1]][1] & 4)) // 胜利
                    {
                        voice = 1;
                    }
                    else if (0 == chessBlock[check_chess[0]][check_chess[1]][1])
                    {
                        voice = 16;
                    }
                    else
                    {
                        voice = 8;
                    }
                    // 添加到历史记录结构体中并写入文件
                    history.i1 = cache_click[0];
                    history.j1 = cache_click[1];
                    history.i2 = check_chess[0];
                    history.j2 = check_chess[1];
                    history.i = chessBlock[cache_click[0]][cache_click[1]][1];
                    history.j = chessBlock[check_chess[0]][check_chess[1]][1];
                    write(data_fd, &history, sizeof(history));

                    // 死将
                    if (4 == (chessBlock[check_chess[0]][check_chess[1]][1] & 4))
                    {
                        // 黑色方
                        if (1 == (chessBlock[check_chess[0]][check_chess[1]][1] & 3))
                        {
                            drawFont(mp, 26, CHESS_MAN * 3 + LINE_WIDTH * 2, HEIGHT_LCD / 2 - 20, 1, colorChess[6]);
                            drawFont(mp, 27, CHESS_MAN * 3 + LINE_WIDTH * 2, HEIGHT_LCD / 2, 1, colorChess[6]);
                            drawFont(mp, 28, CHESS_MAN * 3 + LINE_WIDTH * 2, HEIGHT_LCD / 2 + 20, 1, colorChess[6]);
                        }
                        else
                        {
                            drawFont(mp, 26, WIDTH_LCD - (CHESS_MAN * 3 + LINE_WIDTH * 2), HEIGHT_LCD / 2 + 20, 3, colorChess[6]);
                            drawFont(mp, 27, WIDTH_LCD - (CHESS_MAN * 3 + LINE_WIDTH * 2), HEIGHT_LCD / 2, 3, colorChess[6]);
                            drawFont(mp, 28, WIDTH_LCD - (CHESS_MAN * 3 + LINE_WIDTH * 2), HEIGHT_LCD / 2 - 20, 3, colorChess[6]);
                        }
                    }
                    chessBlock[check_chess[0]][check_chess[1]][1] = chessBlock[cache_click[0]][cache_click[1]][1];
                    chessBlock[cache_click[0]][cache_click[1]][1] = 0;
                    drawBlock(mp, check_chess[0], check_chess[1]);
                    drawBlock(mp, cache_click[0], cache_click[1]);
                    first_chess++;
                    first_chess %= 2;
                }
                else
                {
                    voice |= 32;
                    drawBlock(mp, cache_click[0], cache_click[1]);
                }
                cache_click[0] = -1;
                cache_click[1] = -1;
            }
            else if (chessBlock[check_chess[0]][check_chess[1]][1] > 0 && (chessBlock[check_chess[0]][check_chess[1]][1] & 3) % 2 == first_chess)
            {
                cache_click[0] = check_chess[0];
                cache_click[1] = check_chess[1];
                drawCircle(
                    mp,
                    cache_click[1] * (CHESS_WIDTH + LINE_WIDTH * 2) + (WIDTH_LCD - 9 * (CHESS_WIDTH + LINE_WIDTH * 2)) / 2,
                    cache_click[0] * (CHESS_WIDTH + LINE_WIDTH * 2) + (HEIGHT_LCD - 8 * (CHESS_WIDTH + LINE_WIDTH * 2)) / 2,
                    CHESS_MAN - 2, CHESS_MAN - 5,
                    0xFFA61808);
            }
            check_chess[0] = -1;
            check_chess[1] = -1;
        }
    }

    pthread_cancel(tid_touch);
    pthread_cancel(tid_voice);
    close(touch_fd);
    munmap(mp, WIDTH_LCD * HEIGHT_LCD * 4);
    close(lcd_fd);
    close(data_fd);
}
void drawBlock(char *mp, int i, int j)
{
    /*
    对于位于棋盘左下角的绝对坐标
    相对于屏幕上的点的坐标
     */
    // 获取x绝对坐标
    int x1 = (WIDTH_LCD - 9 * (CHESS_WIDTH + LINE_WIDTH * 2)) / 2;
    // 获取y绝对坐标
    int y1 = (HEIGHT_LCD - 8 * (CHESS_WIDTH + LINE_WIDTH * 2)) / 2;
    /*
    获取位置(i,j)的坐标点
     */
    int x2 = j * (CHESS_WIDTH + LINE_WIDTH * 2) + x1;
    int y2 = i * (CHESS_WIDTH + LINE_WIDTH * 2) + y1;

    // 设置画的单条线长
    int line = CHESS_WIDTH / 2 + LINE_WIDTH;

    // 那么实际上坐标(i,j)就在坐标(x2,y2)上
    // 画出单色背景
    drawLine(mp, x2 - line, y2 - line, x2 + line, y2 + line, colorChess[1]);
    /*
    按照对应的坐标画出单个棋子
     */

    // 右      1    下
    if (1 == (chessBlock[i][j][0] & 1))
    {
        drawLine(mp, x2 - LINE_WIDTH, y2 - LINE_WIDTH, x2 + LINE_WIDTH, y2 + line, colorChess[2]);
    }
    // 上      2    右
    if (2 == (chessBlock[i][j][0] & 2))
    {
        drawLine(mp, x2 - LINE_WIDTH, y2 - LINE_WIDTH, x2 + line, y2 + LINE_WIDTH, colorChess[2]);
    }
    // 左      4    上
    if (4 == (chessBlock[i][j][0] & 4))
    {
        drawLine(mp, x2 - LINE_WIDTH, y2 - line, x2 + LINE_WIDTH, y2 + LINE_WIDTH, colorChess[2]);
    }
    // 下      8    左
    if (8 == (chessBlock[i][j][0] & 8))
    {
        drawLine(mp, x2 - line, y2 - LINE_WIDTH, x2 + LINE_WIDTH, y2 + LINE_WIDTH, colorChess[2]);
    }

    line = line + LINE_WIDTH * 5;
    // 右上      16
    if (16 == (chessBlock[i][j][0] & 16))
    {
        drawSlash(mp, x2, y2, x2 + line, y2 + line, colorChess[2]);
    }
    // 左上        32
    if (32 == (chessBlock[i][j][0] & 32))
    {
        drawSlash(mp, x2, y2, x2 + line, y2 - line, colorChess[2]);
    }
    // 左下        64
    if (64 == (chessBlock[i][j][0] & 64))
    {
        drawSlash(mp, x2, y2, x2 - line, y2 - line, colorChess[2]);
    }
    // 右下        128
    if (128 == (chessBlock[i][j][0] & 128))
    {
        drawSlash(mp, x2, y2, x2 - line, y2 + line, colorChess[2]);
    }
    // 标记点      256
    if (256 == (chessBlock[i][j][0] & 256))
    {
        drawMark(mp, x2, y2, i);
    }
    // 棋盘上的棋子
    if (chessBlock[i][j][1] > 0)
    {
        drawCircle(mp, x2, y2, CHESS_MAN, 0, colorChess[3]);
        for (int m = 0; m < 7; m++)
        {
            if (0 != (chessBlock[i][j][1] & (4 << m)))
            {
                drawFont(mp, m + ((chessBlock[i][j][1] & 3) == 2 ? 0 : 7), x2, y2, ((chessBlock[i][j][1] & 3) == 2 ? 1 : 3), colorChess[((chessBlock[i][j][1] & 3) == 2 ? 5 : 4)]);
                return;
            }
        }
    }
}
void initChessBlock(char *mp)
{
    memset(chessBlock, 0, sizeof(chessBlock));
    // 初始化棋盘
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            // 画出边,角处
            if (0 == i || 0 == j || 8 == i || 9 == j)
            {
                chessBlock[i][j][0] |= (0 == i ? 1 : 0) | (0 == j ? 2 : 0) | (8 == i ? 4 : 0) | (9 == j ? 8 : 0);
                if ((0 == i && 0 != j && 9 != j) || (8 == i && 0 != j && 9 != j))
                    chessBlock[i][j][0] |= 2 | 8;
                if ((0 == j && 0 != i && 8 != i) || (9 == j && 0 != i && 8 != i))
                    chessBlock[i][j][0] |= 1 | 4;
            }
            // 方块内十字
            else // if (i < 8 && i > 0 && j > 0 && j < 9)
            {
                if (j == 4)
                    chessBlock[i][j][0] |= 1 | 4 | 8;
                else if (j == 5)
                    chessBlock[i][j][0] |= 1 | 2 | 4;
                else
                    chessBlock[i][j][0] |= 1 | 2 | 4 | 8;
            }
            // 标记点
            if (((3 == j || 6 == j) && 0 == i % 2) || ((2 == j || 7 == j) && 1 == i % 6))
            {
                chessBlock[i][j][0] |= 256;
                // 兵
                if (3 == j || 6 == j)
                    chessBlock[i][j][1] |= 256;
                // 炮
                else
                    chessBlock[i][j][1] |= 128;
            }
            // X形
            if ((1 == j || 8 == j) && 4 == i)
            {
                chessBlock[i][j][0] |= 16 | 32 | 64 | 128;
                chessBlock[i - 1][j - 1][0] |= 16;
                chessBlock[i + 1][j - 1][0] |= 32;
                chessBlock[i + 1][j + 1][0] |= 64;
                chessBlock[i - 1][j + 1][0] |= 128;
            }

            // 棋子
            if (0 == j || 9 == j)
                // 车
                if (0 == i || 8 == i)
                    chessBlock[i][j][1] |= 64;
                // 马
                else if (1 == i || 7 == i)
                    chessBlock[i][j][1] |= 32;
                // 象
                else if (2 == i || 6 == i)
                    chessBlock[i][j][1] |= 16;
                // 士
                else if (3 == i || 5 == i)
                    chessBlock[i][j][1] |= 8;
                // 将
                else
                    chessBlock[i][j][1] |= 4;
            // 颜色
            if (chessBlock[i][j][1] > 0)
                chessBlock[i][j][1] |= (j > 4 ? 1 : 2);
        }
    }
    // 将军的位置
    admiral[0][0] = 4;
    admiral[0][1] = 0;
    admiral[1][0] = 4;
    admiral[1][1] = 9;
    // 初始化棋盘背景颜色
    drawLine(mp, 0, 0, WIDTH_LCD, HEIGHT_LCD, colorChess[0]);
    // 菜单键
    //  黑方
    drawLine(mp, 0, 0, CHESS_MAN * 4, HEIGHT_LCD, colorChess[1]);
    drawLine(mp, CHESS_MAN * 4, 0, CHESS_MAN * 4 + LINE_WIDTH * 2, HEIGHT_LCD, colorChess[2]);
    drawLine(mp, 0, HEIGHT_LCD / 3 - LINE_WIDTH, CHESS_MAN * 4, HEIGHT_LCD / 3 + LINE_WIDTH, colorChess[2]);
    drawLine(mp, 0, HEIGHT_LCD * 2 / 3 - LINE_WIDTH, CHESS_MAN * 4, HEIGHT_LCD * 2 / 3 + LINE_WIDTH, colorChess[2]);
    // 红方
    drawLine(mp, WIDTH_LCD - CHESS_MAN * 4, 0, WIDTH_LCD, HEIGHT_LCD, colorChess[1]);
    drawLine(mp, WIDTH_LCD - CHESS_MAN * 4 - LINE_WIDTH * 2, 0, WIDTH_LCD - CHESS_MAN * 4, HEIGHT_LCD, colorChess[2]);
    drawLine(mp, WIDTH_LCD - CHESS_MAN * 4, HEIGHT_LCD / 3 - LINE_WIDTH, WIDTH_LCD, HEIGHT_LCD / 3 + LINE_WIDTH, colorChess[2]);
    drawLine(mp, WIDTH_LCD - CHESS_MAN * 4, HEIGHT_LCD * 2 / 3 - LINE_WIDTH, WIDTH_LCD, HEIGHT_LCD * 2 / 3 + LINE_WIDTH, colorChess[2]);
    // 菜单文字
    for (int i = 0; i < 12; i++)
    {
        drawFont(mp, i + 14, CHESS_MAN * 2, (1 + ((i <= 3) ? -1 : (i >= 8 ? 1 : 0))) * HEIGHT_LCD / 3 + (i % 4 + 1) * HEIGHT_LCD / 15, 1, colorChess[2]);
        drawFont(mp, i + 14, WIDTH_LCD - CHESS_MAN * 2, HEIGHT_LCD - ((1 + ((i <= 3) ? -1 : (i >= 8 ? 1 : 0))) * HEIGHT_LCD / 3 + (i % 4 + 1) * HEIGHT_LCD / 15), 3, colorChess[2]);
    }
    // 初始化棋盘
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            drawBlock(mp, i, j);
        }
    }
}
// 画方块 参数  屏幕上像素点  颜色
void drawLine(char *mp, int x1, int y1, int x2, int y2, int color)
{
    // printf("x1=%d\ty1=%d\tx2=%d\ty2=%d\t\n",x1,y1,x2,y2);
    for (int i = y1; i < y2; i++)
        for (int j = x1; j < x2; j++)
            memcpy(mp + (i * WIDTH_LCD + j) * 4, &color, 4);
    // printf("画直线");
}
// 画斜线
void drawSlash(char *mp, int x1, int y1, int x2, int y2, int color)
{
    if (x2 > x1)
        for (int i = x1 - 1, j = y1; i < x2; i++, j += (y2 - y1 > 0 ? 1 : -1))
            for (int k = 0; k < LINE_WIDTH * 2; k++)
                memcpy(mp + (j * WIDTH_LCD + i + k) * 4, &color, 4);
    else
        for (int i = x1 - 1, j = y1; i > x2; i--, j += (y2 - y1 > 0 ? 1 : -1))
            for (int k = 0; k < LINE_WIDTH * 2; k++)
                memcpy(mp + (j * WIDTH_LCD + i + k) * 4, &color, 4);
}
// 参数  屏幕上像素点  半径
void drawCircle(char *mp, int x, int y, int r1, int r2, int color)
{
    for (int i = y - r1; i <= y + r1; i++)
        for (int j = x - r1; j <= x + r1; j++)
            if ((i - y) * (i - y) + (j - x) * (j - x) <= r1 * r1 && (i - y) * (i - y) + (j - x) * (j - x) >= r2 * r2)
                memcpy(mp + (i * WIDTH_LCD + j) * 4, &color, 4);
}

// 画标记点
void drawMark(char *mp, int x, int y, int i)
{
    int n = LINE_WIDTH * 2 + 2;
    int line = 5;
    if (0 != i)
    {
        drawLine(mp, x + n - LINE_WIDTH, y - n - LINE_WIDTH, x + n + line, y - n + LINE_WIDTH, colorChess[2]);
        drawLine(mp, x + n - LINE_WIDTH, y - n - line, x + n + LINE_WIDTH, y - n + LINE_WIDTH, colorChess[2]);
        drawLine(mp, x - n - LINE_WIDTH, y - n - line, x - n + LINE_WIDTH, y - n + LINE_WIDTH, colorChess[2]);
        drawLine(mp, x - n - line, y - n - LINE_WIDTH, x - n + LINE_WIDTH, y - n + LINE_WIDTH, colorChess[2]);
    }
    if (8 != i)
    {
        drawLine(mp, x - n - line, y + n - LINE_WIDTH, x - n + LINE_WIDTH, y + n + LINE_WIDTH, colorChess[2]);
        drawLine(mp, x - n - LINE_WIDTH, y + n - LINE_WIDTH, x - n + LINE_WIDTH, y + n + line, colorChess[2]);
        drawLine(mp, x + n - LINE_WIDTH, y + n - LINE_WIDTH, x + n + LINE_WIDTH, y + n + line, colorChess[2]);
        drawLine(mp, x + n - LINE_WIDTH, y + n - LINE_WIDTH, x + n + line, y + n + LINE_WIDTH, colorChess[2]);
    }
}
// 棋盘规则
int chessRule(int i1, int j1, int i2, int j2)
{
    int chess_type = 4 | 8 | 16 | 32 | 64 | 128 | 256;
    chess_type &= chessBlock[i1][j1][1];
    // printf("chess_type=%d\n", chess_type);
    // return 1;
    // 禁止互相残杀 自己踩自己
    if (((chessBlock[i1][j1][1] & 3) == (chessBlock[i2][j2][1] & 3)) || (i1 == i2 && j1 == j2))
    {
        return 0;
    }
    // 将
    else if (4 == chess_type)
    {

        if (i1 >= 3 &&
            i1 <= 5 &&
            i2 >= 3 &&
            i2 <= 5 &&
            j1 >= ((chessBlock[i1][j1][1] & 3) == 2 ? 0 : 7) &&
            j1 <= ((chessBlock[i1][j1][1] & 3) == 2 ? 2 : 9) &&
            j2 >= ((chessBlock[i1][j1][1] & 3) == 2 ? 0 : 7) &&
            j2 <= ((chessBlock[i1][j1][1] & 3) == 2 ? 2 : 9) &&
            1 == abs((i1 + j1) - (i2 + j2)))
        {

            chess_type = (chessBlock[i1][j1][1] & 3) == 2 ? 0 : 1;
            admiral[chess_type][0] = i2;
            admiral[chess_type][1] = j2;
            return 1;
        }
        else if (4 == (chessBlock[i2][j2][1] & 4))
        {
            for (int i = (j1 < j2 ? j1 + 1 : j2 + 1); i < (j1 < j2 ? j2 - 1 : j1 - 1); i++)
                if (0 != chessBlock[i1][i][1])
                    return 0;
            return 1;
        }
    }
    // 士
    else if (8 == chess_type)
    {

        if (i1 >= 3 &&
            i1 <= 5 &&
            i2 >= 3 &&
            i2 <= 5 &&
            j1 >= ((chessBlock[i1][j1][1] & 3) == 2 ? 0 : 7) &&
            j1 <= ((chessBlock[i1][j1][1] & 3) == 2 ? 2 : 9) &&
            j2 >= ((chessBlock[i1][j1][1] & 3) == 2 ? 0 : 7) &&
            j2 <= ((chessBlock[i1][j1][1] & 3) == 2 ? 2 : 9) &&
            1 == abs(i1 - i2) &&
            1 == abs(j1 - j2))
            return 1;
    }
    // 象
    else if (16 == chess_type)
    {

        if (
            j2 <= ((chessBlock[i1][j1][1] & 3) == 2 ? 5 : 9) &&
            j2 >= ((chessBlock[i1][j1][1] & 3) == 2 ? 0 : 5) &&
            0 == chessBlock[i1 + (i2 > i1 ? 1 : -1)][j1 + (j2 > j1 ? 1 : -1)][1] &&
            2 == abs(i1 - i2) &&
            2 == abs(j1 - j2))
            return 1;
    }
    // 马
    else if (32 == chess_type)
    {

        if (
            0 == chessBlock[i1 + ((2 == abs(i1 - i2)) ? (i1 > i2 ? -1 : 1) : 0)][j1 + ((2 == abs(j1 - j2)) ? (j1 > j2 ? -1 : 1) : 0)][1] &&
            ((1 == abs(i1 - i2) && 2 == abs(j1 - j2)) || (2 == abs(i1 - i2) && 1 == abs(j1 - j2))) &&
            i1 != i2 &&
            j1 != j2)
            return 1;
    }
    // 车
    else if (64 == chess_type)
    {
        if (i1 == i2 && j1 != j2)
        {
            for (int i = (j1 < j2 ? j1 + 1 : j2 + 1); i < (j1 < j2 ? j2 - 1 : j1 - 1); i++)
                if (0 != chessBlock[i1][i][1])
                    return 0;
        }
        else if (i1 != i2 && j1 == j2)
        {
            for (int i = (i1 < i2 ? i1 + 1 : i2 + 1); i < (i1 < i2 ? i2 - 1 : i1 - 1); i++)
                if (0 != chessBlock[i][j1][1])
                    return 0;
        }
        else
            return 0;

        return 1;
    }
    // 炮
    else if (128 == chess_type)
    {
        if (0 == chessBlock[i2][j2][1])
        {
            // printf("(%d,%d)\n",i1,j1);
            if (i1 == i2 && j1 != j2)
            {
                for (int i = (j1 < j2 ? j1 + 1 : j2 + 1); i < (j1 < j2 ? j2 : j1); i++)
                    if (0 != chessBlock[i1][i][1])
                        return 0;
            }
            else if (i1 != i2 && j1 == j2)
            {
                for (int i = (i1 < i2 ? i1 + 1 : i2 + 1); i < (i1 < i2 ? i2 : i1); i++)
                    if (0 != chessBlock[i][j1][1])
                        return 0;
            }
            else
            {
                return 0;
            }

            return 1;
        }
        else if (0 != chessBlock[i2][j2][1])
        {
            chess_type = 0;

            if (i1 == i2 && j1 != j2)
            {
                for (int i = (j1 < j2 ? j1 + 1 : j2 + 1); i < (j1 < j2 ? j2 : j1); i++)
                    if (0 != chessBlock[i1][i][1])
                        chess_type++;
            }
            else if (i1 != i2 && j1 == j2)
            {
                for (int i = (i1 < i2 ? i1 + 1 : i2 + 1); i < (i1 < i2 ? i2 : i1); i++)
                    if (0 != chessBlock[i][j1][1])
                        chess_type++;
            }
            // printf("(%d,%d)=%d\n",i1,j1,chess_type);
            if (1 == chess_type)
                return 1;
        }

        return 0;
    }
    // 兵
    else if (256 == chess_type)
    {
        if ((chessBlock[i1][j1][1] & 3) == 2)
        {
            if (j1 <= 4 && 1 == j2 - j1 && i1 == i2)
                return 1;
            else if (j1 >= 5 && !(-1 == j2 - j1) && ((1 == abs(i1 - i2) && j1 == j2) || (1 == abs(j1 - j2) && i1 == i2)))
                return 1;
        }
        else
        {
            if (j1 >= 5 && -1 == j2 - j1 && i1 == i2)
                return 1;
            else if (j1 <= 4 && !(1 == j2 - j1) && ((1 == abs(i1 - i2) && j1 == j2) || (1 == abs(j1 - j2) && i1 == i2)))
                return 1;
        }
    }
    return 0;
}
// 多线程点击
void *ptTouch(void *arg)
{
    int x1 = 0, y1 = 0, i, j;
    // 获取x绝对坐标
    const int x2 = (WIDTH_LCD - 9 * (CHESS_WIDTH + LINE_WIDTH * 2)) / 2;
    // 获取y绝对坐标
    const int y2 = (HEIGHT_LCD - 8 * (CHESS_WIDTH + LINE_WIDTH * 2)) / 2;
    int touch_fd = *((int *)arg);
    struct input_event ie;
    while (1)
    {
        read(touch_fd, &ie, sizeof(ie));
        if (ABS_X == ie.code)
        {
            x1 = ie.value;
        }
        if (ABS_Y == ie.code)
        {
            y1 = ie.value;
        }
        if (x1 != 0 && y1 != 0)
        {
            // printf("(x1,y1)=(%d,%d)\n", x1, y1);
            x1 = x1 * WIDTH_LCD / WIDTH_TOUCH;
            y1 = y1 * HEIGHT_LCD / HEIGHT_TOUCH;
            if (!(x1 < x2 - CHESS_WIDTH / 2 || x1 > WIDTH_LCD - x2 + CHESS_WIDTH / 2 || y1 < y2 - CHESS_WIDTH / 2 || y1 > HEIGHT_LCD - y2 + CHESS_WIDTH / 2))
            {
                j = (x1 - x2) % (CHESS_WIDTH + LINE_WIDTH * 2);
                i = (y1 - y2) % (CHESS_WIDTH + LINE_WIDTH * 2);
                j = (int)((x1 - x2) / (CHESS_WIDTH + LINE_WIDTH * 2)) + ((j > CHESS_WIDTH / 2 + LINE_WIDTH) ? 1 : 0);
                i = (int)((y1 - y2) / (CHESS_WIDTH + LINE_WIDTH * 2)) + ((i > CHESS_WIDTH / 2 + LINE_WIDTH) ? 1 : 0);

                check_chess[0] = i;
                check_chess[1] = j;
                // printf("(i,j)=(%d,%d)\n", i, j);
            }
            else if (x1 < CHESS_MAN * 4 || x1 > (WIDTH_LCD - CHESS_MAN * 4))
            {
                check_chess[0] = 11;
                if (y1 < HEIGHT_LCD / 3)
                {
                    check_chess[0] += ((x1 < CHESS_MAN * 4) ? -1 : 1);
                }
                else if (y1 > HEIGHT_LCD * 2 / 3)
                {
                    check_chess[0] += ((x1 < CHESS_MAN * 4) ? 1 : -1);
                }
                check_chess[1] = check_chess[0];
            }
            x1 = 0;
            y1 = 0;
        }
    }
}
// 多线程播放音乐
void *ptPlayMusic(void *arg)
{
    chdir(VOICE_PATH);
    while (1)
    {
        if (1 == (voice & 1))
        {
            system("madplay 1.mp3 -Q &");
            voice = 0;
        }
        else if (2 == (voice & 2))
        {
            system("madplay 2.mp3 -Q &");
            voice = 0;
        }
        else if (4 == (voice & 4))
        {
            system("madplay 3.mp3 -Q &");
            voice = 0;
        }
        else if (8 == (voice & 8))
        {
            system("madplay 4.mp3 -Q &");
            voice = 0;
        }
        else if (16 == (voice & 16))
        {
            voice = 0;
            system("madplay 5.mp3 -Q &");
        }
        else if (32 == (voice & 32))
        {
            voice = 0;
            system("madplay 6.mp3 -Q &");
        }
    }
}
// 打印文字在屏幕上,文字的左上角位置和打印方向    文字 头向  1右边 2上边  3左边  4下边
void drawFont(char *mp, int k, int x, int y, char direction, int color)
{
    int m, n;
    k *= 2;
    if (1 == direction)
    {
        for (int i = y - 8; i < y + 8; i++)
            for (int j = x - 8; j < x + 8; j++)
            {
                m = (15 - (j - (x - 8))) * 2 + (i - (y - 8)) / 8;
                n = 7 - ((i - (y - 8)) % 8);
                // printf("m=%d,n=%d\n",m,n);
                if (0 != (FONT_DATA[k][m] & (1 << n)))
                    memcpy(mp + (j + i * WIDTH_LCD) * 4, &color, 4);
            }
    }
    else if (3 == direction)
    {
        for (int i = y - 8; i < y + 8; i++)
            for (int j = x - 8; j < x + 8; j++)
            {
                m = (j + 8 - x) * 2 + (15 - (i - (y - 8))) / 8;
                n = (15 - (i - (y - 8))) % 8;
                // printf("m=%d,n=%d\n",m,n);
                if (0 != (FONT_DATA[k][m] & (1 << (7 - n))))
                    memcpy(mp + (j + i * WIDTH_LCD) * 4, &color, 4);
            }
    }
}
