#include <stdlib.h>

#include "ts.h"
#include "debug.h"


char text[1024];




/*触摸屏代码*/
int ts_func(int *x,int *y)
{
	XPRINTF("ts_func 请用手触摸屏幕.......\n");
	//1）打开触摸屏设备文件 ---
	int tsfd = open("/dev/input/event0",O_RDWR);
	if(tsfd == -1)
	{
		XPRINTF("open /dev/input/event0 error\n");
		return -1;
	}
	
	while(1)
	{
		//2)分析 触摸屏的数据是什么数据类型？？？
		//输入子系统计算完结果之后会将这个结果存储在结构体中
		struct input_event info;
		
		//3）读取触摸屏数据 --如果你没有用手触摸屏幕，当前会阻塞在这里
		read(tsfd,&info,sizeof(struct input_event));
		
		//判断当前发生的是触摸屏事件,接着判断触发的是X轴事件
		if(info.type == EV_ABS && info.code == ABS_X)
			*x = info.value;
		//判断当前发生的是触摸屏事件,接着判断触发的是Y轴事件
		if(info.type == EV_ABS && info.code == ABS_Y)
			*y = info.value;
		
		//松开的时候，才进行打印  type:1 code:330 value:0
		if(info.type == EV_KEY && info.code == BTN_TOUCH &&  info.value == 0){
			
			//如果你的板子是黑色的，那么需要进行坐标转换(1024,600) ---(800,480)
			//*x = (*x *1.0)/1024 * 800 ;
			//*y = (*y *1.0)/600  * 480 ;
			bzero(text,1024);
			sprintf(text,"(%d,%d)\n",*x,*y);
			XPRINTF(text);
			break;
		}
	}
	
	//5）关闭触摸屏文件
	close(tsfd);
}
/*滑屏代码*/
int sling_ts_func(int *x,int *y)
{
	XPRINTF("sling_ts_func 请用手触摸屏幕.......\n");
	//1）打开触摸屏设备文件 ---
	int tsfd = open("/dev/input/event0",O_RDWR);
	if(tsfd == -1)
	{
		XPRINTF("open /dev/input/event0 error\n");
		return -1;
	}
	
	//输入子系统计算完结果之后会将这个结果存储在结构体中
	struct input_event info;
	enum Direction direction;
	
	while(1)
	{
		int x1=0,x2=0,y1=0,y2=0;
		//1、获取第一次按下的坐标值
		while(1)
		{
			//3）读取触摸屏数据 --如果你没有用手触摸屏幕，当前会阻塞在这里
			read(tsfd,&info,sizeof(struct input_event));
			
			//判断当前发生的是触摸屏事件,接着判断触发的是X轴事件
			if(info.type == EV_ABS && info.code == ABS_X)
				x1 = info.value;
			//判断当前发生的是触摸屏事件,接着判断触发的是Y轴事件
			if(info.type == EV_ABS && info.code == ABS_Y)
				y1 = info.value;
			if(x1 !=0 && y1 !=0)
			{
				//如果你的板子是黑色的，那么需要进行坐标转换(1024,600) ---(800,480)
				//x1 = (*x *1.0)/1024 * 800 ;
				//y1 = (*y *1.0)/600  * 480 ;
				bzero(text,1024);
				sprintf(text,"第一次按下 (%d,%d)\n",x1,y1);
				XPRINTF(text);

				break;
			}	
		}
		
		//2、获取松开的坐标值
		while(1)
		{
			//3）读取触摸屏数据 --如果你没有用手触摸屏幕，当前会阻塞在这里
			read(tsfd,&info,sizeof(struct input_event));
			
			//判断当前发生的是触摸屏事件,接着判断触发的是X轴事件
			if(info.type == EV_ABS && info.code == ABS_X)
				x2 = info.value;
			//判断当前发生的是触摸屏事件,接着判断触发的是Y轴事件
			if(info.type == EV_ABS && info.code == ABS_Y)
				y2 = info.value;
			
			//松开的时候，才进行打印  type:1 code:330 value:0
			if(info.type == EV_KEY && info.code == BTN_TOUCH &&  info.value == 0){
				
				//如果你的板子是黑色的，那么需要进行坐标转换(1024,600) ---(800,480)
				//x2 = (*x *1.0)/1024 * 800 ;
				//y2 = (*y *1.0)/600  * 480 ;
				bzero(text,1024);
				sprintf(text,"松开 (%d,%d)\n",x2,y2);
				XPRINTF(text);
				break;
			}
		}
		//3、计算当前的坐标差，判断是否进行了 向上 向下  向左  向右 滑屏
		int valY = y1 - y2;
		int valX = x1 - x2;
		
		if((x2-x1 > 50) && (abs(valY)<30))//判断向右滑屏  abs函数是求绝对值
		{			
			direction = Right;
			break;
		}
		else if((x1-x2 > 50) && (abs(valY)<30))//判断向左滑屏
		{			
			direction = Left;
			break;
		}
		else if((y1-y2 > 50) && (abs(valX)<30))//判断向上滑屏
		{			
			direction = Up;
			break;
		}
		else if((y2-y1 > 50) && (abs(valX)<30))//判断向下滑屏
		{			
			direction = Down;
			break;
		}
		else if((valX<5 && valY<5) || (x2==0 && y2==0))//判断当前是单击
		{
			direction = Click;
			*x = x1;
			*y = y1;
			break;
		}
	}
	
	//5）关闭触摸屏文件
	close(tsfd);
	
	bzero(text,1024);
	sprintf(text,"direction:%d\n",direction);
	XPRINTF(text);
	
	return direction;
}
