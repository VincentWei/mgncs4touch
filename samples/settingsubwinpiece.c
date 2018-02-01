
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

#define MSG_SETTING_SUB_WIN 1234
#define TITLEFONT_COLOR       0xFF192952
#define SUBTITLEFONT_COLOR    0xFF868686
#define DETAILFONT_COLOR      0xFF1882bd
#define CONTENTFONT_COLOR     0xFF192952
#define BACKGRAND_COLOR       0x7F5CCC5C

extern mNavigationPanelPiece *m_nav ;

static int ITEMPIECE_MARGIN    = 3;
static int ITEMPIECE_CLEARANCE = 5;


static mItemPiece *subWinCreateItemPiece(mSettingSubWinPiece *self, int row);
static int getCurItemPieceRow(char* candidacy, char* s2);

static mNavigationPanelPiece *getControllerHandle(void)
{
    return m_nav;
}

/*
 * CreateRadioItem : create radio item piece
 *
 * Params          : title
 *                 : btn_text
 *                 : default_value
 *                 : btnStyle
 *                 : btnAlign
 *
 * Return          : mItemPiece
 */
mItemPiece *CreateRadioItemEx (const char *title,
        const char *btn_text, BOOL default_value,
        int btnStyle, enum enumNCSAlign btnAlign,
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

    if (btnAlign == NCS_ALIGN_RIGHT) {
        _c(item)->setLayout (item, NCS_ALIGN_LEFT);

        argb = TITLEFONT_COLOR;

        _c(item)->setTitle (item, (char *)title, NULL, &argb);

        _c(item)->setLayout (item, NCS_ALIGN_RIGHT);

        _c(item)->setAccessoryControl (item,
                (char *)btn_text, default_value,
                ITEMPIECE_AC_RADIO, 0);
    }
    else {
        _c(item)->setLayout (item, NCS_ALIGN_LEFT);

        _c(item)->setAccessoryControl (item,
                (char *)btn_text, default_value,
                ITEMPIECE_AC_RADIO, 0);

        argb = TITLEFONT_COLOR;

        _c(item)->setTitle (item, (char *)title, NULL, &argb);
    }

    _c(item)->activeLayout (item);

    return item;
}


static void mSettingSubWinPiece_construct(mSettingSubWinPiece *self, DWORD add_data)
{
    Class(mTableViewPiece).construct((mTableViewPiece *)self, NCS_TABLEVIEW_INDEX_STYLE);

    self->itemname = (char *)add_data;
    self->candidacy[0] = "test 1";
    self->candidacy[1] = "test 2";
    self->candidacy[2] = "test 3";
    self->candidacy[3] = "test 4";
}

static int mSettingSubWinPiece_numberOfRowsInSection(mSettingSubWinPiece *self, int section)
{
    return 4;
}

static mTableViewItemPiece *mSettingSubWinPiece_createItemForRow(mSettingSubWinPiece *self, const mIndexPath *indexpath){
    int row;
    RECT rc;

    row = indexpath->row;
    mTableViewItemPiece *item = NEWPIECEEX(mTableViewItemPiece,0);
    SetRect(&rc, 0, 0, SETTING_ITEM_W, SETTING_ITEM_H);

    _c(item)->setRect(item, &rc);
    _c(item)->setUserPiece(item, (mHotPiece *)subWinCreateItemPiece(self, row));

    return item;
}

static void onClickReloadData(mWidget* _self, int message, WPARAM wParam, LPARAM lParam) 
{
    assert(message == MSG_SETTING_SUB_WIN);

/*
    mSettingSubWinPiece *self = (mSettingSubWinPiece *)wParam;
    mSettingMainWinPiece *mainpiece = 
        (mSettingMainWinPiece *)SettingActivity::m_settingMainWinPiece;

    GET_SETTINGSERVICE()->setCurrent(self->itemname,self->candidacy[row]);

    //_c(self)->reloadData(self);
    _c(mainpiece)->myreloadData(mainpiece,TYPE_ALL);

    */
    return ;
}

static void mSettingSubWinPiece_rowDidSelectAtIndexPath(mSettingSubWinPiece *self, const mIndexPath *indexpath){
    mNavigationPanelPiece *nav = getControllerHandle();

    int row = indexpath->row;

    //assert(self->candidacy.size() > 0);
    //std::string cur = GET_SETTINGSERVICE()->getCurrent(self->itemname);

    if(getCurItemPieceRow(self->candidacy[0], NULL) != row)
    {
        mWidget* _w = _c(self)->getOwner(self);

#if 1
        ncsSetComponentHandler((mComponent*)_w, MSG_SETTING_SUB_WIN, (void *)onClickReloadData);
        PostMessage(_w->hwnd, MSG_SETTING_SUB_WIN, (WPARAM)self, (LPARAM)row);
#else
        mSettingMainWinPiece *mainpiece = 
            (mSettingMainWinPiece *)SettingActivity::m_settingMainWinPiece;

        GET_SETTINGSERVICE()->setCurrent(self->itemname,self->candidacy[row]);
        _c(self)->reloadData(self);
        _c(mainpiece)->myreloadData(mainpiece,TYPE_ALL); 
#endif
    }

    //fix me:pop when lbuttonup
    _c(nav)->pop(nav);
}

static void mSettingSubWinPiece_destroy(mSettingSubWinPiece *self)
{
    Class(mTableViewPiece).destroy((mTableViewPiece*)self);
}

static mItemPiece *subWinCreateItemPiece(mSettingSubWinPiece *self, int row){
    mItemPiece *item;
    int defaultvalue = 0;

    if(getCurItemPieceRow(self->candidacy[0], NULL) == row)
        defaultvalue = 1;

    item = CreateRadioItemEx(self->candidacy[row], NULL,
            defaultvalue,0,NCS_ALIGN_LEFT, 0);
    return item;
}

static int getCurItemPieceRow(char* candidacy, char* s2){
    return 0;
}

BEGIN_MINI_CLASS(mSettingSubWinPiece, mTableViewPiece)
    CLASS_METHOD_MAP(mSettingSubWinPiece, construct)
    CLASS_METHOD_MAP(mSettingSubWinPiece, numberOfRowsInSection)
    CLASS_METHOD_MAP(mSettingSubWinPiece, createItemForRow)
    CLASS_METHOD_MAP(mSettingSubWinPiece, rowDidSelectAtIndexPath)  
    CLASS_METHOD_MAP(mSettingSubWinPiece, destroy)
END_MINI_CLASS
