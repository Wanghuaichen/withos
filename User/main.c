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
#include "delay.h"
#include "led.h"
#include "MusicPlayer.h"
#include "mylcd.h"
#include "BUTTON.H"
#include "wm.h"
#include "stringutils.h"
void SysTick_Configuration(void);

void delay_clk(u16 n)
{
		while(n){
			--n;
		}
}
#define delay_ms(n) delay_clk(n*1000)
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	
#define bigger_stk_size 128
OS_STK  myTask_STK[bigger_stk_size];
void myTask(void *data)
{
	
	u8 cnt = 0;
	u16 presscnt = 0;
	char str[5];
	u8 flag = 0;
	U16 BUTTONsizeX = 50, BUTTONsizeY = 30;
	u16 x1 = 400-80, y1 = 240-50;
	WM_HWIN hdesktop;
	BUTTON_Handle hbutton;
	TEXT_Handle htext;
	#define mytextid 666
	#define mybuttonid 888
	
/*	GUI_SetBkColor(GUI_BLACK); 	
	GUI_SetColor(GUI_BLACK);
	WM_SetDesktopColor(DesktopColor); 
	hdesktop = WM_GetDesktopWindow();//获取桌面的句柄
	*/
	
	htext = TEXT_Create(x1, y1 -100, 80*2, 50, mytextid, WM_CF_SHOW, "text",TEXT_CF_RIGHT);
	TEXT_SetTextColor(hText,GUI_WHITE);
	
	hbutton = BUTTON_Create(x1, y1, BUTTONsizeX, BUTTONsizeY, mybuttonid, WM_CF_SHOW);
	BUTTON_SetBkColor(hbutton, 0, GUI_GREEN);
	BUTTON_SetBkColor(hbutton, 1, GUI_YELLOW);
	BUTTON_SetText(hbutton, "click!");

/*	htext = TEXT_CreateAsChild(x1, y1 -100, 80*2, 50, hdesktop, mytextid, WM_CF_SHOW, "text",TEXT_CF_RIGHT);
	TEXT_SetTextColor(hText,GUI_WHITE);
	
	hbutton = BUTTON_CreateAsChild(x1, y1, BUTTONsizeX, BUTTONsizeY, hdesktop, mybuttonid, WM_CF_SHOW);
	BUTTON_SetBkColor(hbutton, 0, GUI_GREEN);
	BUTTON_SetBkColor(hbutton, 1, GUI_YELLOW);
	BUTTON_SetText(hbutton, "click!");*/
	GUI_Exec();
  //GUI_InvertRect(x1, y1, x2, y2);

	if(!htext){
			;//GUI_DispString("Text is not created!");	
	}
	else{
			;//GUI_DispString("Text is created!");	
	}

	if(!hbutton){
			;//GUI_DispString("Button is not created!");	
	}
	else{
			;//GUI_DispString("Button is created!");	
	}
	
	LED0 = 0;
	while(1)	{
		++cnt;
		if(cnt == 50){
				cnt = 0;
			LED0 = ~LED0;
		}
/*	if(GUI_GetKey() == mybuttonid){
			++presscnt;
			int2str(str, presscnt);
			TEXT_SetText(htext, str);
	}*/
	delay_ms(20);
	GUI_Exec();
	}
	
}

#define mySTART_STK_SIZE 32
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  myTASK_START_STK[mySTART_STK_SIZE];
void startTask(void *data)
{
		OSTaskCreate(myTask,	   //task pointer
					(void *)0,	       //parameter
					(OS_STK *)&myTask_STK[bigger_stk_size-1],//task stack top pointer
					17 ); //task priority

	OSTaskDel(OS_PRIO_SELF);
	
}


int main(void)
{
  /* Add your application code here */


	SysTick_Configuration(); 
	MCU_Init();
	OSInit();
	LED_Init();
	delay_init();//SystemCoreClock);	    	 
	//GUI_Delay(100);
	//OSTimeDly(10); //deadloop

			//GUI_SetBkColor(GUI_BLUE);
		//GUI_Clear();
//LED0 = 0;LED1=0;
//GUI_DispString("Hello World!");	
//	myTask(0);
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
