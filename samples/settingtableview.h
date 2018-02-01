#ifndef _SETTING_ACTIVITY_HH_
#define _SETTING_ACTIVITY_HH_
//#include "NCSActivity.hh"
//#include "global.h"
//#include "SettingService.hh"


//globe define////
#define MAX_PAGE_NUMBER 10
#define COLUMNS_PER_PAGE 4
#define ROWS_PER_PAGE 4
#define ITEMS_PER_PAGE 16

#define SCREEN_W       800
#define SCREEN_H       480

#define TITLE_X        0
#define TITLE_Y        0
#define TITLE_H        22
#define TITLE_W        SCREEN_W
#define ACTIVITY_X     0
#define ACTIVITY_Y     22
#define ACTIVITY_W     SCREEN_W
#define ACTIVITY_H     (SCREEN_H - TITLE_H)
///////////


#define SETTING_IDC_CTNR   100
#define SETTING_TIMER   108
#define SETTING_ITEM_MAX   8

#define SETTING_TITLE_H   NAVIGATIONBAR_H
#define SETTING_SEPARATOR_H 2
#define SETTING_ITEM_H ((ACTIVITY_H - SETTING_TITLE_H - \
        (SETTING_ITEM_MAX - 1) * SETTING_SEPARATOR_H)/SETTING_ITEM_MAX)

#define SETTING_ITEM_W ACTIVITY_W   
#define SETTING_TABLE_H (ACTIVITY_H - SETTING_TITLE_H)

#define SETTING_OK_BTN_W 60
#define SETTING_OK_BTN_H NAVIGATIONBAR_DEFAULT_BUTTON_H 

#define SETTING_OK_BTN_TEXT_H 16
#define SETTING_ITEM_TEXT_H 18

#define SETTING_OK_BTN_COLOR 0xff192952 

typedef enum setting_win_type_e{
    TYPE_MAIN_WIN_UI,
    TYPE_SUB_WIN_UI,
    TYPE_CLOCK_WIN_UI,
    TYPE_WIN_MAX_NUM
}setting_win_type;


enum ExtraType {
    TYPE_EDIT,
    TYPE_CHECKBOX,
    TYPE_INDICATOR,
    TYPE_TEXT,
    TYPE_NULL
};

#define TYPE_EDIT 0
#define TYPE_CHECKBOX 1
#define TYPE_INDICATOR 2
#define TYPE_TEXT 3
#define TYPE_NULL 4


#define SETTING_STR_MAX_LEN 32

//static mNavigationPanelPiece *getControllerHandle(void);
//static mNavigationItem *getNavItem(setting_win_type type);

#endif/*end*/
