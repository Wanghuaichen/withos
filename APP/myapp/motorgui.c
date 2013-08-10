#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "progbar.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
WM_HWIN hConfigDlg;
WM_HWIN hmainDlg;
static int _MultiSel;
static int _OwnerDrawn;
static int _VarY = 1;
static int _PrevTime;

#define lcdWidth 800
#define lcdHeight 480

#define gapYProgbarUp 30
#define gapYLabelProgbar 50
#define gapYModeButtonLabel 50
#define gapYConfigButtonModeButton 30
#define gapYConfigButtonDown 30

//#define gapXSwitchButtonModeButton 100


#define labelHeight 60
#define labelWidth 800

#define progBarHeight 60
#define progBarWidth 800

#define numOfModeButton 3
#define modeButtonWidth 100
#define modeButtonHeight 60

#define numOfSwitchButton 2
#define switchButtonHeight 60
#define switchButtonWidth 100

#define configButtonWidth 100
#define configButtonHeight 60

#define progBarX 0
#define progBarY gapYProgbarUp

#define labelX 0
#define labelY (progBarY + progBarHeight)

#define buttonBaseY (labelY + labelHeight + gapYModeButtonLabel)
#define buttonYSpace (lcdHeight - buttonBaseY - gapYConfigButtonModeButton - configButtonHeight - gapYConfigButtonDown)

#define switchButtonX (lcdWidth / 3 - switchButtonWidth / 2)
#define switchButtonGapY (buttonYSpace - numOfSwitchButton * switchButtonHeight)/(1+numOfSwitchButton)
#define switchButtonY(i) (buttonBaseY + switchButtonGapY + (i-1)*(switchButtonHeight + switchButtonGapY))

#define modeButtonX (lcdWidth / 3 * 2 - modeButtonWidth / 2)
#define modeButtonGapY (buttonYSpace - numOfModeButton * modeButtonHeight)/(1+numOfModeButton)
#define modeButtonY(i) (buttonBaseY + modeButtonGapY + (i-1)*(modeButtonHeight + modeButtonGapY))

#define configButtonY 
/*#define modeButtonGapX ((lcdWidth - numOfMode * modeButtonWidth) / (1 + numOfMode))
//#define modeButtonX(i) (modeButtonGapX + (modeButtonGapX + modeButtonWidth) * (i - 1))
#define modeButtonY (gapYLabelUp + labelHeight + gapYLabelModeButton)
#define labelX(i) (modeButtonX(i) - (labelWidth / 2))*/


#define GUI_ID_BUTTON_(x) (GUI_ID_USER + x)
#define GUI_ID_TEXT_(x) (GUI_ID_USER + 10 + x)
#define GUI_ID_FRAMEWIN_(x) (GUI_ID_USER + 20 + x)


/*********************************************************************
*
*       Bitmap data for user drawn list box
*/
const GUI_COLOR ColorsSmilie0[] = {
     0xFFFFFF,0x000000,0x0000FF
};

const GUI_COLOR ColorsSmilie1[] = {
     0xFFFFFF,0x000000,0x00FFFF
};

const GUI_LOGPALETTE PalSmilie0 = {
  3,	/* number of entries */
  1, 	/* Has transparency */
  &ColorsSmilie0[0]
};

const GUI_LOGPALETTE PalSmilie1 = {
  3,	/* number of entries */
  1, 	/* Has transparency */
  &ColorsSmilie1[0]
};

const unsigned char acSmilie0[] = {
  0x00, 0x55, 0x40, 0x00,
  0x01, 0xAA, 0x90, 0x00,
  0x06, 0xAA, 0xA4, 0x00,
  0x19, 0x6A, 0x59, 0x00,
  0x69, 0x6A, 0x5A, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xAA, 0xAA, 0x40,
  0x1A, 0x95, 0xA9, 0x00,
  0x06, 0x6A, 0x64, 0x00,
  0x01, 0xAA, 0x90, 0x00,
  0x00, 0x55, 0x40, 0x00
};

const unsigned char acSmilie1[] = {
  0x00, 0x55, 0x40, 0x00,
  0x01, 0xAA, 0x90, 0x00,
  0x06, 0xAA, 0xA4, 0x00,
  0x19, 0x6A, 0x59, 0x00,
  0x69, 0x6A, 0x5A, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xA6, 0xAA, 0x40,
  0x6A, 0xAA, 0xAA, 0x40,
  0x1A, 0x6A, 0x69, 0x00,
  0x06, 0x95, 0xA4, 0x00,
  0x01, 0xAA, 0x90, 0x00,
  0x00, 0x55, 0x40, 0x00
};

const GUI_BITMAP bmSmilie0 = {
 13, /* XSize */
 13, /* YSize */
 4,  /* BytesPerLine */
 2,  /* BitsPerPixel */
 acSmilie0,   /* Pointer to picture data (indices) */
 &PalSmilie0  /* Pointer to palette */
};

const GUI_BITMAP bmSmilie1 = {
 13, /* XSize */
 13, /* YSize */
 4,  /* BytesPerLine */
 2,  /* BitsPerPixel */
 acSmilie1,   /* Pointer to picture data (indices) */
 &PalSmilie1  /* Pointer to palette */
};

/*********************************************************************
*
*       Default contents of list box
*/
static const GUI_ConstString _ListBox[] = {
  "English", "Deutsch", NULL
};

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created manually, but could also be created by a GUI-builder.
*///

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,  "Main Panel",    GUI_ID_FRAMEWIN_(1),                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
	{ PROGBAR_CreateIndirect,   "Progress",          0,       progBarX,  progBarY,  progBarWidth,  progBarHeight, WM_CF_SHOW },
	{ TEXT_CreateIndirect,      "Time: ",    GUI_ID_TEXT_(1),       labelX,  labelY,  labelWidth,  labelHeight, TEXT_CF_LEFT },
	{	TEXT_CreateIndirect,      "Speed: ",    GUI_ID_TEXT_(2),       labelX,  labelY+labelHeight,  labelWidth,  labelHeight, TEXT_CF_LEFT },
/* Buttons */
  { BUTTON_CreateIndirect,    "Default MODE 1",      GUI_ID_BUTTON_(1),     modeButtonX,  modeButtonY(1),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "Default MODE 2",      GUI_ID_BUTTON_(2),     modeButtonX,  modeButtonY(2),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "Default MODE 3",      GUI_ID_BUTTON_(3),     modeButtonX,  modeButtonY(3),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "More Modes",      GUI_ID_BUTTON_(4),     0,  lcdHeight - 1 - configButtonHeight - gapYConfigButtonDown,  configButtonWidth,  configButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Go!",      GUI_ID_BUTTON_(5),     switchButtonX,  switchButtonY(1),  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Stop!",      GUI_ID_BUTTON_(6),     switchButtonX,  switchButtonY(2),  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},

};

static const GUI_WIDGET_CREATE_INFO _configDialogCreate[] = {
	{FRAMEWIN_CreateIndirect,  "Config Panel",    GUI_ID_FRAMEWIN_(2),                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
  { LISTBOX_CreateIndirect,   0,                         GUI_ID_MULTIEDIT0,  10,  10, 100, 100, 0, 100 },
/* Check box for multi select mode */
  { CHECKBOX_CreateIndirect,  0,                         GUI_ID_CHECK0,     120,  10,   0,   0 },
  { TEXT_CreateIndirect,      "Multi select",            GUI_ID_TEXT_(4),                 140,  10,  80,  15, TEXT_CF_LEFT },
/* Check box for owner drawn list box */
  { CHECKBOX_CreateIndirect,  0,                         GUI_ID_CHECK1,     120,  35,   0,   0 },
  { TEXT_CreateIndirect,      "Owner drawn",              GUI_ID_TEXT_(2),                140,  35,  80,  15, TEXT_CF_LEFT },
  { BUTTON_CreateIndirect,    "Input New Mode",      GUI_ID_BUTTON_(7),     modeButtonX,  modeButtonY(1),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "OK",      GUI_ID_BUTTON_(8),     modeButtonX,  modeButtonY(2),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "CANCEL",      GUI_ID_BUTTON_(9),     modeButtonX,  modeButtonY(3),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
};
/*********************************************************************
*
*       _cbBkWindow
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font24_ASCII);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _GetItemSizeX
*/
static int _GetItemSizeX(WM_HWIN hWin, int ItemIndex) {
  char acBuffer[100];
  int  DistX;
  LISTBOX_GetItemText(hWin, ItemIndex, acBuffer, sizeof(acBuffer));
  DistX = GUI_GetStringDistX(acBuffer);
  return DistX + bmSmilie0.XSize + 16;
}

/*********************************************************************
*
*       _GetItemSizeY
*/
static int _GetItemSizeY(WM_HWIN hWin, int ItemIndex) {
  int DistY;
  DistY = GUI_GetFontDistY() + 1;
  if (LISTBOX_GetMulti(hWin)) {
    if (LISTBOX_GetItemSel(hWin, ItemIndex)) {
      DistY += 8;
    }
  } else if (LISTBOX_GetSel(hWin) == ItemIndex) {
    DistY += 8;
  }
  return DistY;
}

/*********************************************************************
*
*       _OwnerDraw
*
* Purpose:
*   This is the owner draw function.
*   It allows complete customization of how the items in the listbox are
*   drawn. A command specifies what the function should do;
*   The minimum is to react to the draw command (WIDGET_ITEM_DRAW);
*   If the item x-size differs from the default, then this information
*   needs to be returned in reaction to WIDGET_ITEM_GET_XSIZE.
*   To insure compatibility with future version, all unhandled commands
*   must call the default routine LISTBOX_OwnerDraw.
*/
static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  WM_HWIN hWin;
  int Index;
  hWin     = pDrawItemInfo->hWin;
  Index    = pDrawItemInfo->ItemIndex;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_GET_XSIZE:
    return _GetItemSizeX(hWin, Index);
  case WIDGET_ITEM_GET_YSIZE:
    return _GetItemSizeY(hWin, Index);
  case WIDGET_ITEM_DRAW:
    {
      int MultiSel, Sel, YSize, FontDistY;
      int IsDisabled, IsSelected;
      int ColorIndex = 0;
      char acBuffer[100];
      const GUI_BITMAP * pBm;
      const GUI_FONT* pOldFont = 0;
      GUI_COLOR aColor[4] = {GUI_BLACK, GUI_WHITE, GUI_WHITE, GUI_GRAY};
      GUI_COLOR aBkColor[4] = {GUI_WHITE, GUI_GRAY, GUI_BLUE, 0xC0C0C0};
      IsDisabled = LISTBOX_GetItemDisabled(pDrawItemInfo->hWin, pDrawItemInfo->ItemIndex);
      IsSelected = LISTBOX_GetItemSel(hWin, Index);
      MultiSel   = LISTBOX_GetMulti(hWin);
      Sel        = LISTBOX_GetSel(hWin);
      YSize      = _GetItemSizeY(hWin, Index);
      /* Calculate color index */
      if (MultiSel) {
        if (IsDisabled) {
          ColorIndex = 3;
        } else {
          ColorIndex = (IsSelected) ? 2 : 0;
        }
      } else {
        if (IsDisabled) {
          ColorIndex = 3;
        } else {
          if (pDrawItemInfo->ItemIndex == Sel) {
            ColorIndex = WM_HasFocus(pDrawItemInfo->hWin) ? 2 : 1;
          } else {
            ColorIndex = 0;
          }
        }
      }
      /* Draw item */
      GUI_SetBkColor(aBkColor[ColorIndex]);
      GUI_SetColor  (aColor[ColorIndex]);
      LISTBOX_GetItemText(pDrawItemInfo->hWin, pDrawItemInfo->ItemIndex, acBuffer, sizeof(acBuffer));
      GUI_Clear();
      if ((ColorIndex == 1) || (ColorIndex == 2)) {
        pOldFont = GUI_SetFont(&GUI_Font13B_1);
      }
      FontDistY  = GUI_GetFontDistY();
      GUI_DispStringAt(acBuffer, pDrawItemInfo->x0 + bmSmilie0.XSize + 16, pDrawItemInfo->y0 + (YSize - FontDistY) / 2);
      if (pOldFont) {
        GUI_SetFont(pOldFont);
      }
      GUI_DispCEOL();
      /* Draw bitmap */
      pBm = MultiSel ? IsSelected ? &bmSmilie1 : &bmSmilie0 : (pDrawItemInfo->ItemIndex == Sel) ? &bmSmilie1 : &bmSmilie0;
      GUI_DrawBitmap(pBm, pDrawItemInfo->x0 + 7, pDrawItemInfo->y0 + (YSize - pBm->YSize) / 2);
      /* Draw focus rectangle */
      if (MultiSel && (pDrawItemInfo->ItemIndex == Sel)) {
        GUI_RECT rFocus;
        GUI_RECT rInside;
        WM_GetInsideRectEx(pDrawItemInfo->hWin, &rInside);
        rFocus.x0 = pDrawItemInfo->x0;
        rFocus.y0 = pDrawItemInfo->y0;
        rFocus.x1 = rInside.x1;
        rFocus.y1 = pDrawItemInfo->y0 + YSize - 1;
        GUI_SetColor(GUI_WHITE - aBkColor[ColorIndex]);
        GUI_DrawFocusRect(&rFocus, 0);
      }
    }
    break;
  default:
    return LISTBOX_OwnerDraw(pDrawItemInfo);
  }
  return 0;
}

/*********************************************************************
*
*       _cbCallback
*/

static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id;
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;

  switch (pMsg->MsgId) {		
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */

		//GUI_MessageBox("This text is shown\nin a message box", "Caption/Title", GUI_MESSAGEBOX_CF_MOVEABLE);
      switch (NCode) {
        //case WM_NOTIFICATION_SEL_CHANGED:
         // LISTBOX_InvalidateItem(hListBox, LISTBOX_ALL_ITEMS);
         // break;
        case WM_NOTIFICATION_RELEASED:      /* React only if released */
          switch (Id) {
            case GUI_ID_BUTTON_(4):
              WM_BringToTop(hConfigDlg);
							//WM_SetFocus(hConfigDlg);
							WM_InvalidateWindow(WM_HBKWIN);
							break;
						
            case GUI_ID_BUTTON_(1):
              //GUI_EndDialog(hDlg, 1);
              break;
            case GUI_ID_BUTTON_(2):
              break;	
            case GUI_ID_BUTTON_(3):
              break;	
          }
          break;
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}
static void _cbCallbackConfigPanel(WM_MESSAGE * pMsg) {
  int NCode, Id;
  WM_HWIN hDlg, hListBox, hItem;
  hDlg = pMsg->hWin;
  hListBox = WM_GetDialogItem(hDlg, GUI_ID_MULTIEDIT0);

  switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
      LISTBOX_SetText(hListBox, _ListBox);
      LISTBOX_AddString(hListBox, "Français");
      LISTBOX_AddString(hListBox, "Japanese");
      LISTBOX_AddString(hListBox, "Italiano");
      LISTBOX_AddString(hListBox, "Español");
      LISTBOX_AddString(hListBox, "Greek");
      LISTBOX_AddString(hListBox, "Hebrew");
      LISTBOX_AddString(hListBox, "Dutch");
      LISTBOX_AddString(hListBox, "Other language ...");
      LISTBOX_SetScrollStepH(hListBox, 6);
      LISTBOX_SetAutoScrollH(hListBox, 1);
      LISTBOX_SetAutoScrollV(hListBox, 1);
      LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
      hItem  = WM_GetDialogItem(hDlg, GUI_ID_CHECK1);
      CHECKBOX_Check(hItem);
			//WM_InvalidateWindow(WM_HBKWIN);
      break;
/*    case WM_KEY:
      switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
        case GUI_KEY_ESCAPE:
          GUI_EndDialog(hDlg, 1);
          break;
        case GUI_KEY_ENTER:
          GUI_EndDialog(hDlg, 0);
          break;
      }
      break;*/
    case WM_TOUCH_CHILD:
      WM_SetFocus(hListBox);
      break;		
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */

      switch (NCode) {
        case WM_NOTIFICATION_RELEASED:      /* React only if released */
          switch (Id) {

            case GUI_ID_BUTTON_(7):
							//do sth and go back
              break;
            case GUI_ID_BUTTON_(8):
							//do sth and go back
              WM_BringToTop(hmainDlg);
							//WM_SetFocus(hmainDlg);
							WM_Invalidate(WM_HBKWIN);
              break;		
						case GUI_ID_BUTTON_(9):
							//do sth and go back
              WM_BringToTop(hmainDlg);
							//WM_SetFocus(hmainDlg);
							WM_Invalidate(WM_HBKWIN);
              break;
            case GUI_ID_CHECK0:
              _MultiSel ^= 1;
              LISTBOX_SetMulti(hListBox, _MultiSel);
              WM_SetFocus(hListBox);
              LISTBOX_InvalidateItem(hListBox, LISTBOX_ALL_ITEMS);
              break;
            case GUI_ID_CHECK1:
              _OwnerDrawn ^= 1;
              if (_OwnerDrawn) {
                LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
              } else {
                LISTBOX_SetOwnerDraw(hListBox, NULL);
              }
              LISTBOX_InvalidateItem(hListBox, LISTBOX_ALL_ITEMS);
              break;
          }
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
#include "led.h"
#include "delay.h"
void motorMain(void) {
	unsigned char ledcnt = 0;
	LED_Init();
  GUI_Init();
//  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */	
	
	hmainDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);
	WM_BringToTop(hmainDlg);
	//WM_SetFocus(hmainDlg);
	WM_InvalidateWindow(WM_HBKWIN);
	
	
	
	LED0 = 0;
  while (1) {
    _MultiSel   = 0;
    _OwnerDrawn = 1;
    GUI_TOUCH_Exec();
    GUI_Exec();//GUI_Delay(1000);
		delay_ms(15);
		if(++ledcnt == 60){
				ledcnt = 0;
				LED0 = ~LED0;
		}
  }
}

/*************************** End of file ****************************/

