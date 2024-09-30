## GPIO
### GPIO的八大模式
- 输入模式
  - 模拟输入
  - 上拉输入
  - 下拉输入
  - 浮空输入
- 输出模式
  - 推挽输出
  - 开漏输出
  - 推挽复用输出
  - 开漏复用输出


输入的三种模式的区别
| 输入模式 | 应用场合                                                                                            |
| -------- | --------------------------------------------------------------------------------------------------- |
| 浮空输入 | 常用的模式,<br/>优点:输入阻抗高,MCU采集的状态是单纯的外部信号,<br/>缺点:无IO信号时,读取的端口不确定 |
| 上拉输入 | 当输入的信号比较弱,任意受到干扰不稳定,选择上拉                                                      |
| 下拉输入 | 一般不常用                                                                                          |

开漏输出和推挽输出区别
| 输出模式 | 特点                                                                                                 | 应用场合                                        |
| -------- | ---------------------------------------------------------------------------------------------------- | ----------------------------------------------- |
| 开漏输出 | 优点:可实现电平转换,输出电平取决于上拉电阻电源.可以线于<br/>缺点:高电平驱动能力差,取决于内部上拉电阻 | 一般用于I2C,SMBUS等通信需要线与功能的总线电路中 |
| 推挽输出 | 优点:输出高低电平与电源电压基本没有压差,可以提供较强的高电平与低电平驱动能力<br/>缺点:不支持线与功能 | 可以直接驱动小于20mA的小功率外设,如发光二极管等 |

推挽输出时,驱动能力与芯片的IO电流特性有关

### GPIO常用寄存器

<table>
  <tbody>
    <!-- 标题 -->
    <tr>
      <th>寄存器类型</th>
      <th>分类</th>
      <th>描述</th>
    </tr>
    <!-- 内容主题 -->
<tr>
<td rowspan="4">配置寄存器
</td><td>端口模式寄存器(GPIOx_MODER)</td><td>控制GPIOx的工作模式</td></tr>
<tr><td>端口输出类型寄存器(GPIOx_OTYPER)</td><td>控制GPIOx的输出类型</td></tr>
<tr><td>端口输出速度寄存器(GPIOx_OSPEEDR)</td><td>控制GPIOx的输出速度</td></tr>
<tr><td>端口上拉/下拉寄存器(GPIOx_PUPDR)</td><td>控制GPIOx的上拉/下拉</td></tr>
<tr><td rowspan="2">数据寄存器
</td><td>端口输入寄存器(GPIOx_IDR)</td><td>读取GPIOx的输入数据</td></tr>
<tr><td>端口输出寄存器(GPIOx_ODR)</td><td>控制GPIOx的输出电平</td></tr>
<tr><td rowspan="1">置位/复位寄存器
</td><td>端口置位/复位寄存器(GPIOx_BSSR)</td><td>设置GPIO端口的输出位是1还是0</td></tr>
<tr><td rowspan="1">复用寄存器
</td><td>复用功能寄存器(GPIOx_AFRL\AFRH)</td><td>设置IO口的复用功能</td></tr>
</table>

### GPIO的相关宏/枚举
```c
typedef struct
{
  uint32_t GPIO_Pin; //端口引脚 
  GPIOMode_TypeDef GPIO_Mode;//GPIO的端口模式
  GPIOSpeed_TypeDef GPIO_Speed;// GPIO的端口输出速度取值
  GPIOOType_TypeDef GPIO_OType;//输出模式
  GPIOPuPd_TypeDef GPIO_PuPd;// GPIO的上拉/下拉寄存器
}GPIO_InitTypeDef;
// 端口引脚有
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /* All pins selected */
// GPIO的端口模式取值
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO 输入模式 */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO 输出模式 */
  GPIO_Mode_AF   = 0x02, /*!< GPIO 复用 */
  GPIO_Mode_AN   = 0x03  /*!< GPIO 模拟 */
}GPIOMode_TypeDef;
// 输出模式
typedef enum
{ 
  GPIO_OType_PP = 0x00,//推挽模式
  GPIO_OType_OD = 0x01//开漏模式
}GPIOOType_TypeDef;
// GPIO的端口输出速度取值
typedef enum
{ 
  GPIO_Low_Speed     = 0x00, /*!< Low speed    */
  GPIO_Medium_Speed  = 0x01, /*!< Medium speed */
  GPIO_Fast_Speed    = 0x02, /*!< Fast speed   */
  GPIO_High_Speed    = 0x03  /*!< High speed   */
}GPIOSpeed_TypeDef;
// GPIO的上拉/下拉寄存器
typedef enum
{ 
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;

```
### 相关函数
#### 初始化对应引脚
```c

/*  Function used to set the GPIO configuration to the default reset state ****/
void GPIO_DeInit(GPIO_TypeDef* GPIOx);

/* Initialization and Configuration functions *********************************/
// 使用GPIO_InitTypeDef结构体初始化GPIOx
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO Read and Write functions **********************************************/
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
//设置某个引脚 置位/复位
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* GPIO Alternate functions configuration function ****************************/
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);

```

```c

// 使用该函数初始化某个端口引脚
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

// 例如
GPIO_InitTypeDef GPIO_InitStructure;
// 配置GPIOF的9号码引脚为输出
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      // 输出模式
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽模式
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   // 无上下拉电阻

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // 对应LED0的控制
GPIO_Init(GPIOF, &GPIO_InitStructure);//使用GPIO_InitStructure初始化F端口的8号引脚
```
#### 位带操作
```c
/* 
GPIOx
    端口
GPIO_Pin
    端口引脚
BitVal
    写为0/1
 */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
// 比GPIO_SetBits()和GPIO_ResetBits()方便
```


## 练习
由于led一边是vcc,所以说要复位为低电平的时候才能让灯亮
### 灯每隔1秒闪烁

main.c
```c

#include "stdio.h"
#include "stdint.h"
#include "led.h"

int main(void)
{
	initLed();
	showLed();
	return 0;
}

```
led.h
```c

#include "stdint.h"
// #include "delay.h"
// #include "stm32f4xx.h"

#ifndef __LED_H__
#define __LED_H__
// rcc时钟基地址
#define RCC_AHB1_BASE (0x40023800)
// RCC AHB1 外设时钟使能寄存器
#define RCC_AHB1ENR_ADDR *((uint32_t *)(RCC_AHB1_BASE + 0x30))

// GPIO F 端口基地址
#define GPIOF_BASE (0x40021400)
// GPIO 端口模式寄存器
#define GPIOF_MODER *((uint32_t *)(GPIOF_BASE + 0x00))
// GPIO 端口输出类型寄存器
#define GPIOF_OTYPER *((uint32_t *)(GPIOF_BASE + 0x04))
// GPIO 端口输出速度寄存器
#define GPIOF_OSPEEDR *((uint32_t *)(GPIOF_BASE + 0x08))
// GPIO 端口上拉/下拉寄存器
#define GPIOF_PUPDR *((uint32_t *)(GPIOF_BASE + 0x0C))
// GPIO 端口输出数据寄存器
#define GPIOF_ODR *((uint32_t *)(GPIOF_BASE + 0x14))
// GPIO 端口置位/复位寄存器 
#define GPIOF_BSRR *((uint32_t *)(GPIOF_BASE + 0x18))

// 初始化灯,使能
void initLed(void);
// 控制对应灯的开关
void showLed(void);
// 延时
void daley(void);
#endif

```
led.c
```c
#include "led.h"
// 初始化灯,使能
void initLed(void)
{
    // 先开启对应端口使能
    RCC_AHB1ENR_ADDR |= 1 << 5;
}
// 控制对应灯的开关
void showLed(void)
{
    // 2、输出模式      01 通用输出模式
    GPIOF_MODER &= ~(0x01 << 19); // 19位清0
    GPIOF_MODER |= (0x01 << 18);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOF_OTYPER &= ~(0x01 << 9); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOF_OSPEEDR |= (0x01 << 19); // 19位置1
    GPIOF_OSPEEDR |= (0x01 << 18); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOF_PUPDR &= ~(0x01 << 19); // 19位清0
    GPIOF_PUPDR &= ~(0x01 << 18); // 18位清0

    //GPIOF_BSRR |= (1 << 9);
    while (1)
    {
        //给引脚9的输出数据寄存器置位
        GPIOF_BSRR |= (0x01 << 9);
        daley();
        //给引脚9的输出数据寄存器复位	
        GPIOF_BSRR |= (0x01 << 9 + 16);
        daley();
    }
}

// 延时
void daley(void)
{
    int i = 0;
    for (i = 0; i <  10000000; i++)
        ;
}


```

### 两个灯循环闪烁

led.c
```c

#include "led.h"
// 初始化灯,使能
void initLed(void)
{
    // 先开启对应端口使能
    RCC_AHB1ENR_ADDR |= 1 << 5;
}
// 控制对应灯的开关
void showLed(void)
{
    // 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOF_MODER &= ~(0x05 << 19); // 19位清0
    GPIOF_MODER |= (0x05 << 18);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOF_OTYPER &= ~(0x03 << 9); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOF_OSPEEDR |= (0x05 << 19); // 19位置1
    GPIOF_OSPEEDR |= (0x05 << 18); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOF_PUPDR &= ~(0x05 << 19); // 19位清0
    GPIOF_PUPDR &= ~(0x05 << 18); // 18位清0

    // 给引脚9的输出数据寄存器置位
    GPIOF_BSRR |= (0x01 << 9);
    // 给引脚10的输出数据寄存器置位 
    GPIOF_BSRR |= (0x01 << 10);
    while (1)
    {
        // 给引脚9的输出数据寄存器复位
        GPIOF_BSRR |= (0x01 << 9 + 16);
        daley(7000);
        // 给引脚9的输出数据寄存器置位
        GPIOF_BSRR |= (0x01 << 9);
        // 给引脚10的输出数据寄存器复位
        GPIOF_BSRR |= (0x01 << 10 + 16);
        daley(7000);
        // 给引脚10的输出数据寄存器置位
        GPIOF_BSRR |= (0x01 << 10);
    }
}

// 延时
void daley(int mtime)
{
    int i = 0;
    for (i = 0; i < 1000 * mtime; i++)
        ;
}

```
### 四个灯的跑马灯
led.h
```c

#include "stdint.h"
// #include "delay.h"
// #include "stm32f4xx.h"

#ifndef __LED_H__
#define __LED_H__
// rcc时钟基地址
#define RCC_AHB1_BASE (0x40023800)
// RCC AHB1 外设时钟使能寄存器
#define RCC_AHB1ENR_ADDR *((uint32_t *)(RCC_AHB1_BASE + 0x30))

// GPIO F 端口基地址
#define GPIOF_BASE (0x40021400)
// GPIO 端口模式寄存器
#define GPIOF_MODER *((uint32_t *)(GPIOF_BASE + 0x00))
// GPIO 端口输出类型寄存器
#define GPIOF_OTYPER *((uint32_t *)(GPIOF_BASE + 0x04))
// GPIO 端口输出速度寄存器
#define GPIOF_OSPEEDR *((uint32_t *)(GPIOF_BASE + 0x08))
// GPIO 端口上拉/下拉寄存器
#define GPIOF_PUPDR *((uint32_t *)(GPIOF_BASE + 0x0C))
// GPIO 端口输出数据寄存器
#define GPIOF_ODR *((uint32_t *)(GPIOF_BASE + 0x14))
// GPIO 端口置位/复位寄存器 
#define GPIOF_BSRR *((uint32_t *)(GPIOF_BASE + 0x18))

// GPIO E 端口基地址
#define GPIOE_BASE (0x40021000)
// GPIO 端口模式寄存器
#define GPIOE_MODER *((uint32_t *)(GPIOE_BASE + 0x00))
// GPIO 端口输出类型寄存器
#define GPIOE_OTYPER *((uint32_t *)(GPIOE_BASE + 0x04))
// GPIO 端口输出速度寄存器
#define GPIOE_OSPEEDR *((uint32_t *)(GPIOE_BASE + 0x08))
// GPIO 端口上拉/下拉寄存器
#define GPIOE_PUPDR *((uint32_t *)(GPIOE_BASE + 0x0C))
// GPIO 端口输出数据寄存器
#define GPIOE_ODR *((uint32_t *)(GPIOE_BASE + 0x14))
// GPIO 端口置位/复位寄存器 
#define GPIOE_BSRR *((uint32_t *)(GPIOE_BASE + 0x18))

// 初始化灯,使能
void initLed(void);
// 控制对应灯的开关
void showLed(void);
// 延时
void daley(int mtime);
#endif

```
led.c
```c

#include "led.h"
// 初始化灯,使能
void initLed(void)
{
    // 先开启对应端口使能  f端口
    RCC_AHB1ENR_ADDR |= 1 << 5;
    // 先开启对应端口使能  e端口
    RCC_AHB1ENR_ADDR |= 1 << 4;
}
// 控制对应灯的开关
void showLed(void)
{
    // f端口 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOF_MODER &= ~(0x05 << 19); // 19位清0
    GPIOF_MODER |= (0x05 << 18);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOF_OTYPER &= ~(0x03 << 9); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOF_OSPEEDR |= (0x05 << 19); // 19位置1
    GPIOF_OSPEEDR |= (0x05 << 18); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOF_PUPDR &= ~(0x05 << 19); // 19位清0
    GPIOF_PUPDR &= ~(0x05 << 18); // 18位清0
    
    // e端口 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOE_MODER &= ~(0x05 << 27); // 19位清0
    GPIOE_MODER |= (0x05 << 26);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOE_OTYPER &= ~(0x03 << 13); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOE_OSPEEDR |= (0x05 << 27); // 19位置1
    GPIOE_OSPEEDR |= (0x05 << 26); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOE_PUPDR &= ~(0x05 << 27); // 19位清0
    GPIOE_PUPDR &= ~(0x05 << 26); // 18位清0

    // f端口 给引脚9的输出数据寄存器置位
    GPIOF_BSRR |= (0x01 << 9);
    // f端口 给引脚10的输出数据寄存器置位 
    GPIOF_BSRR |= (0x01 << 10);
    
    // e端口 给引脚13的输出数据寄存器置位
    GPIOE_BSRR |= (0x01 << 13);
    // e端口 给引脚14的输出数据寄存器置位 
    GPIOE_BSRR |= (0x01 << 14);
    while (1)
    {
        // f端口 给引脚9的输出数据寄存器复位
        GPIOF_BSRR |= (0x01 << 9 + 16);
        daley(7000);
        // f端口 给引脚9的输出数据寄存器置位
        GPIOF_BSRR |= (0x01 << 9);
        // f端口 给引脚10的输出数据寄存器复位
        GPIOF_BSRR |= (0x01 << 10 + 16);
        daley(7000);
        // f端口 给引脚10的输出数据寄存器置位
        GPIOF_BSRR |= (0x01 << 10);


        //  e端口  给引脚9的输出数据寄存器复位
        GPIOE_BSRR |= (0x01 << 13 + 16);
        daley(7000);
        //  e端口  给引脚9的输出数据寄存器置位
        GPIOE_BSRR |= (0x01 << 13);
        //  e端口  给引脚10的输出数据寄存器复位
        GPIOE_BSRR |= (0x01 << 14 + 16);
        daley(7000);
        //  e端口  给引脚10的输出数据寄存器置位
        GPIOE_BSRR |= (0x01 << 14);
    }
}

// 延时
void daley(int mtime)
{
    int i = 0;
    for (i = 0; i < 1000 * mtime; i++)
        ;
}

```
### 按键控制点灯 
按下按键1关闭灯,再按就开启灯

main.c
```c

// #define USE_STDPERIPH_DRIVER

#include "stdio.h"
#include "stdint.h"
#include "led.h"
// #include "stm32f4xx.h"

int main(void)
{
	int i = 0;
	// 初始化led(使能)
	initLed();
	for (i = 0; i < 6; i++)
	{
		if (i == 4)
		{
			i = 0;
		}
		showLed(i % 4, 7000);
	}
	return 0;
}

```
led.h
```c

#include "stdint.h"
// #include "delay.h"
// #include "stm32f4xx.h"

#ifndef __LED_H__
#define __LED_H__
// rcc时钟基地址
#define RCC_AHB1_BASE (0x40023800)
// RCC AHB1 外设时钟使能寄存器
#define RCC_AHB1ENR_ADDR *((uint32_t *)(RCC_AHB1_BASE + 0x30))

// GPIO F 端口基地址
#define GPIOF_BASE (0x40021400)
// GPIO 端口模式寄存器
#define GPIOF_MODER *((uint32_t *)(GPIOF_BASE + 0x00))
// GPIO 端口输出类型寄存器
#define GPIOF_OTYPER *((uint32_t *)(GPIOF_BASE + 0x04))
// GPIO 端口输出速度寄存器
#define GPIOF_OSPEEDR *((uint32_t *)(GPIOF_BASE + 0x08))
// GPIO 端口上拉/下拉寄存器
#define GPIOF_PUPDR *((uint32_t *)(GPIOF_BASE + 0x0C))
// GPIO 端口输出数据寄存器
#define GPIOF_ODR *((uint32_t *)(GPIOF_BASE + 0x14))
// GPIO 端口置位/复位寄存器 
#define GPIOF_BSRR *((uint32_t *)(GPIOF_BASE + 0x18))

// GPIO E 端口基地址
#define GPIOE_BASE (0x40021000)
// GPIO 端口模式寄存器
#define GPIOE_MODER *((uint32_t *)(GPIOE_BASE + 0x00))
// GPIO 端口输出类型寄存器
#define GPIOE_OTYPER *((uint32_t *)(GPIOE_BASE + 0x04))
// GPIO 端口输出速度寄存器
#define GPIOE_OSPEEDR *((uint32_t *)(GPIOE_BASE + 0x08))
// GPIO 端口上拉/下拉寄存器
#define GPIOE_PUPDR *((uint32_t *)(GPIOE_BASE + 0x0C))
// GPIO 端口输出数据寄存器
#define GPIOE_ODR *((uint32_t *)(GPIOE_BASE + 0x14))
// GPIO 端口置位/复位寄存器 
#define GPIOE_BSRR *((uint32_t *)(GPIOE_BASE + 0x18))

// GPIO A 端口基地址
#define GPIOA_BASE (0x40020000)
// GPIO 端口模式寄存器
#define GPIOA_MODER *((uint32_t *)(GPIOA_BASE + 0x00))
// GPIO 端口输出类型寄存器
#define GPIOA_OTYPER *((uint32_t *)(GPIOA_BASE + 0x04))
// GPIO 端口输出速度寄存器
#define GPIOA_OSPEEDR *((uint32_t *)(GPIOA_BASE + 0x08))
// GPIO 端口上拉/下拉寄存器
#define GPIOA_PUPDR *((uint32_t *)(GPIOA_BASE + 0x0C))
//GPIO 端口输入数据寄存器
#define GPIOA_IDR *((uint32_t *)(GPIOA_BASE + 0x10))
// GPIO 端口输出数据寄存器
#define GPIOA_ODR *((uint32_t *)(GPIOA_BASE + 0x14))
// GPIO 端口置位/复位寄存器 
#define GPIOA_BSRR *((uint32_t *)(GPIOA_BASE + 0x18))

// 初始化灯,使能
void initLed(void);
// 控制对应灯的开关
void showLed(int i,int time);
// 延时
void daley(int mtime);
#endif

```
led.c
```c

#include "led.h"
// 初始化灯,使能
void initLed(void)
{
    // 先开启对应端口使能  f端口
    RCC_AHB1ENR_ADDR |= 1 << 5;
    // 先开启对应端口使能  e端口
    RCC_AHB1ENR_ADDR |= 1 << 4;
    // 先开启对应端口使能  A端口
    RCC_AHB1ENR_ADDR |= 1 << 0;

    // f端口 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOF_MODER &= ~(0x05 << 19); // 19位清0
    GPIOF_MODER |= (0x05 << 18);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOF_OTYPER &= ~(0x03 << 9); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOF_OSPEEDR |= (0x05 << 19); // 19位置1
    GPIOF_OSPEEDR |= (0x05 << 18); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOF_PUPDR &= ~(0x05 << 19); // 19位清0
    GPIOF_PUPDR &= ~(0x05 << 18); // 18位清0

    // e端口 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOE_MODER &= ~(0x05 << 27); // 19位清0
    GPIOE_MODER |= (0x05 << 26);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOE_OTYPER &= ~(0x03 << 13); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOE_OSPEEDR |= (0x05 << 27); // 19位置1
    GPIOE_OSPEEDR |= (0x05 << 26); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOE_PUPDR &= ~(0x05 << 27); // 19位清0
    GPIOE_PUPDR &= ~(0x05 << 26); // 18位清0

    // f端口 给引脚9的输出数据寄存器置位
    GPIOF_BSRR |= (0x01 << 9);
    // f端口 给引脚10的输出数据寄存器置位
    GPIOF_BSRR |= (0x01 << 10);

    // e端口 给引脚13的输出数据寄存器置位
    GPIOE_BSRR |= (0x01 << 13);
    // e端口 给引脚14的输出数据寄存器置位
    GPIOE_BSRR |= (0x01 << 14);

    // 开关的配置 第0个按键
    GPIOA_MODER &= ~0x03; // 清0,默认输入模式
    GPIOA_PUPDR |= 0x01; // 上拉电阻
    GPIOA_PUPDR &= ~0x02; // 上拉电阻
}
// 控制对应灯的开关
void showLed(int i, int time)
{
    static int is_show = 0;
    // static int butt=0;
    //高电平 开关断开
    //低电平,开关按下
    if (!(GPIOA_IDR & 0x01))
    {
        // 等待开关松开
        while (!(GPIOA_IDR & 0x01));
        is_show++;
        is_show %= 2;
        
    }
    if (is_show==0)
        switch (i)
        {
        case 0:
            // f端口 给引脚9的输出数据寄存器复位
            GPIOF_BSRR |= (0x01 << 9 + 16);
            daley(time);
            // f端口 给引脚9的输出数据寄存器置位
            GPIOF_BSRR |= (0x01 << 9);
            break;
        case 1:
            // f端口 给引脚10的输出数据寄存器复位
            GPIOF_BSRR |= (0x01 << 10 + 16);
            daley(time);
            // f端口 给引脚10的输出数据寄存器置位
            GPIOF_BSRR |= (0x01 << 10);
            break;
        case 2:
            //  e端口  给引脚9的输出数据寄存器复位
            GPIOE_BSRR |= (0x01 << 13 + 16);
            daley(time);
            //  e端口  给引脚9的输出数据寄存器置位
            GPIOE_BSRR |= (0x01 << 13);
            break;
        case 3:
            //  e端口  给引脚10的输出数据寄存器复位
            GPIOE_BSRR |= (0x01 << 14 + 16);
            daley(time);
            //  e端口  给引脚10的输出数据寄存器置位
            GPIOE_BSRR |= (0x01 << 14);
            break;

        default:
            break;
        }
}

// 延时
void daley(int mtime)
{
    int i = 0;
    for (i = 0; i < 1000 * mtime; i++)
        ;
}

```
### 封装gpio
添加gpio宏结构体

gpio.h
```c

#include "stdint.h"
// #include "delay.h"
// #include "stm32f4xx.h"

#ifndef __GPIO_H__
#define __GPIO_H__

// 定义结构体容纳寄存器
typedef struct
{
    uint32_t GPIO_MODER;   // GPIO 端口模式寄存器
    uint32_t GPIO_OTYPER;  // GPIO 端口输出类型寄存器
    uint32_t GPIO_OSPEEDR; // GPIO 端口输出速度寄存器
    uint32_t GPIO_PUPDR;   // GPIO 端口上拉/下拉寄存器
    uint32_t GPIO_IDR;     // GPIO 端口输入数据寄存器
    uint32_t GPIO_ODR;     // GPIO 端口输出数据寄存器
    uint32_t GPIO_BSRR;    // GPIO 端口置位/复位寄存器
} GPIO_Type;

// GPIO F 端口地址
#define GPIOF ((GPIO_Type *)0x40021400)

// GPIO E 端口地址
#define GPIOE ((GPIO_Type *)0x40021000)

// GPIO A 端口地址
#define GPIOA ((GPIO_Type *)0x40020000)

// 定义对应位的地址
#define gpio_pin_0				(uint16_t)(0x0001)
#define gpio_pin_1				(uint16_t)(0x0002)
#define gpio_pin_2				(uint16_t)(0x0004)
#define gpio_pin_3				(uint16_t)(0x0008)
#define gpio_pin_4				(uint16_t)(0x0010)
#define gpio_pin_5				(uint16_t)(0x0020)
#define gpio_pin_6				(uint16_t)(0x0040)
#define gpio_pin_7				(uint16_t)(0x0080)
#define gpio_pin_8				(uint16_t)(0x0100)
#define gpio_pin_9				(uint16_t)(0x0200)
#define gpio_pin_10				(uint16_t)(0x0400)
#define gpio_pin_11				(uint16_t)(0x0800)
#define gpio_pin_12				(uint16_t)(0x1000)
#define gpio_pin_13				(uint16_t)(0x2000)
#define gpio_pin_14				(uint16_t)(0x4000)
#define gpio_pin_15				(uint16_t)(0x8000)
#define gpio_pin_all			(uint16_t)(0xFFFF)

// 定义 是 否 0 1
enum {RESET=0, SET=!RESET};


/* 
功能：给指定端口引脚置位
GPIOx：指定端口，x = (A ... I)
pins: 指定引脚：(0 ... 15)
 */
void GPIO_SetBits(GPIO_Type *GPIOx, uint16_t pins);
/* 
功能：给指定端口引脚复位
GPIOx：指定端口，x = (A ... I)
pins: 指定引脚：(0 ... 15)
 */
void GPIO_RestBits(GPIO_Type *GPIOx, uint16_t pins);
/*
返回对应位的高低电平

 */
uint8_t GPIO_ReadInputDatabits(GPIO_Type *GPIOx, uint16_t pins);

#endif

```
gpio.c
```c
#include "gpio.h"


/* 
功能：给指定端口引脚置位
GPIOx：指定端口，x = (A ... I)
pins: 指定引脚：(0 ... 15)
 */
void GPIO_SetBits(GPIO_Type *GPIOx, uint16_t pins)
{
    GPIOx->GPIO_BSRR |= pins;
}
/* 
功能：给指定端口引脚复位
GPIOx：指定端口，x = (A ... I)
pins: 指定引脚：(0 ... 15)
 */
void GPIO_RestBits(GPIO_Type *GPIOx,uint16_t pins)
{
    GPIOx->GPIO_BSRR |= pins << 16;
}

/* 
功能：读取指定端口的电平状态
GPIOx：指定端口，x = (A ... I)
pins: 指定引脚：(0 ... 15)
返回值：SET：读到高电平， RESET：读到低电平
 */
uint8_t GPIO_ReadInputDatabits(GPIO_Type *GPIOx, uint16_t pins)
{
	return GPIOx->GPIO_IDR & pins ? SET : RESET;
}

```

led.h
```c

#include "stdint.h"
#include "gpio.h"
// #include "delay.h"
// #include "stm32f4xx.h"

#ifndef __LED_H__
#define __LED_H__
// rcc时钟基地址
#define RCC_AHB1_BASE (0x40023800)
// RCC AHB1 外设时钟使能寄存器
#define RCC_AHB1ENR_ADDR *((uint32_t *)(RCC_AHB1_BASE + 0x30))


// 初始化灯,使能
void initLed(void);
// 控制对应灯的开关
void showLed(int i,int time);
// 延时
void daley(int mtime);
#endif

```
led.h

```c

#include "led.h"
// 初始化灯,使能
void initLed(void)
{
    // 先开启对应端口使能  f端口
    RCC_AHB1ENR_ADDR |= 1 << 5;
    // 先开启对应端口使能  e端口
    RCC_AHB1ENR_ADDR |= 1 << 4;
    // 先开启对应端口使能  A端口
    RCC_AHB1ENR_ADDR |= 1 << 0;

    // f端口 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOF->GPIO_MODER &= ~(0x05 << 19); // 19位清0
    GPIOF->GPIO_MODER |= (0x05 << 18);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOF->GPIO_OTYPER &= ~(0x03 << 9); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOF->GPIO_OSPEEDR |= (0x05 << 19); // 19位置1
    GPIOF->GPIO_OSPEEDR |= (0x05 << 18); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOF->GPIO_PUPDR &= ~(0x05 << 19); // 19位清0
    GPIOF->GPIO_PUPDR &= ~(0x05 << 18); // 18位清0

    // e端口 9号引脚和10号引脚一起
    //  2、输出模式      01 通用输出模式
    GPIOE->GPIO_MODER &= ~(0x05 << 27); // 19位清0
    GPIOE->GPIO_MODER |= (0x05 << 26);  // 18位置1
    // 3、推挽输出      0 ：输出推挽（复位状态）
    GPIOE->GPIO_OTYPER &= ~(0x03 << 13); // 9位清0
    // 4、速度 高速     11 ：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
    GPIOE->GPIO_OSPEEDR |= (0x05 << 27); // 19位置1
    GPIOE->GPIO_OSPEEDR |= (0x05 << 26); // 18位置1
    // 5、上拉  01：上拉    10：下拉
    GPIOE->GPIO_PUPDR &= ~(0x05 << 27); // 19位清0
    GPIOE->GPIO_PUPDR &= ~(0x05 << 26); // 18位清0

    // f端口 给引脚9的输出数据寄存器置位
    GPIOF->GPIO_BSRR |= (0x01 << 9);
    // f端口 给引脚10的输出数据寄存器置位
    GPIOF->GPIO_BSRR |= (0x01 << 10);

    // e端口 给引脚13的输出数据寄存器置位
    GPIOE->GPIO_BSRR |= (0x01 << 13);
    // e端口 给引脚14的输出数据寄存器置位
    GPIOE->GPIO_BSRR |= (0x01 << 14);

    // 开关的配置 第0个按键
    GPIOA->GPIO_MODER &= ~0x03; // 清0,默认输入模式
    GPIOA->GPIO_PUPDR |= 0x01;  // 上拉电阻
    GPIOA->GPIO_PUPDR &= ~0x02; // 上拉电阻
}
// 控制对应灯的开关
void showLed(int i, int time)
{
    static int is_show = 0;
    // static int butt=0;
    // 高电平 开关断开
    // 低电平,开关按下
    if (!GPIO_ReadInputDatabits(GPIOA,gpio_pin_0))
    {
        // 等待开关松开
        while (!GPIO_ReadInputDatabits(GPIOA,gpio_pin_0))
            ;
        is_show++;
        is_show %= 2;
    }
    if (is_show == 0)
        switch (i)
        {
        case 0:
            // f端口 给引脚9的输出数据寄存器复位
            GPIO_RestBits(GPIOF,gpio_pin_9);
            daley(time);
            // f端口 给引脚9的输出数据寄存器置位
            GPIO_SetBits(GPIOF,gpio_pin_9);
            break;
        case 1:
            // f端口 给引脚10的输出数据寄存器复位
            GPIO_RestBits(GPIOF,gpio_pin_10);
            daley(time);
            // f端口 给引脚10的输出数据寄存器置位
            GPIO_SetBits(GPIOF,gpio_pin_10);
            break;
        case 2:
            //  e端口  给引脚9的输出数据寄存器复位
            GPIO_RestBits(GPIOE,gpio_pin_13);
            daley(time);
            //  e端口  给引脚9的输出数据寄存器置位
            GPIO_SetBits(GPIOE,gpio_pin_13);
            break;
        case 3:
            //  e端口  给引脚10的输出数据寄存器复位
            GPIO_RestBits(GPIOE,gpio_pin_14);
            daley(time);
            //  e端口  给引脚10的输出数据寄存器置位
            GPIO_SetBits(GPIOE,gpio_pin_14);
            break;

        default:
            break;
        }
}

// 延时
void daley(int mtime)
{
    int i = 0;
    for (i = 0; i < 1000 * mtime; i++)
        ;
}

```
实际上标准库已经将对应函数这里,之类的封装好了  
后面学习只需要导入标准库,使用标准库提供的函数和宏就行了
