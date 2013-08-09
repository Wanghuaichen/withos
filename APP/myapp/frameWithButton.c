#include "frameWithButton.h"
#include "includes.h"
#include "wm.h"
#include "gui.h"
#include "framewin.h"
#include "button.h"
#include "delay.h"
#include "led.h"

void _cbBkFrameWithButton(WM_MESSAGE *pMsg)
{
	static unsigned counter = 0;
	static unsigned counter1 = 0;
	static unsigned counter2 = 0;
	static unsigned counter3 = 0;
	static unsigned counter4 = 0;
  int NCode, Id,i;
	BUTTON_Handle hbutton = WM_GetFirstChild(pMsg ->hWin);
	
	
	GUI_DispStringHCenterAt("callback is working", 400, 430);
	GUI_DispDecMin(++counter);	
	
	if(!hbutton){
		GUI_DispString(" : Button handle is invalid!");
	}
	else{
		GUI_DispString(" : Button handle is valid!");
	}
	
	GUI_GotoXY(400, 460);
	GUI_DispDecMin(pMsg ->MsgId);
	
  switch (pMsg->MsgId) {
		
    case WM_PAINT:
			BUTTON_SetDefaultBkColor(GUI_YELLOW, BUTTON_CI_UNPRESSED);
			BUTTON_SetDefaultBkColor(GUI_BLUE, BUTTON_CI_PRESSED);
			GUI_DispString("\npaint	");
			BUTTON_SetText(hbutton, "abutton");
      break;
		case WM_TOUCH:
			GUI_GotoXY(200, 100);
			GUI_DispString("touch:");			
			break;
		
    case WM_NOTIFY_PARENT:
			
			GUI_GotoXY(600, 100);
			BUTTON_SetText(pMsg ->hWinSrc, "button message");
      Id    = WM_GetId(pMsg->hWinSrc);     
      NCode = pMsg->Data.v;     
			GUI_GotoXY(200, 300);
			GUI_DispString("notify parent:	");	
			GUI_DispDecMin(++counter4);
		
      switch (NCode) {
        case WM_NOTIFICATION_RELEASED:   
					GUI_GotoXY(600, 150);
					BUTTON_SetText(pMsg ->hWinSrc, "released");
					GUI_DispString("released:	");
					GUI_DispDecMin(++counter1);
					break;
				
        case WM_NOTIFICATION_CLICKED:   
					GUI_GotoXY(600, 200);
					BUTTON_SetText(pMsg ->hWinSrc, "clicked");
					GUI_DispString("clicked	");
					GUI_DispDecMin(++counter2);
					break;
				
				default:
					BUTTON_SetText(pMsg ->hWinSrc, "default	");
      }
      break;
			
		case WM_PID_STATE_CHANGED:
				GUI_GotoXY(600, 250);
				GUI_DispString("WM_PID_STATE_CHANGED");
				GUI_DispDecMin(++counter3);

    default:
      WM_DefaultProc(pMsg);
			GUI_DispString("default");
  }
}


void frameWithButton(void)
{
	
	u8 ledcnt = 0;
	FRAMEWIN_Handle hframe;
	BUTTON_Handle hbutton;
	GUI_Init();
/*	GUI_SetBkColor(GUI_GREEN);
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();*/
	
	
	//	BUTTON_SetDefaultBkColor(GUI_YELLOW, BUTTON_CI_PRESSED);
		//BUTTON_SetDefaultBkColor(GUI_BLUE, BUTTON_CI_UNPRESSED);
	
	
	hframe = FRAMEWIN_Create("the frame", &_cbBkFrameWithButton, WM_CF_SHOW, 5, 5, 160, 170); 
	hbutton = BUTTON_CreateAsChild(25, 25, 60, 60, hframe, GUI_ID_OK, WM_CF_SHOW);
	
	LED0 = 0;
	while(1){
		GUI_Exec();
		delay_ms(5);
		++ledcnt;
		if(ledcnt == 50){
			LED0 = ~LED0;
			ledcnt = 0;
		}
		
	}
}