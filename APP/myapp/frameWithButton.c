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

  int NCode, Id,i;
	BUTTON_Handle hbutton = WM_GetFirstChild(pMsg ->hWin);
	
	GUI_DispStringHCenterAt("callback is working", 400, 430);
	if(!hbutton){
		GUI_DispString("\nButton handle is invalid!");
	}
	else{
		GUI_DispString("\nButton handle is valid!");
	}
	BUTTON_SetText(hbutton, "abutton");
	WM_InvalidateWindow(hbutton);
	
	
  switch (pMsg->MsgId) {
		
    case WM_PAINT:
			BUTTON_SetDefaultBkColor(GUI_YELLOW, BUTTON_CI_UNPRESSED);
			BUTTON_SetDefaultBkColor(GUI_BLUE, BUTTON_CI_PRESSED);
			GUI_DispString("\npaint	");
      break;
		
    case WM_NOTIFY_PARENT:
			BUTTON_SetText(pMsg ->hWinSrc, "button message");
      Id    = WM_GetId(pMsg->hWinSrc);     
      NCode = pMsg->Data.v;     
			GUI_DispString("notify parent:	");		
      switch (NCode) {
        case WM_NOTIFICATION_RELEASED:   
					BUTTON_SetText(pMsg ->hWinSrc, "released");
					GUI_DispString("released	");
					break;
				default:
					BUTTON_SetText(pMsg ->hWinSrc, "default	");
      }
      //break;
			
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
	GUI_SetBkColor(GUI_GREEN);
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();
	
	
		BUTTON_SetDefaultBkColor(GUI_YELLOW, BUTTON_CI_PRESSED);
		BUTTON_SetDefaultBkColor(GUI_BLUE, BUTTON_CI_UNPRESSED);
	
	
	hframe = FRAMEWIN_Create("the frame", _cbBkFrameWithButton, WM_CF_SHOW, 0, 0, 800, 400); 
	hbutton = BUTTON_CreateAsChild(400-50, 240 - 30, 100, 60, hframe, GUI_ID_OK, WM_CF_SHOW);
	
	LED0 = 0;
	while(1){
		GUI_Exec();
		delay_ms(20);
		++ledcnt;
		if(ledcnt == 50){
			LED0 = !LED0;
			ledcnt = 0;
		}
		
	}
}