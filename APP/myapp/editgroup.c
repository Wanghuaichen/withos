#include "framewin.h"
#include "text.h"
#include "edit.h"
#include "editgroup.h"
#include "stringutils.h"
#define numOfSteps 10

unsigned int speed[numOfSteps];
unsigned int duration[numOfSteps];

TEXT_Handle stepHandles[numOfSteps];
EDIT_Handle speedHandles[numOfSteps];
EDIT_Handle durationHandles[numOfSteps];

#define numOfEditsInARow 5

#define titleWidth 35
#define headerHeight 30
#define footerHeight 30
#define leftMargin 10
#define rightMargin 30


#define editGapRatioX (4)

#define innerGapY 5
#define interGapY 20
#define editHeight 50
#define editMaxLen 5
#define textHeight 12

#define innerGapX(x1, x2) ((x2 - x1 - titleWidth - leftMargin - rightMargin) / ((editGapRatioX+1)*numOfEditsInARow-1))
#define editWidth(x1, x2) (innerGapX(x1, x2) * editGapRatioX)

#define editY(i, k, y1) (y1 + headerHeight + (textHeight + 2*editHeight+2*innerGapY+interGapY)*(i-1) + (editHeight + innerGapY)*(k-1) + (k==1?0:1)*(textHeight-editHeight))
#define editX(j, x1, x2) (x1 + titleWidth + leftMargin + (editWidth(x1,x2)+innerGapX(x1, x2)) * (j-1))


void drawEditGroup(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, WM_HWIN hfather)
{
		int ii, jj, nn;
		char buf[5];
		for(nn = 0; nn < numOfSteps; ++nn){
				ii = nn / numOfEditsInARow + 1;
				jj = nn % numOfEditsInARow + 1;
				int2str(buf, nn+1);
				stepHandles[nn] = TEXT_CreateAsChild(editX(jj, x1, x2), editY(ii, 1, y1),  editWidth(x1, x2), textHeight, hfather, EDIT_Group1_ID(nn), WM_CF_SHOW, buf, TEXT_CF_HCENTER);
				speedHandles[nn] = EDIT_CreateAsChild(editX(jj, x1, x2), editY(ii, 2, y1),  editWidth(x1, x2), editHeight, hfather, EDIT_Group2_ID(nn), WM_CF_SHOW, editMaxLen);
				durationHandles[nn] = EDIT_CreateAsChild(editX(jj, x1, x2), editY(ii, 3, y1),  editWidth(x1, x2), editHeight, hfather, EDIT_Group3_ID(nn), WM_CF_SHOW, editMaxLen);
				if(!(nn % numOfEditsInARow)){
						TEXT_CreateAsChild(x1, editY(ii, 1, y1), titleWidth - 1, 12, hfather, GUI_ID_USER + 200, WM_CF_SHOW, "Steps:", TEXT_CF_HCENTER);
						TEXT_CreateAsChild(x1, editY(ii, 2, y1) + editHeight / 2 - 6, titleWidth - 1, 12, hfather, GUI_ID_USER + 201, WM_CF_SHOW, "Speed:", TEXT_CF_HCENTER);
						TEXT_CreateAsChild(x1, editY(ii, 3, y1) + editHeight / 2 - 6, titleWidth +8, 12, hfather, GUI_ID_USER + 202, WM_CF_SHOW, "Duration:", TEXT_CF_HCENTER);
				}
		}

}

void setEditGroupVis(unsigned char isVisibale)
{
	;
}
/*
#include "delay.h"
#include "led.h"
void motorMain2(void) {
	unsigned char ledcnt = 0;
	FRAMEWIN_Handle hframe;
	LED_Init();
  GUI_Init();
//  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);
	
	hframe = FRAMEWIN_Create("ertt", 0, WM_CF_SHOW, 0, 0, 750, 450);
	drawEditGroup(30, 30, 400, 300, hframe);
	WM_InvalidateWindow(WM_HBKWIN);	
	
	LED0 = 0;
  while (1) {

    GUI_TOUCH_Exec();
    GUI_Exec();//GUI_Delay(1000);
		delay_ms(15);
		if(++ledcnt == 60){
				ledcnt = 0;
				LED0 = ~LED0;
		}
  }
}*/