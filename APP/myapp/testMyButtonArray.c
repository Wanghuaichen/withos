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


#define WindowWidth 400
#define WindowHeight 400
#define TitleHeight 10
#define TotalNum 24
#define KeyGapRatioX 200
#define KeyGapRatioY 160
#define KeyMarginRatioH 90
#define KeyMarginRatioV 90
#define ButtonXYRatio (1)

//XYRatio = XNum/YNum * (2/KeyMarginRatioH + ) / () * ButtonXYRatio
#define ClientHeight (WindowHeight - TitleHeight)
#define ClientWidth WindowWidth
#define XYRatio (ClientWidth/ClientHeight)

/*#define YNum ((2*ButtonXYRatio/KeyMarginRatioH - 2*XYRatio/KeyMarginRatioV + XYRatio/KeyGapRatioY - ButtonXYRatio/KeyGapRatioX+(ButtonXYRatio+ButtonXYRatio/KeyGapRatioX)*TotalNum) / (ButtonXYRatio+ButtonXYRatio/KeyGapRatioX+XYRatio+XYRatio/KeyGapRatioY))
#define XNum (TotalNum / YNum)
#define ButtonWidth ((ClientWidth) / (2/KeyMarginRatioH+XNum+(XNum-1)/KeyGapRatioX))
#define ButtonHeight (ButtonWidth / ButtonXYRatio)

#define MarginH (ButtonWidth / KeyMarginRatioH)
#define MarginV (ButtonHeight / KeyMarginRatioV)

#define GapX (ButtonWidth / KeyGapRatioX)
#define GapY (ButtonHeight / KeyGapRatioY)*/

#if (XYRatio*1*1) > (TotalNum)
#ifndef YNum
#define YNum 1
#endif
#endif
#if (XYRatio *2*2) > (TotalNum)
#ifndef YNum
#define YNum 2
#endif
#endif
#if (XYRatio *3*3) > (TotalNum)
#ifndef YNum
#define YNum 3
#endif
#endif
#if (XYRatio *4*4) > (TotalNum)
#ifndef YNum
#define YNum 4
#endif
#endif
#if (XYRatio *5*5) > (TotalNum)
#ifndef YNum
#define YNum 5
#endif
#endif
#if (XYRatio *6*6) > (TotalNum)
#ifndef YNum
#define YNum 6
#endif
#endif
#if (XYRatio *7*7) > (TotalNum)
#ifndef YNum
#define YNum 7
#endif
#endif
#if (XYRatio *8*8) > (TotalNum)
#ifndef YNum
#define YNum 8
#endif
#endif
#if (XYRatio *9*9) > (TotalNum)
#ifndef YNum
#define YNum 9
#endif
#endif
#ifndef YNum
#define YNum 10
#endif

#define XNum (YNum*XYRatio/ButtonXYRatio)

#define ButtonWidth (ClientWidth / ((2/KeyMarginRatioH) + (XNum) + ((XNum-1)/KeyGapRatioX)))
#define ButtonHeight (ClientHeight / ((2/KeyMarginRatioV) + (YNum) + ((YNum-1)/KeyGapRatioY)))

#define MarginH (ButtonWidth / KeyMarginRatioH)
#define MarginV (ButtonHeight / KeyMarginRatioV)

#define GapX (ButtonWidth / KeyGapRatioX)
#define GapY (ButtonHeight / KeyGapRatioY)

#define FunctionKeyWidth (ButtonWidth*2+GapX)
#define ErrorX 20
#define ErrorY 20
#include "led.h"
#include "delay.h"
//#define _dbging
void testSize(void)
{
    WM_HWIN hFW,hWin,hbtn;
		unsigned char i, ii, jj, ledcnt = 0;
		unsigned x0, y0;
	
#define stringX 100
#define stringY 40
#define stringHeight 16

	GUI_Init();
	
		LED_Init();
#ifndef _dbging	
		FRAMEWIN_SetDefaultTitleHeight(TitleHeight);
    hFW=FRAMEWIN_Create("KeyBaord", 0, WM_CF_SHOW,0, 0,WindowWidth+ErrorX,WindowHeight+ErrorY);
		hWin=WM_GetClientWindow(hFW);
		for(i = 0; i < TotalNum; ++i){
				ii = (i/XNum)+1;
				jj = (i%(unsigned char)XNum) + 1;
				y0 = 	MarginV + (ii-1)*(ButtonHeight+GapY);
				x0 = 	MarginH + (jj-1)*(ButtonWidth+GapX);
				BUTTON_CreateAsChild(x0,y0,ButtonWidth,ButtonHeight,hWin,1,BUTTON_CF_SHOW);
		}
		#endif
		LED0 = 0;
		while(1){
				delay_ms(15);
				if(++ledcnt == 60){
						ledcnt = 0;
						LED0 = ~LED0;
				}			
				GUI_Exec();
				GUI_TOUCH_Exec();	

#ifdef _dbging				
GUI_DispStringAt("TotalNum        ", stringX, stringY);
GUI_DispDecMin(TotalNum);

GUI_DispStringAt("KeyGapRatioX        ", stringX, stringY+1*stringHeight);
GUI_DispDecMin(KeyGapRatioX);

GUI_DispStringAt("KeyGapRatioY        ", stringX, stringY+2*stringHeight);
GUI_DispDecMin(KeyGapRatioY);

GUI_DispStringAt("KeyMarginRatioH        ", stringX, stringY+2*stringHeight);
GUI_DispDecMin(KeyMarginRatioH);

GUI_DispStringAt("KeyMarginRatioV        ", stringX, stringY+3*stringHeight);
GUI_DispDecMin(KeyMarginRatioV);

GUI_DispStringAt("WindowHeight - TitleHeight        ", stringX, stringY+4*stringHeight);
GUI_DispDecMin(WindowHeight - TitleHeight);

GUI_DispStringAt("YNum        ", stringX, stringY+5*stringHeight);
GUI_DispDecMin(YNum);

GUI_DispStringAt("TotalNum//XYRatio//ButtonXYRatio        ", stringX, stringY+6*stringHeight);
GUI_DispDecMin(TotalNum/XYRatio/ButtonXYRatio);

GUI_DispStringAt("KeyButtonWidth (ClientWidth // ((2/KeyMarginRatioH) + (XNum) + ((XNum-1)//KeyGapRatioX)))        ", stringX, stringY+7*stringHeight);
GUI_DispDecMin(ButtonWidth);

GUI_DispStringAt("(ClientHeight / ((2/KeyMarginRatioV) + (YNum) + ((YNum-1)/KeyGapRatioY)))        ", stringX, stringY+8*stringHeight);
GUI_DispDecMin(ButtonHeight);

GUI_DispStringAt("KeyMarginRatioH        ", stringX, stringY+9*stringHeight);
GUI_DispDecMin(KeyMarginRatioH);	
#endif
		}
}