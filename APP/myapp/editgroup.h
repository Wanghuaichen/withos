#ifndef _editgroup
#define _editgroup
#include "wm.h"
#define EDIT_Group1_ID(i) (GUI_ID_USER + 100 + i)
#define EDIT_Group2_ID(i) (GUI_ID_USER + 1000 + i)
#define EDIT_Group3_ID(i) (GUI_ID_USER + 1100 + i)

void drawEditGroup(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, WM_HWIN hfather);
#endif