#ifndef _FileBrowser_h
#define _FileBrowser_h

#include "stm32f10x.h"
#include "includes.h"
#include "GUI.H"
#include "My_UI.h"
#include "BUTTON.h"
#include "progbar.h"
#include "framewin.h"
#include "TEXT.h"
#include "WM.h"
#include "LISTBOX.h"
#include "MENU.h"
#include "Fat_API.h"
#include "SD.h"	
extern const GUI_FONT GUI_FontHZ_MicrosoftFont_13;
extern const GUI_FONT GUI_FontHZ_YouYuan_24;

extern FATFS fs; 
extern u8 SD_Buffer[];
extern u8 FileList[MaxFileCount][30];//最多支持100个文件
extern char FilePath[];
extern u16 filecount;

extern MenuInfo    OtherMenu_Info;
extern u8 Menu_ID;  //菜单界面的ID
extern SingleList  OtherMenu_pHead;    /* 其他界面的控件链表表头指针      */
extern MenuInfo    OtherMenu_Info;
extern u8 TouchCmd;            //标志着是点触还是滑动操作。
extern GUI_COLOR  ThemeColor;
extern u8 MenuDisnable;
extern WM_HWIN hDesktopWindow; //桌面的句柄

#endif
