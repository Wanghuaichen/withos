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
void MainTask3(void);
void testnotify(void);
#define MainTask_STK_SIZE 3072
OS_STK  MainTask_STK[MainTask_STK_SIZE];
void testSize(void);
int main(void)
{
	SysTick_Configuration(); 
	MCU_Init();
	OSInit();
	LED_Init();
	delay_init();

	OSTaskCreate(testSize,	   //task pointer
					(void *)0,	       //parameter
					(OS_STK *)&MainTask_STK[MainTask_STK_SIZE-1],//task stack top pointer
					START_TASK_Prio ); //task priority	
	OSStart();                 //开始多任务执行		
	return 0;	   
}
/*
#include "led.h"
#include "delay.h"
FRAMEWIN_Handle hframe1, hframe2;
static void _cbframe(WM_MESSAGE * pMsg) 
{
	  int NCode, Id;
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;

  switch (pMsg->MsgId) {		
		
		case WM_INIT_DIALOG:
			break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);    
      NCode = pMsg->Data.v;              

      switch (NCode) {
        //case WM_NOTIFICATION_SEL_CHANGED:
         // LISTBOX_InvalidateItem(hListBox, LISTBOX_ALL_ITEMS);
         // break;
        case WM_NOTIFICATION_RELEASED: 
					WM_BringToTop(hframe2);
					WM_InvalidateWindow(WM_HBKWIN);	
					break;
        case WM_NOTIFICATION_CLICKED: 
					break;	
			}
}
}
void testnotify(void)
{

	EDIT_Handle hedit;
	BUTTON_Handle hbutton;
	unsigned char ledcnt = 0;
	LED_Init();
  GUI_Init();	
	LED0 = 0;
	
	hframe1 = FRAMEWIN_Create("1", _cbframe, WM_CF_SHOW, 0, 0, 700, 400);
	hframe2 = FRAMEWIN_Create("2", _cbframe, WM_CF_SHOW, 0, 0, 70, 40);
	hedit = EDIT_CreateAsChild(0, 0,  100, 100, WM_GetClientWindow(hframe1), 5000, WM_CF_SHOW, 100);
	hbutton = BUTTON_CreateAsChild(105, 105, 100, 100, WM_GetClientWindow(hframe1), 5001, WM_CF_SHOW);
  while (1) {
    GUI_Exec();//GUI_Delay(1000);
		GUI_TOUCH_Exec();
		delay_ms(15);
		if(++ledcnt == 60){
				ledcnt = 0;
				LED0 = ~LED0;
		}
  }	
}*/

//系统时钟配置
void SysTick_Configuration(void)
{
 	SysTick->CTRL&=~(1<<2); //SYSTICK使用外部时钟源
	SysTick->CTRL|=1<<1;    //开启SYSTICK中断
	SysTick->LOAD=10000000/OS_TICKS_PER_SEC;  //由于运行频率是80MHz，所以8分频后为10MHz
	SysTick->CTRL|=1<<0;    //开启SYSTICK
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
