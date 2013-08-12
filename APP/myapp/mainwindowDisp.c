#include "gui.h"
#include "wm.h"
extern WM_HWIN hmainDlg;
extern WM_HWIN hkeyboard;
void dispInMainWindow(void)
{
		WM_HWIN htext;
		htext = WM_GetDialogItem(hmainDlg, TEXT_ID_mainPanelTime);
}