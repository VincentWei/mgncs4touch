/*
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "mbtnnavbar.h"
#include "pieces/mshapepushbuttonpiece.h"
#include "pieces/mshapeboxpiece.h"
#include "pieces/msimagepiece.h"

#define NDEBUG    1
#include "mtouchdebug.h"


#define BNB_TEXT_LEN        128


static BOOL _onLeftButtonClick(mBtnNavBar* self, mHotPiece* sender,
        int event_id, DWORD param)
{
    if (event_id == NCSN_ABP_CLICKED) {
        ncsNotifyParentEx((mWidget*)self, NCSN_BNB_BTNCLICK,
                (DWORD)NCSD_BNB_LEFT);
    }

    return FALSE;
}


static BOOL _onRightButtonClick(mBtnNavBar* self, mHotPiece* sender,
        int event_id, DWORD param)
{
    if (event_id == NCSN_ABP_CLICKED) {
        ncsNotifyParentEx((mWidget*)self, NCSN_BNB_BTNCLICK,
                (DWORD)NCSD_BNB_RIGHT);
    }

    return FALSE;
}


static mHotPiece* create_left_piece(mBtnNavBar* self)
{
    mShapePushButtonPiece* shapePiece = NULL;
    int event_ids[]= { NCSN_ABP_CLICKED, 0 };

    DWORD dwStyle = GetWindowStyle(self->hwnd);

    if (dwStyle & NCSS_BNB_LRECT) {
        shapePiece->shape = SHAPE_RECT;
    } else if (dwStyle & NCSS_BNB_LOPT) {
        shapePiece = NEWPIECE(mShapePushButtonPiece);
        shapePiece->shape = SHAPE_LEFT;
    }

    if (NULL != shapePiece) {
        ncsAddEventListeners((mObject*)shapePiece, (mObject*)self,
                (NCS_CB_ONPIECEEVENT)_onLeftButtonClick, event_ids);
    }

    return (mHotPiece*)shapePiece;
}


static mHotPiece* create_right_piece(mBtnNavBar* self)
{
    mShapePushButtonPiece* shapePiece = NULL;
    int event_ids[]= { NCSN_ABP_CLICKED, 0 };

    DWORD dwStyle = GetWindowStyle(self->hwnd);

    if (dwStyle & NCSS_BNB_RRECT) {
        shapePiece = NEWPIECE(mShapePushButtonPiece);
        shapePiece->shape = SHAPE_RECT;
    } else if (dwStyle & NCSS_BNB_ROPT) {
        shapePiece = NEWPIECE(mShapePushButtonPiece);
        shapePiece->shape = SHAPE_RIGHT;
    }

    if (NULL != shapePiece) {
        ncsAddEventListeners((mObject*)shapePiece, (mObject*)self,
                (NCS_CB_ONPIECEEVENT)_onRightButtonClick, event_ids);
    }

    return (mHotPiece*)shapePiece;
}


static void mBtnNavBar_construct(mBtnNavBar* self, DWORD param)
{
    Class(mWidget).construct((mWidget*)self, param);

    self->margin_w  = 3;
    self->button_h  = 0;
    self->leftbt_w  = 0;
    self->rightbt_w = 0;
}


static BOOL mBtnNavBar_onCreate(mBtnNavBar* self, DWORD add)
{
    RECT rc;
    int btnW = 0;
    mHBoxLayoutPiece* hboxpiece = (mHBoxLayoutPiece*)self->body;

    GetClientRect(self->hwnd, &rc);

    if (self->leftbt_w == 0 && self->rightbt_w == 0) {
        btnW = ((RECTW(rc)) - (self->margin_w << 1)) / 3;
        self->leftbt_w = self->rightbt_w = btnW > 60 ? 60 : btnW;
    }

    _M(hboxpiece, setCellInfo, BNB_LEFT_PIECE_ID, self->leftbt_w,
            NCS_LAYOUTPIECE_ST_FIXED, FALSE);
    _M(hboxpiece, setCellInfo, BNB_RIGHT_PIECE_ID, self->rightbt_w,
            NCS_LAYOUTPIECE_ST_FIXED, FALSE);
    _M(hboxpiece, setProperty, NCSP_BOXLAYOUTPIECE_MARGIN,
            self->button_h == 0 ? 0 : ((RECTH(rc) - self->button_h) >> 1));

    _M(self, setProperty, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

    return TRUE;
}


static mObject* mBtnNavBar_createBody(mBtnNavBar* self)
{
    mHBoxLayoutPiece* hboxpiece = NEWPIECEEX(mHBoxLayoutPiece, BNB_MAX_PIECE_ID);

    _M(hboxpiece, setCell, BNB_LEFT_PIECE_ID,  create_left_piece(self));
    _M(hboxpiece, setCell, BNB_RIGHT_PIECE_ID, create_right_piece(self));

    return (mObject*)hboxpiece;
}


static BOOL get_text_rect(RECT* src, RECT* dst, int margin_w, int lbutton_w, int rbutton_w)
{
    if (src == NULL || dst == NULL)
        return FALSE;

    memcpy(dst, src, sizeof(RECT));

    if (lbutton_w > rbutton_w) {
        dst->left  += margin_w + lbutton_w + 2;
        dst->right -= margin_w + lbutton_w - 2;
    } else if (lbutton_w <= rbutton_w) {
        dst->left  += margin_w + rbutton_w + 2;
        dst->right -= margin_w + rbutton_w - 2;
    }

    return TRUE;
}


static void resetBodyRect(mBtnNavBar* self, RECT* rcClient)
{
    if (self->body) {
        RECT rc = {0 + self->margin_w, 0, RECTWP(rcClient) - self->margin_w, RECTHP(rcClient)};
        /*
        if (self->button_h > 0 && self->button_h + (self->margin_w << 1) < RECTH(rc)){
            rc.top += (RECTH(rc) - self->button_h - (self->margin_w << 1)) >> 1;
            rc.bottom = rc.top + self->button_h + (self->margin_w << 1);
        }
        */
        _M((mHotPiece*)(self->body), setRect, &rc);
    }
}


static int mBtnNavBar_onSizeChanged(mBtnNavBar* self, RECT* rtClient)
{
    resetBodyRect(self, rtClient);

    return 0;
}


static void mBtnNavBar_onPaint(mBtnNavBar* self, HDC hdc, const PCLIPRGN pinv_clip)
{
    RECT text_rc, rc;
    char text[BNB_TEXT_LEN] = { 0 };
    int oldMode = 0;
    gal_pixel oldTxtColor = 0;

    GetClientRect(self->hwnd, &rc);
    GetWindowText(self->hwnd, text, BNB_TEXT_LEN);
    get_text_rect(&rc, &text_rc, self->margin_w, self->leftbt_w, self->rightbt_w );

    SelectFont(hdc, self->caption_font);
    oldTxtColor = SetTextColor(hdc,
            ncsColor2Pixel(hdc, ncsGetElement(self, NCS4TOUCH_FGC_BNB_TEXT)));
    oldMode = SetBkMode(hdc, BM_TRANSPARENT);
    DrawText(hdc, text, -1, &text_rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    SetBkMode(hdc, oldMode);
    SetTextColor(hdc, oldTxtColor);

    Class(mWidget).onPaint((mWidget*)self, hdc, pinv_clip);
}


static BOOL mBtnNavBar_setProperty(mBtnNavBar* self, int id, DWORD value)
{
    DWORD dwStyle = GetWindowStyle(self->hwnd);

    if (id >= NCSP_BNB_MAX)
        return FALSE;

    switch (id) {
        case NCSP_WIDGET_RDR:
            if (strcmp((char*)value, NCS4TOUCH_RENDERER) == 0)
                break;
            else
                return FALSE;

        case NCSP_BNB_MARGIN:
            if (self->margin_w != value) {
                RECT rc;
                self->margin_w = value;

                GetClientRect (self->hwnd, &rc);
                rc.left += self->margin_w;
                rc.right -= self->margin_w;
                _M((mHotPiece*)(self->body), setRect, &rc);

                InvalidateRect(self->hwnd, NULL, TRUE);
            }
            return TRUE;

        case NCSP_BNB_FONT:
            if (self->caption_font != (PLOGFONT)value) {
                self->caption_font = (PLOGFONT)value;
                InvalidateRect(self->hwnd, NULL, TRUE);
                return TRUE;
            }
            return FALSE;

        case NCSP_BNB_LEFT_W:
            if (self->leftbt_w != value) {
                self->leftbt_w = value;
                _M((mHBoxLayoutPiece*)self->body, setCellInfo, BNB_LEFT_PIECE_ID,
                        self->leftbt_w , NCS_LAYOUTPIECE_ST_FIXED, TRUE);
            }
            return TRUE;

        case NCSP_BNB_RIGHT_W:
            if (self->rightbt_w != value) {
                self->rightbt_w = value;
                _M((mHBoxLayoutPiece*)self->body, setCellInfo, BNB_RIGHT_PIECE_ID,
                        self->rightbt_w , NCS_LAYOUTPIECE_ST_FIXED, TRUE);
            }
            return TRUE;

        case NCSP_BNB_LSTRING: {
            mShapePushButtonPiece* spbpiece = NULL;

            if (((dwStyle & NCSS_BNB_LRECT) || (dwStyle & NCSS_BNB_LOPT))
                    && (NULL != (spbpiece = (mShapePushButtonPiece*)GetLeftBtnPiece(self)))) {
                _M(spbpiece, setProperty, NCSP_LABELPIECE_LABEL, value);
                InvalidateRect(self->hwnd, NULL, TRUE);
            }
            return TRUE;
        }
        case NCSP_BNB_RSTRING: {
            mShapePushButtonPiece* spbpiece = NULL;

            if (((dwStyle & NCSS_BNB_RRECT) || (dwStyle & NCSS_BNB_ROPT))
                    && (NULL != (spbpiece = (mShapePushButtonPiece*)GetRightBtnPiece(self)))) {
                _M(spbpiece, setProperty, NCSP_LABELPIECE_LABEL, value);
                InvalidateRect(self->hwnd, NULL, TRUE);
            }
            return TRUE;
        }
        case NCSP_BNB_BTN_H: {
            RECT rc;
            GetWindowRect(self->hwnd, &rc);

            if (value < 0 || value > RECTH(rc))
             return FALSE;

            self->button_h = value;
            _M((mHBoxLayoutPiece*)self->body, setProperty, NCSP_BOXLAYOUTPIECE_MARGIN,
                    self->button_h == 0 ? 0 : ((RECTH(rc) - self->button_h) >> 1));
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;
        }
        default:
            break;
    }

    return Class(mWidget).setProperty((mWidget*)self, id, value);
}


static DWORD mBtnNavBar_getProperty(mBtnNavBar* self, int id)
{
    if (id >= NCSP_BNB_MAX)
        return 0;

    switch (id) {
        case NCSP_BNB_MARGIN:
            return (DWORD)self->margin_w;

        case NCSP_BNB_FONT:
            return (DWORD)self->caption_font;

        case NCSP_BNB_LEFT_W:
            return (DWORD)self->leftbt_w;

        case NCSP_BNB_RIGHT_W:
            return (DWORD)self->rightbt_w;

        case NCSP_BNB_LSTRING:
            return (DWORD)self->left_str;

        case NCSP_BNB_RSTRING:
            return (DWORD)self->right_str;

        case NCSP_BNB_BTN_H:
            return self->button_h;

        default:
            break;
    }

    return Class(mWidget).getProperty((mWidget*)self, id);
}


static void mBtnNavBar_destroy(mBtnNavBar* self)
{
    if (self->left_str) {
        free(self->left_str);
        self->left_str = NULL;
    }

    if (self->right_str) {
        free(self->right_str);
        self->right_str = NULL;
    }

    Class(mWidget).destroy((mWidget*)self);
}


static BOOL mBtnNavBar_onEraseBkgnd(mBtnNavBar* self, HDC hdc, const RECT* pinv)
{
    RECT rc;
    int line = 0, gh = 0;
    int r0 = 0, g0 = 0, b0 = 0;
    int r1 = 0, g1 = 0, b1 = 0;
    int r  = 0, g  = 0, b  = 0;
    DWORD endColor;
    DWORD beginColor;

    if (self->bkimg.img.pbmp != NULL) {
        return Class(mWidget).onEraseBkgnd((mWidget*)self, hdc, pinv);
    }

    endColor   = Pixel2DWORD(hdc, GetWindowBkColor(self->hwnd));
    beginColor = ncsCommRDRCalc3dboxColor(endColor, NCSR_COLOR_LIGHTEST);

    GetClientRect(self->hwnd, &rc);

    r0 = GetRValue(beginColor);
    g0 = GetGValue(beginColor);
    b0 = GetBValue(beginColor);
    r1 = GetRValue(endColor);
    g1 = GetGValue(endColor);
    b1 = GetBValue(endColor);

    gh = RECTHP(&rc) / 2;

    for (line = 0; line < gh; line++) {
        r = r0 + line * (r1 - r0) / gh;
        g = g0 + line * (g1 - g0) / gh;
        b = b0 + line * (b1 - b0) / gh;
        SetPenColor(hdc, RGB2Pixel(hdc, r, g, b));
        LineEx(hdc, rc.left, rc.top + line, rc.right, rc.top + line);
    }

    SetBrushColor(hdc, RGB2Pixel(hdc, r1, g1, b1));
    FillBox(hdc, rc.left, rc.top + gh, RECTWP(&rc), RECTHP(&rc)-gh);

    SetPenColor(hdc, RGB2Pixel(hdc, r0, g0, b0));
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

    return TRUE;
}


static LRESULT mBtnNavBar_wndProc(mBtnNavBar* self,  UINT msg, WPARAM w, LPARAM l)
{
    if (msg == MSG_SETTEXT) {
        SetWindowCaption(self->hwnd, (const char*)l);
        InvalidateRect(self->hwnd, NULL, TRUE);
        return strlen((const char*)l);
    }

    return Class(mWidget).wndProc((mWidget*)self, msg, w, l);
}


BEGIN_CMPT_CLASS(mBtnNavBar, mWidget)
    CLASS_METHOD_MAP(mBtnNavBar, construct)
    CLASS_METHOD_MAP(mBtnNavBar, createBody)
    CLASS_METHOD_MAP(mBtnNavBar, onCreate)
    CLASS_METHOD_MAP(mBtnNavBar, onSizeChanged)
    CLASS_METHOD_MAP(mBtnNavBar, onPaint)
    CLASS_METHOD_MAP(mBtnNavBar, wndProc)
    CLASS_METHOD_MAP(mBtnNavBar, destroy)
    CLASS_METHOD_MAP(mBtnNavBar, setProperty)
    CLASS_METHOD_MAP(mBtnNavBar, getProperty)
    CLASS_METHOD_MAP(mBtnNavBar, onEraseBkgnd)
END_CMPT_CLASS

