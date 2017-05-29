/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Beijing FMSoft Technology Co., Ltd.
 * Room 902, Floor 9, Taixing, No.11, Huayuan East Road, Haidian
 * District, Beijing, P. R. CHINA 100191.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Beijing FMSoft Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance you entered into with FMSoft.
 *
 *          http://www.minigui.com
 *
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>

#include "mpcomm.h"
#include "mpadrdr.h"

// #define NDEBUG	1
#include "mpaddebug.h"


#define CAP_BTN_INTERVAL        2

#define CHECKNOT_RET_ERR(cond) \
        if (!(cond)) return -1

#define CHECK_RET_VAL(cond, val) \
        if (cond) return val

#define IS_BORDER_VISIBLE(win_info) \
        ((win_info)->dwStyle & WS_BORDER || \
        (win_info)->dwStyle & WS_THINFRAME || \
        (win_info)->dwStyle & WS_THICKFRAME)

#define IS_VSCROLL_VISIBLE(win_info) \
        ((win_info)->dwStyle & WS_VSCROLL &&  \
         !((win_info)->vscroll.status & SBS_HIDE))

#define IS_HSCROLL_VISIBLE(win_info) \
        ((win_info)->dwStyle & WS_HSCROLL &&  \
         !((win_info)->hscroll.status & SBS_HIDE))

#define IS_LEFT_VSCOLLBAR(win_info) \
        ((win_info)->dwExStyle & WS_EX_LEFTSCROLLBAR)


static WINDOW_ELEMENT_RENDERER* current_rdr = NULL;

static int (*old_fashion_calc_we_area)(HWND, int, RECT*) = NULL;
static int (*old_fashion_hit_test)(HWND, int, int) = NULL;
static void (*old_fashion_draw_border)(HWND, HDC, BOOL) = NULL;
static void (*old_fashion_calc_thumb_area)(HWND, BOOL, LFSCROLLBARINFO*) = NULL;
static int (*old_fashion_calc_we_metrics)(HWND, LFRDR_WINSTYLEINFO*, int) = NULL;

static int pad_calc_we_metrics(HWND hWnd, LFRDR_WINSTYLEINFO* style_info, int which);


static inline int get_window_scrollbar(HWND hWnd, BOOL is_vertival)
{
    if (is_vertival) {
        return pad_calc_we_metrics(hWnd, NULL, LFRDR_METRICS_VSCROLL_W);
    } else {
        return pad_calc_we_metrics(hWnd, NULL, LFRDR_METRICS_HSCROLL_H);
    }
}

// this fuction only for round corner controls.
static int get_window_border(HWND hWnd, int dwStyle, int win_type)
{
    int _dwStyle = dwStyle;

    if (hWnd != HWND_NULL) {
        _dwStyle = GetWindowStyle(hWnd);
    }

    if (_dwStyle& WS_BORDER) {
        if (_dwStyle & WS_THICKFRAME)
            return 2;
        else if (_dwStyle & WS_THINFRAME)
            return 1;
        else
            return GetWindowElementAttr(hWnd, WE_METRICS_WND_BORDER);
    } else if (_dwStyle & WS_THINFRAME) {
        return 1;
    } else if (_dwStyle & WS_THICKFRAME) {
        return 2;
	}

    return 0;
}

static int find_interval(int* array, int len, int val)
{
    int i = 0;

    for (i = 0; i < len - 1; i++) {
        if (array[i]<=val && val<array[i+1])
            break;
    }

    if (i == len - 1)
        return -1;
    else
        return i;
}

static int calc_hscroll_area(HWND hWnd, int which, RECT* we_area)
{
    int border = 0;
    int scrollbar = 0;
    int left_inner = 0;
    int right_inner = 0;
    const WINDOWINFO* win_info = GetWindowInfo(hWnd);
    const LFSCROLLBARINFO* sbar_info = &(win_info->hscroll);

    int win_w = win_info->right - win_info->left;
    int win_h = win_info->bottom - win_info->top;

    border = get_window_border(hWnd, 0, 0);
    scrollbar = GetWindowElementAttr(hWnd, WE_METRICS_SCROLLBAR);

    if (IS_VSCROLL_VISIBLE(win_info)) {
        if (IS_LEFT_VSCOLLBAR(win_info))
            left_inner = scrollbar;
        else
            right_inner = scrollbar;
    }

    we_area->bottom = win_h - border;
    we_area->top = we_area->bottom - scrollbar;

    switch (which) {
        case HT_HSCROLL:
            we_area->left = border + left_inner;
            we_area->right = win_w - border - right_inner;
            return 0;

        case HT_SB_HTHUMB:
            we_area->left = border + left_inner + scrollbar
                            + sbar_info->barStart;
            we_area->right = we_area->left + sbar_info->barLen;
            return 0;

        case HT_SB_LEFTARROW:
            we_area->left = border + left_inner;
            we_area->right = we_area->left + scrollbar;
            return 0;

        case HT_SB_RIGHTARROW:
            we_area->right = win_w - border - right_inner;
            we_area->left = we_area->right - scrollbar;
            return 0;

        case HT_SB_LEFTSPACE:
            we_area->left = border + left_inner + scrollbar;
            we_area->right = we_area->left + sbar_info->barStart;
            return 0;

        case HT_SB_RIGHTSPACE:
            we_area->left = border + left_inner + scrollbar
                            + sbar_info->barStart + sbar_info->barLen;
            we_area->right = win_w - border - right_inner - scrollbar;
            return 0;

        default:
            return -1;
    }

    return -1;
}

static int calc_vscroll_area(HWND hWnd, int which, RECT* we_area)
{
    int border = 0;
    int scrollbar = 0;
    int bottom_inner = 0;

    const WINDOWINFO* win_info = GetWindowInfo(hWnd);
    const LFSCROLLBARINFO* sbar_info = &(win_info->vscroll);

    int win_w = win_info->right - win_info->left;
    int win_h = win_info->bottom - win_info->top;

    border = get_window_border(hWnd, 0, 0);
    scrollbar = GetWindowElementAttr(hWnd, WE_METRICS_SCROLLBAR);

    if (IS_HSCROLL_VISIBLE(win_info))
        bottom_inner = scrollbar;

    if (IS_LEFT_VSCOLLBAR(win_info)) {
        we_area->left = border;
        we_area->right = we_area->left + scrollbar;
    } else {
        we_area->right = win_w - border;
        we_area->left = we_area->right - scrollbar;
    }

    switch (which) {
        case HT_VSCROLL:
            we_area->top = border;
            we_area->bottom = win_h - border - bottom_inner ;
            return 0;

        case HT_SB_VTHUMB:
            we_area->top = border + scrollbar
                         + sbar_info->barStart;
            we_area->bottom = we_area->top + sbar_info->barLen;
            return 0;

        case HT_SB_UPARROW:
            we_area->top = border;
            we_area->bottom = we_area->top + scrollbar;
            return 0;

        case HT_SB_DOWNARROW:
            we_area->bottom = win_h - border - bottom_inner;
            we_area->top = we_area->bottom - scrollbar;
            return 0;

        case HT_SB_UPSPACE:
            we_area->top = border + scrollbar;
            we_area->bottom = we_area->top + sbar_info->barStart;
            return 0;

        case HT_SB_DOWNSPACE:
            we_area->top = border + scrollbar
                         + sbar_info->barStart + sbar_info->barLen;
            we_area->bottom = win_h - border - scrollbar - bottom_inner;
            return 0;

        default:
            return -1;
    }

    return -1;
}

static int pad_calc_we_metrics(HWND hWnd, LFRDR_WINSTYLEINFO* style_info, int which)
{
    switch (which & LFRDR_METRICS_MASK) {
        case LFRDR_METRICS_BORDER:
            if (style_info)
                return get_window_border(HWND_NULL,
                        style_info->dwStyle, style_info->winType);
            else if (hWnd != HWND_NULL)
                return get_window_border(hWnd, 0, 0);
            else
                return GetWindowElementAttr(hWnd, WE_METRICS_WND_BORDER);

        case LFRDR_METRICS_VSCROLL_W: {
            int _idx = 0;
            if (style_info) {
                if (!(style_info->dwStyle & WS_VSCROLL)) {
                    return 0;
                }
                _idx = WE_METRICS_SCROLLBAR & WE_ATTR_INDEX_MASK;
                return current_rdr->we_metrics[_idx];
            }
            else if (hWnd != HWND_NULL) {
                const WINDOWINFO *win_info = GetWindowInfo(hWnd);
                if (!IS_VSCROLL_VISIBLE(win_info))
                    return 0;
            }
            return GetWindowElementAttr(hWnd, WE_METRICS_SCROLLBAR);
        }

        case LFRDR_METRICS_HSCROLL_H: {
            int _idx = 0;
            if (style_info) {
                if (!(style_info->dwStyle & WS_HSCROLL)) {
                    return 0;
                }
                _idx = WE_METRICS_SCROLLBAR & WE_ATTR_INDEX_MASK;
                return current_rdr->we_metrics[_idx];
            } else if (hWnd != HWND_NULL) {
                const WINDOWINFO *win_info = GetWindowInfo(hWnd);
                if (!IS_HSCROLL_VISIBLE(win_info))
                    return 0;
            }
            return GetWindowElementAttr(hWnd, WE_METRICS_SCROLLBAR);
        }

        case LFRDR_METRICS_MINWIN_WIDTH: {
            return (pad_calc_we_metrics(hWnd, style_info, LFRDR_METRICS_BORDER)<< 1);
        }

        case LFRDR_METRICS_MINWIN_HEIGHT: {
            return (pad_calc_we_metrics(hWnd, style_info, LFRDR_METRICS_BORDER)<< 1);
        }
    }
    return old_fashion_calc_we_metrics(hWnd, style_info, which);
}

static int pad_calc_we_area(HWND hWnd, int which, RECT* we_area)
{
    const WINDOWINFO* win_info = NULL;
    int border = 0;
    int win_w = 0;
    int win_h = 0;

    win_info = GetWindowInfo(hWnd);
    win_w = win_info->right - win_info->left;
    win_h = win_info->bottom - win_info->top;

    switch (which) {
        case HT_BORDER:
            we_area->left   = 0;
            we_area->top    = 0;
            we_area->right  = win_w;
            we_area->bottom = win_h;
            return 0;

        case HT_CLIENT:
            border = get_window_border(hWnd, 0, 0);

            we_area->top = border;
            we_area->bottom = win_h - border - get_window_scrollbar(hWnd, FALSE);

            if (IS_LEFT_VSCOLLBAR(win_info)) {
                we_area->left = border + get_window_scrollbar(hWnd, TRUE);
                we_area->right = win_w - border;
            } else {
                we_area->left = border;
                we_area->right = win_w - border - get_window_scrollbar(hWnd, TRUE);
            }

            return 0;

        case HT_HSCROLL:
            CHECKNOT_RET_ERR(IS_HSCROLL_VISIBLE(win_info));
            return calc_hscroll_area(hWnd, which, we_area);

        case HT_VSCROLL:
            CHECKNOT_RET_ERR(IS_VSCROLL_VISIBLE(win_info));
            return calc_vscroll_area(hWnd, which, we_area);

        default:
            if (which & HT_SB_MASK) {
                if (which < HT_SB_UPARROW) {
                    CHECKNOT_RET_ERR(IS_HSCROLL_VISIBLE(win_info));
                    return calc_hscroll_area(hWnd, which, we_area);
                } else {
                    CHECKNOT_RET_ERR(IS_VSCROLL_VISIBLE(win_info));
                    return calc_vscroll_area(hWnd, which, we_area);
                }
            } else {
                return -1;
            }
    }

    return old_fashion_calc_we_area(hWnd, which, we_area);
}

static void pad_calc_thumb_area(HWND hWnd, BOOL vertical, LFSCROLLBARINFO* sb_info)
{
    RECT rc;
    div_t divt;
    int move_range = 0;

    if (vertical) {
        *sb_info = GetWindowInfo(hWnd)->vscroll;
        calc_vscroll_area(hWnd, HT_VSCROLL, &rc);
        sb_info->arrowLen  = RECTW (rc);
        move_range = RECTH (rc) - (sb_info->arrowLen << 1);
    } else {
        *sb_info = GetWindowInfo(hWnd)->hscroll;
        calc_hscroll_area(hWnd, HT_HSCROLL, &rc);
        sb_info->arrowLen  = RECTH (rc);
        move_range = RECTW (rc) - (sb_info->arrowLen << 1);
    }

    if (move_range < 0)
        move_range = 0;

    divt = div (move_range, sb_info->maxPos - sb_info->minPos + 1);
    sb_info->barLen = sb_info->pageStep * divt.quot +
        sb_info->pageStep * divt.rem /
        (sb_info->maxPos - sb_info->minPos + 1);

    if ((sb_info->barLen || sb_info->pageStep != 0) &&
            sb_info->barLen < LFRDR_SB_MINBARLEN)
        sb_info->barLen = LFRDR_SB_MINBARLEN;

    if (sb_info->minPos == sb_info->maxPos) {
        sb_info->barStart = 0;
        sb_info->barLen   = move_range;
    } else {
        divt = div (move_range, sb_info->maxPos - sb_info->minPos + 1);
        sb_info->barStart = (int) (
            (sb_info->curPos - sb_info->minPos) * divt.quot +
            (sb_info->curPos - sb_info->minPos) * divt.rem /
            (sb_info->maxPos - sb_info->minPos + 1) + 0.5);

        if (sb_info->barStart + sb_info->barLen > move_range)
            sb_info->barStart = move_range - sb_info->barLen;
        if (sb_info->barStart < 0)
            sb_info->barStart = 0;
    }
}

static int test_caption(HWND hWnd, int x, int y)
{
    RECT rc;

    if (pad_calc_we_area(hWnd, HT_ICON, &rc) == 0)
        CHECK_RET_VAL(PtInRect(&rc, x, y), HT_ICON);

    if (pad_calc_we_area(hWnd, HT_MINBUTTON, &rc) == 0)
        CHECK_RET_VAL(PtInRect(&rc, x, y), HT_MINBUTTON);

    if (pad_calc_we_area(hWnd, HT_MAXBUTTON, &rc) == 0)
        CHECK_RET_VAL(PtInRect(&rc, x, y), HT_MAXBUTTON);

    if (pad_calc_we_area(hWnd, HT_CLOSEBUTTON, &rc) == 0)
        CHECK_RET_VAL(PtInRect(&rc, x, y), HT_CLOSEBUTTON);

    return HT_CAPTION;
}

static int test_scroll(const LFSCROLLBARINFO* sb_info,
        int left, int right, int x, BOOL is_vertival)
{
    static int x_poses[5] = {
        HT_SB_LEFTARROW, HT_SB_LEFTSPACE, HT_SB_HTHUMB,
        HT_SB_RIGHTSPACE, HT_SB_RIGHTARROW
    };

    int array[6];
    int x_pos = 0;

    array[0] = left;
    array[1] = left+sb_info->arrowLen;
    array[2] = array[1] + sb_info->barStart;
    array[3] = array[2] + sb_info->barLen;
    array[5] = right;
    array[4] = array[5] - sb_info->arrowLen;

    x_pos = find_interval(array, 6, x);

    if (x_pos == -1)
        return HT_SB_UNKNOWN;
    else
        return x_poses[x_pos]|((is_vertival) ? HT_SB_VMASK : 0);
}

static int pad_hit_test(HWND hWnd, int x, int y)
{
    static const int ht_inner_border[4] = {
        HT_CAPTION, HT_MENUBAR, HT_VSCROLL, HT_HSCROLL
    };

    static const int ht_on_border[3][3] = {
        {HT_CORNER_TL,   HT_BORDER_TOP,    HT_CORNER_TR},
        {HT_BORDER_LEFT, HT_UNKNOWN,       HT_BORDER_RIGHT},
        {HT_CORNER_BL,   HT_BORDER_BOTTOM, HT_CORNER_BR},
    };

    RECT rc;
    int border = 0;
    int x_pos = 0;
    int y_pos = 0;
    int array[5];
    int tmp = 0;
    int win_w = 0;
    int win_h = 0;

    const WINDOWINFO* win_info = GetWindowInfo(hWnd);
    win_w = win_info->right - win_info->left;
    win_h = win_info->bottom - win_info->top;

    /*mouse not in this window*/
    CHECK_RET_VAL(!PtInRect((RECT*)win_info, x, y), HT_OUT);

    /*mouse in client area*/
    CHECK_RET_VAL(PtInRect((RECT*)&(win_info->cl), x, y), HT_CLIENT);

    /*change x y to hwnd coordiante*/
    x -= win_info->left;
    y -= win_info->top;

    border = get_window_border(hWnd, 0, 0);
    SetRect(&rc, border, border, win_w-border, win_h-border);

    /*mouse on border*/
    if (!PtInRect(&rc, x, y)) {
        if (!(win_info->dwStyle & WS_CAPTION) || (win_info->dwStyle & WS_DLGFRAME))
            return HT_UNKNOWN;

        tmp = GetWindowElementAttr(hWnd, WE_METRICS_CAPTION);
        array[0] = 0;
        array[1] = array[0] + tmp;
        array[3] = win_w;
        array[2] = array[3] - tmp;
        x_pos = find_interval(array, 4, x);

        array[0] = 0;
        array[1] = array[0] + tmp;
        array[3] = win_h;
        array[2] = array[3] - tmp;
        y_pos = find_interval(array, 4, y);

        if (x_pos!=-1 && y_pos!=-1)
            return ht_on_border[y_pos][x_pos];
        else
            return HT_UNKNOWN;
    } else {    /*mouse inner border*/
        array[2] = win_info->ct - win_info->top;
        array[1] = array[2];
        array[0] = array[1];
        array[3] = win_info->cb - win_info->top;
        array[4] = array[3] + get_window_scrollbar(hWnd, FALSE);
        y_pos = find_interval(array, 5, y);
        if (y_pos != -1) {
            switch (ht_inner_border[y_pos]) {
                case HT_CAPTION:
                    return test_caption(hWnd, x, y);

                case HT_MENUBAR:
                    return HT_MENUBAR;

                case HT_HSCROLL:
                    return test_scroll(&(win_info->hscroll),
                           win_info->cl - win_info->left,
                           win_info->cr - win_info->left,
                           x, FALSE);

                case HT_VSCROLL:
                    return test_scroll(&(win_info->vscroll),
                            win_info->ct - win_info->top,
                            win_info->cb - win_info->top,
                            y, TRUE);
                default:
                    return HT_UNKNOWN;
            }
        } else {
            return HT_UNKNOWN;
        }
    }

}

static void pad_draw_border(HWND hWnd, HDC hdc, BOOL is_active)
{
    RECT rect;
    ECONER con;
    DWORD color = 0;
    DWORD exstyle = 0;
    int border = 0;

    if (GetWindowStyle(hWnd) & WS_BORDER) {
        border = (int)GetWindowElementAttr(hWnd, WE_METRICS_WND_BORDER);
        if (border < 0)
            return;

        if (pad_calc_we_area(hWnd, HT_BORDER, &rect) == -1)
            return;

        color = GetWindowElementAttr(hWnd,
                is_active ? WE_FGC_ACTIVE_WND_BORDER : WE_FGC_INACTIVE_WND_BORDER);

        exstyle = GetWindowExStyle(hWnd);

        if ((exstyle & WS_EX_TROUNDCNS) && (exstyle & WS_EX_BROUNDCNS))
            con = ECT_BOTH;
        else if (exstyle & WS_EX_TROUNDCNS)
            con = ECT_TOPR;
        else if (exstyle & WS_EX_BROUNDCNS)
            con = ECT_BOTTOMR;
        else
            con = ECT_NORMAL;

        DrawVariableRoundRect(hdc, &rect, color, border, con);

        return;
    }

    return old_fashion_draw_border(hWnd, hdc, is_active);
}

static int pad_init(PWERENDERER renderer)
{
    return 0;
}

static int pad_deinit(PWERENDERER renderer)
{
    return 0;
}

BOOL RegisterPadRDR(void)
{
    if (NULL == (current_rdr = GetWindowRendererFromName(NCS4PAD_RENDERER))) {
        LOGE("%s :: get fashion renderer Error.", __FUNCTION__);
        return FALSE;
    }

    old_fashion_calc_we_area 	= current_rdr->calc_we_area;
    old_fashion_hit_test 		= current_rdr->hit_test;
    old_fashion_draw_border 	= current_rdr->draw_border;
    old_fashion_calc_thumb_area = current_rdr->calc_thumb_area;
    old_fashion_calc_we_metrics = current_rdr->calc_we_metrics;

    current_rdr->init            = pad_init;
    current_rdr->deinit          = pad_deinit;
    current_rdr->calc_we_area    = pad_calc_we_area;
    current_rdr->hit_test        = pad_hit_test;
    current_rdr->draw_border     = pad_draw_border;
    current_rdr->calc_thumb_area = pad_calc_thumb_area;
    current_rdr->calc_we_metrics = pad_calc_we_metrics;

    return TRUE;
}

