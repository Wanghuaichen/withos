#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "listview.h"
#include "progbar.h"
#include "keyboard.h"
#include <string.h>
#include "led.h"
#include "delay.h"
#include "editgroup.h"
#include "motordrive.h"
#include "filerw.h"
//主要窗体的句柄

unsigned char hConfigDlgFlag = 0;
unsigned char hmainDlgFlag = 1;
unsigned char hkeyboardFlag = 0;
unsigned char hListViewDlgFlag = 0;
WM_HWIN hConfigDlg;
WM_HWIN hmainDlg;
WM_HWIN hkeyboard;
WM_HWIN hListViewDlg;
unsigned char showModeCounter = 0;
//gui从用户获取的操作指令
#define OP_NO_OPERATION 0
#define OP_SHOW_MODE_MAIN 1
#define OP_SHOW_MODE_CONF 2
#define OP_MORE_MODES 64
#define OP_GO 128

#define OP_ADD_NEW_MODE 4
#define OP_SUBMIT_ADD 8
#define OP_CONF_OK 16
#define OP_CONF_CANCEL 32
#define OP_EDIT_MODE 2048

#define OP_DEFAULT_MODE(i) (OP_GO << i)

unsigned operationCode  = OP_NO_OPERATION;
WM_HWIN hCurrentObj;

#define lcdWidth 800
#define lcdHeight 480

#define gapYProgbarUp 30
#define gapYLabelProgbar 50
#define gapYModeButtonLabel 50
#define gapYConfigButtonModeButton 30
#define gapYConfigButtonDown 30

//#define gapXSwitchButtonModeButton 100

//窗体描述
#define labelHeight 50
#define labelWidth 800

#define progBarHeight 60
#define progBarWidth 800

#define numOfModeButton 3
#define modeButtonWidth 100
#define modeButtonHeight 60

#define numOfSwitchButton 2
#define switchButtonHeight 60
#define switchButtonWidth 100

#define configButtonWidth 100
#define configButtonHeight 60

#define progBarX 0
#define progBarY gapYProgbarUp

#define labelX 0
#define labelY (progBarY + progBarHeight + 20)

#define buttonBaseY (labelY + labelHeight + gapYModeButtonLabel)
#define buttonYSpace (lcdHeight - buttonBaseY - gapYConfigButtonModeButton - configButtonHeight - gapYConfigButtonDown)

#define switchButtonX (lcdWidth / 3 - switchButtonWidth / 2)
#define switchButtonGapY (buttonYSpace - numOfSwitchButton * switchButtonHeight)/(1+numOfSwitchButton)
#define switchButtonY(i) (buttonBaseY + switchButtonGapY + (i-1)*(switchButtonHeight + switchButtonGapY))

#define modeButtonX (lcdWidth / 3 * 2 - modeButtonWidth / 2)
#define modeButtonGapY (buttonYSpace - numOfModeButton * modeButtonHeight)/(1+numOfModeButton)
#define modeButtonY(i) (buttonBaseY + modeButtonGapY + (i-1)*(modeButtonHeight + modeButtonGapY))

#define configButtonY 

//EDIT group
#define editGroupX0 50
#define editGroupY0 50
/*#define modeButtonGapX ((lcdWidth - numOfMode * modeButtonWidth) / (1 + numOfMode))
//#define modeButtonX(i) (modeButtonGapX + (modeButtonGapX + modeButtonWidth) * (i - 1))
#define modeButtonY (gapYLabelUp + labelHeight + gapYLabelModeButton)
#define labelX(i) (modeButtonX(i) - (labelWidth / 2))*/


#define BUTTON_Id_MoreModes (GUI_ID_USER+1)
#define BUTTON_Id_Go (GUI_ID_USER+2)
#define BUTTON_Id_Stop (GUI_ID_USER+3)
#define BUTTON_Id_DefaultMode1 (GUI_ID_USER+4)
#define BUTTON_Id_DefaultMode2 (GUI_ID_USER+5)
#define BUTTON_Id_DefaultMode3 (GUI_ID_USER+6)

#define BUTTON_Id_DeleteMode (GUI_ID_USER+7)
#define BUTTON_Id_Ok (GUI_ID_USER+8)
#define BUTTON_Id_Cancel (GUI_ID_USER+9)
#define BUTTON_Id_EditMode (GUI_ID_USER+10)
#define BUTTON_Id_SubmitEdit (GUI_ID_USER+11)


#define FRAME_ID_mainPanel (GUI_ID_USER+12)
#define FRAME_ID_confPanel (GUI_ID_USER+13)

#define TEXT_ID_mainPanelTime (GUI_ID_USER+14)
#define TEXT_ID_mainPanelSpeed (GUI_ID_USER+15)
#define LISTBOX_Id (GUI_ID_USER+16)
#define GUI_ID_TEXT_ModeName (GUI_ID_USER+17)
#define GUI_ID_LISTVIEW (GUI_ID_USER+18)


#define BUTTON_Id_Ok_listview (GUI_ID_USER+18)
/*********************************************************************
*
*       Default contents of list box
*/
static const GUI_ConstString _ListBox[] = {
  "Default Mode 1", "Default Mode 2", "Default Mode 3", 0
};

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created manually, but could also be created by a GUI-builder.
*///

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,  "Main Panel",    FRAME_ID_mainPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
	{ PROGBAR_CreateIndirect,   "Progress",          0,       progBarX,  progBarY,  progBarWidth,  progBarHeight, WM_CF_SHOW },
	{ TEXT_CreateIndirect,      "Time: ",    TEXT_ID_mainPanelTime,       labelX,  labelY,  labelWidth,  labelHeight, TEXT_CF_LEFT },
	{	TEXT_CreateIndirect,      "Speed: ",    TEXT_ID_mainPanelSpeed,       labelX,  labelY+labelHeight,  labelWidth,  labelHeight, TEXT_CF_LEFT },
/* Buttons */
  //{ BUTTON_CreateIndirect,    "Default MODE 1",      BUTTON_Id_DefaultMode1,     modeButtonX,  modeButtonY(1),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  //{ BUTTON_CreateIndirect,    "Default MODE 2",      BUTTON_Id_DefaultMode2,     modeButtonX,  modeButtonY(2),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "Default MODE 3",      BUTTON_Id_DefaultMode3,     modeButtonX,  modeButtonY(3),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "More Modes",      BUTTON_Id_MoreModes,     0,  420,  configButtonWidth,  configButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Go!",      BUTTON_Id_Go,     400 - switchButtonWidth/2,  240 - switchButtonHeight/2,  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	//{BUTTON_CreateIndirect,    "Stop!",      BUTTON_Id_Stop,     switchButtonX,  switchButtonY(2),  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	//{ LISTBOX_CreateIndirect,   0,                LISTBOX_Id,  5,  labelY+labelHeight*2, 200, 260, 0, 100 },
};

static const GUI_WIDGET_CREATE_INFO _configDialogCreate[] = {
	{FRAMEWIN_CreateIndirect,  "Config Panel",    FRAME_ID_confPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
  
  { EDIT_CreateIndirect, 			0, GUI_ID_TEXT_ModeName, editGroupX0+100, editGroupY0-32, 200, 32, WM_CF_SHOW},
	{ TEXT_CreateIndirect,      "Mode Name:",     GUI_ID_TEXT_ModeName,    editGroupX0,  editGroupY0-32,  200,  32, TEXT_CF_LEFT },
  //{ BUTTON_CreateIndirect,    "Delete Mode",      BUTTON_Id_DeleteMode,     10,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "OK",      BUTTON_Id_Ok,     10+modeButtonWidth+5-100,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "CANCEL",      BUTTON_Id_Cancel,     10+2*modeButtonWidth+5*2-100,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "EDIT MODE",      BUTTON_Id_EditMode,     50+60+60+3*modeButtonWidth+5*3,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "SUBMIT",      BUTTON_Id_SubmitEdit,     50+40+120+4*modeButtonWidth+5*4,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},

};

static const GUI_WIDGET_CREATE_INFO _listViewCreate[] = {
	{FRAMEWIN_CreateIndirect,  "Config Panel",    FRAME_ID_confPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
  {LISTVIEW_CreateIndirect,  "list view",    GUI_ID_LISTVIEW,                  30,  30, lcdWidth/2, lcdHeight/2, WM_CF_SHOW },
  { EDIT_CreateIndirect, 			0, GUI_ID_TEXT_ModeName, editGroupX0+100, editGroupY0-32, 200, 32, WM_CF_SHOW},
	{ TEXT_CreateIndirect,      "Mode Name:",     GUI_ID_TEXT_ModeName,    editGroupX0,  editGroupY0-32,  200,  32, TEXT_CF_LEFT },
  //{ BUTTON_CreateIndirect,    "Delete Mode",      BUTTON_Id_DeleteMode,     10,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "OK",      BUTTON_Id_Ok_listview,     10+modeButtonWidth+5-100,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "CANCEL",      BUTTON_Id_Cancel,     10+2*modeButtonWidth+5*2-100,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "EDIT MODE",      BUTTON_Id_EditMode,     50+60+60+3*modeButtonWidth+5*3,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "SUBMIT",      BUTTON_Id_SubmitEdit,     50+40+120+4*modeButtonWidth+5*4,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},

};

/*********************************************************************
*
*       _cbCallback
*/
static void _cbCallbackConfigPanel(WM_MESSAGE * pMsg);
static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id, itemtot, curitem;
  WM_HWIN hDlg, hListBox;//, hItem;
  hDlg = pMsg->hWin;
  hListBox = WM_GetDialogItem(hDlg, LISTBOX_Id);
  switch (pMsg->MsgId) {		
		
    case WM_INIT_DIALOG:
			
      LISTBOX_SetText(hListBox, _ListBox);
      LISTBOX_AddString(hListBox, "Add New Mode ..");
      LISTBOX_SetScrollStepH(hListBox, 6);
      LISTBOX_SetAutoScrollH(hListBox, 1);
      LISTBOX_SetAutoScrollV(hListBox, 1);
      //LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
			LISTBOX_SetFont(hListBox, &GUI_Font32_ASCII);
			LISTBOX_SetScrollbarWidth(hListBox, 20);
      //CHECKBOX_Check(hItem);
			WM_InvalidateWindow(WM_HBKWIN);			
      break;		
		
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */

		//GUI_MessageBox("This text is shown\nin a message box", "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);
      switch (NCode) {			
        case WM_NOTIFICATION_RELEASED:      /* React only if released */
          switch (Id) {
						case LISTBOX_Id:
								itemtot = LISTBOX_GetNumItems(hListBox);
								curitem = LISTBOX_GetSel(hListBox);								

								if(curitem == itemtot - 1){
										hmainDlgFlag = 0;
										hConfigDlgFlag = 1;
								}
								else{
										LISTBOX_GetItemText(hListBox, curitem, fnamebuf, 30);//显示当前配置;
										operationCode |= OP_SHOW_MODE_MAIN;
								}
								//TEXT_SetText(WM_GetDialogItem(hDlg, TEXT_ID_mainPanelTime), buf);
								break;
								
            case BUTTON_Id_MoreModes:
							//WM_SetFocus(hConfigDlg);
							hListViewDlgFlag = 1;//WM_SetFocus(hmainDlg);
							hmainDlgFlag = 0;
							//WM_DisableWindow(hmainDlg);
							break;
					
						
            case BUTTON_Id_Go:
								operationCode |= OP_GO;
              break;			
          }
          break;
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}


static void _cbListView(WM_MESSAGE * pMsg) {
  int NCode, Id, itemtot, curitem;
  WM_HWIN hDlg, hListBox, hListView;//, hItem;
  hDlg = pMsg->hWin;
  hListBox = WM_GetDialogItem(hDlg, LISTBOX_Id);
	hListView = WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW);
  switch (pMsg->MsgId) {		
		
    case WM_INIT_DIALOG:{
			LISTVIEW_AddColumn(hListView, 100, "Field1", GUI_TA_CENTER);
			LISTVIEW_AddColumn(hListView, 100, "Field2", GUI_TA_CENTER);
			LISTVIEW_AddColumn(hListView, 100, "Field3", GUI_TA_CENTER);
      break;		
		}
		
    case WM_NOTIFY_PARENT:{
      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */

		//GUI_MessageBox("This text is shown\nin a message box", "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);
      switch (NCode) {			
        case WM_NOTIFICATION_RELEASED:      /* React only if released */
          switch (Id) {
								
            case BUTTON_Id_Ok_listview:
							hListViewDlgFlag = 0;
							hmainDlgFlag = 1;
							break;		
          }
          break;
      }
      break;
			
		}
			
    default:
      WM_DefaultProc(pMsg);
  }
}

//自动改变键盘位置相关变量
#include "stringutils.h"
#define _keyboardXLeft 0
#define _keyboardXRight 360
#define _keyboardYTop 0
#define _keyboardYBot 240
unsigned char moveFlag = 0;
unsigned keyboardX = _keyboardXLeft, keyboardY = _keyboardYTop;

//moreModes 进入配置页面
static void _cbCallbackConfigPanel(WM_MESSAGE * pMsg) {
	//static char buf[20];
  int NCode, Id, itemtot, curitem;
  WM_HWIN hDlg, hListBox, hItem;
	static unsigned char touchchildSwitch = 0;
  hDlg = pMsg->hWin;

  switch (pMsg->MsgId) {			
		case WM_INIT_DIALOG:
				EDIT_SetDefaultTextAlign(EDIT_CF_LEFT);
				EDIT_SetMaxLen(WM_GetDialogItem(hDlg, GUI_ID_TEXT_ModeName), 50);
				break;
    case WM_TOUCH_CHILD:
				//break;
				Id    = WM_GetId(pMsg->hWinSrc);
				if((Id == BUTTON_Id_DeleteMode) || (BUTTON_Id_Ok == Id) || (BUTTON_Id_Cancel == Id) || (BUTTON_Id_EditMode == Id)
							|| (Id == BUTTON_Id_SubmitEdit || (Id == LISTBOX_Id))){
						break;
				}

						hkeyboardFlag = 1;
						/*if(Id == EDIT_Group2_ID(0) || Id == EDIT_Group2_ID(1) || Id == EDIT_Group3_ID(0) || Id == EDIT_Group3_ID(1)
								|| Id == EDIT_Group2_ID(5) || Id == EDIT_Group2_ID(6) || Id == EDIT_Group3_ID(5) || Id == EDIT_Group3_ID(6))
						{
										if(!WM_IsWindow(hkeyboard)){							
												keyboardX = _keyboardXRight;
												keyboardY = _keyboardYTop;												
										}
										else{
												hkeyboardFlag = 0;//WM_BringToTop(hkeyboard);
												moveFlag = 2;											
												//WM_InvalidateWindow(WM_HBKWIN);											
										}
						}
						else{							
										if(!WM_IsWindow(hkeyboard)){	
													
													keyboardX = _keyboardXLeft;
													keyboardY = _keyboardYTop;												
										}
										else{
												hkeyboardFlag = 0;//WM_BringToTop(hkeyboard);
												moveFlag = 1;//WM_MoveWindow(hkeyboard, _keyboardXLeft-keyboardX, _keyboardYTop-keyboardY);												
												//WM_InvalidateWindow(WM_HBKWIN);											
										}						
						}		*/
						if(Id == EDIT_Group2_ID(0) || Id == EDIT_Group2_ID(1) || Id == EDIT_Group2_ID(2) || Id == EDIT_Group2_ID(3) || Id == EDIT_Group2_ID(4)
								|| Id == EDIT_Group3_ID(0) || Id == EDIT_Group3_ID(1) || Id == EDIT_Group3_ID(2) || Id == EDIT_Group3_ID(3) || Id == EDIT_Group3_ID(4)
						
								|| Id == GUI_ID_TEXT_ModeName	)
						{
										if(!WM_IsWindow(hkeyboard)){							
												keyboardX = _keyboardXRight;
												keyboardY = _keyboardYBot;												
										}
										else{
												hkeyboardFlag = 0;//WM_BringToTop(hkeyboard);
												moveFlag = 2;											
												//WM_InvalidateWindow(WM_HBKWIN);											
										}
						}
						else{							
										if(!WM_IsWindow(hkeyboard)){	
													
													keyboardX = _keyboardXLeft;
													keyboardY = _keyboardYTop;												
										}
										else{
												hkeyboardFlag = 0;//WM_BringToTop(hkeyboard);
												moveFlag = 1;//WM_MoveWindow(hkeyboard, _keyboardXLeft-keyboardX, _keyboardYTop-keyboardY);												
												//WM_InvalidateWindow(WM_HBKWIN);											
										}						
						}						
						
      break;		
		
		
    case WM_NOTIFY_PARENT:

      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */
      switch (NCode) {
				case WM_NOTIFICATION_CLICKED:
						break;
				/*case WM_NOTIFICATION_MOVED_OUT:
								WM_BringToTop(hkeyboard);
								WM_Invalidate(WM_HBKWIN);
						break;
				case WM_NOTIFICATION_VALUE_CHANGED:
								WM_BringToTop(hkeyboard);
								WM_Invalidate(WM_HBKWIN);
						break;*/
				case WM_NOTIFICATION_SEL_CHANGED:
						break;				
        case WM_NOTIFICATION_RELEASED:      /* React only if released */

          switch (Id) {


            case BUTTON_Id_DeleteMode:
							//do sth and go back
              break;
            case BUTTON_Id_Ok:
							//do sth and go back
							//GUI_EndDialog(hConfigDlg, 0);//WM_DeleteWindow(hConfigDlg);
							if(WM_IsWindow(hkeyboard)){									
									;//hkeyboard = 0;
							}
							//hConfigDlgFlag = 0;//WM_SetFocus(hmainDlg);
							hkeyboardFlag = 0;
							hmainDlgFlag = 1;
							operationCode |= OP_SUBMIT_ADD;
              break;		
							
						case BUTTON_Id_Cancel:
							//do sth and go back
							if(WM_IsWindow(hkeyboard)){									
									//WM_DisableWindow(hkeyboard);								
									//WM_DeleteWindow(hkeyboard);
									//clearKeyBaord(hkeyboard);
									;//hkeyboard = 0;
							}			
							hkeyboardFlag = 0;
							hmainDlgFlag = 1;							
							//WM_Invalidate(WM_HBKWIN);
              break;
            case BUTTON_Id_EditMode:
								hkeyboardFlag = 0;
              break;		
            case BUTTON_Id_SubmitEdit:
								
              break;						
 /*           case GUI_ID_CHECK0:
              _MultiSel ^= 1;
              LISTBOX_SetMulti(hListBox, _MultiSel);
              WM_SetFocus(hListBox);
              LISTBOX_InvalidateItem(hListBox, LISTBOX_ALL_ITEMS);
              break;
            case GUI_ID_CHECK1:
              _OwnerDrawn ^= 1;
              if (_OwnerDrawn) {
                LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
              } else {
                LISTBOX_SetOwnerDraw(hListBox, NULL);
              }
              LISTBOX_InvalidateItem(hListBox, LISTBOX_ALL_ITEMS);
              break;*/
          }
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/


void motorMain(void) {
	
	unsigned char ledcnt = 0, i;
	WM_HWIN hedit;
	LED_Init();
  GUI_Init();
  //WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */	
	//hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);	
	//hmainDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	
	LED0 = 0;
  while (1) {
		if(operationCode & 	OP_SHOW_MODE_MAIN){
			;
		}
		if(operationCode & 	OP_SHOW_MODE_CONF){
			showModeCounter++;
			if(showModeCounter == 2){
					showModeCounter = 0;
					//do the thing
					//read from file and write to speed and duration
					for(i = 0; i < 10; i++){
							hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group2_ID(i));
							EDIT_SetText(hedit, "0");
					}
					for(i = 0; i < 10; i++){
							hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group3_ID(i));
							EDIT_SetText(hedit, "0");			
					}					
			}
		}		
		if(operationCode & 	OP_ADD_NEW_MODE){
				hmainDlgFlag = 0;
				hConfigDlg = 1;
			
/*			showModeCounter=0;
			for(i = 0; i < 10; i++){
					hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group2_ID(i));
					EDIT_SetText(hedit, "");
			}
			for(i = 0; i < 10; i++){
					hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group3_ID(i));
					EDIT_SetText(hedit, "");			
			}
			hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group2_ID(0));
			WM_SetFocus(hedit);*/			
		}		
		if(operationCode & 	OP_SUBMIT_ADD){
			
			
			showModeCounter=0;
			for(i = 0; i < 10; i++){//获取所有edit数值，然后存入speed，duration作为暂存
					hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group2_ID(i));
					speed[i] = (unsigned)EDIT_GetFloatValue(hedit);				
			}
			for(i = 0; i < 10; i++){//获取所有edit数值，然后存入speed，duration作为暂存
					hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group3_ID(i));
					duration[i] = (unsigned)EDIT_GetFloatValue(hedit);				
			}
			strcpy(fnamebuf, "speed");
			hedit = WM_GetDialogItem(hConfigDlg, GUI_ID_TEXT_ModeName);
			EDIT_GetText(hedit, fnamebuf+5, 30-5-1);
			writeToFile(speed, fileReadBufMax, fnamebuf);//再把数据存入文件
			
			strcpy(fnamebuf, "duration");
			EDIT_GetText(hedit, fnamebuf+8, 30-8-1);
			writeToFile(speed, fileReadBufMax, fnamebuf);		

			
		}			
		if(operationCode & 	OP_CONF_OK){
			showModeCounter=0;
			//write speed and duration into sdcard
		}	
		if(operationCode & 	OP_EDIT_MODE){
			showModeCounter=0;
			//获取hConfigDlg所有edit空间，并显示所选mode
			//把focus放到第一个edit上面
		}		
		if(operationCode & 	OP_CONF_CANCEL){
			showModeCounter=0;
		}		
		if(operationCode & 	OP_MORE_MODES){
			;
		}		
		if(operationCode & 	OP_GO){
			;
		}		
		if(operationCode & 	OP_DEFAULT_MODE(1)){
			;
		}	
		if(operationCode & 	OP_DEFAULT_MODE(2)){
			;
		}
		if(operationCode & 	OP_DEFAULT_MODE(3)){
			;
		}		
		operationCode = 0;		
		
    if(!hmainDlgFlag && WM_IsWindow(hmainDlg)){
				GUI_EndDialog(hmainDlg, 0);
				WM_DeleteWindow(hmainDlg);
				hmainDlg = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}
    if(!hConfigDlgFlag && WM_IsWindow(hConfigDlg)){
				GUI_EndDialog(hConfigDlg, 0);
				WM_DeleteWindow(hConfigDlg);
				hConfigDlg = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}	
    if(!hListViewDlgFlag && WM_IsWindow(hListViewDlg)){
				GUI_EndDialog(hListViewDlg, 0);
				WM_DeleteWindow(hListViewDlg);
				hListViewDlg = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}		
    if(!hkeyboardFlag && WM_IsWindow(hkeyboard)){
				clearKeyBaord(hkeyboard);
				WM_DeleteWindow(hkeyboard);
				hkeyboard = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}			
    GUI_Exec();//GUI_Delay(1000);
		GUI_TOUCH_Exec();		
    if(hmainDlgFlag && !WM_IsWindow(hmainDlg)){
				hmainDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
				FRAMEWIN_SetTitleVis(hmainDlg, 0);
				WM_BringToTop(hmainDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
		}
    if(hListViewDlgFlag && !WM_IsWindow(hListViewDlg)){
				hListViewDlg = GUI_CreateDialogBox(_listViewCreate, GUI_COUNTOF(_listViewCreate), &_cbListView, 0, 0, 0);
				FRAMEWIN_SetTitleVis(hListViewDlg, 0);
				WM_BringToTop(hListViewDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
		}		
    if(hConfigDlgFlag && !WM_IsWindow(hConfigDlg)){
				hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);				
				drawEditGroup(editGroupX0, editGroupY0, 799, 479, WM_GetClientWindow(hConfigDlg));
				_setHook(hConfigDlg);//WM_GetClientWindow(hConfigDlg));//到底挂上谁？？
				WM_BringToTop(hConfigDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
		}		
    if(hkeyboardFlag && !WM_IsWindow(hkeyboard)){
				hkeyboard = CreateKeyBaord(keyboardX, keyboardY);
				WM_BringToTop(hkeyboard);
				//WM_SetStayOnTop(hkeyboard, 1);
				//WM_InvalidateWindow(WM_HBKWIN);	
		}	

				if(moveFlag == 1){
						keyboardX = _keyboardXLeft;
						keyboardY = _keyboardYTop;
						clearKeyBaord(hkeyboard);
						WM_DeleteWindow(hkeyboard);
						hkeyboard = 0;
						WM_InvalidateWindow(WM_HBKWIN);	
						hkeyboardFlag = 1;
						moveFlag = 0;
						GUI_Exec();
				}
				else if(moveFlag == 2){			
						keyboardX = _keyboardXLeft;
						keyboardY = _keyboardYBot;
						clearKeyBaord(hkeyboard);
						WM_DeleteWindow(hkeyboard);
						hkeyboard = 0;
						WM_InvalidateWindow(WM_HBKWIN);	
						hkeyboardFlag = 1;		
						moveFlag = 0;
						GUI_Exec();
				}
	

		delay_ms(15);
		if(++ledcnt == 60){
				ledcnt = 0;
				LED0 = ~LED0;
		}
  }
}

/*************************** End of file ****************************/

