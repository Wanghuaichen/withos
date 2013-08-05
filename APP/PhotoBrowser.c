#include "PhotoBrowser.h"

#define Exit      1
#define OpenFile  2
#define Back      3

u8 Exit_Browser;
char  buf1[38] = {"0:/PHOTOS/"};

/********************************************建立资源表**************************************************/
const GUI_WIDGET_CREATE_INFO PhotoBrowseDialog[] = {
	{ FRAMEWIN_CreateIndirect,"图片列表(PHOTOS)",0,0,0,240,320,FRAMEWIN_CF_MOVEABLE},
  { LISTBOX_CreateIndirect,0,GUI_ID_LISTBOX0,2,4,231,262,WM_CF_SHOW},
  { BUTTON_CreateIndirect,"查看",Button_ID8,40, 270,50, 25},
	{ BUTTON_CreateIndirect,"退出",Button_ID9,130,270,50,25}
};
/********************************************************************************************************** 
** Function name:      PhotoBrowseCallBack
** Descriptions:       图片列表对话框回调函数
** input parameters:   WM_MESSAGE *pMsg：消息
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
static void PhotoBrowseCallBack(WM_MESSAGE *pMsg)
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
		  FatFs_Open("0:/PHOTOS");//打开SD卡，获取根目录列表
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
						strcpy(buf1,"0:/PHOTOS/");
						LISTBOX_GetItemText(hListBox,LISTBOX_GetSel(hListBox),buf,30);
						buf1[10] = '\0';
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
** Function name:      Task_PhotoBrowser
** Descriptions:       图片浏览器任务
** input parameters:   void *pdata
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void Task_PhotoBrowser(void *pdata)
{	
// 	WM_HWIN hWM;
	GUI_CreateDialogBox(PhotoBrowseDialog, GUI_COUNTOF(PhotoBrowseDialog), PhotoBrowseCallBack, 0, 0, 0);
	Exit_Browser = 0;
	/*******************************************************************************************************/
	while(1){
		if(TouchCmd == 1){
			TouchCmd = 0;	
			if(Exit_Browser == 3){
				Exit_Browser = 0;
		    GUI_CreateDialogBox(PhotoBrowseDialog, GUI_COUNTOF(PhotoBrowseDialog), PhotoBrowseCallBack, 0, 0, 0);
			}
	  }
		if(Exit_Browser == Exit){
			Exit_Browser = 0;
// 			WM_DeleteWindow(hWM);
			WM_SetDesktopColor(DesktopColor);
		  Go_MainMenu();              //进入主界面
			OSTaskDel(OS_PRIO_SELF);    //删除自己
		} else if(Exit_Browser == OpenFile) {
			Exit_Browser = 3;
// 			WM_DeleteWindow(hWM);
		  WM_SetDesktopColor(DesktopColor);
			GUI_Exec();//重绘
		  BmpDecode((u8 *)buf1,0,0,240,320,1);
		}
	  OSTimeDly(10);   //保证50Hz的执行频率
	}
}
