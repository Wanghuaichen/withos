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
	OSStart();                 //��ʼ������ִ��		
	return 0;	   
}



//ϵͳʱ������
void SysTick_Configuration(void)
{
 	SysTick->CTRL&=~(1<<2); //SYSTICKʹ���ⲿʱ��Դ
	SysTick->CTRL|=1<<1;    //����SYSTICK�ж�
	SysTick->LOAD=10000000/OS_TICKS_PER_SEC;  //��������Ƶ����80MHz������8��Ƶ��Ϊ10MHz
	SysTick->CTRL|=1<<0;    //����SYSTICK
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
