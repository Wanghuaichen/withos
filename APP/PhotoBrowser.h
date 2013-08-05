#ifndef _PhotoBrowser_h
#define _PhotoBrowser_h
#include "includes.h"
#include "My_UI.h"
#include "GUI.H"
#include "bmp.h"
#include "Fat_API.h"
extern u8 FileList[][30];//最多支持100个文件
extern char FilePath[];
extern u16 filecount;
extern u8 TouchFlag;

extern MenuInfo    OtherMenu_Info;
extern u8 Menu_ID;  //菜单界面的ID
extern SingleList  OtherMenu_pHead;    /* 其他界面的控件链表表头指针      */
extern MenuInfo    OtherMenu_Info;
extern u8 TouchCmd;            //标志着是点触还是滑动操作。
extern GUI_COLOR  ThemeColor;
extern GUI_COLOR  DesktopColor;
extern u8 MenuDisnable;
extern WM_HWIN hDesktopWindow; //桌面的句柄
#endif
