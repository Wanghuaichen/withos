#include "stm32f10x.h"
#include "Hardwear.h"
#include "Initialize.h"
#include "ff.h"
#include "includes.h"
#include "APP.h"
#include "bmp.h"
#include "delay.h"
#include "led.h"
#include "MusicPlayer.h"
#include "mylcd.h"
#include "BUTTON.H"
#include "wm.h"
#include "stringutils.h"
void SysTick_Configuration(void);
void motorMain(void) ;


#define MainTask_STK_SIZE 4096
OS_STK  MainTask_STK[MainTask_STK_SIZE];

int main(void)
{
	SysTick_Configuration(); 
	MCU_Init();
	OSInit();
	LED_Init();
	delay_init();

	OSTaskCreate(motorMain,	   //task pointer
					(void *)0,	       //parameter
					(OS_STK *)&MainTask_STK[MainTask_STK_SIZE-1],//task stack top pointer
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
