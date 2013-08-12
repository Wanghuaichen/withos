#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "progbar.h"
#include "keyboard.h"
#include "editgroup.h"
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
WM_HWIN hConfigDlg;
unsigned char hConfigDlgFlag = 0;
unsigned char hmainDlgFlag = 1;
unsigned char hkeyboardFlag = 0;
WM_HWIN hmainDlg;
WM_HWIN hkeyboard;


static int _MultiSel;
//static int _OwnerDrawn;
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


#define BUTTON_Id_MoreModes (GUI_ID_USER+1)
#define BUTTON_Id_Go (GUI_ID_USER+2)
#define BUTTON_Id_Stop (GUI_ID_USER+3)
#define BUTTON_Id_DefaultMode1 (GUI_ID_USER+4)
#define BUTTON_Id_DefaultMode2 (GUI_ID_USER+5)
#define BUTTON_Id_DefaultMode3 (GUI_ID_USER+6)

#define BUTTON_Id_DeleteMode (GUI_ID_USER+7)
#define BUTTON_Id_Ok (GUI_ID_USER+8)
#define BUTTON_Id_Cancel (GUI_ID_USER+9)
#define BUTTON_Id_EditMode (GUI_ID_USER+10)
#define BUTTON_Id_SubmitEdit (GUI_ID_USER+11)


#define FRAME_ID_mainPanel (GUI_ID_USER+12)
#define FRAME_ID_confPanel (GUI_ID_USER+13)

#define TEXT_ID_mainPanelTime (GUI_ID_USER+14)
#define TEXT_ID_mainPanelSpeed (GUI_ID_USER+15)
#define LISTBOX_Id (GUI_ID_USER+16)
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
/*
const GUI_LOGPALETTE PalSmilie0 = {
  3,	// number of entries 
  1, 	// Has transparency 
  &ColorsSmilie0[0]
};

const GUI_LOGPALETTE PalSmilie1 = {
  3,	// number of entries 
  1, 	// Has transparency 
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
};*/
/*
const GUI_BITMAP bmSmilie0 = {
 13, // XSize 
 13, // YSize 
 4,  // BytesPerLine 
 2,  // BitsPerPixel 
 acSmilie0,   // Pointer to picture data (indices) 
 &PalSmilie0  // Pointer to palette 
};

const GUI_BITMAP bmSmilie1 = {
 13, // XSize 
 13, // YSize 
 4,  // BytesPerLine 
 2,  // BitsPerPixel 
 acSmilie1,   // Pointer to picture data (indices) 
 &PalSmilie1  // Pointer to palette 
};
*/
/*********************************************************************
*
*       Default contents of list box
*/
static const GUI_ConstString _ListBox[] = {
  "Default Mode 1", "Default Mode 2", "Default Mode 3", 0
};

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created manually, but could also be created by a GUI-builder.
*///

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,  "Main Panel",    FRAME_ID_mainPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
	{ PROGBAR_CreateIndirect,   "Progress",          0,       progBarX,  progBarY,  progBarWidth,  progBarHeight, WM_CF_SHOW },
	{ TEXT_CreateIndirect,      "Time: ",    TEXT_ID_mainPanelTime,       labelX,  labelY,  labelWidth,  labelHeight, TEXT_CF_LEFT },
	{	TEXT_CreateIndirect,      "Speed: ",    TEXT_ID_mainPanelSpeed,       labelX,  labelY+labelHeight,  labelWidth,  labelHeight, TEXT_CF_LEFT },
/* Buttons */
  { BUTTON_CreateIndirect,    "Default MODE 1",      BUTTON_Id_DefaultMode1,     modeButtonX,  modeButtonY(1),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "Default MODE 2",      BUTTON_Id_DefaultMode2,     modeButtonX,  modeButtonY(2),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "Default MODE 3",      BUTTON_Id_DefaultMode3,     modeButtonX,  modeButtonY(3),  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "More Modes",      BUTTON_Id_MoreModes,     0,  lcdHeight - 1 - configButtonHeight - gapYConfigButtonDown,  configButtonWidth,  configButtonHeight, WM_CF_SHOW},
	{BUTTON_CreateIndirect,    "Go!",      BUTTON_Id_Go,     switchButtonX,  switchButtonY(1),  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},
	//{BUTTON_CreateIndirect,    "Stop!",      BUTTON_Id_Stop,     switchButtonX,  switchButtonY(2),  switchButtonWidth,  switchButtonHeight, WM_CF_SHOW},

};

static const GUI_WIDGET_CREATE_INFO _configDialogCreate[] = {
	{FRAMEWIN_CreateIndirect,  "Config Panel",    FRAME_ID_confPanel,                  0,  0, lcdWidth - 1, lcdHeight - 1, 0 },
  { LISTBOX_CreateIndirect,   0,                LISTBOX_Id,  10,  30, 200, 300, 0, 100 },
  //{ EDIT_CreateIndirect, 			0, 9082, 600, 410, 100, 50, WM_CF_SHOW},
	//{ TEXT_CreateIndirect,      "output",              GUI_ID_TEXT_(5),                350,  150,  80,  30, TEXT_CF_LEFT },
  //{ BUTTON_CreateIndirect,    "Delete Mode",      BUTTON_Id_DeleteMode,     10,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
  { BUTTON_CreateIndirect,    "OK",      BUTTON_Id_Ok,     10+modeButtonWidth+5,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	{ BUTTON_CreateIndirect,    "CANCEL",      BUTTON_Id_Cancel,     10+2*modeButtonWidth+5*2,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "EDIT MODE",      BUTTON_Id_EditMode,     60+60+3*modeButtonWidth+5*3,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},
	//{ BUTTON_CreateIndirect,    "SUBMIT",      BUTTON_Id_SubmitEdit,     40+120+4*modeButtonWidth+5*4,  350,  modeButtonWidth,  modeButtonHeight, WM_CF_SHOW},

};

/*********************************************************************
*
*       _GetItemSizeX
*/
/*static int _GetItemSizeX(WM_HWIN hWin, int ItemIndex) {
  char acBuffer[100];
  int  DistX;
  LISTBOX_GetItemText(hWin, ItemIndex, acBuffer, sizeof(acBuffer));
  DistX = GUI_GetStringDistX(acBuffer);
  return DistX + bmSmilie0.XSize + 16;
}*/

/*********************************************************************
*
*       _GetItemSizeY
*/
/*static int _GetItemSizeY(WM_HWIN hWin, int ItemIndex) {
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
}*/

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
/*static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
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
      // Calculate color index 
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
      // Draw item 
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
      // Draw bitmap 
      pBm = MultiSel ? IsSelected ? &bmSmilie1 : &bmSmilie0 : (pDrawItemInfo->ItemIndex == Sel) ? &bmSmilie1 : &bmSmilie0;
      GUI_DrawBitmap(pBm, pDrawItemInfo->x0 + 7, pDrawItemInfo->y0 + (YSize - pBm->YSize) / 2);
      // Draw focus rectangle 
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
}*/

/*********************************************************************
*
*       _cbCallback
*/
static void _cbCallbackConfigPanel(WM_MESSAGE * pMsg);
static void _cbCallback(WM_MESSAGE * pMsg) {
  int NCode, Id;
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;

  switch (pMsg->MsgId) {		
		
		case WM_INIT_DIALOG:
			break;
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
            case BUTTON_Id_MoreModes:
							//WM_SetFocus(hConfigDlg);
							hConfigDlgFlag = 1;//WM_SetFocus(hmainDlg);
							hmainDlgFlag = 0;
							//WM_DisableWindow(hmainDlg);
							break;
						
            case BUTTON_Id_DefaultMode1:
              //GUI_EndDialog(hDlg, 1);
              break;
            case BUTTON_Id_DefaultMode2:
              break;	
            case BUTTON_Id_DefaultMode3:
              break;	
            case BUTTON_Id_Go:
              break;			
            case BUTTON_Id_Stop:
              break;							
          }
          break;
      }
      break;
			
    default:
      WM_DefaultProc(pMsg);
  }
}

#include "stringutils.h"
#define _keyboardXLeft 50
#define _keyboardXRight 460
#define _keyboardYTop 50
#define _keyboardYBot 50
unsigned char moveFlag = 0;
unsigned keyboardX = _keyboardXLeft, keyboardY = _keyboardYTop;
static void _cbCallbackConfigPanel(WM_MESSAGE * pMsg) {
	//static char buf[20];
  int NCode, Id, itemtot, curitem;
  WM_HWIN hDlg, hListBox, hItem;
	static unsigned char touchchildSwitch = 0;
  hDlg = pMsg->hWin;
  hListBox = WM_GetDialogItem(hDlg, LISTBOX_Id);

  switch (pMsg->MsgId) {
		
    case WM_INIT_DIALOG:
			
      LISTBOX_SetText(hListBox, _ListBox);
      LISTBOX_AddString(hListBox, "Add New Mode ..");
      LISTBOX_SetScrollStepH(hListBox, 6);
      LISTBOX_SetAutoScrollH(hListBox, 1);
      LISTBOX_SetAutoScrollV(hListBox, 1);
      //LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
			LISTBOX_SetFont(hListBox, &GUI_Font32_ASCII);
			LISTBOX_SetScrollbarWidth(hListBox, 20);

      //CHECKBOX_Check(hItem);
			WM_InvalidateWindow(WM_HBKWIN);
			
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
				//break;
				Id    = WM_GetId(pMsg->hWinSrc);
				if((Id == BUTTON_Id_DeleteMode) || (BUTTON_Id_Ok == Id) || (BUTTON_Id_Cancel == Id) || (BUTTON_Id_EditMode == Id)
							|| (Id == BUTTON_Id_SubmitEdit || (Id == LISTBOX_Id))){
						break;
				}

						hkeyboardFlag = 1;
						if(Id == EDIT_Group2_ID(0) || Id == EDIT_Group2_ID(1) || Id == EDIT_Group3_ID(0) || Id == EDIT_Group3_ID(1)
								|| Id == EDIT_Group2_ID(5) || Id == EDIT_Group2_ID(6) || Id == EDIT_Group3_ID(5) || Id == EDIT_Group3_ID(6))
						{
										if(!WM_IsWindow(hkeyboard)){							
												keyboardX = _keyboardXRight;
												keyboardY = _keyboardYTop;												
										}
										else{
												hkeyboardFlag = 0;//WM_BringToTop(hkeyboard);
												moveFlag = 2;											
												//WM_InvalidateWindow(WM_HBKWIN);											
										}
						}
						else{							
										if(!WM_IsWindow(hkeyboard)){	
													
													keyboardX = _keyboardXLeft;
													keyboardY = _keyboardYTop;												
										}
										else{
												hkeyboardFlag = 0;//WM_BringToTop(hkeyboard);
												moveFlag = 1;//WM_MoveWindow(hkeyboard, _keyboardXLeft-keyboardX, _keyboardYTop-keyboardY);												
												//WM_InvalidateWindow(WM_HBKWIN);											
										}						
						}					
						
      break;		
		
		
    case WM_NOTIFY_PARENT:

      Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
      NCode = pMsg->Data.v;                 /* Notification code */
      switch (NCode) {
				case WM_NOTIFICATION_CLICKED:
						break;
				/*case WM_NOTIFICATION_MOVED_OUT:
								WM_BringToTop(hkeyboard);
								WM_Invalidate(WM_HBKWIN);
						break;
				case WM_NOTIFICATION_VALUE_CHANGED:
								WM_BringToTop(hkeyboard);
								WM_Invalidate(WM_HBKWIN);
						break;*/
				case WM_NOTIFICATION_SEL_CHANGED:
						break;				
        case WM_NOTIFICATION_RELEASED:      /* React only if released */

          switch (Id) {
						case LISTBOX_Id:
								itemtot = LISTBOX_GetNumItems(hListBox);
								curitem = LISTBOX_GetSel(hListBox);

								
								/*int2str(buf, itemtot);
								TEXT_SetText(WM_GetDialogItem(hDlg, EDIT_Group1_ID(1)), buf);
								int2str(buf, curitem);
								TEXT_SetText(WM_GetDialogItem(hDlg, EDIT_Group1_ID(2)), buf);
								WM_Invalidate(WM_HBKWIN);	*/
								if(curitem == itemtot - 1){
									
										hkeyboardFlag = 1;
										if(!WM_IsWindow(hkeyboard)){
												keyboardX = _keyboardXRight;
												keyboardY = _keyboardYTop;											
												//hkeyboard = CreateKeyBaord();												
										}
										else{
												hkeyboardFlag = 0;
												moveFlag = 2;//WM_MoveWindow(hkeyboard, _keyboardXRight-keyboardX, _keyboardYTop-keyboardY);
												//WM_BringToTop(hkeyboard);
												//WM_InvalidateWindow(WM_HBKWIN);
										}
										
									//�������̣��༭�µ�����
										//focus����һ��edit��������
										//WM_BringToTop(hkeyboard);
										//WM_Invalidate(WM_HBKWIN);	
								}
								else{
									;//��ʾ��ǰ����;
								}
								//TEXT_SetText(WM_GetDialogItem(hDlg, TEXT_ID_mainPanelTime), buf);
								break;

            case BUTTON_Id_DeleteMode:

							//do sth and go back
              break;
            case BUTTON_Id_Ok:
							//do sth and go back
							//GUI_EndDialog(hConfigDlg, 0);//WM_DeleteWindow(hConfigDlg);
							if(WM_IsWindow(hkeyboard)){									
									//WM_DisableWindow(hkeyboard);								
									//WM_DeleteWindow(hkeyboard);
									//clearKeyBaord(hkeyboard);
									;//hkeyboard = 0;
							}
							//hConfigDlgFlag = 0;//WM_SetFocus(hmainDlg);
							hkeyboardFlag = 0;
							hmainDlgFlag = 1;
							//WM_Invalidate(WM_HBKWIN);
							//WM_DeleteWindow(hmainDlg);
              break;		
						case BUTTON_Id_Cancel:
							//do sth and go back
							if(WM_IsWindow(hkeyboard)){									
									//WM_DisableWindow(hkeyboard);								
									//WM_DeleteWindow(hkeyboard);
									//clearKeyBaord(hkeyboard);
									;//hkeyboard = 0;
							}			
							hkeyboardFlag = 0;
							hmainDlgFlag = 1;							
							//WM_Invalidate(WM_HBKWIN);
              break;
            case BUTTON_Id_EditMode:
								hkeyboardFlag = 0;
              break;		
            case BUTTON_Id_SubmitEdit:
							
              break;						
 /*           case GUI_ID_CHECK0:
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
              break;*/
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
void createFrame(void)
{
		hkeyboard = FRAMEWIN_Create("", 0, WM_CF_SHOW, 50, 50, 350, 350);
}

#include "led.h"
#include "delay.h"
#include "editgroup.h"
void motorMain(void) {
	unsigned char ledcnt = 0;
	LED_Init();
  GUI_Init();
  //WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */	
	//hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);	
	//hmainDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	
	LED0 = 0;
  while (1) {
    if(!hmainDlgFlag && WM_IsWindow(hmainDlg)){
				GUI_EndDialog(hmainDlg, 0);
				WM_DeleteWindow(hmainDlg);
				hmainDlg = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}
    if(!hConfigDlgFlag && WM_IsWindow(hConfigDlg)){
				GUI_EndDialog(hConfigDlg, 0);
				WM_DeleteWindow(hConfigDlg);
				hConfigDlg = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}	
    if(!hkeyboardFlag && WM_IsWindow(hkeyboard)){
				clearKeyBaord(hkeyboard);
				WM_DeleteWindow(hkeyboard);
				hkeyboard = 0;
				WM_InvalidateWindow(WM_HBKWIN);	
		}			
    GUI_Exec();//GUI_Delay(1000);
		GUI_TOUCH_Exec();		
    if(hmainDlgFlag && !WM_IsWindow(hmainDlg)){
				hmainDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
				FRAMEWIN_SetTitleVis(hmainDlg, 0);
				WM_BringToTop(hmainDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
		}
    if(hConfigDlgFlag && !WM_IsWindow(hConfigDlg)){
				hConfigDlg = GUI_CreateDialogBox(_configDialogCreate, GUI_COUNTOF(_configDialogCreate), &_cbCallbackConfigPanel, 0, 0, 0);				
				drawEditGroup(250, 10, 799, 479, WM_GetClientWindow(hConfigDlg));
				_setHook(hConfigDlg);//WM_GetClientWindow(hConfigDlg));//���׹���˭����
				WM_BringToTop(hConfigDlg);
				WM_InvalidateWindow(WM_HBKWIN);	
		}		
    if(hkeyboardFlag && !WM_IsWindow(hkeyboard)){
				hkeyboard = CreateKeyBaord(keyboardX, keyboardY);
				WM_BringToTop(hkeyboard);
				//WM_SetStayOnTop(hkeyboard, 1);
				//WM_InvalidateWindow(WM_HBKWIN);	
		}	

				if(moveFlag == 1){
						keyboardX = _keyboardXLeft;
						clearKeyBaord(hkeyboard);
						WM_DeleteWindow(hkeyboard);
						hkeyboard = 0;
						WM_InvalidateWindow(WM_HBKWIN);	
						hkeyboardFlag = 1;
						moveFlag = 0;
						GUI_Exec();
				}
				else if(moveFlag == 2){			
						keyboardX = _keyboardXRight;
						clearKeyBaord(hkeyboard);
						WM_DeleteWindow(hkeyboard);
						hkeyboard = 0;
						WM_InvalidateWindow(WM_HBKWIN);	
						hkeyboardFlag = 1;		
						moveFlag = 0;
						GUI_Exec();
				}
				

		delay_ms(15);
		if(++ledcnt == 60){
				ledcnt = 0;
				LED0 = ~LED0;
		}
  }
}

/*************************** End of file ****************************/

