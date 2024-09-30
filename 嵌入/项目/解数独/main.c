#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
int suku[9 * 9] =
    {
        7, 1, 8, 2, 3, 5, 0, 6, 0,
        3, 0, 0, 0, 4, 0, 0, 0, 0,
        9, 2, 0, 6, 7, 0, 0, 0, 0,

        0, 0, 0, 3, 9, 0, 0, 4, 8,
        5, 0, 6, 0, 0, 0, 3, 0, 7,
        8, 3, 0, 0, 5, 7, 0, 0, 0,

        0, 0, 0, 0, 6, 2, 0, 5, 3,
        0, 0, 0, 0, 8, 0, 0, 0, 1,
        0, 8, 0, 5, 1, 3, 9, 2, 4}; */
int suku[9 * 9] =
    {
        0, 0, 0, 0, 0, 0, 8, 0, 0,
        4, 0, 0, 2, 0, 8, 0, 5, 1,
        0, 8, 3, 9, 0, 0, 0, 0, 7,

        0, 4, 0, 5, 0, 0, 0, 8, 2,
        0, 0, 5, 0, 0, 0, 4, 0, 0,
        8, 7, 0, 0, 0, 9, 0, 3, 0,

        2, 0, 0, 0, 0, 7, 1, 6, 0,
        3, 6, 0, 1, 0, 5, 0, 0, 4,
        0, 0, 4, 0, 0, 0, 0, 0, 0

};
// 打印一个数独
void printSudo(void);
// 在第(i,j)位置填入指定数字
int pullNum(int index);
// 判断第(i,j)位置数字是否满足规则
int numIsTrue(int index);
// 解决数独的函数
int main(int argc, char **argv, char **envp)
{
    // 初始化数独
    // memset(suku,0,sizeof(suku));

    // 先打印一下开始的数独
    printSudo();
    if (pullNum(0))
    {
        printf("\n正确的!\n\n");
    }
    else
        printf("\n错误的!\n\n");
    // 运行结束后打印一下数独
    printSudo();
    return 0;
}

// 打印一个数独
void printSudo(void)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            printf(" %d ", suku[i * 9 + j]);
        printf("  \n");
    }
}
// 在第(i,j)位置填入指定数字
int pullNum(int index)
{
    // 如果在第9行9列外,说明该位置可以
    if (index >= 9 * 9)
        return 1;
    // 如果该位置有数字就直接跳转到下一步
    if (suku[index])
        return pullNum(index + 1);
    // 尝试填入1-9的数字
    for (int i = 0; i < 9; i++)
    {
        suku[index] = i + 1;
        // 如果满足规则 就判断下一个是否正确
        if (numIsTrue(index))
        {
            // 判断下一个数字是否可以使用
            if (pullNum(index + 1))
                return 1;
        }
    }
    // 遍历错误
    suku[index] = 0;
    return 0;
}
// 判断第(i,j)位置数字是否满足规则 i行,j列
int numIsTrue(int index)
{
    // 定义i,j
    int i = index / 9;
    int j = index % 9;
    // 定义i,j所在的中点
    int mind_i = i / 3 * 3 + 1;
    int mind_j = j / 3 * 3 + 1;
    // 在3*3的方格里面没有重复的
    for (int m = -1; m < 2; m++)
        for (int n = -1; n < 2; n++)
        {
            // 下标重叠或者是在和0比较的
            if (index == (mind_i + m) * 9 + (mind_j + n) || suku[(mind_i + m) * 9 + (mind_j + n)] == 0)
                continue;
            // 如果有相等的,就说明这个数填错了
            if (suku[index] == suku[(mind_i + m) * 9 + (mind_j + n)])
                return 0;
        }
    // 在行里面没有重复的
    // 在列里面没有重复的
    for (int m = 0; m < 9; m++)
    {
        // 下标重叠或者是在和0比较的
        if (index == m * 9 + j || suku[m * 9 + j] == 0)
            continue;
        // 如果有相等的,就说明这个数填错了
        if (suku[index] == suku[m * 9 + j])
            return 0;
    }
    for (int m = 0; m < 9; m++)
    {
        // 下标重叠或者是在和0比较的
        if (index == i * 9 + m || suku[i * 9 + m] == 0)
            continue;
        // 如果有相等的,就说明这个数填错了
        if (suku[index] == suku[i * 9 + m])
            return 0;
    }
    return 1;
}