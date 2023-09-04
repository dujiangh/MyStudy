#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <stdbool.h>

#define UINT  unsigned short
#define DWORD unsigned  int
#define LONG unsigned  int
#define WORD unsigned short

#define BMP_WIDTH 	800
#define BMP_HEIGHT 	480


int lcdfd;
int *lcd_p;

//前面14个字节 文件信息头
typedef struct tagBITMAPFILEHEADER{
	UINT bfType;  //2
	DWORD bfSize; //DWORD 4
	UINT bfReserved1; //2
	UINT bfReserved2; //2
	DWORD bfOffBits; //DWORD 4
}BITMAPFILEHEADER;

//40个字节 位图信息头
typedef struct tagBITMAPINFOHEADER{ /* bmih */
	DWORD biSize; //DWORD 4
	LONG biWidth; //4  图像的宽度
	LONG biHeight;//4  图像的高度
	WORD biPlanes; //2
	WORD biBitCount;//2 
	DWORD biCompression; //DWORD 4
	DWORD biSizeImage; //DWORD 4
	LONG biXPelsPerMeter;//4
	LONG biYPelsPerMeter;//4
	DWORD biClrUsed; //DWORD 4
	DWORD biClrImportant; //DWORD 4
}BITMAPINFOHEADER;

//得到bmp图片的宽度
int getBmpWidth(BITMAPINFOHEADER *head)
{
	//printf("Width:%d\n",head->biWidth);//图像的宽度
	return head->biWidth;
}
//得到bmp图片的高度
int getBmpHeight(BITMAPINFOHEADER *head)
{
	//printf("Height:%d\n",head->biHeight);//图像的高度
	return head->biHeight;
}
//得到bmp图片的大小
int getBmpSize(BITMAPINFOHEADER *head)
{
	//printf("fileSize:%d\n",head->biSizeImage+54);//文件的大小
	return head->biSizeImage+54;
}
//初始化液晶屏 和 内存映射
bool initLcdAndMmap()
{
	//1、打开液晶屏文件  
	lcdfd = open("/dev/fb0",O_RDWR);
	if(lcdfd == -1)
	{
		perror("open lcd error");//perror将错误的原因打印出来
		return false;
	}
	//将液晶屏文件 通过内存映射mmap的方式 映射到虚拟内存空间的某一块空间上，得到这一块内存的起始地址，后续我们操作这片内存空间，就相当于直接操作液晶屏文件
	lcd_p = mmap(NULL, //你要映射的内存空间的起始地址，为NULL系统自动给你分配
		 800*480*4,//你要映射的内存空间的大小
		 PROT_READ|PROT_WRITE,//映射的权限 
		 MAP_SHARED,//1、进程共享  2、对应的文件会同步发生变化 	
		 lcdfd,//映射液晶屏文件
		 0//偏移量 ，默认为0
		);
	if(lcd_p == MAP_FAILED)	
	{
		printf("mmap lcd error\n");
		return false;
	}
	
	return true;
}
//解除初始化
void uinitLcd()
{
	//解除映射
	munmap(lcd_p, 800*480*4);
	//5、关闭液晶屏文件、bmp图片文件	
	close(lcdfd);
}

//在(0,0)的位置上显示一张800*480的图片
int showBmp(char *bmpPath)
{
	int i,j;
	int tempbuf[800*480];
	int lcdbuf[800*480];
	
	//2、打开bmp图片文件  
	int bmpfd = open(bmpPath,O_RDONLY);
	if(bmpfd == -1)
	{
		perror("open bmpfile error");//perror将错误的原因打印出来
		return -1;
	}
	
	//读取bmp图片的文件头 + 信息头数据
	BITMAPFILEHEADER fileHead;
	BITMAPINFOHEADER infoHead;
	
	read(bmpfd,&fileHead,14);//读取14个字节
	read(bmpfd,&infoHead,40);//读取40个字节
	
	//判断是否宽度等于 800 
	if(BMP_WIDTH != getBmpWidth(&infoHead))
	{
		printf("你要打印的图片宽度 不等于 800 请重新修改图片 error\n");
		//5、bmp图片文件	
		close(bmpfd);
		return -1;
	}
	//判断是否高度等于 480
	if(BMP_HEIGHT != getBmpHeight(&infoHead))
	{
		printf("你要打印的图片高度 不等于 480 请重新修改图片 error\n");
		//5、bmp图片文件	
		close(bmpfd);
		return -1;
	}
	
	//3、读取bmp图片的数据
	char bmpbuf[800*480*3]={0};
	read(bmpfd,bmpbuf,800*480*3);
	//将bmp图片每个像素点的 B G R -->A R G  B
	for(i=0,j=0; i<800*480;i++,j+=3)
	{
		tempbuf[i] = 0x00<<24 | bmpbuf[j+2]<<16 | bmpbuf[j+1]<<8 | bmpbuf[j];
	}
	//上下180度颠倒 
	for(j=0; j<480; j++)
	{
		for(i=0; i<800; i++)
		{
			lcd_p[j*800+i] = tempbuf[(479-j)*800+i];
		}
	}
	
	close(bmpfd);
}

//在(0,0)的位置上显示一张800*480切割4倍后的200*120的图片
int showBmp_solv(char *bmpPath)
{
	int i,j;
	int tempbuf[800*480];
	int lcdbuf[800*480];
	int minbuf[200*120];
	
	//2、打开bmp图片文件  
	int bmpfd = open(bmpPath,O_RDONLY);
	if(bmpfd == -1)
	{
		perror("open bmpfile error");//perror将错误的原因打印出来
		return -1;
	}
	
	//读取bmp图片的文件头 + 信息头数据
	BITMAPFILEHEADER fileHead;
	BITMAPINFOHEADER infoHead;
	
	read(bmpfd,&fileHead,14);//读取14个字节
	read(bmpfd,&infoHead,40);//读取40个字节
	
	//判断是否宽度等于 800 
	if(BMP_WIDTH != getBmpWidth(&infoHead))
	{
		printf("你要打印的图片宽度 不等于 800 请重新修改图片 error\n");
		//5、bmp图片文件	
		close(bmpfd);
		return -1;
	}
	//判断是否高度等于 480
	if(BMP_HEIGHT != getBmpHeight(&infoHead))
	{
		printf("你要打印的图片高度 不等于 480 请重新修改图片 error\n");
		//5、bmp图片文件	
		close(bmpfd);
		return -1;
	}
	
	//3、读取bmp图片的数据
	char bmpbuf[800*480*3]={0};
	read(bmpfd,bmpbuf,800*480*3);
	//将bmp图片每个像素点的 B G R -->A R G  B
	for(i=0,j=0; i<800*480;i++,j+=3)
	{
		tempbuf[i] = 0x00<<24 | bmpbuf[j+2]<<16 | bmpbuf[j+1]<<8 | bmpbuf[j];
	}
	//上下180度颠倒 
	for(j=0; j<480; j++)
	{
		for(i=0; i<800; i++)
		{
			lcdbuf[j*800+i] = tempbuf[(479-j)*800+i];
		}
	}
	//切割算法 
	//思路：每隔4行取一行像素，在每一行中，每4个像素点取一个像素点
	//将lcdbuf --> 800*480  切割成  200*120 存储到 minbuf
	for(j=0; j<120; j++)
	{
		for(i=0; i<200; i++)
		{
			minbuf[j*200+i] = lcdbuf[j*4*800+i*4]; //i == 0 4 8 12 16
		}
	}
	//切割之后的数据存储在 minbuf ---200*120
	//显示 ，赋值到 lcd_p
	for(j=0; j<120; j++)
	{
		for(i=0; i<200; i++)
		{
			lcd_p[j*800+i] = minbuf[j*200+i];
		}
	}
	
	close(bmpfd);
}
//在(posX,posY)的位置上显示一张800*480切割4倍后的200*120的图片
//
int showBmp_solv_xy(char *bmpPath,int posX,int posY)
{
	int i,j;
	int tempbuf[800*480];
	int lcdbuf[800*480];
	int minbuf[200*120];
	
	//2、打开bmp图片文件  
	int bmpfd = open(bmpPath,O_RDONLY);
	if(bmpfd == -1)
	{
		perror("open bmpfile error");//perror将错误的原因打印出来
		return -1;
	}
	
	//读取bmp图片的文件头 + 信息头数据
	BITMAPFILEHEADER fileHead;
	BITMAPINFOHEADER infoHead;
	
	read(bmpfd,&fileHead,14);//读取14个字节
	read(bmpfd,&infoHead,40);//读取40个字节
	
	//判断是否宽度等于 800 
	if(BMP_WIDTH != getBmpWidth(&infoHead))
	{
		printf("你要打印的图片宽度 不等于 800 请重新修改图片 error\n");
		//5、bmp图片文件	
		close(bmpfd);
		return -1;
	}
	//判断是否高度等于 480
	if(BMP_HEIGHT != getBmpHeight(&infoHead))
	{
		printf("你要打印的图片高度 不等于 480 请重新修改图片 error\n");
		//5、bmp图片文件	
		close(bmpfd);
		return -1;
	}
	
	//3、读取bmp图片的数据
	char bmpbuf[800*480*3]={0};
	read(bmpfd,bmpbuf,800*480*3);
	//将bmp图片每个像素点的 B G R -->A R G  B
	for(i=0,j=0; i<800*480;i++,j+=3)
	{
		tempbuf[i] = 0x00<<24 | bmpbuf[j+2]<<16 | bmpbuf[j+1]<<8 | bmpbuf[j];
	}
	//上下180度颠倒 
	for(j=0; j<480; j++)
	{
		for(i=0; i<800; i++)
		{
			lcdbuf[j*800+i] = tempbuf[(479-j)*800+i];
		}
	}
	//切割算法 
	//思路：每隔4行取一行像素，在每一行中，每4个像素点取一个像素点
	//将lcdbuf --> 800*480  切割成  200*120 存储到 minbuf
	for(j=0; j<120; j++)
	{
		for(i=0; i<200; i++)
		{
			minbuf[j*200+i] = lcdbuf[j*4*800+i*4]; //i == 0 4 8 12 16
		}
	}
	//切割之后的数据存储在 minbuf ---200*120
	//显示 ，赋值到 lcd_p
	//在指定的位置(posX,posY)上显示
	for(j=0; j<120; j++)
	{
		for(i=0; i<200; i++)
		{
			//lcd_p[posY*800+posX+j*800+i] = minbuf[j*200+i];
			lcd_p[(posY+j)*800+i+posX] = minbuf[j*200+i];
		}
	}
	
	close(bmpfd);
}
