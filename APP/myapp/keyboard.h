#ifndef _keyboard
#define _keyboard
#include "wm.h"
#include "edit.h"

typedef struct
{/*创建键盘的参数结构*/
    WM_HWIN hWin;
    char* keymap;
    int x0,y0,xSize,ySize,xSpan,ySpan;
    int num;
} KEYBAORDCFG;


int _hookFW(WM_MESSAGE * pMsg);
void _setHook(WM_HWIN hWin);
void _cbKeyBaord(WM_MESSAGE * pMsg);
void clearKeyBaord(WM_HWIN hWin);
WM_HWIN CreateKeyBaord(unsigned, unsigned);
extern unsigned char showModeCounter;


WM_HWIN CreateNumKeyBaord(unsigned x0, unsigned y0);
WM_HWIN CreateAlphaKeyBaord(unsigned x0, unsigned y0);
#endif