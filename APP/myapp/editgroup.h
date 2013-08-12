#ifndef _editgroup
#define _editgroup
#include "wm.h"
#define EDIT_Group1_ID(i) (GUI_ID_USER + 100 + i)
#define EDIT_Group2_ID(i) (GUI_ID_USER + 1000 + i)
#define EDIT_Group3_ID(i) (GUI_ID_USER + 1100 + i)


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
#define editMaxLen 10
#define textHeight 12

#define innerGapX(x1, x2) ((x2 - x1 - titleWidth - leftMargin - rightMargin) / ((editGapRatioX+1)*numOfEditsInARow-1))
#define editWidth(x1, x2) (innerGapX(x1, x2) * editGapRatioX)

#define editY(i, k, y1) (y1 + headerHeight + (textHeight + 2*editHeight+2*innerGapY+interGapY)*(i-1) + (editHeight + innerGapY)*(k-1) + (k==1?0:1)*(textHeight-editHeight))
#define editX(j, x1, x2) (x1 + titleWidth + leftMargin + (editWidth(x1,x2)+innerGapX(x1, x2)) * (j-1))

void drawEditGroup(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, WM_HWIN hfather);
#endif