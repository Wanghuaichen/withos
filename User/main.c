/**
  ******************************************************************************
  * @file   main.c 
  ******************************************************************************
  * @attention
	*
  * ����һ��ͨ��UI���򣬳���д���˳�������п�ܣ���ֻ��Ҫ�����ʵ���ɳ�Ϊ���Լ���UI����
	* ����uC/OS-IIʵ������Ĺ����򻯳���ṹ���ڴ˻���֮�ϣ������ʵ������Ҫ�Ĺ��ܣ�����MP3��������ͼƬ�������ʾ������Ƶ�׷����ǡ�������������������ֻ�����벻����û���������ģ�
	* ��������豸���߱����޵�UI���棨��Ȼ�Ƿ���Matro����ˣ�����ȫΪ���Ի�������
	* ȱ�㣺1)��Ҫ�ϴ��RAM��������Ҫ�������Ӧ�ô�С����ȡ�ᡣ
	        2)����Ĳ�����ʽ���ã���Ҫ�������Ӵ��졣����Ҫ��û��ʵ�尴����������ֻ��һ���ͻ�úܶ࣬����WP��3��������Android��Ҫ4������iphone������Ҫ���������أ�����
						��ȫ����������̫��ʵ�ַǳ��򵥵Ĳ���ģʽ��ֻ��������Ļ�ռ䣬ר������һ���������̶���ť��
						������һ�㣬ϣ������о�һ�£�����Լ��Ĵ��⡭��
	*
	*
	*                                ������--���ϴ���  ^_^
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
	hdesktop = WM_GetDesktopWindow();//��ȡ����ľ��
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
