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

//主要窗体的句柄以及有效标记
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
#define OP_SHOW_MODE_MAIN 1
#define OP_SHOW_MODE 2
#define OP_MORE_MODES 64

#define OP_GO 128
#define OP_DELETE_MODE 4
#define OP_SUBMIT_ADD 8
#define OP_CONF_OK 16
#define OP_CONF_CANCEL 32
#define OP_MOVE_DOWN 2048
#define OP_MOVE_UP (4096)
#define OP_DEFAULT_MODE(i) (OP_GO << i)

unsigned operationCode  = 0;
WM_HWIN hCurrentObj;
//unsigned selectedItemInListView = -1;
unsigned char ifAListViewItemIsSelectedFlag = 0;

//窗体描述
#define lcdWidth 800
#define lcdHeight 480

#define gapYProgbarUp 30
#define gapYLabelProgbar 50
#define gapYModeButtonLabel 50
#define gapYConfigButtonModeButton 30
#define gapYConfigButtonDown 30

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

#define configButtonWidth 150
#define configButtonHeight 90

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

//control id
//main panel
#define FRAME_ID_mainPanel (GUI_ID_USER+1)
#define BUTTON_Id_MoreModes (GUI_ID_USER+2)
#define BUTTON_Id_Progbar (GUI_ID_USER + 3)
#define BUTTON_Id_Mode1 (GUI_ID_USER + 4)
#define BUTTON_Id_Mode2 (GUI_ID_USER + 5)
#define BUTTON_Id_Mode3 (GUI_ID_USER + 6)
#define TEXT_ID_mainPanelTime (GUI_ID_USER+7)
#define TEXT_ID_mainPanelSpeed (GUI_ID_USER+8)
//listview panel
#define FRAME_ID_ListViewPanel (GUI_ID_USER+9)
#define BUTTON_Id_Ok_listview (GUI_ID_USER+10)
#define BUTTON_Id_EditMode (GUI_ID_USER+11)
#define BUTTON_Id_Up (GUI_ID_USER+12)
#define BUTTON_Id_Down (GUI_ID_USER+13)
#define BUTTON_Id_AddNewMode (GUI_ID_USER+14)
#define BUTTON_Id_DeleteMode (GUI_ID_USER+15)
#define GUI_ID_LISTVIEW (GUI_ID_USER+16)
#define BUTTON_Id_Back (GUI_ID_USER + 17)
#define BUTTON_Id_ClearFlash (GUI_ID_USER + 26)

//edit panel
#define FRAME_ID_confPanel (GUI_ID_USER+18)
#define BUTTON_Id_Ok (GUI_ID_USER+19)
#define BUTTON_Id_Cancel (GUI_ID_USER+20)
#define BUTTON_Id_EditMode (GUI_ID_USER+21)
#define BUTTON_Id_SubmitEdit (GUI_ID_USER+22)
#define GUI_ID_TEXT_ModeName (GUI_ID_USER+23)
#define GUI_ID_EDIT_ModeName (GUI_ID_USER + 24)
#define LISTBOX_Id (GUI_ID_USER+25)
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
#define mainPanelButtonGapX 40
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,  "Main Panel",    FRAME_ID_mainPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
	{ PROGBAR_CreateIndirect,   "Progress",          BUTTON_Id_Progbar,       progBarX,  progBarY,  progBarWidth,  progBarHeight, WM_CF_SHOW },
	{ TEXT_CreateIndirect,      "Time: ",    TEXT_ID_mainPanelTime,       labelX,  labelY,  labelWidth,  labelHeight, TEXT_CF_LEFT },
	{	TEXT_CreateIndirect,      "Speed: ",    TEXT_ID_mainPanelSpeed,       labelX,  labelY+labelHeight,  labelWidth,  labelHeight, TEXT_CF_LEFT },
/* Buttons */
  //{ BUTTON_CreateIndirect,    "Default MODE 1",      BUTTON_Id_DefaultMode1,     modeButtonX,  modeButtonY(1),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  //{ BUTTON_CreateIndirect,    "Default MODE 2",      BUTTON_Id_DefaultMode2,     modeButtonX,  modeButtonY(2),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "Default MODE 3",      BUTTON_Id_DefaultMode3,     modeButtonX,  modeButtonY(3),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "More Modes",  BUTTON_Id_MoreModes,     0,  lcdHeight - 20 -configButtonHeight,  configButtonWidth,  configButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Run Mode2",      BUTTON_Id_Mode2,     400 - switchButtonWidth/2,  240 - switchButtonHeight/2,  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Run Mode1",      BUTTON_Id_Mode1,     400 - mainPanelButtonGapX - switchButtonWidth - switchButtonWidth/2,  240 - switchButtonHeight/2,  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Run Mode3",      BUTTON_Id_Mode3,     400 + mainPanelButtonGapX + switchButtonWidth - switchButtonWidth/2,  240 - switchButtonHeight/2,  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
//{BUTTON_CreateIndirect,    "Stop!",      BUTTON_Id_Stop,     switchButtonX,  switchButtonY(2),  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	//{ LISTBOX_CreateIndirect,   0,                LISTBOX_Id,  5,  labelY+labelHeight*2, 200, 260, 0, 100 },
};

#define ConfButtonWidth 80
#define ConfButtonHeight 60


static const GUI_WIDGET_CREATE_INFO _configDialogCreate[] = {
	{FRAMEWIN_CreateIndirect,  "Config Panel",    FRAME_ID_confPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
  
  { EDIT_CreateIndirect, 			0, GUI_ID_EDIT_ModeName, 400-150, editGroupY0-32, 300, 50, WM_CF_SHOW},
	{ TEXT_CreateIndirect,      "Mode Name:",     GUI_ID_TEXT_ModeName,    editGroupX0,  editGroupY0-32,  200,  32, TEXT_CF_LEFT },
  { BUTTON_CreateIndirect,    "OK",      BUTTON_Id_Ok,     0/*10+modeButtonWidth+5-100*/,  lcdHeight - ConfButtonHeight -10,  ConfButtonWidth,  ConfButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "CANCEL",      BUTTON_Id_Cancel,     800-modeButtonWidth/*10+2*modeButtonWidth+5*2-100*/,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "EDIT MODE",      BUTTON_Id_EditMode,     50+60+60+3*modeButtonWidth+5*3,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "SUBMIT",      BUTTON_Id_SubmitEdit,     50+40+120+4*modeButtonWidth+5*4,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "clear flash",      BUTTON_Id_ClearFlash,     400,  400,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
};

#define ListViewButtonBackX0 0
#define ListViewButtonBackY0 0
#define ListViewButtonBackWidth 60
#define ListViewButtonBackHeight 480


#define ListViewX0 (ListViewButtonBackX0  + ListViewButtonBackWidth + 10)
#define ListViewY0 (ListViewButtonBackY0 + 40)
#define ListViewWidth lcdWidth - 180
#define ListViewHeight lcdHeight - 80
#define ListViewButtonGapX 12
#define ListViewButtonGapY 15

#define ListViewButtonWidth (lcdWidth - (ListViewX0+ListViewWidth)-10)//70
#define ListViewButtonHeight ((lcdHeight - 4*ListViewButtonGapY)/5)

#define ListViewButtonX0 (ListViewX0 + ListViewWidth + ListViewButtonGapX)

#define ListViewButtonUpY0 0//(ListViewY0+ListViewButtonGapY+40)
#define ListViewButtonDownY0 (ListViewButtonHeight+ListViewButtonGapY)//(ListViewButtonUpY0 + ListViewButtonGapY+ListViewButtonHeight)
#define ListViewButtonAddNewModeY0 (2*(ListViewButtonHeight+ListViewButtonGapY))//(ListViewButtonDownY0 + ListViewButtonGapY+ListViewButtonHeight)
#define ListViewButtonEditModeY0 (3*(ListViewButtonHeight+ListViewButtonGapY))//(ListViewButtonAddNewModeY0 + ListViewButtonGapY+ListViewButtonHeight)
#define ListViewButtonDeleteModeY0 (4*(ListViewButtonHeight+ListViewButtonGapY))//(ListViewButtonEditModeY0 + ListViewButtonGapY+ListViewButtonHeight)

static const GUI_WIDGET_CREATE_INFO _listViewCreate[] = {
	{FRAMEWIN_CreateIndirect,  "Config Panel",    FRAME_ID_ListViewPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
  {LISTVIEW_CreateIndirect,  "list view",    GUI_ID_LISTVIEW,       ListViewX0,  ListViewY0, ListViewWidth, ListViewHeight, WM_CF_SHOW },
  //{ EDIT_CreateIndirect, 			0, GUI_ID_TEXT_ModeName, editGroupX0+100, editGroupY0-32, 200, 32, WM_CF_SHOW},
	//{ TEXT_CreateIndirect,      "Mode Name:",     GUI_ID_TEXT_ModeName,    editGroupX0,  editGroupY0-32,  200,  32, TEXT_CF_LEFT },
  { BUTTON_CreateIndirect,    "Delete Mode",    BUTTON_Id_DeleteMode,      ListViewButtonX0,  ListViewButtonDeleteModeY0,  ListViewButtonWidth,  ListViewButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "Add New Mode",   BUTTON_Id_AddNewMode,     ListViewButtonX0,  ListViewButtonAddNewModeY0,  ListViewButtonWidth,  ListViewButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "Up",      				BUTTON_Id_Up,          ListViewButtonX0,  ListViewButtonUpY0,  ListViewButtonWidth,  ListViewButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "Down",      			BUTTON_Id_Down,        ListViewButtonX0,  ListViewButtonDownY0,  ListViewButtonWidth,  ListViewButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "Edit Mode",      BUTTON_Id_EditMode,      ListViewButtonX0,  ListViewButtonEditModeY0,  ListViewButtonWidth,  ListViewButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "Back",      			BUTTON_Id_Back,        ListViewButtonBackX0,  ListViewButtonBackY0,  ListViewButtonBackWidth,  ListViewButtonBackHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "Back",      			BUTTON_Id_Back,        ListViewButtonBackX0,  ListViewButtonBackY0,  ListViewButtonBackWidth,  ListViewButtonBackHeight, WM_CF_SHOW},

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
      break;		
		
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */

      switch (NCode) {			
        case WM_NOTIFICATION_RELEASED:
          switch (Id) {
            case BUTTON_Id_MoreModes:
							hListViewDlgFlag = 1;
							hmainDlgFlag = 0;
							break;		
            case BUTTON_Id_Mode1:
								operationCode |= OP_DEFAULT_MODE(1);
							break;
            case BUTTON_Id_Mode2:
							operationCode |= OP_DEFAULT_MODE(2);
							break;
            case BUTTON_Id_Mode3:
							operationCode |= OP_DEFAULT_MODE(3);
							break;						
          }
          break;
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}


char *bufMotorMain[10] = {
		"1", "2", "3", "4", "5", "6", "7"
};;
unsigned char ListViewAddRowFlag;
char * _aTable_1[3][2];
/*{
  { "1", "Default Mode 1" },
};*/

#define addOrEditFlagEdit 1
#define addOrEditFlagWrite 0
unsigned char addOrEditFlag = addOrEditFlagWrite;
unsigned editIndex = 0;
WM_HWIN hListView;
unsigned selectedListViewItem = 0;
static void _cbListView(WM_MESSAGE * pMsg) {
  int NCode, Id, itemtot, curitem, i, j;
  WM_HWIN hDlg, hListBox, hListView;//, hItem;
  hDlg = pMsg->hWin;
  hListBox = WM_GetDialogItem(hDlg, LISTBOX_Id);
	hListView = WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW);
	
  switch (pMsg->MsgId) {		
		
    case WM_INIT_DIALOG:{
			LISTVIEW_AddColumn(hListView, 100, "Mode Index", GUI_TA_CENTER);
			LISTVIEW_AddColumn(hListView, 100, "Mode Name", GUI_TA_CENTER);
			LISTVIEW_SetColumnWidth(hListView, 1, ListViewWidth/2);
			LISTVIEW_SetRowHeight(hListView, 32);
			//显示flash中已经存储的数据
				i = 0;
				while(readData(curModeNameBuf, curSpeed, curDuration, i)){
						
						_aTable_1[0][0] = bufMotorMain[i];
						_aTable_1[0][1] = curModeNameBuf;
						LISTVIEW_AddRow(hListView, _aTable_1[0]);
						++i;
				}			

			WM_InvalidateWindow(WM_HBKWIN);	
      break;		
		}
		
    case WM_NOTIFY_PARENT:{
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;

		//GUI_MessageBox("This text is shown\nin a message box", "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);
      switch (NCode) {			
        case WM_NOTIFICATION_RELEASED:      /* React only if released */
          switch (Id) {
            case BUTTON_Id_Back:
							hListViewDlgFlag = 0;
							hmainDlgFlag = 1;
							break;								
						case BUTTON_Id_EditMode:
								if( (i = LISTVIEW_GetSel(hListView)) >= 0 && i < groupIndexCounter){
										operationCode |= OP_SHOW_MODE;		
										readData(curModeNameBuf, curSpeed, curDuration, i);
										addOrEditFlag = addOrEditFlagEdit;
										editIndex = i;
								}
									hListViewDlgFlag = 0;
									hConfigDlgFlag = 1;	
								break;								
            case BUTTON_Id_AddNewMode:
									operationCode |= OP_SHOW_MODE;
									addOrEditFlag = addOrEditFlagWrite;
									*curModeNameBuf = 0;
									for(i = 0; i < maxstep; ++i){
											curSpeed[i] = 0;
											curDuration[i] = 0;
									}
						
									hListViewDlgFlag = 0;
									hConfigDlgFlag = 1;	
								break;
						case GUI_ID_LISTVIEW:
							
							break;
						case BUTTON_Id_DeleteMode:
									operationCode |= OP_DELETE_MODE;
									selectedListViewItem = LISTVIEW_GetSel(hListView);
						
							break;
						case BUTTON_Id_Up:
									operationCode |= OP_MOVE_UP;
									selectedListViewItem = LISTVIEW_GetSel(hListView);
						
							break;	
						case BUTTON_Id_Down:
									operationCode |= OP_MOVE_DOWN;
									selectedListViewItem = LISTVIEW_GetSel(hListView);
						
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

void test(EDIT_Handle);
//自动改变键盘位置相关变量
#include "stringutils.h"
#define _keyboardXLeft 0
#define _keyboardXRight 450
#define _keyboardYTop 0
#define _keyboardYBot 0
unsigned char moveFlag = 0;
unsigned keyboardX = _keyboardXLeft, keyboardY = _keyboardYTop;
char _cbConfigBuf[10];
	char bufStr2int[10];

static void _cbCallbackConfigPanel(WM_MESSAGE * pMsg) {
	//static char buf[20];
  int NCode, Id, itemtot, curitem, i;
	unsigned tmp;
  WM_HWIN hDlg, hListBox, hItem, hedit;
	static unsigned char touchchildSwitch = 0;
  hDlg = pMsg->hWin;

  switch (pMsg->MsgId) {			
		case WM_INIT_DIALOG:
				EDIT_SetDefaultTextAlign(EDIT_CF_LEFT);
				EDIT_SetMaxLen(WM_GetDialogItem(hDlg, GUI_ID_EDIT_ModeName), 50);
				break;
		
    case WM_TOUCH_CHILD:
				Id    = WM_GetId(pMsg->hWinSrc);
				if((Id == BUTTON_Id_DeleteMode) || (BUTTON_Id_Ok == Id) || (BUTTON_Id_Cancel == Id) || (BUTTON_Id_EditMode == Id)
							|| (Id == BUTTON_Id_SubmitEdit || (Id == LISTBOX_Id))
							|| (Id == BUTTON_Id_ClearFlash)
				
					){
						break;
				}

						hkeyboardFlag = 1;
						if(Id == EDIT_Group2_ID(0) || Id == EDIT_Group2_ID(1) || Id == EDIT_Group2_ID(2) || Id == EDIT_Group2_ID(3) || Id == EDIT_Group2_ID(4)
								|| Id == EDIT_Group3_ID(0) || Id == EDIT_Group3_ID(1) || Id == EDIT_Group3_ID(2) || Id == EDIT_Group3_ID(3) || Id == EDIT_Group3_ID(4)
						
								|| Id == GUI_ID_EDIT_ModeName	)
						{
										if(!WM_IsWindow(hkeyboard)){							
												keyboardX = _keyboardXRight;
												keyboardY = _keyboardYBot;	
												moveFlag = 0;
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
													moveFlag = 0;
										}
										else{
												hkeyboardFlag = 0;
												moveFlag = 1;									
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
							operationCode |= OP_CONF_OK;//test(hedit);//operationCode |= OP_SUBMIT_ADD;	
							ListViewAddRowFlag = 1;
							{
								hedit = WM_GetDialogItem(hConfigDlg, GUI_ID_EDIT_ModeName);
								EDIT_GetText(hedit, curModeNameBuf, ModeNameLenMax);
								for(i = 0; i < 10; i++){//获取所有edit数值，然后存入speed，duration作为暂存
										hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group2_ID(i));					
										EDIT_GetText(hedit, bufStr2int, 10);
										istr2num(&tmp, bufStr2int);
										curSpeed[i] = tmp;				
								}
								for(i = 0; i < 10; i++){//获取所有edit数值，然后存入speed，duration作为暂存
										hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group3_ID(i));
										EDIT_GetText(hedit, bufStr2int, 10);
										istr2num(&tmp, bufStr2int);
										curDuration[i] = tmp;			
								}			
							}
							{								
									if(addOrEditFlag == addOrEditFlagWrite){
											writeData(curModeNameBuf, curSpeed, curDuration);
											refreshGroupIndex();
									}
									else{
											editData(curModeNameBuf, curSpeed, curDuration, editIndex);	
											editIndex = -1;
									}								
							}
						case BUTTON_Id_Cancel:
							hkeyboardFlag = 0;
							hListViewDlgFlag = 1;
							//hConfigDlgFlag;
              break;
						case BUTTON_Id_ClearFlash:
								clearFlash();
							break;
						
          }
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       MainTask
*/
void motorMain(void) {

	unsigned char ledcnt = 0, i, j;
	unsigned tmp;
	WM_MESSAGE theMsg;
	WM_HWIN hedit;
	LED_Init();
  GUI_Init();
  //WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */	
	//hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);	
	//hmainDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	//clearFlash();
	initData();

	LED0 = 0;
  while (1) {
		if(operationCode & 	OP_SHOW_MODE_MAIN){
			operationCode &= ~OP_SHOW_MODE_MAIN;
		}
		
		if(operationCode & OP_DELETE_MODE){
			
				operationCode &= ~OP_DELETE_MODE;
				for(j = selectedListViewItem; j < groupIndexCounter-1; ++j)
						groupIndex[j] = groupIndex[j+1];
				--groupIndexCounter;
				refreshGroupIndex();	

				GUI_EndDialog(hListViewDlg, 0);
				WM_DeleteWindow(hListViewDlg);
				hListViewDlg = GUI_CreateDialogBox(_listViewCreate, GUI_COUNTOF(_listViewCreate), &_cbListView, 0, 0, 0);
				FRAMEWIN_SetTitleVis(hListViewDlg, 0);				
				WM_BringToTop(hListViewDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
			
		}

		if(operationCode & OP_MOVE_UP){
			
				operationCode &= ~OP_MOVE_UP;
				if(selectedListViewItem > 0 && selectedListViewItem < groupIndexCounter){
						groupIndex[selectedListViewItem] ^= groupIndex[selectedListViewItem - 1];
						groupIndex[selectedListViewItem-1] ^= groupIndex[selectedListViewItem];
						groupIndex[selectedListViewItem] ^= groupIndex[selectedListViewItem - 1];
						refreshGroupIndex();	
						GUI_EndDialog(hListViewDlg, 0);
						WM_DeleteWindow(hListViewDlg);
						hListViewDlg = GUI_CreateDialogBox(_listViewCreate, GUI_COUNTOF(_listViewCreate), &_cbListView, 0, 0, 0);
						FRAMEWIN_SetTitleVis(hListViewDlg, 0);				
						WM_BringToTop(hListViewDlg);
						WM_InvalidateWindow(WM_HBKWIN);	
				}			
		}
		
		if(operationCode & OP_MOVE_DOWN){
			
				operationCode &= ~OP_MOVE_DOWN;
				if(selectedListViewItem >= 0 && selectedListViewItem < groupIndexCounter - 1){
						groupIndex[selectedListViewItem] ^= groupIndex[selectedListViewItem + 1];
						groupIndex[selectedListViewItem+1] ^= groupIndex[selectedListViewItem];
						groupIndex[selectedListViewItem] ^= groupIndex[selectedListViewItem + 1];
						refreshGroupIndex();	
						GUI_EndDialog(hListViewDlg, 0);
						WM_DeleteWindow(hListViewDlg);
						hListViewDlg = GUI_CreateDialogBox(_listViewCreate, GUI_COUNTOF(_listViewCreate), &_cbListView, 0, 0, 0);
						FRAMEWIN_SetTitleVis(hListViewDlg, 0);				
						WM_BringToTop(hListViewDlg);
						WM_InvalidateWindow(WM_HBKWIN);	
				}				
		}		
		
		if(operationCode & 	OP_SUBMIT_ADD){			
			showModeCounter=0;	
			operationCode &= ~OP_SUBMIT_ADD;
		}		
		
		if(operationCode & 	OP_CONF_OK){
			operationCode &= ~OP_CONF_OK;
			showModeCounter=0;
			
			//collect data into ram buf			
			//write newly added or revised data into flash

			//if buf is top 3, write it into one of the 3 buf			
			//write buf into ListView Row buf		

		}	
			
		if(operationCode & 	OP_CONF_CANCEL){
			operationCode &= ~OP_CONF_CANCEL;
			showModeCounter=0;
		}		
		if(operationCode & 	OP_MORE_MODES){
			operationCode &= ~OP_MORE_MODES;
			;
		}		
	
		if(operationCode & 	OP_DEFAULT_MODE(1)){
			operationCode &= ~OP_DEFAULT_MODE(1);
		}	
		if(operationCode & 	OP_DEFAULT_MODE(2)){
			operationCode &= ~OP_DEFAULT_MODE(2);
		}
		if(operationCode & 	OP_DEFAULT_MODE(3)){
			operationCode &= ~OP_DEFAULT_MODE(3);
		}		
		
		
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
    GUI_Exec();
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
    if(hConfigDlgFlag  && !WM_IsWindow(hConfigDlg)){
			
					hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);					
					drawEditGroup(editGroupX0, editGroupY0, 799, 479, WM_GetClientWindow(hConfigDlg));
					_setHook(hConfigDlg);//WM_GetClientWindow(hConfigDlg));//到底挂上谁？？
					WM_BringToTop(hConfigDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
		}
    if(hkeyboardFlag && !WM_IsWindow(hkeyboard)){
				hkeyboard = CreateNumKeyBaord(keyboardX, keyboardY);
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
				
				
	if((operationCode & 	OP_SHOW_MODE) && WM_IsWindow(hConfigDlg)){
				operationCode &= ~OP_SHOW_MODE;			
					
						hedit = WM_GetDialogItem(hConfigDlg, GUI_ID_EDIT_ModeName);
						EDIT_SetText(hedit, curModeNameBuf);
					
						for(i = 0; i < maxstep; ++i){
								hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group2_ID(i));
								int2str(_cbConfigBuf, curSpeed[i]);
								EDIT_SetText(hedit, _cbConfigBuf);
							//curSpeed[i] = (unsigned)EDIT_GetFloatValue(hedit);
						}
						for(i = 0; i < maxstep; ++i){
								hedit = WM_GetDialogItem(hConfigDlg, EDIT_Group3_ID(i));
								int2str(_cbConfigBuf, curDuration[i]);
								EDIT_SetText(hedit, _cbConfigBuf);
						}					
						WM_InvalidateWindow(WM_HBKWIN);	
	
		}	

		
		delay_ms(15);
		if(++ledcnt == 60){
				ledcnt = 0;
				LED0 = ~LED0;
		}
  }
}

/*************************** End of file ****************************/

