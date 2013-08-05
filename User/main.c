/**
  ******************************************************************************
  * @file   main.c 
  ******************************************************************************
  * @attention
	*
  * 这是一个通用UI程序，初步写出了程序的运行框架，你只需要将其充实即可成为你自己的UI程序。
	* 运用uC/OS-II实现任务的管理，简化程序结构。在此基础之上，你可以实现你想要的功能，（像MP3播放器，图片浏览器，示波器，频谱分析仪…………………………只有你想不到的没有做不到的）
	* 并且你的设备将具备惊艳的UI界面（当然是仿照Matro风格了），完全为个性化而生！
	* 缺点：1)需要较大的RAM量，这需要根据你的应用大小而做取舍。
	        2)界面的操作方式不好，需要你来发挥创造。（主要是没有实体按键，哪怕是只有一个就会好很多，况且WP有3个按键，Android需要4个，像iphone还得需要个物理按键呢！），
						完全触摸操作不太好实现非常简单的操作模式（只得牺牲屏幕空间，专门设置一个区域做固定按钮）
						关于这一点，希望大家研究一下，提出自己的创意……
	*
	*
	*                                创建者--柯南大侠  ^_^
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Hardwear.h"
#include "Initialize.h"
#include "ff.h"
#include "includes.h"
#include "APP.h"
#include "bmp.h"
void SysTick_Configuration(void);
OS_STK  TASK_START_STK[START_STK_SIZE];

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Add your application code here */
	MCU_Init();
	SysTick_Configuration();   //系统时钟初始化
	OSInit(); 
	OSTaskCreate(TaskStart,	   //task pointer
					(void *)0,	       //parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],//task stack top pointer
					START_TASK_Prio ); //task priority
	OSStart();                 //开始多任务执行	
	return 0;	   
}

//系统时钟配置
void SysTick_Configuration(void)
{
 	SysTick->CTRL&=~(1<<2); //SYSTICK使用外部时钟源
	SysTick->CTRL|=1<<1;    //开启SYSTICK中断
	SysTick->LOAD=10000000/OS_TICKS_PER_SEC;  //由于运行频率是80MHz，所以8分频后为10MHz
	SysTick->CTRL|=1<<0;    //开启SYSTICK
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
