///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch/mgncs4touch.h"

static BOOL onkeydown(mWidget *widget, int message, int code, DWORD keyStatus) {
    mHScrollViewPiece *view = (mHScrollViewPiece *)((mContainerCtrl *)widget)->body;
    RECT rc;
    int offset_x = 0;
    int offset_y = 0;

    _c(view)->getViewport(view, &rc);
    switch (code) {
        case SCANCODE_J:
            offset_y = 10;
            break;
        case SCANCODE_K:
            offset_y = -10;
            break;
        case SCANCODE_L:
            offset_x = 10;
            break;
        case SCANCODE_H:
            offset_x = -10;
            break;
        case SCANCODE_S:
            {
                static BOOL show = TRUE;
                _c(view)->showScrollBar(view, show);
                show = !show;
            }
            break;
        default:
            return FALSE;
    }
    _c(view)->moveViewport(view, rc.left + offset_x, rc.top + offset_y);
    return TRUE;
}

static NCS_EVENT_HANDLER g_handles[] = {
    { MSG_KEYDOWN, onkeydown },
    { 0, NULL }
};

static int s_onMouseEvent(mHotPiece *_self, int message, WPARAM wParam, LPARAM lParam, mObject *owner){
    printf("%s(message=%d)\n", __FUNCTION__, message);
    return -1;
}

static BOOL mymain_onCreate(mMainWnd* self, DWORD dwAddData )
{
    mContainerCtrl *ctnr;
    mPanelPiece *content;
    mHScrollViewPiece *view;
    mHotPiece* backPiece;
    int i;
    RECT rc;
    static BITMAP bmp[3];
    static char* label[3];

    LoadBitmap(HDC_SCREEN, &bmp[0], "res/news.png");
    label [0] = "news";
    LoadBitmap(HDC_SCREEN, &bmp[1], "res/file.png");
    label [1] = "file";
    LoadBitmap(HDC_SCREEN, &bmp[2], "res/internet.png");
    label [2] = "internet";

    GetClientRect(self->hwnd, &rc);
    ctnr = (mContainerCtrl*)ncsCreateWindow(NCSCTRL_CONTAINERCTRL,
            "ContainerCtrl",
            WS_VISIBLE, 0, 0,
            0, 10, RECTW(rc), RECTH(rc)-10*2,
            self->hwnd,
            NULL, NULL, NULL, 0);

    content = (mPanelPiece*)NEWPIECE(mPanelPiece);
    _c(content)->appendEventHandler(content, MSG_LBUTTONDOWN, s_onMouseEvent);
    _c(content)->appendEventHandler(content, MSG_LBUTTONUP, s_onMouseEvent);
    _c(content)->appendEventHandler(content, MSG_MOUSEMOVE, s_onMouseEvent);

    view = NEWPIECE(mHScrollViewPiece);
    GetClientRect(ctnr->hwnd, &rc);
    _c(view)->setRect(view, &rc);
    _c(view)->addContent(view, (mHotPiece*)content, 0, 0);

    _c(ctnr)->setBody(ctnr, (mHotPiece*)view);

    /* add a rect piece as background.*/
    backPiece = (mHotPiece*)NEWPIECE(mRectPiece);
    GetClientRect(ctnr->hwnd, &rc);
    rc.right = rc.left + (bmp[0].bmWidth + 15) * 18 + 15;
    _c(content)->setRect(content, &rc);
    _c(backPiece)->setRect (backPiece, &rc);
    _c(backPiece)->setProperty (backPiece, NCSP_RECTPIECE_FILLCOLOR, MakeRGBA(0xff, 0xff, 0xff, 0xff));

    _c(content)->addContent (content, backPiece, 0, 0);

    /* add 18 imagePiece */
    rc.left = rc.top = 0;
    for (i = 0; i < 18; i++) {
        mHotPiece *imagePiece = (mHotPiece*)NEWPIECE(mImagePiece);
        mHotPiece *labelPiece = (mHotPiece*)NEWPIECE(mLabelPiece);

        rc.right  = bmp[i % 3].bmWidth;
        rc.bottom = bmp[i % 3].bmHeight;
        _c(imagePiece)->setRect (imagePiece, &rc);
        _c(imagePiece)->setProperty (imagePiece, NCSP_IMAGEPIECE_IMAGE, (DWORD)&bmp[i % 3]);

        _c(content)->addContent (content, imagePiece, 15 + (bmp[i % 3].bmWidth + 15) * i, 10);


        rc.bottom = 10;
        rc.right = bmp[i % 3].bmWidth;
        _c(labelPiece)->setRect (labelPiece, &rc);
        _c(labelPiece)->setProperty (labelPiece, NCSP_LABELPIECE_LABEL, (DWORD)label[i % 3]);
        _c(content)->addContent (content, labelPiece, 15 + (bmp[i % 3].bmWidth + 15) * i, bmp[i % 3].bmHeight + 15);
    }

    ncsSetComponentHandlers((mComponent *)ctnr, g_handles, -1);

    return TRUE;
}

static BOOL mymain_onClose(mWidget* self, int message)
{   
    DestroyMainWindow(self->hwnd);
    return TRUE;
}

static NCS_EVENT_HANDLER mymain_handlers [] = {
    {MSG_CREATE, mymain_onCreate },
    {MSG_CLOSE, mymain_onClose },
#if 0   /* Since 1.4.0, deprecated code */
    /* VM (2020-03-08): no need to install the following handlers.
       You can reserve the following code, but they do nothing. */
    {MSG_LBUTTONDOWN, SpeedMeterMessageHandler },
    {MSG_LBUTTONUP, SpeedMeterMessageHandler },
    {MSG_MOUSEMOVE, SpeedMeterMessageHandler },
#endif  /* deprecated code */
    {0, NULL}
};

int MiniGUIMain(int argc, const char *argv[]) {
    MSG Msg;
    mWidget *mymain;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

    ncsInitialize();
	ncs4TouchInitialize();

    mymain = ncsCreateMainWindow(NCSCTRL_MAINWND, "Horizontal Scroll View",
            WS_CAPTION | WS_BORDER | WS_VISIBLE,
            WS_EX_AUTOSECONDARYDC,
            1,
            0, 0, 600, 150,
            HWND_DESKTOP,
            0, 0,
            NULL,
            NULL,
            mymain_handlers,
            0);

    mGEffInit();

    while(GetMessage(&Msg, mymain->hwnd)) {   
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(mymain->hwnd);

    mGEffDeinit();

	ncs4TouchUninitialize();
    ncsUninitialize();

    return 0;
}
