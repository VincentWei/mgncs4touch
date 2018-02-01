#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>
#include <mgncs4touch/mgncs4touch.h>

#include "settingtableview.h"
#include "settingmainwinpiece.h"
#include "settingsubwinpiece.h"

char* s_dataValue [] = {
    "00:25",
    "on",
    "on",
    "on",
    "Automatic",
    "Line1",
    "English",
    "on",
    "on",
    "on",
    "off",
    "1 minute",
    "1.png",
};

int s_dataType [] = {
    TYPE_EDIT,
    TYPE_CHECKBOX,
    TYPE_CHECKBOX,
    TYPE_CHECKBOX,
    TYPE_INDICATOR,
    TYPE_INDICATOR,
    TYPE_CHECKBOX,
    TYPE_CHECKBOX,
    TYPE_CHECKBOX,
    TYPE_CHECKBOX,
    TYPE_INDICATOR,
    TYPE_INDICATOR,
    TYPE_INDICATOR,
};

extern mHotPiece *m_settingMainWinPiece;
extern mHotPiece *m_settingSubWinPiece;

extern mNavigationPanelPiece *m_nav ;
extern mNavigationItem *m_navItems[TYPE_WIN_MAX_NUM];
extern mHotPiece *m_okBtn;
extern mHotPiece *m_barBack;

extern char* m_settingData[];

extern HWND m_hWnd;
int m_DataLen = 13;


////muser itme////

#define TITLEFONT_COLOR       0xFF192952
#define SUBTITLEFONT_COLOR    0xFF868686
#define DETAILFONT_COLOR      0xFF1882bd
#define CONTENTFONT_COLOR     0xFF192952
#define BACKGRAND_COLOR       0x7F5CCC5C

static int ITEMPIECE_MARGIN    = 3;
static int ITEMPIECE_CLEARANCE = 5;

static int init_flag = 0;
static mIndicatorButtonPieceImages indicator_bmps;
BITMAP indicator_bmp0;
BITMAP indicator_bmp1;



/*
 * InitUserItem : init user item
 *
 * Params       : (none)
 *
 * Return       : (none)
 */
void InitUserItem (void)
{
    if (init_flag == 0) {
        init_flag = 1;

#if 0
        indicator_bmps.bmp[0] =
            (BITMAP *)Load32Resource ("res/common/indicator_green_normal.png",
                RES_TYPE_IMAGE, (DWORD)HDC_SCREEN);

        indicator_bmps.bmp[1] =
            (BITMAP *)Load32Resource ("res/common/indicator_green_click.png",
                RES_TYPE_IMAGE, (DWORD)HDC_SCREEN);
#endif

#if 1
        LoadBitmap (HDC_SCREEN,
                &indicator_bmp0,
                "res/indicator_green_normal.png");

        LoadBitmap (HDC_SCREEN,
                &indicator_bmp1,
                "res/indicator_green_click.png");


        indicator_bmps.bmp[0] = &indicator_bmp0;
        indicator_bmps.bmp[1] = &indicator_bmp1;
#endif
    }
}


/*
 * CreateEditItem : create edit item piece
 *
 * Params         : title
 *                : detail
 *
 * Return         : mItemPiece
 */
mItemPiece *CreateEditItemEx (const char *title,
        const char *detail, DWORD data)
{
    RECT rect;
    ARGB argb;

    mItemPiece *item;

    item = NEWPIECEEX (mItemPiece, data);

    SetRect (&rect, 0, 0, ITEMPIECE_WIDTH, ITEMPIECE_HEIGHT);

    _c(item)->setRect (item, &rect);

    _c(item)->setProperty (item, NCSP_ITEMPIECE_MARGIN, ITEMPIECE_MARGIN);

    _c(item)->setProperty (item, NCSP_ITEMPIECE_CLEARANCE, ITEMPIECE_CLEARANCE);

    _c(item)->setLayout (item, NCS_ALIGN_LEFT);

    argb = TITLEFONT_COLOR;

    _c(item)->setTitle (item, (char *)title, NULL, &argb);

    _c(item)->setLayout (item, NCS_ALIGN_RIGHT);

    argb = DETAILFONT_COLOR;

    _c(item)->setDetail (item, (char *)detail, NULL, &argb);

    _c(item)->activeLayout (item);

    return item;
}

/*
 * CreateSwitchCtrlItem : create switch control item piece
 *
 * Params               : title
 *                      : btn_text
 *                      : default_val
 *
 * Return               : mItemPiece
 */
mItemPiece *CreateSwitchCtrlItemEx (const char *title,
        const char *btn_text, BOOL default_value,
        DWORD data)
{
    RECT rect;
    ARGB argb;

    mItemPiece *item;

    item = NEWPIECEEX (mItemPiece, data);

    SetRect (&rect, 0, 0, ITEMPIECE_WIDTH, ITEMPIECE_HEIGHT);

    _c(item)->setRect (item, &rect);

    _c(item)->setProperty (item, NCSP_ITEMPIECE_MARGIN, ITEMPIECE_MARGIN);

    _c(item)->setProperty (item, NCSP_ITEMPIECE_CLEARANCE, ITEMPIECE_CLEARANCE);

    _c(item)->setLayout (item, NCS_ALIGN_LEFT);

    argb = TITLEFONT_COLOR;

    _c(item)->setTitle (item, (char *)title, NULL, &argb);

    _c(item)->setLayout (item, NCS_ALIGN_RIGHT);

    _c(item)->setAccessoryControl (item,
            (char *)btn_text, default_value,
            ITEMPIECE_AC_SWITCH, 0);

    _c(item)->activeLayout (item);

    return item;
}

/*
 * CreateDetailBtnItem : create detail button item piece
 *
 * Params              : title
 *                     : btn_text
 *                     : default_value
 *
 * Return              : mItemPiece
 */
mItemPiece *CreateDetailBtnItemEx (const char *title,
        const char *btn_text, BOOL default_value,
        DWORD data)
{
    RECT rect;
    ARGB argb;

    mItemPiece *item;

    InitUserItem();

    item = NEWPIECEEX (mItemPiece, data);

    SetRect (&rect, 0, 0, ITEMPIECE_WIDTH, ITEMPIECE_HEIGHT);

    _c(item)->setRect (item, &rect);

    _c(item)->setProperty (item, NCSP_ITEMPIECE_MARGIN, ITEMPIECE_MARGIN);

    _c(item)->setProperty (item, NCSP_ITEMPIECE_CLEARANCE, ITEMPIECE_CLEARANCE);

    _c(item)->setLayout (item, NCS_ALIGN_LEFT);

    argb = TITLEFONT_COLOR;

    _c(item)->setTitle (item, (char *)title, NULL, &argb);

    _c(item)->setLayout (item, NCS_ALIGN_RIGHT);

    _c(item)->setAccessoryControl (item,
            (char *)btn_text, default_value,
            ITEMPIECE_AC_INDICATORBUTTON, (DWORD)&indicator_bmps);

    argb = DETAILFONT_COLOR;

    _c(item)->setDetail (item, (char *)btn_text, NULL, &argb);

    _c(item)->activeLayout (item);

    return item;
}


//////


static mNavigationPanelPiece *getControllerHandle(void)
{
    return m_nav;
}
static mNavigationItem *getNavItem(setting_win_type type)
{
    if (type >= TYPE_MAIN_WIN_UI && type < TYPE_WIN_MAX_NUM)
        return m_navItems[type];
    return NULL;
}

/*
static BOOL updateClockValue(void* target, TimeService *ts, int eventId, DWORD param)
{
    mSettingMainWinPiece *self = (mSettingMainWinPiece *)target;
    _c(self)->myreloadData(self,TYPE_CLOCK);
    return TRUE;
}
*/

static const char *getStringFromRow(mSettingMainWinPiece *self,int row)
{
    return self->data[row];
}

static void mSettingMainWinPiece_construct(mSettingMainWinPiece *self, DWORD add_data)
{
    Class(mTableViewPiece).construct((mTableViewPiece*)self, NCS_TABLEVIEW_INDEX_STYLE);
    //self->act = (SettingActivity *)add_data;
    //self->data = &(SettingActivity::m_settingData);
    self->act = NULL;
    self->data = m_settingData;

    int len = m_DataLen;
    self->cur_data = (char (*)[SETTING_STR_MAX_LEN])malloc(len * SETTING_STR_MAX_LEN);
    assert(self->cur_data);
    //assert(self->act);

    //TIMESERVICE->addEventListener((void *)self,TimeService::MINUTE,updateClockValue);
    //TIMESERVICE->addEventListener((void *)self,TimeService::HOUR,updateClockValue);
}

static int mSettingMainWinPiece_numberOfSections(mSettingMainWinPiece* self)
{
    return 1;
}
static int mSettingMainWinPiece_numberOfRowsInSection(mSettingMainWinPiece* self, int section)
{
    return m_DataLen;
}

static BOOL tableitem_event_cb(mWidget *_self, mHotPiece *piece,
        int event_id, DWORD param)
{
    unsigned int row = (unsigned int)param;
    mSettingMainWinPiece *self = (mSettingMainWinPiece *)_self;

    //ExtraType extra_type;
    //const char *rowdata;

    assert(row < m_DataLen);
    /*rowdata = */getStringFromRow(self,row);

    /*
    ExtraType extra_type;
    extra_type = GET_SETTINGSERVICE()->getItemType(rowdata);
    if (extra_type == TYPE_TEXT)
    {
    }
    else if (extra_type == TYPE_CHECKBOX)
    {
        if (event_id == NCSN_ITEMPIECE_CHECKBOX_ON)
        {
            GET_SETTINGSERVICE()->setCurrent(rowdata,"on");
        }
        else if (event_id == NCSN_ITEMPIECE_CHECKBOX_OFF)
        {
            GET_SETTINGSERVICE()->setCurrent(rowdata,"off");
        }
    }
    else if (extra_type == TYPE_INDICATOR)
    {
    }
    else if (extra_type == TYPE_EDIT)
    {
    }
    */
    return TRUE;
}


int getPieceItemType(int row) {
    return s_dataType[row];
}

static mItemPiece *settingCreateUserPiece(mSettingMainWinPiece* self,const mIndexPath* indexpath)
{
    unsigned int row = (unsigned)indexpath->row;
    mItemPiece *setitem = NULL;
    
    int extra_type;

    const char* rowdata;

    rowdata = getStringFromRow(self,row);

    extra_type = getPieceItemType(row);

    if (extra_type == TYPE_TEXT)
    {
        setitem = CreateEditItemEx(rowdata,self->cur_data[row], 0);
    }
    else if (extra_type == TYPE_CHECKBOX)
    {
        int event_ids[] = {NCSN_ITEMPIECE_CHECKBOX_ON ,NCSN_ITEMPIECE_CHECKBOX_OFF , 0};

        if (strcmp(s_dataValue[row], "on") == 0)
        {
            setitem = CreateSwitchCtrlItemEx(rowdata,NULL, 1,row);
        }
        else
        {
            setitem = CreateSwitchCtrlItemEx(rowdata,NULL, 0,row);
        }

        ncsAddEventListeners((mObject*)setitem, (mObject*)self,
                (NCS_CB_ONPIECEEVENT)tableitem_event_cb, event_ids);
    }
    else if (extra_type == TYPE_INDICATOR)
    {
        setitem = CreateDetailBtnItemEx(rowdata,self->cur_data[row], 1,row);
    }
    else if (extra_type == TYPE_EDIT)
    {
        setitem = CreateEditItemEx(rowdata,self->cur_data[row], 0);
    }
    else
    {
        printf("========%s======TYPE error\n",__FUNCTION__);
        setitem = CreateEditItemEx(rowdata,"data error", 0);
        /*assert(0);*/
    }

    return setitem;
}

static mTableViewItemPiece* mSettingMainWinPiece_createItemForRow(
                mSettingMainWinPiece* self, const mIndexPath* indexpath)
{
    mTableViewItemPiece *item = NEWPIECEEX(mTableViewItemPiece,0);

    RECT rc = {0,0,SETTING_ITEM_W,SETTING_ITEM_H};

    assert(item);
    _c(item)->setRect(item,&rc);

    mItemPiece *setitem  = settingCreateUserPiece(self,indexpath);
    if (setitem == NULL)
    {
        printf("===%s==create settting item error row = %d\n",__FUNCTION__,indexpath->row);
        return NULL;
    }
    _c(setitem)->setRect(setitem,&rc);

    _c(item)->setUserPiece(item,(mHotPiece*)setitem);
    return item;
}

void createUISubWin(const char *name);

static void mSettingMainWinPiece_rowDidSelectAtIndexPath(
        mSettingMainWinPiece* self, const mIndexPath* indexpath)
{
    unsigned int row = (unsigned)indexpath->row;
    
    int extra_type;
    const char* rowdata;

    assert(row < m_DataLen);
    rowdata = getStringFromRow(self,row);
    
    extra_type = TYPE_INDICATOR;
    
    if (extra_type == TYPE_INDICATOR)
    {
        mSettingSubWinPiece *subpiece;
        mNavigationItem *nextItem;
        mNavigationPanelPiece *nav = getControllerHandle();

        //self->act->createUISubWin(rowdata);
        //subpiece = (mSettingSubWinPiece *)self->act->m_settingSubWinPiece;
        createUISubWin(rowdata);
        subpiece = (mSettingSubWinPiece *)m_settingSubWinPiece;

        assert(subpiece);
        subpiece->itemname = rowdata; 
        //subpiece->candidacy = GET_SETTINGSERVICE()->getCandidacy(rowdata);
        subpiece->candidacy[0] = "Line1";
        subpiece->candidacy[1] = "Line2";

        _c(subpiece)->reloadData(subpiece);

        nextItem = getNavItem(TYPE_SUB_WIN_UI);
        assert(nextItem);
        _c(nav)->push(nav,nextItem);
    }
    else if (extra_type == TYPE_EDIT)
    {
        if (strcmp(rowdata,"Clock") == 0)
        {
            mNavigationItem *nextItem;
            mNavigationPanelPiece *nav = getControllerHandle();
            //createUIClockWin(m_hWnd);
            nextItem = getNavItem(TYPE_CLOCK_WIN_UI);
            assert(nextItem);
            _c(nav)->push(nav,nextItem);
        }
    }
}

static BOOL mSettingMainWinPiece_willSelectRowAtIndexPath(
        mSettingMainWinPiece* self, const mIndexPath* indexpath)
{
    unsigned int row = (unsigned)indexpath->row;

    assert(row < m_DataLen);
    int extra_type = getPieceItemType(row);
    if (extra_type == TYPE_CHECKBOX)
    {
        return FALSE;
    }
    return TRUE;
}

static void mSettingMainWinPiece_myreloadData(mSettingMainWinPiece *self,MY_RELOAD_TYPE type)
{
    int i;
    for (i=0;i<(signed)m_DataLen;i++)
    {
        memset(self->cur_data[i],0x00,SETTING_STR_MAX_LEN);
        if (i == 0)
        {
            strncpy(self->cur_data[i], "14:10", SETTING_STR_MAX_LEN -1 );
            if (type == TYPE_CLOCK)
                break;
        }
        else
        {
            strncpy(self->cur_data[i], s_dataValue[i], SETTING_STR_MAX_LEN - 1);
        }
    }
    _c(self)->reloadData(self);
    return;
}

static void mSettingMainWinPiece_destroy(mSettingMainWinPiece *self)
{
    Class(mTableViewPiece).destroy((mTableViewPiece*)self);
    free(self->cur_data);
}



BEGIN_MINI_CLASS(mSettingMainWinPiece, mTableViewPiece)
	CLASS_METHOD_MAP(mSettingMainWinPiece, construct)
	CLASS_METHOD_MAP(mSettingMainWinPiece, numberOfSections)
	CLASS_METHOD_MAP(mSettingMainWinPiece, numberOfRowsInSection)
	CLASS_METHOD_MAP(mSettingMainWinPiece, createItemForRow)
	CLASS_METHOD_MAP(mSettingMainWinPiece, rowDidSelectAtIndexPath)
	CLASS_METHOD_MAP(mSettingMainWinPiece, willSelectRowAtIndexPath)
	CLASS_METHOD_MAP(mSettingMainWinPiece, myreloadData)
	CLASS_METHOD_MAP(mSettingMainWinPiece, destroy)
END_MINI_CLASS

