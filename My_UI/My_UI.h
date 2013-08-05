#ifndef _My_UI_h
#define _My_UI_h
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "GUI.H"
#include "BUTTON.h"
#include "progbar.h"
#include "framewin.h"
#include "TEXT.h"
#include "WM.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "LISTVIEW.h"

#define _folder   1
#define _mp3      2
#define _txt      3
#define _bmp      4

#define TouchUnPress    0   //首次检测到没有按下
#define TouchUnPressed  1   //没有按下
#define TouchPress      2   //首次检测到按下
#define TouchPressed    3   //按下
/*   菜单界面区分的ID号   */
#define MainMenu_ID         1 
#define MusicPlayeMenu_ID   3 
#define FileBrowseMenu_ID   5
#define SetMenu_ID          10
#define TextMenu_ID         15

#define  Button_ID0   0x17a
#define  Button_ID1   0x17b
#define  Button_ID2   0x17c
#define  Button_ID3   0x17d
#define  Button_ID4   0x17e
#define  Button_ID5   0x17f
#define  Button_ID6   160
#define  Button_ID7   170
#define  Button_ID8   180
#define  Button_ID9   190
#define  Button_ID10  200
#define  Button_ID11  210
#define  Button_ID12  220
#define  Button_ID13  230
// #define _up   1
// #define _down 2

typedef struct SingleListNode {
	WM_HMEM                  handle;
	struct SingleListNode   *next;

}SingleListNode;                                                      /* 声明SingleListNode为新的类型名*/
typedef SingleListNode    *SingleList;                                /* 定义指针类型                  */

typedef struct {
	WM_HMEM                  FirstHandle;
	WM_HMEM                  EndHandle;
}MenuInfo;                 

SingleList SingleListNodeCreate(void);
SingleList SingleListInsert(SingleList pList,SingleList pos,WM_HMEM x);
void SingleListFree(SingleList pHead);
void ClearStruct(unsigned char *pta, int size ) ;

u8 CheckFile(char *buf);

void MainMenu_Init(void); /* 主界面初始化*/
void Go_MainMenu(void);   /* 回到主界面*/
void Out_MainMenu(void);  /*退出主界面，注意主界面只是不在屏幕上显示而已，但仍然存在，在屏幕显示区域之外*/
void Out_OtherMenu(void);
void WM_MoveCtrl(void);   /* 菜单界面的滑动控制*/
void WM_GoTop(void);      /* 回界面顶部*/
void WM_Move(SingleList pHead ,int dx,int dy); /* 实现链表内的所有控件滑动*/
void SetThemeColor(GUI_COLOR color);
#endif
