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
#include <mgeff/mgeff.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "pieces/mnsdrawpiece.h"
#include "mitembar.h"

// #define NDEBUG   1
#include "mtouchdebug.h"

#define _TEXT_LEN   128

enum mItemBarLayout {
    ITB_DTICK_PIECE_ID  = 0,
    ITB_IMG_PIECE_ID    = 1,
    // 2 is the text piece (NULL)
    ITB_DANGLE_PIECE_ID = 3,
    ITB_PIECE_MAX_ID,
};

#define SET_CELL_INFO_FIXED(header, id, size) \
    _M(header, setCellInfo, id, size, NCS_LAYOUTPIECE_ST_FIXED, TRUE)

#define getItemBarPiece(self, id) \
    _M((mHBoxLayoutPiece*)(self->body), getCell, id)


static void getLayoutRect(mItemBar* self, PRECT prc)
{
    mHBoxLayoutPiece* p = (mHBoxLayoutPiece*)self->body;
    int m = _M(p, getProperty, NCSP_BOXLAYOUTPIECE_MARGIN);

    _M(p, getRect, prc);

    prc->left   += m; 
    prc->top    += m; 
    prc->right  -= m; 
    prc->bottom -= m; 
}


static void resetThisLayout(mItemBar* self, mHBoxLayoutPiece* hheader)
{
    RECT rect;
    DWORD dwStyle = 0;
    int size = 0;
    PLOGFONT font = NULL;

    dwStyle = GetWindowStyle(self->hwnd);

    if (hheader == NULL)
        hheader = (mHBoxLayoutPiece*)(self->body);

    getLayoutRect(self, &rect);

    font = GetWindowFont(self->hwnd);

    if (dwStyle & NCSS_ITEMBAR_CHECKABLE) {
        self->tickSize = font->size * 2;
    } else {
        self->tickSize = 1;
    }

    if (dwStyle & NCSS_ITEMBAR_HASCHILD) {
        self->angleSize = font->size * 2;
    } else {
        self->angleSize = 1;
    }

    size = self->tickSize + self->angleSize + self->imageSize;
    self->ltextSize = ((RECTW(rect) - size) >> 1) - 4;
    self->rtextSize = ((RECTW(rect) - size) >> 1) - 4;

    SET_CELL_INFO_FIXED(hheader, ITB_DTICK_PIECE_ID,  self->tickSize );
    SET_CELL_INFO_FIXED(hheader, ITB_IMG_PIECE_ID,    self->imageSize);
    SET_CELL_INFO_FIXED(hheader, ITB_DANGLE_PIECE_ID, self->angleSize);
}


static int parseString(const char* strSrc, 
        char* str1, char* str2, int str1Len, int str2Len)
{
    char* p1 = NULL;
    char* p2 = NULL;

    if (NULL == strSrc)
        return -1;

    p1 = (char*)strSrc;
    p2 = strchr(strSrc, '\n');

    if (p2 == NULL) {
        // there is no '\n', so the second section is NULL.
        snprintf(str1, str1Len, "%s", strSrc);
        memset(str2, 0, str2Len);
        return 0;
    }

    p2++;
    str1Len = MIN(str1Len, (p2 - p1));
    str2Len = MIN(str2Len, (strlen(p2) + 1));

    snprintf(str1, str1Len, "%s", p1);
    snprintf(str2, str2Len, "%s", p2);

    return 0;
}


static void mItemBar_construct(mItemBar* self, DWORD param)
{
    self->dotHit    = FALSE;
    self->tickSize  = 1;
    self->imageSize = 1;
    self->ltextSize = 1;
    self->rtextSize = 1;
    self->angleSize = 1;

    Class(mWidget).construct((mWidget*)self, param);
}


static mObject* mItemBar_createBody(mItemBar* self)
{
    mHBoxLayoutPiece* hheader = NEWPIECEEX(mHBoxLayoutPiece, ITB_PIECE_MAX_ID);

    _M(hheader, setCell, ITB_DTICK_PIECE_ID,  (mHotPiece*)NEWPIECE(mNSDrawPiece));
    _M(hheader, setCell, ITB_IMG_PIECE_ID,    (mHotPiece*)NEWPIECE(mImagePiece));
    _M(hheader, setCell, ITB_DANGLE_PIECE_ID, (mHotPiece*)NEWPIECE(mNSDrawPiece));

    return (mObject*)hheader;
}


static BOOL mItemBar_onCreate(mItemBar* self, DWORD addData)
{
    Class(mWidget).onCreate((mWidget*)self, addData);

    if (GetWindowStyle(self->hwnd) & NCSS_ITEMBAR_HASCHILD) {
        _M((mNSDrawPiece*)getItemBarPiece(self, ITB_DANGLE_PIECE_ID), 
                setProperty, NCSP_NSDRAW_PIECE_ANGLE, NSDRAW_SHAPE_ANGLE);
    }

    _M(self, setProperty, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

    return TRUE;
}


static void getTextRect(mItemBar* self, PRECT l, PRECT r)
{
    RECT rc;

    getLayoutRect(self, &rc);

    l->top    = r->top    = rc.top;
    l->bottom = r->bottom = rc.bottom;
    l->left   = rc.left + self->tickSize + self->imageSize;
    l->right  = l->left + self->ltextSize;
    r->left   = l->right + 1;
    r->right  = rc.right - self->angleSize;
}


static BOOL DrawItemBk(mItemBar* self, HDC hdc)
{
    RECT rc;
    DWORD color = 0;
    DWORD exstyle = 0;
    ECONER con = ECT_NORMAL;
    int radius = 5;

    radius = ncsGetElement((mWidget*)self, NCS4TOUCH_RRECT_RADIUS);
    color = ncsGetElement((mWidget*)self, 
            self->dotHit ? NCS4TOUCH_BGC_ITB_LIGHT : NCS4TOUCH_BGC_ITB);
    exstyle = GetWindowExStyle(self->hwnd); 

    if ((exstyle & WS_EX_TROUNDCNS) && (exstyle & WS_EX_BROUNDCNS))
        con = ECT_BOTH;
    else if (exstyle & WS_EX_TROUNDCNS)
        con = ECT_TOPR;
    else if (exstyle & WS_EX_BROUNDCNS)
        con = ECT_BOTTOMR;
    else
        con = ECT_NORMAL;

    GetClientRect(self->hwnd, &rc);
    DrawVariableRoundRect(hdc, &rc, color, radius, con);

    return TRUE;
}


static void mItemBar_onPaint(mItemBar* self, HDC hdc, const PCLIPRGN pClip)
{
    RECT ltext_rc, rtext_rc;
    DWORD lcolor = 0, rcolor = 0;
    char str1[_TEXT_LEN], str2[_TEXT_LEN];

    DrawItemBk(self, hdc);

    Class(mWidget).onPaint((mWidget*)self, hdc, pClip);

    lcolor = ncsGetElement((mWidget*)self, 
            self->dotHit ? NCS4TOUCH_FGC_ITB_LTEXT_LIGHT : NCS4TOUCH_FGC_ITB_LTEXT);
    rcolor = ncsGetElement((mWidget*)self,
            self->dotHit ? NCS4TOUCH_FGC_ITB_RTEXT_LIGHT : NCS4TOUCH_FGC_ITB_RTEXT);

    parseString(GetWindowCaption(self->hwnd), str1, str2, _TEXT_LEN, _TEXT_LEN);

    getTextRect(self, &ltext_rc, &rtext_rc);

    SetBkMode(hdc, BM_TRANSPARENT);
    SetTextColor(hdc, ncsColor2Pixel(hdc, lcolor));
    DrawText(hdc, str1, -1, &ltext_rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
    SetTextColor(hdc, ncsColor2Pixel(hdc, rcolor));
    DrawText(hdc, str2, -1, &rtext_rc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
}


static LRESULT mItemBar_wndProc(mItemBar* self, UINT msg, WPARAM w, LPARAM l)
{
    DWORD dwStyle;
    mNSDrawPiece* drawTick  = NULL;
    mNSDrawPiece* drawAngle = NULL;

    drawTick = (mNSDrawPiece*)getItemBarPiece(self, ITB_DTICK_PIECE_ID);
    drawAngle = (mNSDrawPiece*)getItemBarPiece(self, ITB_DANGLE_PIECE_ID);

    switch (msg) {
        case MSG_LBUTTONDOWN:
            SetCapture(self->hwnd);
            self->dotHit = TRUE;
            if (NULL != drawTick) {
                _M(drawTick, setProperty, NCSP_NSDRAW_PIECE_DOTHIT, TRUE);
            }
            if (NULL != drawAngle) {
                _M(drawAngle, setProperty, NCSP_NSDRAW_PIECE_DOTHIT, TRUE);
            }
            InvalidateRect(self->hwnd, NULL, TRUE);
            return 0;

        case MSG_LBUTTONUP:
            if (GetCapture() == self->hwnd) {
                self->dotHit = FALSE;
                if (NULL != drawAngle) {
                    _M(drawAngle, setProperty, NCSP_NSDRAW_PIECE_DOTHIT, FALSE);
                }
                if (NULL != drawTick) {
                    _M(drawTick, setProperty, NCSP_NSDRAW_PIECE_DOTHIT, FALSE);
                    dwStyle = GetWindowStyle(self->hwnd);
                    if (dwStyle & NCSS_ITEMBAR_CHECKABLE) {
                        if (NSDRAW_SHAPE_TICK == _M(drawTick, 
                                    getProperty, NCSP_NSDRAW_PIECE_TICK)) {
                            _M(drawTick, setProperty, NCSP_NSDRAW_PIECE_TICK, -1);
                        } else {
                            ncsNotifyParent ((mWidget *)self, NCSN_ITEMBAR_CHECKED);
                            _M(drawTick, setProperty,
                                    NCSP_NSDRAW_PIECE_TICK, NSDRAW_SHAPE_TICK);
                        }
                    }
                }
                ncsNotifyParent ((mWidget *)self, NCSN_WIDGET_CLICKED);
                InvalidateRect(self->hwnd, NULL, TRUE);
                ReleaseCapture();
            }
            return 0;

        case MSG_SETTEXT:
            SetWindowCaption(self->hwnd, (const char*)l);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return 0;
    }

    return Class(mWidget).wndProc((mWidget*)self, msg, w, l);
}


static BOOL mItemBar_setProperty(mItemBar* self, int id, DWORD value)
{
    if (id > NCSP_ITEMBAR_MAX)
        return FALSE;

    switch (id) {
        case NCSP_WIDGET_RDR:
            if (strcmp((char*)value, NCS4TOUCH_RENDERER) == 0)
                break;
            else 
                return FALSE;

        case NCSP_ITEMBAR_CHECKED:
            _M((mNSDrawPiece*)getItemBarPiece(self, ITB_DTICK_PIECE_ID), 
                    setProperty, NCSP_NSDRAW_PIECE_TICK, 
                    value == 0 ? -1 : NSDRAW_SHAPE_TICK);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_ITEMBAR_IMG: {
            RECT rect;
            mImagePiece* imagePiece = NULL;
            getLayoutRect(self, &rect);
            self->imageSize = (value == 0 ? 1 : RECTH(rect));
            imagePiece = (mImagePiece*)getItemBarPiece(self, ITB_IMG_PIECE_ID);
            _M(imagePiece, setProperty, NCSP_IMAGEPIECE_DRAWMODE , NCS_DM_NORMAL);
            _M(imagePiece, setProperty, NCSP_IMAGEPIECE_IMAGE, value);
            resetThisLayout(self, NULL);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;
        }
        case NCSP_ITEMBAR_MARGIN:
            _M((mHBoxLayoutPiece *)self->body, 
                    setProperty, NCSP_BOXLAYOUTPIECE_MARGIN, value);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        default:
            break;
    }

    return Class(mWidget).setProperty((mWidget*)self, id, value);
}


static DWORD mItemBar_getProperty(mItemBar* self, int id)
{
    if (id > NCSP_ITEMBAR_MAX)
        return (DWORD)-1;

    switch (id) {
        case NCSP_ITEMBAR_CHECKED: {
            mNSDrawPiece* drawTick  = NULL;
            drawTick = (mNSDrawPiece*)getItemBarPiece(self, ITB_DTICK_PIECE_ID);
            return _M(drawTick, getProperty, NCSP_NSDRAW_PIECE_TICK);
        }
        case NCSP_ITEMBAR_IMG: {
            mImagePiece* imagePiece = NULL;
            imagePiece = (mImagePiece*)getItemBarPiece(self, ITB_IMG_PIECE_ID);
            return _M(imagePiece, getProperty, NCSP_IMAGEPIECE_IMAGE);
        }
        case NCSP_ITEMBAR_MARGIN:
            return _M((mHBoxLayoutPiece *)self->body, 
                    getProperty, NCSP_BOXLAYOUTPIECE_MARGIN);

        default:
            break;
    }

    return Class(mWidget).getProperty((mWidget*)self, id);
}


static int mItemBar_onSizeChanged(mItemBar* self, RECT* rtClient)
{
    Class(mWidget).onSizeChanged ((mWidget*)self, rtClient);
    resetThisLayout (self, NULL);

    return 0;
}


BEGIN_CMPT_CLASS(mItemBar, mWidget)
    CLASS_METHOD_MAP(mItemBar, construct)
    CLASS_METHOD_MAP(mItemBar, createBody)
    CLASS_METHOD_MAP(mItemBar, onCreate)
    CLASS_METHOD_MAP(mItemBar, onSizeChanged)
    CLASS_METHOD_MAP(mItemBar, onPaint)
    CLASS_METHOD_MAP(mItemBar, wndProc)
    CLASS_METHOD_MAP(mItemBar, setProperty)
    CLASS_METHOD_MAP(mItemBar, getProperty)
END_CMPT_CLASS

