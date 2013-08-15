#include "math.h"
#include "lcd.h"
#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "DIALOG.h"
#include "LISTBOX.h"
#include "EDIT.h"
#include "SLIDER.h"
#include "FRAMEWIN.h"
#include "RADIO.h"
#include "SCROLLBAR.h"
#include "TEXT.h"
#include "MULTIEDIT.h"
#include "LISTVIEW.h"
#include "MULTIPAGE.h"
#include "GUI_HOOK.h"
#include "FRAMEWIN_Private.h"

#include <string.h>
#include <ctype.h>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "DIALOG.h"
#include "LISTBOX.h"
#include "EDIT.h"
#include "SLIDER.h"
#include "FRAMEWIN.h"
#include "RADIO.h"
#include "SCROLLBAR.h"
#include "TEXT.h"
#include "MULTIEDIT.h"
#include "LISTVIEW.h"
#include "MULTIPAGE.h"
#include "GUI_HOOK.h"
#include "FRAMEWIN_Private.h"

#include <string.h>
#include <ctype.h>

#define NULL 0
#include "keyboard.h"


/*******************************************************************
*
* function declare
*/

void clearKeyBaord(WM_HWIN hWin);
void createKeys(KEYBAORDCFG* kbcfg);

/*******************************************************************
*
* 键盘的按键配置和键盘布局全局变量
*/

extern EDIT_Handle hEdtDebug,hEdtMsgC;

static WM_HWIN ghTarget;
static char* gCurrentKeyMap;
static U8 gShift;
static WM_HWIN gKeyBaord;
static GUI_HOOK gHook;
#define keyMapShift 3000
#define scaleFactorX 2
#define scaleFactorY 2
static char keys[]=
{
    '1','2','3','4','5','6','7','8','9','0',
    '+','.','/','-','*'
};

unsigned WindowWidth = 700;
unsigned WindowHeight = 400;
unsigned TitleHeight = 10;
unsigned TotalNum = 32;
#define KeyGapRatioX 200
#define KeyGapRatioY 160
#define KeyMarginRatioH 10
#define KeyMarginRatioV 10
#define ButtonXYRatio (1.0)
#define ClientHeight (WindowHeight - TitleHeight)
#define ClientWidth WindowWidth
#define XYRatio (ClientWidth/ClientHeight)
unsigned YNum;
unsigned XNum;// = (YNum*XYRatio/ButtonXYRatio);
unsigned ButtonWidth;// (ClientWidth / ((2/KeyMarginRatioH) + (XNum) + ((XNum-1)/KeyGapRatioX)))
unsigned ButtonHeight;// (ClientHeight / ((2/KeyMarginRatioV) + (YNum) + ((YNum-1)/KeyGapRatioY)))
unsigned MarginH;// (ButtonWidth / KeyMarginRatioH)
unsigned MarginV;// (ButtonHeight / KeyMarginRatioV)
unsigned GapX;// (ButtonWidth / KeyGapRatioX)
unsigned GapY;// (ButtonHeight / KeyGapRatioY)
unsigned FunctionKeyWidth;// (ButtonWidth*2+GapX)
#define ErrorX 0
#define ErrorY 0
void configButtonArray(void)
{
		double tmpYNum, a, b, c, delta;
		double theRatio;
		theRatio = XYRatio;
		a = 1.0*ClientWidth / ClientHeight + 1.0*ClientWidth / ClientHeight/KeyGapRatioY;
		b = 1.0*2*ClientWidth / ClientHeight/KeyMarginRatioV - 1.0*ClientWidth / ClientHeight/KeyGapRatioY - 1.0*2*ButtonXYRatio/KeyMarginRatioH + 1.0*ButtonXYRatio/KeyGapRatioX;
		c = -1.0*ButtonXYRatio*TotalNum*(1+1.0/KeyGapRatioX);
		delta = sqrt(b*b - 4*a*c);
		tmpYNum = (-b + delta) / 2 / a;
		YNum = (unsigned)tmpYNum;
		XNum = (unsigned)(TotalNum / YNum)+1;
		tmpYNum = ((double)ClientWidth / ((2/(double)KeyMarginRatioH) + ((double)XNum) + (((double)XNum-1)/(double)KeyGapRatioX)));
		ButtonWidth = (unsigned)tmpYNum;
		tmpYNum = ((double)ClientHeight / ((2/(double)KeyMarginRatioV) + ((double)YNum) + (((double)YNum-1)/(double)KeyGapRatioY)));
		ButtonHeight = (unsigned)tmpYNum;
		
		MarginH = (unsigned)ButtonWidth / KeyMarginRatioH;
		MarginV = (unsigned)(ButtonHeight / KeyMarginRatioV);
		GapX = (unsigned)(ButtonWidth / KeyGapRatioX);
		GapY = (unsigned)(ButtonHeight / KeyGapRatioY);
}


void createNumKeys(KEYBAORDCFG* kbcfg)
//创建按键
{
    int x0,y0,xSize,ySize,xSpan,ySpan;
    int i,starX,startY,num, ii, jj;
    WM_HWIN hWin;
    BUTTON_Handle hbtn;
    char tmp[20],*keyMap;

		configButtonArray();
    num=kbcfg->num;
    hWin=kbcfg->hWin;
    keyMap=kbcfg->keymap;
		for(i = 0; i < num; ++i){
				ii = (i/XNum)+1;
				jj = (i%(unsigned char)XNum) + 1;
				y0 = 	MarginV + (ii-1)*(ButtonHeight+GapY);
				x0 = 	MarginH + (jj-1)*(ButtonWidth+GapX);
				hbtn = BUTTON_CreateAsChild(x0,y0,ButtonWidth,ButtonHeight,hWin,keyMap[i]+keyMapShift,BUTTON_CF_SHOW);
			  tmp[0]=keyMap[i];
				tmp[1] = 0;
        BUTTON_SetText(hbtn,tmp);
		}		
		
		y0 += (ButtonHeight+GapY);
		x0 = MarginH;
    hbtn=BUTTON_CreateAsChild(x0, y0,((XNum)*ButtonWidth+(XNum-1)*GapX)/2, ButtonHeight,hWin,20+keyMapShift,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"English");
		x0 += ((XNum)*ButtonWidth+(XNum-1)*GapX)/2 +GapX;
    hbtn=BUTTON_CreateAsChild(x0,y0,((XNum)*ButtonWidth+(XNum-1)*GapX)-((XNum)*ButtonWidth+(XNum-1)*GapX)/2-GapX/*ClientWidth - 2*MarginH -GapX + ((XNum)*ButtonWidth+(XNum-1)*GapX)/2-((XNum)*ButtonWidth+(XNum-1)*GapX)*/, ButtonHeight,hWin,8+keyMapShift,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"backspace");
		y0 += (ButtonHeight+GapY);
		x0 = MarginH;
    hbtn=BUTTON_CreateAsChild(x0,y0,(XNum)*ButtonWidth+(XNum-1)*GapX,ButtonHeight,hWin,13+keyMapShift,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"Enter");

}

WM_HWIN CreateNumKeyBaord(unsigned x0, unsigned y0)
{
    WM_HWIN hFW,hWin;
    KEYBAORDCFG kbcfg;
		WindowWidth = 300;
		WindowHeight = 400;
		TitleHeight = 10;	
		TotalNum = 21;
    hFW=FRAMEWIN_Create("KeyBaord", &_cbKeyBaord, WM_CF_SHOW,x0, y0,WindowWidth,WindowHeight);
    FRAMEWIN_SetMoveable(hFW,FRAMEWIN_SF_MOVEABLE);
    //FRAMEWIN_AddCloseButton(hFW,FRAMEWIN_BUTTON_RIGHT,1);
    hWin=WM_GetClientWindow(hFW);
    gKeyBaord=hFW;
		FRAMEWIN_SetTitleHeight(hFW, TitleHeight);

    gCurrentKeyMap=keys;
    kbcfg.hWin=hWin;
    kbcfg.keymap=gCurrentKeyMap;
    kbcfg.x0=8;
		kbcfg.y0=10;
    kbcfg.xSize=15*scaleFactorX;
    kbcfg.ySize=18*scaleFactorY;
    kbcfg.xSpan=2*scaleFactorX;
    kbcfg.ySpan=3*scaleFactorY;
    kbcfg.num=sizeof(keys)/sizeof(char);
    createNumKeys(&kbcfg);

    //WM_SetCallback(hWin,_cbKeyBaord);
		return hFW;
}
void createAlphaKeys(KEYBAORDCFG* kbcfg)
//创建按键
{
    int x0,y0,xSize,ySize,xSpan,ySpan;
    int i,starX,startY,num, ii, jj;
    WM_HWIN hWin;
    BUTTON_Handle hbtn;
    char tmp[20],*keyMap;

		configButtonArray();
    num=kbcfg->num;
    hWin=kbcfg->hWin;
    keyMap=kbcfg->keymap;
		for(i = 0; i < num; ++i){
				ii = (i/XNum)+1;
				jj = (i%(unsigned char)XNum) + 1;
				y0 = 	MarginV + (ii-1)*(ButtonHeight+GapY);
				x0 = 	MarginH + (jj-1)*(ButtonWidth+GapX);
				hbtn = BUTTON_CreateAsChild(x0,y0,ButtonWidth,ButtonHeight,hWin,'a'+i+keyMapShift,BUTTON_CF_SHOW);
			  tmp[0]='a'+i;
				tmp[1] = 0;
        BUTTON_SetText(hbtn,tmp);
		}		
		
		y0 += (ButtonHeight+GapY);
		x0 = MarginH;
    hbtn=BUTTON_CreateAsChild(x0, y0,((XNum)*ButtonWidth+(XNum-1)*GapX)/2, ButtonHeight,hWin,20+keyMapShift,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"Num");
		x0 += ((XNum)*ButtonWidth+(XNum-1)*GapX)/2 +GapX;
    hbtn=BUTTON_CreateAsChild(x0,y0,((XNum)*ButtonWidth+(XNum-1)*GapX)-((XNum)*ButtonWidth+(XNum-1)*GapX)/2-GapX/*ClientWidth - 2*MarginH -GapX + ((XNum)*ButtonWidth+(XNum-1)*GapX)/2-((XNum)*ButtonWidth+(XNum-1)*GapX)*/, ButtonHeight,hWin,8+keyMapShift,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"backspace");
		y0 += (ButtonHeight+GapY);
		x0 = MarginH;
    hbtn=BUTTON_CreateAsChild(x0,y0,(XNum)*ButtonWidth+(XNum-1)*GapX,ButtonHeight,hWin,13+keyMapShift,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"Enter");

}
WM_HWIN CreateAlphaKeyBaord(unsigned x0, unsigned y0)
{
    WM_HWIN hFW,hWin;
    KEYBAORDCFG kbcfg;
		WindowWidth = 300;
		WindowHeight = 400;
		TitleHeight = 10;	
		TotalNum = 38;
    hFW=FRAMEWIN_Create("KeyBaord", &_cbKeyBaord, WM_CF_SHOW,x0, y0,WindowWidth,WindowHeight);
    FRAMEWIN_SetMoveable(hFW,FRAMEWIN_SF_MOVEABLE);
    //FRAMEWIN_AddCloseButton(hFW,FRAMEWIN_BUTTON_RIGHT,1);
    hWin=WM_GetClientWindow(hFW);
    gKeyBaord=hFW;
		FRAMEWIN_SetTitleHeight(hFW, TitleHeight);

    gCurrentKeyMap=keys;
    kbcfg.hWin=hWin;
    kbcfg.keymap=gCurrentKeyMap;
    kbcfg.x0=8;
		kbcfg.y0=10;
    kbcfg.xSize=15*scaleFactorX;
    kbcfg.ySize=18*scaleFactorY;
    kbcfg.xSpan=2*scaleFactorX;
    kbcfg.ySpan=3*scaleFactorY;
    kbcfg.num=26;
    createAlphaKeys(&kbcfg);

    //WM_SetCallback(hWin,_cbKeyBaord);
		return hFW;
}

extern unsigned YNum;
extern unsigned XNum;// = (YNum*XYRatio/ButtonXYRatio);
extern unsigned ButtonWidth;// (ClientWidth / ((2/KeyMarginRatioH) + (XNum) + ((XNum-1)/KeyGapRatioX)))
extern unsigned ButtonHeight;// (ClientHeight / ((2/KeyMarginRatioV) + (YNum) + ((YNum-1)/KeyGapRatioY)))
extern unsigned MarginH;// (ButtonWidth / KeyMarginRatioH)
extern unsigned MarginV;// (ButtonHeight / KeyMarginRatioV)
extern unsigned GapX;// (ButtonWidth / KeyGapRatioX)
extern unsigned GapY;// (ButtonHeight / KeyGapRatioY)
extern unsigned FunctionKeyWidth;// (ButtonWidth*2+GapX)


#include "led.h"
#include "delay.h"
//#define _dbging
WM_HWIN CreateNumKeyBaord(unsigned x0, unsigned y0);
void testSize(void)
{
    WM_HWIN hFW,hWin,hbtn;
		unsigned char i, ii, jj, ledcnt = 0;
		unsigned x0, y0;
	
		GUI_Init();
	
		LED_Init();
		CreateAlphaKeyBaord(0 , 0);
/*		FRAMEWIN_SetDefaultTitleHeight(TitleHeight);
    hFW=FRAMEWIN_Create("KeyBaord", 0, WM_CF_SHOW,0, 0,WindowWidth+ErrorX,WindowHeight+ErrorY);
		hWin=WM_GetClientWindow(hFW);
		for(i = 0; i < TotalNum; ++i){
				ii = (i/XNum)+1;
				jj = (i%(unsigned char)XNum) + 1;
				y0 = 	MarginV + (ii-1)*(ButtonHeight+GapY);
				x0 = 	MarginH + (jj-1)*(ButtonWidth+GapX);
				BUTTON_CreateAsChild(x0,y0,ButtonWidth,ButtonHeight,hWin,1,BUTTON_CF_SHOW);
		}*/

		LED0 = 0;
		while(1){
				delay_ms(15);
				if(++ledcnt == 60){
						ledcnt = 0;
						LED0 = ~LED0;
				}			
				GUI_Exec();
				GUI_TOUCH_Exec();	
		}
}