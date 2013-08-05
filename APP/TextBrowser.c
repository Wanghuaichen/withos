#include "TextBrowser.h"

#define Exit      1
#define OpenFile  2
#define Back      3

extern u8 Exit_Browser;
extern char  buf1[];
extern u8 File_Buffer[1024];
extern FIL f_file;

/********************************************建立资源表**************************************************/
const GUI_WIDGET_CREATE_INFO TextBrowseDialog[] = {
	{ FRAMEWIN_CreateIndirect,"文本列表(TEXT)",0,0,0,240,320,FRAMEWIN_CF_MOVEABLE},
  { LISTBOX_CreateIndirect,0,GUI_ID_LISTBOX0,2,4,231,262,WM_CF_SHOW},
  { BUTTON_CreateIndirect,"打开",Button_ID8,40, 270,50, 25},
	{ BUTTON_CreateIndirect,"退出",Button_ID9,130,270,50,25}
};
const GUI_WIDGET_CREATE_INFO TextViewDialog[] = {
	{ FRAMEWIN_CreateIndirect,"",0,0,0,240,320,FRAMEWIN_CF_MOVEABLE},
  { MULTIEDIT_CreateIndirect,0,GUI_ID_MULTIEDIT0,2,4,231,262,WM_CF_SHOW},
  { BUTTON_CreateIndirect,"上一页",Button_ID7,2, 270,60, 25},
  { BUTTON_CreateIndirect,"退出",Button_ID8,90, 270,60, 25},
	{ BUTTON_CreateIndirect,"下一页",Button_ID9,173,270,60,25}
};

/********************************************************************************************************** 
** Function name:      TextBrowseCallBack
** Descriptions:       文本列表对话框回调函数
** input parameters:   WM_MESSAGE *pMsg：消息
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
static void TextBrowseCallBack(WM_MESSAGE *pMsg)
{
	int Id,NCode;
	char  buf[30];
// 	u16   file_num = 0;
  LISTBOX_Handle hListBox;
	WM_HWIN hWin = pMsg->hWin;
	hListBox = WM_GetDialogItem(hWin,GUI_ID_LISTBOX0);
	switch(pMsg -> MsgId) {
		
		case WM_INIT_DIALOG:
			MenuDisnable++;
		  LISTBOX_SetAutoScrollV (hListBox, 1);
// 		FRAMEWIN_AddCloseButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT,0);
      FRAMEWIN_AddMaxButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
 	    FRAMEWIN_AddMinButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
		  FatFs_Open("0:/TEXT");//打开SD卡，获取根目录列表
			for(Id = 2;Id < filecount;Id++){ //前两项是“.”和“..”目录项，应跳过。
		    LISTBOX_AddString(hListBox, (char*)FileList[Id]);
	    }
			break;
	  case WM_NOTIFY_PARENT:
		  Id = WM_GetId(pMsg ->hWinSrc);
		  NCode = pMsg ->Data.v;
		  switch(NCode) {
				
		    case WM_NOTIFICATION_RELEASED:
			    if (Id == Button_ID8) {
						strcpy(buf1,"0:/TEXT/");
						LISTBOX_GetItemText(hListBox,LISTBOX_GetSel(hListBox),buf,30);
						buf1[8] = '\0';
						strcat(buf1,buf);
						ClearPath(FilePath);
            Exit_Browser = OpenFile;
						GUI_EndDialog(pMsg->hWin, 1);
						MenuDisnable--;
					} else if(Id == Button_ID9) {
						ClearPath(FilePath);	
						Exit_Browser = Exit;
						GUI_EndDialog(pMsg->hWin, 1);
						MenuDisnable--;
					}
					break;
				default:
				  break;
		  }		
      break;			
		default:
      WM_DefaultProc(pMsg);
		  break;
	 }
}
/********************************************************************************************************** 
** Function name:      TextViewCallBack
** Descriptions:       文本列表对话框回调函数
** input parameters:   WM_MESSAGE *pMsg：消息
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
static void TextViewCallBack(WM_MESSAGE *pMsg)
{
	int Id,NCode;
// 	u16   file_num = 0;
	switch(pMsg -> MsgId) {
		
		case WM_INIT_DIALOG:
			MenuDisnable++;
// 		FRAMEWIN_AddCloseButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT,0);
      FRAMEWIN_AddMaxButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
 	    FRAMEWIN_AddMinButton(pMsg->hWin, FRAMEWIN_BUTTON_RIGHT, 0);
		  FRAMEWIN_SetText(pMsg->hWin, (const char*)buf1);
		  MULTIEDIT_SetReadOnly(WM_GetDialogItem(pMsg->hWin,GUI_ID_MULTIEDIT0), MULTIEDIT_CF_READONLY);
			MULTIEDIT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_MULTIEDIT0),&GUI_FontHZ_MicrosoftFont_13);		
		  MULTIEDIT_SetAutoScrollH(WM_GetDialogItem(pMsg->hWin,GUI_ID_MULTIEDIT0), 1);
		  MULTIEDIT_SetAutoScrollV(WM_GetDialogItem(pMsg->hWin,GUI_ID_MULTIEDIT0), 1);
			break;
	  case WM_NOTIFY_PARENT:
		  Id = WM_GetId(pMsg ->hWinSrc);
		  NCode = pMsg ->Data.v;
		  switch(NCode) {
				
		    case WM_NOTIFICATION_RELEASED:
			    if (Id == Button_ID7) {

					} else if(Id == Button_ID8) {
						Exit_Browser = Back;
						GUI_EndDialog(pMsg->hWin, 1);
						MenuDisnable--;
					} else if(Id == Button_ID9) {

					}
					break;
				default:
				  break;
		  }		
      break;			
		default:
      WM_DefaultProc(pMsg);
		  break;
	 }
}
/********************************************************************************************************** 
** Function name:      Task_TextBrowser
** Descriptions:       文本浏览器任务
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void Task_TextBrowser(void *pdata)
{	
	WM_HWIN hWM;
	OS_CPU_SR  cpu_sr;
	u16 br = 0;
	GUI_CreateDialogBox(TextBrowseDialog, GUI_COUNTOF(TextBrowseDialog), TextBrowseCallBack, 0, 0, 0);
	Exit_Browser = 0;
	/*******************************************************************************************************/
	while(1){
		if(TouchCmd == 1){
			TouchCmd = 0;	
	  }
		if(Exit_Browser == Back){
				Exit_Browser = 0;
		    GUI_CreateDialogBox(TextBrowseDialog, GUI_COUNTOF(TextBrowseDialog), TextBrowseCallBack, 0, 0, 0);
		}
		if(Exit_Browser == Exit){
			Exit_Browser = 0;
// 			WM_DeleteWindow(hWM);
			WM_SetDesktopColor(DesktopColor);
		  Go_MainMenu();              //进入主界面
			OSTaskDel(OS_PRIO_SELF);    //删除自己
		} else if(Exit_Browser == OpenFile) {
			Exit_Browser = 0;
			OS_ENTER_CRITICAL(); 
			f_open(&f_file,(const TCHAR*)buf1,FA_READ);//打开文件	
			f_read(&f_file,File_Buffer,1020,(UINT*)&br);
			f_close(&f_file);//关闭文件
			OS_EXIT_CRITICAL();
			buf1[1023] = 0;
// 			WM_DeleteWindow(hWM);
		  WM_SetDesktopColor(DesktopColor);
			GUI_Exec();//重绘
			hWM = GUI_CreateDialogBox(TextViewDialog, GUI_COUNTOF(TextViewDialog), TextViewCallBack, 0, 0, 0);
			MULTIEDIT_SetText(WM_GetDialogItem(hWM,GUI_ID_MULTIEDIT0), (const char*)File_Buffer);
		}
	  OSTimeDly(10);   //保证50Hz的执行频率
	}
}
