#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include <mgncs4touch/mgncs4touch.h>

#include "settingtableview.h"
#include "settingmainwinpiece.h"
#include "settingsubwinpiece.h"

#define BTN_W 80
#define BTN_H 30
#define ITEM_W ACTIVITY_W
#define ITEM_H 50

#define TABLEVIEW_ITEMNUM 13

void createNavigatorBtns(mNavigationItem* self);
void createCtnrBody(mContainerCtrl* ctnr,mNavigationItem *rootItem);
void createUIMainWin(HWND hwnd);
void createUISubWin(const char *name);
//void createUIClockWin(HWND hwnd);
void initClockWin(HWND hwnd);
void releaseClockView(HWND hwnd);

int onStop(void);
void initResource(void);
void releaseResource(void);

void initSetting(void);
void unInitSetting(void);


HWND m_hWnd;
mHotPiece *m_settingMainWinPiece = NULL;
mHotPiece *m_settingSubWinPiece = NULL;

mNavigationPanelPiece *m_nav = NULL;
mNavigationItem *m_navItems[TYPE_WIN_MAX_NUM] = {NULL};
mHotPiece *m_okBtn;
mHotPiece *m_barBack;

PLOGFONT m_btnFont;
PLOGFONT m_itemTextFont;

char* m_settingData[] = {
   "Clock",
   "Anykey answer",
   "Automatic redial",
   "Shock",
   "Network selection",
   "Phone line in use",
   "Language",
   "Airplane mode",
   "WLAN",
   "Bluetooth",
   "Silent mode",
   "Screen timeout",
   "Photo",
};


ARGB title_gradient_color[] = {0xffd0d571, 0xffb5c363,
    0xff9eb23a,0xff7f9e2c,0xff597311,0xff597311};
float title_gradient_pos[] = {0.0, 0.49,0.50,0.96,0.97,1.0};

#if 0
static mNavigationPanelPiece *getControllerHandle(void)
{
    return m_nav;
}
#endif

static mNavigationItem *getNavItem(setting_win_type type)
{
    if (type >= TYPE_MAIN_WIN_UI && type < TYPE_WIN_MAX_NUM)
        return m_navItems[type];
    return NULL;
}


static void setting_gradient_color(mShapeTransRoundPiece *piece,
        ARGB *colors, float *pos, int num)
{
    HBRUSH brush;
    _c(piece)->setProperty (piece, NCSP_TRANROUND_FILLMODE, NCSP_TRANROUND_GRADIENT_FILL);
    brush = _c(piece)->getBrush(piece);
    MGPlusSetLinearGradientBrushColorsEx(brush,colors,num,pos);
    return;
}

static BOOL settingInitData(char* settingdata[])
{
    /*
    StringArray strs = GET_SETTINGSERVICE()->getAllItemNames();
    if (settingdata != NULL)
        *settingdata = strs;
        */
    return TRUE;
}

mHotPiece * createMainWinView(void)
{
    return (mHotPiece *)NEWPIECEEX(mSettingMainWinPiece, NULL);
}

mHotPiece * createSubWinView(const char* name)
{
    return (mHotPiece *)NEWPIECEEX(mSettingSubWinPiece,(DWORD)name);
}

int onBack(void)
{
    if (_c(m_nav)->currentIsRoot(m_nav))
        return 0;

    _c(m_nav)->pop(m_nav);
    return 1;
}

static BOOL btnOk_event_handler(mWidget *self, mHotPiece *piece,int event_id, DWORD param)
{
/*
    assert(NCSN_ABP_CLICKED);

    ActivityStack::singleton()->pop();
    */
    return TRUE;
}

void createNavigatorBtns(mNavigationItem* self)
{
    m3DButtonPiece *mbutton;
    RECT rc = {0,0,SETTING_OK_BTN_W,SETTING_OK_BTN_H};
    mbutton = (m3DButtonPiece*)_c(self)->createDefaultButton(self, "OK",0);
    assert(mbutton);

    _c(mbutton)->setRect (mbutton, &rc);
    m_okBtn = (mHotPiece *)mbutton;
    ncsAddEventListener((mObject*)m_okBtn, (mObject*)m_okBtn,
            (NCS_CB_ONPIECEEVENT)btnOk_event_handler, NCSN_ABP_CLICKED);

    _c(self)->setProperty(self,NCSP_NAVIGATIONITEM_RIGHT_BUTTON, (DWORD)m_okBtn);
    return;
}

void createCtnrBody(mContainerCtrl* ctnr,mNavigationItem *rootItem)
{
    RECT rc;
    assert(ctnr);
    assert(rootItem);
    m_barBack = (mHotPiece*)NEWPIECE(mShapeTransRoundPiece);
    SetRect(&rc, 0, 0, NAVIGATIONBAR_W, NAVIGATIONBAR_H);
    _c(m_barBack)->setRect(m_barBack, &rc);
    _c(m_barBack)->setProperty (m_barBack, NCSP_TRANROUND_CORNERFLAG,0);
    setting_gradient_color((mShapeTransRoundPiece *)m_barBack,
            title_gradient_color,title_gradient_pos,TABLESIZE(title_gradient_color));
    m_nav = ncsCreateNavigationPanelPieceWithRootView(rootItem);
    SetRect(&rc, 0, 0, ACTIVITY_W, ACTIVITY_H);
    _c(m_nav)->setRect(m_nav, (const RECT*)&rc);
    _c(m_nav)->setProperty(m_nav, NCSP_NAVIGATIONPANELPIECE_BAR_BKG, (DWORD)m_barBack);
    _c(ctnr)->setBody(ctnr, (mHotPiece*)m_nav);
}
void createUIMainWin(HWND hwnd)
{
    RECT rc = {0,0,SETTING_ITEM_W,SETTING_TABLE_H};

    mSettingMainWinPiece *mainWinPiece = (mSettingMainWinPiece *)createMainWinView();
    assert(mainWinPiece);
    _c(mainWinPiece)->setRect(mainWinPiece, &rc);


    _c(mainWinPiece)->myreloadData(mainWinPiece,TYPE_ALL);

    m_navItems[TYPE_MAIN_WIN_UI] = ncsCreateNavigationItem(
            (mHotPiece *)mainWinPiece, "Settings", NAVIGATION_STYLE_NORMAL);

    assert(m_navItems[TYPE_MAIN_WIN_UI]);
    ADDREF(m_navItems[TYPE_MAIN_WIN_UI]);


    m_settingMainWinPiece = (mHotPiece *)mainWinPiece;
}

/*
void createUIClockWin(HWND hwnd)
{
    RECT rc = {0,0,SETTING_ITEM_W,SETTING_TABLE_H};

    mPanelPiece *clockPiece = NULL;

    if (m_navItems[TYPE_CLOCK_WIN_UI] != NULL)
    {
        _c(m_navItems[TYPE_CLOCK_WIN_UI])->setProperty(m_navItems[TYPE_CLOCK_WIN_UI], 
            NCSP_NAVIGATIONITEM_TITLE, (DWORD)"Date and Time");
        return;
    }
    clockPiece = (mPanelPiece *)createClockView(hwnd);
    assert(clockPiece);
    _c(clockPiece)->setRect(clockPiece, &rc);

    m_navItems[TYPE_CLOCK_WIN_UI] = ncsCreateNavigationItem(
            (mHotPiece *)clockPiece, "Clock", NAVIGATION_STYLE_NORMAL);

    assert(m_navItems[TYPE_CLOCK_WIN_UI]);
    ADDREF(m_navItems[TYPE_CLOCK_WIN_UI]);
}
*/

void createUISubWin(const char* name)
{
    RECT rc = {0, 0, SETTING_ITEM_W,SETTING_TABLE_H};
    mSettingSubWinPiece *subWinPiece = NULL;

    if (m_navItems[TYPE_SUB_WIN_UI] != NULL)
    {
        _c(m_navItems[TYPE_SUB_WIN_UI])->setProperty(m_navItems[TYPE_SUB_WIN_UI], 
            NCSP_NAVIGATIONITEM_TITLE, (DWORD)name);
        return;
    }
    subWinPiece = (mSettingSubWinPiece *)createSubWinView(name);
    assert(subWinPiece);
    _c(subWinPiece)->setRect(subWinPiece, &rc);

    
    m_navItems[TYPE_SUB_WIN_UI] = ncsCreateNavigationItem(
            (mHotPiece *)subWinPiece, name, NAVIGATION_STYLE_NORMAL);

    assert(m_navItems[TYPE_SUB_WIN_UI]);
    ADDREF(m_navItems[TYPE_SUB_WIN_UI]);
#if 0
    _c(m_navItems[TYPE_SUB_WIN_UI])->setProperty(m_navItems[TYPE_SUB_WIN_UI], 
            NCSP_NAVIGATIONITEM_BACKGROUND, (DWORD)m_barBack);
#endif
    m_settingSubWinPiece = (mHotPiece *)subWinPiece;
}

void initResource()
{
    m_btnFont = CreateLogFontEx ("ttf", "helvetica", "GB2312",
            FONT_WEIGHT_REGULAR,
            FONT_SLANT_ROMAN,
            FONT_FLIP_NONE,
            FONT_OTHER_NONE,
            FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
            SETTING_OK_BTN_TEXT_H, 0);

    m_itemTextFont = CreateLogFontEx ("ttf", "helvetica", "GB2312",
            FONT_WEIGHT_REGULAR,
            FONT_SLANT_ROMAN,
            FONT_FLIP_NONE,
            FONT_OTHER_NONE,
            FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
            SETTING_ITEM_TEXT_H, 0);
    return;
}

void releaseResource()
{
    DestroyLogFont(m_btnFont);
    DestroyLogFont(m_itemTextFont);
    return;
}

static BOOL on_timer(
        mObject *listener, mObject *sender, int id, DWORD total_count)
{
    mTimer *timer = (mTimer *)sender;
    /*
    mMainWnd* self = (mMainWnd *)listener;
    SettingActivity* act = 
        (SettingActivity*)Activity::getActivityFromHWND (self->hwnd);
    act->createUIClockWin(self->hwnd);
    act->initClockWin(self->hwnd);
    */
    _c(timer)->stop(timer);
    return TRUE;
}

static BOOL main_onCreate(mMainWnd* self, DWORD dwAddData )
{
    mTimer *timer;
    //mContainerCtrl *ctnr = (mContainerCtrl *)ncsGetChildObj(self->hwnd, SETTING_IDC_CTNR);
    //RECT rc = {0, 0, ITEM_W, ITEM_H * TABLEVIEW_ITEMNUM};

    /* table */
    mContainerCtrl* ctnr = (mContainerCtrl*)ncsCreateWindow(NCSCTRL_CONTAINERCTRL,
            "ContainerCtrl",
            WS_VISIBLE, 0, 0,
            0, BTN_H + 10, ITEM_W, TABLEVIEW_ITEMNUM * ITEM_H + 2,
            self->hwnd,
            NULL, NULL, NULL, 0);

    settingInitData(NULL);
    createUIMainWin(self->hwnd);
    createNavigatorBtns(getNavItem(TYPE_MAIN_WIN_UI));
    createCtnrBody(ctnr, getNavItem(TYPE_MAIN_WIN_UI));

    timer = (mTimer*)ncsGetChildObj(self->hwnd, SETTING_TIMER);
    if(timer){
        ncsAddEventListener((mObject *)timer, (mObject *)self, 
                (NCS_CB_ONPIECEEVENT)on_timer, MSG_TIMER);
    }

#if 0
    _c(timer)->start(timer);
#else
    /*
    act->createUIClockWin(self->hwnd);
    act->initClockWin(self->hwnd);
    */
#endif
    return TRUE;
}


static void main_onClose(mWidget* self, int message)
{
    return;
}


static void main_onPaint (mWidget* _this, HDC hdc, const PCLIPRGN clipRgn)
{
    return;
}

static void main_onDestroy(mWidget* self, int message)
{
/*
    SettingActivity* activity= 
        (SettingActivity*)Activity::getActivityFromHWND (self->hwnd);
    assert (activity);
    if (activity->getNavItem(TYPE_MAIN_WIN_UI) != NULL)
    {
        UNREF(activity->getNavItem(TYPE_MAIN_WIN_UI));
    }
    if (activity->getNavItem(TYPE_SUB_WIN_UI) != NULL)
    {
        UNREF(activity->getNavItem(TYPE_SUB_WIN_UI));
    }
    if (activity->getNavItem(TYPE_CLOCK_WIN_UI) != NULL)
    {
        UNREF(activity->getNavItem(TYPE_CLOCK_WIN_UI));
    }
    ncsDestroyNavigationItem(activity->getNavItem(TYPE_MAIN_WIN_UI));
    ncsDestroyNavigationItem(activity->getNavItem(TYPE_SUB_WIN_UI));
    ncsDestroyNavigationItem(activity->getNavItem(TYPE_CLOCK_WIN_UI));
    */

    ncsDestroyNavigationItem(getNavItem(TYPE_MAIN_WIN_UI));
    ncsDestroyNavigationItem(getNavItem(TYPE_SUB_WIN_UI));
    //ncsDestroyNavigationItem(getNavItem(TYPE_CLOCK_WIN_UI));

    return ;
}
static BOOL main_onEraseBk(mWidget *self, HDC hdc, const PRECT clip){
    return TRUE;
}
static NCS_EVENT_HANDLER main_handlers [] = {
    {MSG_CREATE, main_onCreate},
    {MSG_CLOSE, (main_onClose)},
    {MSG_DESTROY,       (main_onDestroy)},
    {MSG_PAINT, (main_onPaint)},
    {MSG_LBUTTONDOWN, (SpeedMeterMessageHandler)},
    {MSG_LBUTTONUP, (SpeedMeterMessageHandler)},
    {MSG_MOUSEMOVE, (SpeedMeterMessageHandler)},
    {MSG_ERASEBKGND, (main_onEraseBk)},
    {0, NULL}
};

#if 0
static NCS_PROP_ENTRY timer_props[] = {
    {NCSP_TIMER_INTERVAL, 50},//unit:10ms
    {0, 0},
};

static NCS_WND_TEMPLATE _ctrl_templ[] = {
    {
        NCSCTRL_CONTAINERCTRL,
        SETTING_IDC_CTNR,
        0, 0, ACTIVITY_W, ACTIVITY_H,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        NULL,
        NULL,
        NULL, NULL, 0, 0
    },
    {
        NCSCTRL_TIMER,
        SETTING_TIMER,
        0,0,0,0,
        WS_VISIBLE,
        WS_EX_NONE,
        "",
        timer_props, NULL, NULL, NULL, 0, 0, 
    },
};

static NCS_MNWND_TEMPLATE mymain_templ = {
        NCSCTRL_DIALOGBOX, 
        1,
        ACTIVITY_X, ACTIVITY_Y, ACTIVITY_W, ACTIVITY_H,
        WS_NONE,
        WS_EX_NONE,
        "sys-setting",
        NULL,
        NULL,
        main_handlers,
        _ctrl_templ,
        TABLESIZE(_ctrl_templ),
        0,
        0, 0,
};
#endif

void initSetting() {

    int i;

    MGNCS_INIT_CLASS (mSettingMainWinPiece);
    MGNCS_INIT_CLASS (mSettingSubWinPiece);

    initResource();

    m_settingMainWinPiece = NULL;
    m_settingSubWinPiece = NULL;
    m_nav = NULL;
    for (i=TYPE_MAIN_WIN_UI;i<TYPE_WIN_MAX_NUM;i++)
    {
        m_navItems[i] = NULL;
    }

}

void unInitSetting () {
    releaseResource();
}

int onStop()
{
    //releaseClockView(m_hWnd);
    return 0;
}


HWND createHWND(NCS_MNWND_TEMPLATE* m_tmpl)
{
    mWidget* window = NULL;
    if (m_tmpl) {
        // Pre-set the additional data,
        // so we can call getActivityFromHWND() to get our Activity's pointer
        // in MSG_CREATE message handler.
        //m_tmpl->user_data = (DWORD)this;
        m_tmpl->user_data = 0;

        window = (mWidget*)ncsCreateMainWindowIndirect
            (m_tmpl, HWND_DESKTOP);
    }
    else {
    }
    return NULL != window ? window->hwnd : HWND_NULL;
}


int MiniGUIMain(int argc, const char *argv[]) {
    MSG Msg;
    mWidget *mymain;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "mSettingTableView" , 0 , 0);
#endif

    ncsInitialize();
	ncs4TouchInitialize();

    initSetting();

    mymain = ncsCreateMainWindow(NCSCTRL_MAINWND, "mSystemTableViewPiece test",
            WS_CAPTION | WS_BORDER | WS_VISIBLE,
            WS_EX_AUTOSECONDARYDC,
            1,
            //0, 0, g_rcScr.right,g_rcScr.bottom,
            0, 0, g_rcScr.right,g_rcScr.bottom,
            HWND_DESKTOP,
            0, 0,
            NULL,
            NULL,
            main_handlers,
            0);

    m_hWnd = mymain->hwnd;
    /*
    m_hWnd = createHWND(&mymain_templ);
    */


    mGEffInit();

    while(GetMessage(&Msg, m_hWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    unInitSetting();

    mGEffDeinit();

	ncs4TouchUninitialize();
    ncsUninitialize();

    return 0;
}


