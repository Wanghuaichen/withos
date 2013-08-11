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

EDIT_Handle hEdtDebug,hEdtMsgC;

static WM_HWIN ghTarget;
static char* gCurrentKeyMap;
static U8 gShift;
static WM_HWIN gKeyBaord;
static GUI_HOOK gHook;

static char keys[]=
{
    '1','2','3','4','5','6','7','8','9','0','-','=',
    'q','w','e','r','t','y','u','i','o','p','[',']',
    'a','s','d','f','g','h','j','k','l',';','\'','\\',
    'z','x','c','v','b','n','m',',','.','/','`'
};
static char keys2[]=
{
    '!','@','#','$','%','^','&','*','(',')','_','+',
    'Q','W','E','R','T','Y','U','I','O','P','{','}',
    'A','S','D','F','G','H','J','K','L',':','\"','|',
    'Z','X','C','V','B','N','M','<','>','?','~'
};

/*******************************************************************
*
* callbacks
*/

 void _cbDesktop(WM_MESSAGE * pMsg)
/*背景窗口的回调函数，用于响应创建键盘按钮*/
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(0x000000);
        GUI_Clear();
        break;
    case WM_NOTIFY_PARENT:
    {
        I32 NCode=pMsg->Data.v;
        I32 Id;
        if ((pMsg->hWinSrc)&&(NCode!=WM_NOTIFICATION_LOST_FOCUS )&&(NCode!=WM_NOTIFICATION_CHILD_DELETED))
        {
            Id=WM_GetId(pMsg->hWinSrc);
            //ghTarget=pMsg->hWinSrc;

            if (Id==GUI_ID_BUTTON0)
            {
                CreateKeyBaord();
            }
        }
    }
    break;
    case WM_KEY:
        //i=((WM_KEY_INFO*)(pMsg->Data.p))->Key;
        break;
    default:
        WM_DefaultProc(pMsg);

    }
}
 int _hookFW(WM_MESSAGE * pMsg)
/*目标窗口的挂钩函数*/
{
    if (pMsg->MsgId==WM_TOUCH_CHILD)//pMsg->MsgId==WM_NOTIFY_PARENT|| 
    {
        GUI_PID_STATE * pState;
      pState = ( GUI_PID_STATE * )( ( WM_MESSAGE * )pMsg->Data.p )->Data.p;
        if (( pState!=0 )&&(pState->Pressed))
        {
            I32 NCode=pMsg->Data.v;
            if ((pMsg->hWinSrc)&&(NCode!=WM_NOTIFICATION_LOST_FOCUS )&&(NCode!=WM_NOTIFICATION_CHILD_DELETED))
            {
                ghTarget=pMsg->hWinSrc;
            }
        }
    }
    return 0;
}
 void _setHook(WM_HWIN hWin)
/*将处理按键的挂钩函数关联到指定窗口*/
{
	

    FRAMEWIN_Obj* pObj;
	GUI_Lock();//asion 20091016 add
	pObj= FRAMEWIN_H2P(hWin);//asion 20091016 edit
    GUI_HOOK_Add(&pObj->pFirstHook,&gHook,&_hookFW);
	GUI_Unlock();//asion 20091016 add
}
 void _cbKeyBaord(WM_MESSAGE * pMsg)
/*键盘的回调函数*/
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(0xc3c3c3);
        GUI_Clear();
        GUI_SetBkColor(0xFFFFFF);
        break;
    case WM_NOTIFY_PARENT:
    {  I32 Id;
		if (pMsg->Data.v!=WM_NOTIFICATION_RELEASED)//asion 20091016 add
		{break;
		}
        //Id=GUI_GetKey();
		Id=WM_GetId(pMsg->hWinSrc);//asion 20091016 edit add

        if (Id!=NULL)//&&(pMsg->hWin!=pMsg->hWinSrc))
        {
            switch (Id)
            {
            case 20:
            {

                KEYBAORDCFG kbcfg={0};
                clearKeyBaord(WM_GetClientWindow( pMsg->hWin));
                gCurrentKeyMap=(gCurrentKeyMap==keys)?keys2:keys;
                kbcfg.hWin= pMsg->hWin;
                kbcfg.keymap=gCurrentKeyMap;
                kbcfg.x0=kbcfg.y0=5;
                kbcfg.xSize=15;
                kbcfg.ySize=18;
                kbcfg.xSpan=2;
                kbcfg.ySpan=3;
                kbcfg.num=sizeof(keys)/sizeof(char);
                createKeys(&kbcfg);
            }
            break;
            case 25:
                gShift=~gShift;
                break;
            default:
                if (WM_IsWindow(ghTarget)&&(iscntrl(Id)||isprint(Id))&&(Id<0x7f))
                {
                    WM_KEY_INFO key={0};
                    WM_MESSAGE msg={0};
                    key.Key=Id;
                    key.PressedCnt=1;
                    msg.MsgId=WM_KEY;
                    msg.Data.p=&key;
                    WM_SendMessage(ghTarget,&msg);
                    //GUI_SendKeyMsg (Id, 1);
                    //WM_BroadcastMessage(&gMsg);
                    WM_SetFocus(ghTarget);
                }
            }

        }
    }

    break;
    default:
        WM_DefaultProc(pMsg);

    }
}

void createKeys(KEYBAORDCFG* kbcfg)
/*创建按键*/
{
    int x0,y0,xSize,ySize,xSpan,ySpan;
    int i,starX,startY,num;
    WM_HWIN hWin;
    BUTTON_Handle hbtn;
    char tmp[20],*keyMap;

    x0=kbcfg->x0;
    y0=kbcfg->y0;
    xSize=kbcfg->xSize;
    ySize=kbcfg->ySize;

    xSpan=kbcfg->xSpan;
    ySpan=kbcfg->ySpan;
    num=kbcfg->num;

    hWin=kbcfg->hWin;
    keyMap=kbcfg->keymap;

    starX=x0;
    startY=y0;
    for (i=0; i<num; i++)
    {

        hbtn=BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,keyMap[i],BUTTON_CF_SHOW);
        //strncpy(tmp,keys+i,1);

        memset(tmp,0,20);
        tmp[0]=keyMap[i];
        BUTTON_SetText(hbtn,tmp);

        x0=x0+xSize+xSpan;
        if ((i+1)%12==0)
        {
            y0=y0+ySize+ySpan;
            x0=starX;
        }
    }
    //hbtn=BUTTON_CreateAsChild(x0,y0,xSize*2+xSpan,ySize,hWin,25,BUTTON_CF_SHOW);
    //BUTTON_SetText(hbtn,"Shift");

    hbtn=BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,GUI_KEY_UP,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"up");

    hbtn=BUTTON_CreateAsChild(x0=starX,y0=y0+ySpan+ySize,(xSize*3+2*xSpan),ySize,hWin,20,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"Shift");
    hbtn=BUTTON_CreateAsChild(x0=x0+3*xSpan+3*xSize,y0,(xSize*6+5*xSpan),ySize,hWin,' ',BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"");

    hbtn=BUTTON_CreateAsChild(x0=x0+7*xSpan+7*xSize,y0,xSize,ySize,hWin,GUI_KEY_LEFT,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"lf");
    hbtn=BUTTON_CreateAsChild(x0=x0+xSpan+xSize,y0,xSize,ySize,hWin,GUI_KEY_DOWN,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"dn");
    hbtn=BUTTON_CreateAsChild(x0=x0+xSpan+xSize,y0,xSize,ySize,hWin,GUI_KEY_RIGHT,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"ri");

    hbtn=BUTTON_CreateAsChild(x0=starX+12*xSize+12*xSpan,y0=startY,xSize,ySize,hWin,8,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"bs");
    hbtn=BUTTON_CreateAsChild(x0,y0+ySpan+ySize,xSize,2*ySize+ySpan,hWin,13,BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"E\nn");

}

void clearKeyBaord(WM_HWIN hWin)
{

    WM_HWIN hCWin;

    for (hCWin=WM_GetFirstChild(hWin); hCWin!=0; hCWin=WM_GetFirstChild(hWin))
    {
        //hCWin=WM_GetNextSibling(hCWin)
        WM_DeleteWindow(hCWin);
    }
}

WM_HWIN dialogTest()
/*测试的编辑框*/
{

    BUTTON_Handle hbtn;
    FRAMEWIN_Handle hFW;

    hFW=FRAMEWIN_Create ("configer", NULL,WM_CF_SHOW, 3,20, 200, 120);
    FRAMEWIN_SetMoveable (hFW,FRAMEWIN_SF_MOVEABLE );
	//FRAMEWIN_AddCloseButton(hFW,FRAMEWIN_BUTTON_RIGHT, 1);


    //hEdtDebug=EDIT_CreateAsChild(200,200,100,20,WM_HBKWIN,GUI_ID_EDIT0,EDIT_CF_LEFT,10);
    hEdtMsgC=EDIT_CreateAsChild(10,10,160,20,WM_GetClientWindow(hFW),GUI_ID_EDIT1,EDIT_CF_LEFT,9);
    EDIT_CreateAsChild(10,50,160,20,WM_GetClientWindow(hFW),GUI_ID_EDIT2,EDIT_CF_LEFT,25);
    //EDIT_SetDecMode(hEdtDebug,0,0,999999,0,0);
    //EDIT_SetDecMode(hEdtMsgC,0,-0xFFFFFFFF,0xFFFFFFFF,0,0);
	EDIT_SetHexMode(hEdtMsgC,0,-0xFFFFFFFF,0x6FFFFFFF);

    hbtn= BUTTON_CreateAsChild (0, 0, 100, 20, WM_HBKWIN, GUI_ID_BUTTON0, BUTTON_CF_SHOW);
    BUTTON_SetText(hbtn,"CreateKeyBaord");
    return hFW;
}

WM_HWIN CreateKeyBaord()
{
    WM_HWIN hFW,hWin;
    KEYBAORDCFG kbcfg;

    hFW=FRAMEWIN_Create("KeyBaord", NULL, WM_CF_SHOW,3, 142,240,130);
    FRAMEWIN_SetMoveable(hFW,FRAMEWIN_SF_MOVEABLE);
    FRAMEWIN_AddCloseButton(hFW,FRAMEWIN_BUTTON_RIGHT,1);
    hWin=WM_GetClientWindow(hFW);
    gKeyBaord=hFW;

    WM_SetStayOnTop(hFW,0);

    gCurrentKeyMap=keys;
    kbcfg.hWin=hWin;
    kbcfg.keymap=gCurrentKeyMap;
    kbcfg.x0=kbcfg.y0=5;
    kbcfg.xSize=15;
    kbcfg.ySize=18;
    kbcfg.xSpan=2;
    kbcfg.ySpan=3;
    kbcfg.num=sizeof(keys)/sizeof(char);
    createKeys(&kbcfg);

    WM_SetCallback(hWin,_cbKeyBaord);
		return hFW;
}
#include "delay.h"
void MainTask3(void)
{
    WM_HWIN hFW;
    GUI_Init();
    //GUI_CURSOR_Show();

    hFW=dialogTest();
    _setHook(hFW);
    CreateKeyBaord();

    WM_SetCallback(WM_HBKWIN,_cbDesktop);
    WM_SetDesktopColor(0x000000);
    //GUI_SetColor(0xFFFFFF);

    while (1){
        delay_ms(10);//GUI_Delay(1000);
			GUI_Exec();
			GUI_TOUCH_Exec();
		}
}