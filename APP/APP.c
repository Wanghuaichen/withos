#include "APP.h"
#include "led.h"
#include "stringutils.h"
#include "delay.h"
/*************************************************�����ջ����********************************************/
OS_STK  TASK_Touch_STK[Touch_STK_SIZE];
OS_STK  TASK_Menu_STK[Menu_STK_SIZE];
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_Setting_STK[Setting_STK_SIZE];
OS_STK  TASK_FileBrowser_STK[FileBrowser_STK_SIZE];
OS_STK  TASK_MusicPlayer_STK[MusicPlayer_STK_SIZE];
OS_STK  TASK_PhotoBrowser_STK[PhotoBrowser_STK_SIZE];
OS_STK  TASK_TextBrowser_STK[TextBrowser_STK_SIZE];
// OS_STK  TASK_STAT_STK[OS_TASK_IDLE_STK_SIZE]; //ͳ�������ջ����
/********************************************************************************************************** 
** Function name:      TaskStart
** Descriptions:       ��ʼ�������ڽ��������ϵͳ�ر�����
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
void TaskStart(void * pdata)
{
	pdata = pdata;
	LED_Init();
// 	OSTaskCreate(OS_TaskStat, (void * )0, (OS_STK *)&TASK_STAT_STK[OS_TASK_IDLE_STK_SIZE-1],OS_TASK_STAT_PRIO);//����ͳ������
// 	OSTimeDly(200);
	//OSTaskCreate(Task_LED_DEMO, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_DEMO_TASK_Prio);
	OSTaskCreate(Task_Menu, (void * )0, (OS_STK *)&TASK_Menu_STK[Menu_STK_SIZE-1], Menu_TASK_Prio);
//	OSTaskCreate(Task_Touch, (void * )0, (OS_STK *)&TASK_Touch_STK[Touch_STK_SIZE-1], Touch_TASK_Prio);
 //	OSStatInit();
	OSTaskDel(OS_PRIO_SELF);//ɾ���Լ�	OSTaskSuspend(START_TASK_Prio);
}
/********************************************************************************************************** 
** Function name:      Task_Touch
** Descriptions:       ������������񣬴�������뽨�������򽫲�����Ӧ������
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
int curkey;
void Task_Touch(void *pdata)
{
	GUI_PID_STATE  TouchPoint;
	unsigned ledSwitchCnt = 0;
	LED1 = 0;
	while(1){
		GUI_TOUCH_Exec() ;
		GUI_TOUCH_GetState(&TouchPoint);
		if (TouchPoint.Pressed) {
			if(TouchFlag == TouchUnPressed){
				TouchFlag = TouchPress;
			}
		} else {
			if(TouchFlag == TouchPressed){
				TouchFlag = TouchUnPress;	
			}
		}
		
		++ledSwitchCnt;
		if(ledSwitchCnt == 50){
				ledSwitchCnt = 0;
				LED1 = ~LED1;
		}
	  delay_ms(20);//OSTimeDly(6);   //��֤50Hz���ϵ�ִ��Ƶ��
	}
}
/********************************************************************************************************** 
** Function name:      Task_Menu
** Descriptions:       �˵��������񣬴�������뽨�������򽫲���֧�ֻ����˵����������˵�û��ʹ�ûص�������
                       �����Լ���������ӵ���Ӧ�İ�ť�£�������ֻ����ɸ��������񼴿ɡ�
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
static void _DemoButton(void) {
  BUTTON_Handle hButton;
  GUI_SetFont(&GUI_Font8x16);
  GUI_DispStringHCenterAt("Click on button...", 160, 90);
hButton = BUTTON_Create(110, 110, 100, 40, GUI_ID_OK, WM_CF_SHOW);
  /* Set the button text */
  BUTTON_SetText(hButton, "Click me...");
  /* Let window manager handle the button */
  while (GUI_WaitKey() != GUI_ID_OK);
  /* Delete the button*/
  BUTTON_Delete(hButton);
  GUI_ClearRect(0, 50, 319, 239);
  GUI_Exec();//GUI_Delay(1000);
	delay_ms(20);
}



	#define BUTTON_RIGHT_MIDDLE 8511
unsigned int buttonCounter = 0;
unsigned char led1cnt = 0;

void Task_Menu(void *pdata)
{
  GUI_Init();	
  MainMenu_Init();
	LED1 = 0;
	
	
	while(1) {
	/*	if(TouchCmd == 1 && Menu_ID == MainMenu_ID){
			TouchCmd = 0;	
		  switch(GUI_GetKey()) {
			
			  case GUI_ID_BUTTON0:
					Out_MainMenu();
			    OSTaskCreate(Task_MusicPlayer, (void * )0, (OS_STK *)&TASK_MusicPlayer_STK[MusicPlayer_STK_SIZE-1], MusicPlayer_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON1: //�˳������沢�����ļ��������
          Out_MainMenu();
				  OSTaskCreate(Task_FileBrowser, (void * )0, (OS_STK *)&TASK_FileBrowser_STK[FileBrowser_STK_SIZE-1], FileBrowser_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON2:
				  Out_MainMenu();
				  OSTaskCreate(Task_TextBrowser, (void * )0, (OS_STK *)&TASK_TextBrowser_STK[TextBrowser_STK_SIZE-1], TextBrowser_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON3: //�˳������沢������������
				  Out_MainMenu();
				  OSTaskCreate(Task_Setting, (void * )0, (OS_STK *)&TASK_Setting_STK[Setting_STK_SIZE-1], Setting_TASK_Prio);
				  break;
			  case GUI_ID_BUTTON4:
				  Out_MainMenu();
				  OSTaskCreate(Task_PhotoBrowser, (void * )0, (OS_STK *)&TASK_PhotoBrowser_STK[PhotoBrowser_STK_SIZE-1], PhotoBrowser_TASK_Prio);
				  break;
				case GUI_ID_BUTTON5:
					GUI_MessageBox("Function does not support!","FFT",GUI_MESSAGEBOX_CF_MOVEABLE);
					break;
				case GUI_ID_BUTTON6:
					GUI_MessageBox("Function does not support!","��Ϸ",GUI_MESSAGEBOX_CF_MOVEABLE);
					break;
				case GUI_ID_BUTTON7:
					GUI_MessageBox("Function does not support!","ʾ����",GUI_MESSAGEBOX_CF_MOVEABLE);
					break;
				default :	
					TouchCmd = 1;
					break;
		  }
	  }*/
		/*if((curkey = GUI_GetKey()) == BUTTON_RIGHT_MIDDLE){
				++buttonCounter;
				int2str(str, buttonCounter);
				TEXT_SetText(hText, str);
		}
		else{
			int2str(str, curkey);
			TEXT_SetText(hText, str);
		}*/
//		curkey = GUI_WaitKey();
	//			++buttonCounter;
		//		int2str(str, buttonCounter);
			//	TEXT_SetText(hText, str);
				
				//_DemoButton();
				
	  WM_MoveCtrl();
		led1cnt++;
		if(led1cnt == 50){
			LED1 = ~LED1;
			led1cnt = 0;
		}
		GUI_Exec();//�ػ�
	  delay_ms(20);//OSTimeDly(3);
	}
}
/********************************************************************************************************** 
** Function name:      Task_LED_DEMO
** Descriptions:       LED��˸�����Ҹ���ʱ������
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/

/*void Task_LED_DEMO(void *pdata)
{
	unsigned char ledCnt = 0;
	
	
	LED0 = 1;
	while(1){ 
		LED0 = ~LED0;

		delay_ms(1000);
	}
}*/

void Task_LED_DEMO(void *pdata)
{
	char  time_chars[] = {0,0,':',0,0,0};
  u32   time_now;
	while(1){ 
		GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    OSTimeDly(250);
		GPIO_SetBits(GPIOE, GPIO_Pin_2);
	  OSTimeDly(250);
		time_now = RTC_GetCounter();
		if((time_now/3600)>23){	
			RTC_Configuration();
			RTC_SetCounter(time_now-24*3600);
    }			
		time_chars[0] = time_now / 36000 + 0x30;
		time_chars[1] = time_now / 3600 % 10 + 0x30;
		time_chars[3] = time_now % 3600 / 600 + 0x30;
		time_chars[4] = time_now % 3600 / 60 % 10 + 0x30;
		TEXT_SetText(hText,time_chars);
// // 		printf("OSCPUUsage = %d\r\n",OSCPUUsage);
	}
}
