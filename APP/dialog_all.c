
#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "DROPDOWN.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created manually, but could also be created by a GUI-builder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Dialog", 0,                 0,   0, 600, 300, WM_CF_SHOW, 0  },
  { BUTTON_CreateIndirect,   "OK",     GUI_ID_OK,        30,   30,  250,  120 , WM_CF_SHOW},
  { BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL,    330,  30,  250,  120 , WM_CF_SHOW},
/*  { TEXT_CreateIndirect,     "LText",  0,                 10,  55,  48,  15, TEXT_CF_LEFT  },
  { TEXT_CreateIndirect,     "RText",  0,                 10,  80,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT0,      60,  55, 100,  15, 0, 50 },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT1,      60,  80, 100,  15, 0, 50 },
  { TEXT_CreateIndirect,     "Hex",    0,                 10, 100,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT2,      60, 100, 100,  15, 0, 6 },
  { TEXT_CreateIndirect,     "Bin",    0,                 10, 120,  48,  15, TEXT_CF_RIGHT },
  { EDIT_CreateIndirect,     NULL,     GUI_ID_EDIT3,      60, 120, 100,  15 },
  { LISTBOX_CreateIndirect,  NULL,     GUI_ID_LISTBOX0,   10,  10,  60,  40 },
  { CHECKBOX_CreateIndirect, NULL,     GUI_ID_CHECK0,     10, 140,   0,   0 },
  { CHECKBOX_CreateIndirect, NULL,     GUI_ID_CHECK1,     30, 140,   0,   0 },
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER0,    60, 140, 180,  20 },
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER1,    10, 170, 230,  30 },
  { DROPDOWN_CreateIndirect,  NULL,    GUI_ID_DROPDOWN0, 170,  10,  80,  60, 0, 3  },
  { DROPDOWN_CreateIndirect,  NULL,    GUI_ID_DROPDOWN1, 170,  60,  80,  60, 0, 3  }*/
};


/*******************************************************************
*
*       static code
*
********************************************************************
*/


/*********************************************************************
*
*       _cbCallback
*/
static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id;
	static unsigned int cnt = 0;
	static unsigned cnt2 = 0;
	static unsigned cnt3 = 0;
	static unsigned int cnt4 = 0;

  WM_HWIN hWin = pMsg->hWin;
	GUI_GotoXY(600, 330);GUI_DispString("message: ");
	GUI_DispDecMin(pMsg ->MsgId);GUI_DispString(" : ");
	GUI_DispDecMin(++cnt);//"callback", 400, 300);
	
  switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
			

			//GUI_DispString("\npaint	");
			
		
      /* Get window handles for all widgets */
   /*   hEdit0   = WM_GetDialogItem(hWin, GUI_ID_EDIT0);
      hEdit1   = WM_GetDialogItem(hWin, GUI_ID_EDIT1);
      hEdit2   = WM_GetDialogItem(hWin, GUI_ID_EDIT2);
      hEdit3   = WM_GetDialogItem(hWin, GUI_ID_EDIT3);
      hListBox = WM_GetDialogItem(hWin, GUI_ID_LISTBOX0);
      hDropd0  = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
      hDropd1  = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1);

      EDIT_SetText(hEdit0, "EDIT widget 0");
      EDIT_SetText(hEdit1, "EDIT widget 1");
      EDIT_SetTextAlign(hEdit1, GUI_TA_LEFT);
      EDIT_SetHexMode(hEdit2, 0x1234, 0, 0xffffff);
      EDIT_SetBinMode(hEdit3, 0x1234, 0, 0xffff);
      LISTBOX_SetText(hListBox, _apListBox);
      WM_DisableWindow (WM_GetDialogItem(hWin, GUI_ID_CHECK1));
      CHECKBOX_Check(  WM_GetDialogItem(hWin, GUI_ID_CHECK0));
      CHECKBOX_Check(  WM_GetDialogItem(hWin, GUI_ID_CHECK1));
      SLIDER_SetWidth( WM_GetDialogItem(hWin, GUI_ID_SLIDER0), 5);
      SLIDER_SetValue( WM_GetDialogItem(hWin, GUI_ID_SLIDER1), 50);
      SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);
      DROPDOWN_AddString(hDropd0, "Item 0");
      DROPDOWN_AddString(hDropd0, "Item 1");
      DROPDOWN_AddString(hDropd0, "Item 2");
      DROPDOWN_AddString(hDropd1, "Item 0");
      DROPDOWN_AddString(hDropd1, "Item 1");
      DROPDOWN_AddString(hDropd1, "Item 2");
      DROPDOWN_AddString(hDropd1, "Item 3");
      DROPDOWN_AddString(hDropd1, "Item 4");*/
      break;
 /*   case WM_KEY:
      switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
      case GUI_KEY_ESCAPE:
        GUI_EndDialog(hWin, 1);
        break;
      case GUI_KEY_ENTER:
        GUI_EndDialog(hWin, 0);
        break;
      }
      break;*/
    case WM_NOTIFY_PARENT:
			
			GUI_GotoXY(600,450);
			GUI_DispString("parent: ");GUI_DispDecMin(++cnt4);
		
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      NCode = pMsg->Data.v;               /* Notification code */
      switch (NCode) {
        case WM_NOTIFICATION_RELEASED:    /* React only if released */
					//GUI_DispStringHCenterAt("child called:", 300, 300);
          if (Id == GUI_ID_OK) {          /* OK Button */
            GUI_DispStringHCenterAt("ok", 100, 350);//GUI_EndDialog(hWin, 0);
          }
          if (Id == GUI_ID_CANCEL) {      /* Cancel Button */
						GUI_DispStringHCenterAt("canceled", 100, 400);//GUI_EndDialog(hWin, 1); 
          }
					//WM_DeleteWindow(pMsg ->hWinSrc);
          break;
					
		
					
      }
			
      break;
			
		case WM_TOUCH:
					 GUI_DispStringHCenterAt("TOUCHED:", 600, 400);
					 GUI_DispDecMin(++cnt2);
			
    default:
			GUI_GotoXY(600,310);
			GUI_DispString("default: ");GUI_DispDecMin(++cnt3);
      WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       MainTask
*
*       Demonstrates a dialog box
*
**********************************************************************
*/
#include "led.h"
void delay_ms(unsigned short);

void MainTask(void *pdata) {
			unsigned char ledcnt1 = 0;
		LED1 = 0;
	
  GUI_Init();
	
			BUTTON_SetDefaultBkColor(GUI_YELLOW, BUTTON_CI_UNPRESSED);
			BUTTON_SetDefaultBkColor(GUI_BLUE, BUTTON_CI_PRESSED);
/*	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();*/
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
	
	GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, 0, 0, 0);
	
	
  while (1) {
				GUI_Exec();
				delay_ms(10);
		
						if(++ledcnt1 == 100){
						ledcnt1 = 0;
						LED1 = ~LED1;
				}
  }
}

void touchAndRedraw(void *pdata){
		unsigned char ledcnt1 = 0;
		LED1 = 0;
		while(1){
				
				GUI_Exec();
				delay_ms(10);
			
				if(++ledcnt1 == 100){
						ledcnt1 = 0;
						LED1 = ~LED1;
				}
		}
}

