#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include "ts.h"
#include "debug.h"

#define BMPPATH	"./res"

int pageIndex=0;//用来控制当前第几页，初始化时是第0页
int pageCount=0;//表示的是总页数

typedef  struct  {   
	char bmpName[256];//图片的路径+名
	int index;//图片的编号
}ElemType_t; 

//1、根据实际的应用场景，设计链表中的结点类型（结构体）
typedef struct node{
	
	ElemType_t data;//数据域 ---此时数据域 就是一个结构体 变量 

	struct node *prev;//指针域 存储上一个结点的地址
	struct node *next;//指针域 存储下一个结点的地址
}listNode_t,*listNode_p ; //listNode_t 相当于 struct node  listNode_p 相当于struct node*

//设计一个相对整条链表而言的头结点
//作用：管理整条链表，存放链表的一些关键性数据
struct list{
	struct node *head;//链表的首结点地址
	struct node *tail;//链表的尾结点地址
	int nodeNumber;//链表的结点数量
	/*其他属性*/
};

//开机界面函数
void interface()
{
	printf("正在开机.\n");
	sleep(1);
	system("clear");
	printf("正在开机..\n");
	sleep(1);
	system("clear");
	printf("正在开机...\n");
	sleep(1);
	system("clear");
	printf("正在开机....\n");
	sleep(1);
	system("clear");
}
/*
new_node:新建一个结点
返回值：
		成功返回 新结点的地址
		失败返回 NULL
*/
struct node* new_node(struct list *listHead,ElemType_t *inputData)
{
	struct node *newNode = malloc(sizeof(struct node));
	if(newNode == NULL)
	{
		printf("malloc newNode error\n");
		return NULL;
	}
	//初始化结点
	strcpy(newNode->data.bmpName,inputData->bmpName) ; 
	newNode->data.index = inputData->index;
	
	newNode->prev = NULL;
	newNode->next = NULL;
	
	//新建一个结点，结点数量加1
	listHead->nodeNumber++;
	
	return newNode;
}
/*
insert_nodeToList_tail: 新建一个值为inputData的结点，插入到链表的尾部中
参数：
		listHead：你要操作的链表的头结点地址
		inputData:新建结点的数据
*/
void insert_nodeToList_tail(struct list *listHead,ElemType_t *inputData)
{
	//printf("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
	if(listHead == NULL)//如果传递过来的链表的头结点是空的
	{
		printf("你传递过来的首结点是NULL的\n");
		return;
	}
	
	//1、新建一个结点
	struct node *newNode = new_node(listHead,inputData);
	if(newNode == NULL)
	{
		printf("new_node error\n");
		return ;
	}
	//链表从无到有,那么新建的这个结点就是链表的首结点
	if(listHead->head == NULL)
	{
		listHead->head = newNode;
		listHead->tail = newNode;
		//当前链表只有一个结点，但是也要进行循环 收尾相连
		newNode->next = newNode;
		newNode->prev = newNode;
	}
	else{//从少到多
		// listHead->tail  就是链表的尾结点
		//第一步 原本链表的尾结点的next指向新建结点
		listHead->tail->next = newNode;
		//第二步
		newNode->prev = listHead->tail;
		//第三步 更新尾结点
		listHead->tail = newNode;
		//第四步  链表的新尾结点 的next指向 首结点 
		listHead->tail->next = listHead->head;
		listHead->head->prev = newNode;//或者 = listHead->tail
		//printf("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
	}
	
}

/*
dirOpt: 在指定的目录dirName下查找后缀名为suffix的所有的文件，并且插入到链表中
参数： 
	listHead:链表头结点的地址
	dirName：你要操作哪个目录，将这个目录的路径+名字传递进来
	suffix:后缀名 ,注意不需要传递 .进来，比如现在记录bmp图片 传递  "bmp"即可
返回值：
	成功返回 加载的文件的数量
	失败返回 -1
*/
int dirOpt(struct list *listHead,char *dirName,char *suffix) //char bmpName[BMP_NUMBER][256]={0};
{
	int fileCount=0; //记录当前加载进来的bmp图片数量
	int i;
	
	//打开目录
	DIR *dirfp = opendir(dirName);
	if(dirfp == NULL)
	{
		perror("opendir error");
		return -1;
	}
	while(1)
	{
		//读取目录
		struct dirent *info = readdir(dirfp);
		if(info == NULL)//如果读取完了 退出
			break;
		
		printf("d_name:%s\n",info->d_name); //info->d_name  ---1.bmp
		
		//去除 . 和 ..     而且 文件 是 普通 文件
		if(info->d_name[0] != '.' && info->d_type == DT_REG)
		{
			//判断当前普通文件是后缀名为.bmp或者.BMP的图片文件
			//1.txt  2.bmp  3.c  4.h 5.BMP
			//思路：文件名字info->d_name中一定有.bmp  或者.BMP 
			//思考有没有一个这样子的函数：在一个字符串（1.bmp）中判断是否包含另外一个字符串(.bmp)
			/*  char *ret1 = strstr(info->d_name, ".bmp"); //strstr 函数在一个字符串中查找指定的字符串
			 char *ret2 = strstr(info->d_name, ".BMP");
			 if(ret1 != NULL || ret2 != NULL) //strstr 如果找到了返回一个地址，没找到返回NULL */
			int ret1 = strcmp(info->d_name+strlen(info->d_name)-3,suffix);
			//int ret2 = strcmp(info->d_name+strlen(info->d_name)-3,"BMP");
			if(ret1 ==0) //|| ret2 == 0) //strcmp比较两个字符串，如果相同返回0
			{
				 //bmpName[0]---> ./reource/1.bmp
				//将图片的路径 和 名字 拼接起来
				char pathName[256]={0};
				sprintf(pathName,"%s/%s",dirName,info->d_name);
				//将拼接起来的bmp图片路径名 作为新结点的数据  插入到链表中
				
				//初始化新结点的数据
				ElemType_t inputData;
				strcpy(inputData.bmpName,pathName);//图片的名字 
				inputData.index = listHead->nodeNumber+1;
				//尾插
				insert_nodeToList_tail(listHead,&inputData);
			}			
		}
		
	}

	//关闭目录文件
	closedir(dirfp);
	
	return fileCount;
}
/*
create_list：从指定的目录中搜索所有的bmp图片，每一张图片的名字作为新结点的数据，插入到链表中
*/
struct list* create_list()
{
	printf("%s start\n",__FUNCTION__);
	//申请链表的头结点内存空间  
	struct list *listHead = malloc(sizeof(struct list));
	if(listHead == NULL)
	{
		printf("malloc list Head error\n");
		return NULL;
	}
	//初始化头结点
	listHead->head = NULL;
	listHead->tail = NULL;
	listHead->nodeNumber = 0;//初始化时，没有数据结点，所以为0
	printf("malloc listHead success\n");
	
	//从指定的目录中自动加载所有的bmp图片
	dirOpt(listHead,BMPPATH,"bmp");
	
	printf("%s end\n",__FUNCTION__);
	//返回整条链表的头结点
	return listHead;
}
/*
print_allToList:打印链表中每个结点的数据
参数：
	listHead---》链表的头结点地址
*/
void print_allToList(struct list *listHead)
{
	//printf("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
	struct node *p=listHead->head;
	int i;
	printf("-------------当前链表结点数量为%d---------------\n",listHead->nodeNumber);
	
	
	//遍历链表中的每个结点
	while(p) 
	{
		printf("[%d]:%s\n",p->data.index,p->data.bmpName);
		p = p->next;
		
		if(p == listHead->head)//退出条件 
			break;
	}

	printf("\n");
}

void maxShowMode()
{
	
	
}
/*
maxShowBmp:在链表listHead中，根据图片编号inputIndex找到对应的图片名字，全屏放大显示
*/
void maxShowBmp(struct list *listHead,int inputIndex)//全屏放大图片
{
	struct node *p=listHead->head;
	int i;
	
	//遍历链表中的每个结点
	while(p) 
	{
		if(p->data.index == inputIndex)//找到了inputIndex编号所在的结点
			break;
			
		p = p->next;
		
		if(p == listHead->head)//退出条件 
			break;
	}	
	
	//没找到
	if(p==NULL)
		return;
	
	//找到了 p结点就是inputIndex编号所在的结点 p->data.bmpName
	//显示图片
	showBmp(p->data.bmpName);

	//接下来就是进入全屏放大模式，上一张  下一张  退出
	maxShowMode();	
}
void prevPage()//上一页
{
	printf("上一页\n");
	
	pageIndex--;
}
void nextPage()//下一页
{
	printf("下一页\n");
	pageIndex++;
}

int main()
{	
	printf("main\n");
	
	//初始化
	if(!initLcdAndMmap()) //if(initLcdAndMmap() = false)
	{
		//如果初始化失败
		printf("initLcdAndMmap error\n");
		return -1;
	}
	initLog();//初始化调试文本
	
	//开机界面
	//interface();
	//从文本里面 初始化
	struct list*listHead = create_list();
	
	bool exitFlag=false;
	int ts_x,ts_y;
	
	
	int index = 0;//表示你点击的区域 换算成 最终链表中图片的编号
	
	while(1)
	{
		//显示主界面  main.bmp
		showBmp("./res/bg/main.bmp");
		
		//在主界面上显示四张缩小四倍后的图片
		//先根据当前页数，算出当前要显示链表中四张图片的起始图片编号
		index = pageIndex*4 + 1;
		//再根据图片编号把四张图片的名字 确定下来
		struct node *p=listHead->head;

		//遍历链表中的每个结点
		while(p) 
		{
			if(p->data.index == index)//找到了index编号所在的结点
				break;
				
			p = p->next;
			
			if(p == listHead->head)//退出条件 
				break;
		}	
		
		printf("图片编号从这里开始显示四张图片:%d\n",index);
		printf("bmpName:%s\n",p->data.bmpName);
		
		showBmp_solv_xy(p->data.bmpName,100,100);//位置1 pos1
		showBmp_solv_xy(p->next->data.bmpName,500,100);//位置2 pos2
		showBmp_solv_xy(p->next->next->data.bmpName,100,300);//位置3 pos3
		showBmp_solv_xy(p->next->next->next->data.bmpName,500,300);//位置4 pos4
		
		int modePos=-1;//表示你点击的位置 
		
		//调用触摸屏 
		ts_func(&ts_x,&ts_y);
		
		printf("ts_x:%d ts_y:%d\n",ts_x,ts_y);
		
		//判断坐标 
		if(ts_x>=100 && ts_x<300 && ts_y>=100 && ts_y<220)//位置1 pos1
		{	
			modePos = 1;
		}
		else if(ts_x>=500 && ts_x<700 && ts_y>=100 && ts_y<220)//位置2 pos2
		{
			modePos = 2;
		}
		else if(ts_x>=100 && ts_x<300 && ts_y>=300 && ts_y<420)//位置3 pos3
		{
			modePos = 3;
		}
		else if(ts_x>=500 && ts_x<700  && ts_y>=300 && ts_y<420)//位置4 pos4
		{
			modePos = 4;
		}
		else if(ts_x>=0 && ts_x<100 && ts_y>=420 && ts_y<480)//上一页 
		{
			modePos = 5;
		}
		else if(ts_x>=700 && ts_x<800 && ts_y>=420 && ts_y<480)//下一页 
		{
			modePos = 6;
		}
		else{ //点击的是其他区域 无效
			printf("你点击的是无效区域\n");
			continue;
		}
		
		//模式选择
		switch(modePos)
		{
			case 1:
			case 2:
			case 3:
			case 4:
					//根据公式算出 当前点击区域 的图片编号
					index = pageIndex*4 + modePos;
					maxShowBmp(listHead,index);//全屏放大图片
					break;
			case 5:
					prevPage();//上一页
					break;
			case 6:
					nextPage();//下一页
					break;
		}
		
	}

	//解除初始化
	uinitLcd();
	
	return 0;
}