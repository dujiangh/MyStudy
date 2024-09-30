#include <stdio.h>
#include "chess.h"
#include "plot.h"
int main(int argc , char** argv,char** envp)
{
    while (1)
    {
        // printf("立方体\n");
        runPlot();
        // printf("象棋\n");
        runChess();
    }
    
    return 0;
}