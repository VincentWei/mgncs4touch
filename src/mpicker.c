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

#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "manimation.h"
#include "mpicker.h"

#define NDEBUG  1
#include "mtouchdebug.h"

#define PCK_EPSINON			0.01
#define USEPARENTRDR(self)	(GetWindowExStyle(self->hwnd) & WS_EX_USEPARENTRDR)
#define GETELEMENT(self,id)	ncsGetElement((USEPARENTRDR(self) ? ncsGetParentObj(self->hwnd) : (mWidget *)self), id)


static int g_finalFrame = 0;

static const char *getNextString(const char *srcStr, char *retStr, int retLen)
{
    const char *p = NULL;

    if (srcStr == NULL || strlen(srcStr) <= 0)
        return NULL;

    p = strchr(srcStr, '\n');

    if (p == NULL) {
        // there is no '\n',
        snprintf(retStr, retLen, "%s", srcStr);
        return NULL;
    }

    snprintf(retStr, MIN (retLen, (p - srcStr + 1)), "%s", srcStr);

    while (*p == '\n')
        p++;

    return p;
}


static void resetItemsByStrings(mPicker* self)
{
    const char *caption = NULL;
    char tmp[256] = {0};

    if (!(GetWindowStyle(self->hwnd) & NCSS_PCK_STRINGS))
        return;

    if (NULL == (caption = GetWindowCaption(self->hwnd)))
        return;

    _c(self)->freeze(self, TRUE);
    _c(self)->removeAll(self);

    do {
        memset(tmp, 0, sizeof(tmp));
        caption = getNextString(caption, tmp, sizeof(tmp));
        if (strlen(tmp) > 0)
            _M(self, addItem, -1, tmp);
    } while (NULL != caption);

    _c(self)->freeze(self, FALSE);
}


static void resetItemsByDigits(mPicker* self)
{
    int val = 0;
    char bitsF[8];
    char tmp[16] = {0};

    if (!(GetWindowStyle(self->hwnd) & NCSS_PCK_NUMBER))
        return;

    _c(self)->freeze(self, TRUE);
    _c(self)->removeAll(self);

    for (val = self->minValue; val <= self->maxValue; val += self->span) {
        if (self->numBits > 0) {
            bitsF[0] = '%';
            bitsF[1] = '.';
            snprintf(bitsF + 2, sizeof(bitsF) - 2, "%dd", self->numBits);
            snprintf(tmp, sizeof(tmp), bitsF, val);
        } else {
            snprintf(tmp, sizeof(tmp), "%d", val);
        }
        _M(self, createItem, 0, 0, -1, -1, tmp + self->showBits, 0, NULL, FALSE);
    }

    _c(self)->freeze(self, FALSE);
}


static void _drawItem(mPicker* self, HITEM hItem, HDC hdc, RECT* pRc)
{
    DWORD c;
    const char* text = _c(self)->getText(self, hItem);

    if (!_M(self, isEnabled, hItem)) {
        c = GETELEMENT(self, NCS4TOUCH_FGC_PCK_DISABLE);
    } else {
        c = GETELEMENT(self, NCS4TOUCH_FGC_PCK_PICKER);
    }

    SetBkMode(hdc, BM_TRANSPARENT);
    SetTextColor(hdc, ncsColor2Pixel(hdc, c));
    DrawText(hdc, text, -1, pRc, self->itemAlign | DT_VCENTER | DT_SINGLELINE);
}


static void prepareBkDC(mPicker* self)
{
    RECT rc, rcP;
    HDC hdc;
	DWORD style;
    DWORD bc = 0, mc = 0, c;
	int corner = ECT_NORMAL;

    if (self->bkDC != HDC_INVALID)
        DeleteCompatibleDC(self->bkDC);

    GetClientRect(self->hwnd, &rc);

    hdc = GetClientDC(self->hwnd);
    self->bkDC = CreateCompatibleDCEx(hdc, RECTW(rc), RECTH(rc));
    ReleaseDC(hdc);

    rcP.left   = rc.left   + self->vMargin;
    rcP.right  = rc.right  - self->vMargin;
    rcP.top    = rc.top    + self->hMargin;
    rcP.bottom = rc.bottom - self->hMargin;

    c = GETELEMENT(self, NCS4TOUCH_BGC_PCK_MAIN);
    DrawRectRing(self->bkDC, &rc, &rcP, c);

	style = GetWindowStyle(self->hwnd);
	if (style & NCSS_PCK_BRC) {
		corner = ECT_BOTH;
	} else if (style & NCSS_PCK_LRC) {
		corner = ECT_LEFTR;
	} else if (style & NCSS_PCK_RRC) {
		corner = ECT_RIGHTR;
	}
    mc = GETELEMENT(self, NCS4TOUCH_BGC_PCK_PICKER);
    bc = ncsCommRDRCalc3dboxColor(mc, NCSR_COLOR_DARKER);
    DrawPickerRect(self->bkDC, &rcP, bc, mc, corner);
}


static void mPicker_construct(mPicker* self, DWORD param)
{
    DWORD dwStyle = 0;

    Class(mAnimation).construct((mAnimation*)self, param);

    self->key         = 0.0f;
    self->itemVisible = 5;
    self->itemAlign   = DT_CENTER;
    self->maxValue    = 9;
    self->minValue    = 0;
    self->span        = 1;
    self->selIdx      = 0;
    self->hMargin     = 0;
    self->vMargin     = 0;
    self->numBits     = 0;
    self->showBits    = 0;
	self->bkDC 		  = HDC_INVALID;
    self->handle     = 0 ;
    dwStyle = GetWindowStyle(self->hwnd);

    if (dwStyle & NCSS_PCK_NUMBER) {
        resetItemsByDigits(self);
    } else if (dwStyle & NCSS_PCK_STRINGS) {
        resetItemsByStrings(self);
    }

    _c(self)->setItemDraw(self, (NCS_CB_DRAWITEM)_drawItem);
}

static void mPicker_destroy (mPicker *self)
{
    if (self->handle) {
        mGEffAnimationStop(self->handle);
        mGEffAnimationDelete(self->handle);
        self->handle = NULL;
    }
	if (self->bkDC != HDC_INVALID) {
		DeleteCompatibleDC(self->bkDC);
		self->bkDC = HDC_INVALID;
	}
	Class(mAnimation).destroy((mAnimation*)self);
}

static BOOL mPicker_onCreate(mPicker* self, DWORD add)
{
    _M(self, setProperty, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

    return TRUE;
}


static BOOL mPicker_onEraseBkgnd(mPicker* self, HDC hdc, const RECT* pinv)
{
    return TRUE;
}


static HITEM _getItem(mPicker* self, int index)
{
    int count = _c(self)->getItemCount(self);

    if (NCSS_PCK_LOOP & GetWindowStyle(self->hwnd)) {
        while (index < 0)
            index += count;
        while (index >= count)
            index -= count;
    }

    return _c(self)->getItem(self, index);
}


static int drawAlphaCover(HDC hdc, PRECT prc, DWORD color, int alpha)
{
	DWORD fc = ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER);
    DWORD hc = ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_LIGHTER);

    HDC tmpDC = CreateCompatibleDCEx(hdc, RECTWP(prc), RECTHP(prc));

	SetBrushColor(tmpDC, ncsColor2Pixel(tmpDC, hc));
    FillBox(tmpDC, 0, 0, RECTWP(prc), RECTHP(prc)>>1);
    SetBrushColor(tmpDC, ncsColor2Pixel(tmpDC, color));
    FillBox(tmpDC, 0, RECTHP(prc)>>1, RECTWP(prc), RECTHP(prc)>>1);

	SetPenColor(tmpDC, ncsColor2Pixel(tmpDC, fc));
    Rectangle(tmpDC, 0, 0, RECTWP(prc)-1, RECTHP(prc)-1);

	SetMemDCAlpha(tmpDC, MEMDC_FLAG_SRCALPHA, alpha);
    BitBlt(tmpDC, 0, 0, 0, 0, hdc, prc->left, prc->top, -1);

	DeleteCompatibleDC(tmpDC);
    return 0;
}


static void mPicker_onPaint(mPicker* self, HDC hdc, const PCLIPRGN _clip)
{
    HITEM hItem;
    RECT rc, rcDraw, rcSel;
	BOOL isCompDC = FALSE;
    HDC tmpDc = HDC_INVALID;
    int i = 0, w = 0, count = 0, vis = 0, index = 0;
    float cursor = 0.0, move = 0.0;
    DWORD dwStyle = GetWindowStyle(self->hwnd);

    if (0 == (count = _c(self)->getItemCount(self)))
        return;

    GetClientRect(self->hwnd, &rc);
    rc.left   += self->vMargin;
    rc.right  -= self->vMargin;
    rc.top    += self->hMargin;
    rc.bottom -= self->hMargin;

    vis = self->itemVisible + 1;
    cursor = self->key;

    printf ("cursor: %f; count: %d\n", cursor, count);
    if (dwStyle & NCSS_PCK_LOOP) {
        while (cursor >= count)
            cursor -= count;
        while (cursor < 0)
            cursor += count;
    }

    index = cursor;
    move = cursor - index;

    tmpDc = CreateCompatibleDC(hdc);
	if (tmpDc != HDC_INVALID) {
		isCompDC = TRUE;
	} else {
		tmpDc = hdc;
	}

	if (self->bkDC != HDC_INVALID) {
		BitBlt(self->bkDC, 0, 0, 0, 0, tmpDc, 0, 0, 0);
	}
    w = RECTH(rc) / self->itemVisible;

    SelectClipRect(tmpDc, &rc);

    for (i = 0; i < vis; ++i) {
        if (dwStyle & NCSS_PCK_LOOP)
            hItem = _getItem(self, index - vis / 2 + 1 + i);
        else
            hItem = _c(self)->getItem(self, index - vis / 2 + 1 + i);

        rcDraw.left   = rc.left;
        rcDraw.top    = rc.top + w * (i - move);
        rcDraw.right  = rc.right;
        rcDraw.bottom = rcDraw.top + w;

        self->itemOps.drawItem((mItemView *)self, hItem, tmpDc, &rcDraw);
    }

    rcSel.left = rc.left+1;
    rcSel.right = rc.right-1;
    rcSel.top = rc.top + RECTH(rc) / self->itemVisible * (self->itemVisible >> 1);
    rcSel.bottom = rcSel.top + RECTH(rc) / self->itemVisible;
    drawAlphaCover(tmpDc, &rcSel, GETELEMENT(self, NCS4TOUCH_BGC_PCK_SELECT), 112);

	if (isCompDC) {
		BitBlt(tmpDc, 0, 0, 0, 0, hdc, 0, 0, 0);
		DeleteCompatibleDC(tmpDc);
	}

    if (g_finalFrame) {
        _c(self)->endAnimation(self);
        g_finalFrame = 0;
    }
}


static int mPicker_onKeyDown(mPicker* self, int scancode, int state)
{
    //FIXME, this keydown message process is for testing, need to delete later.
    switch (scancode) {
        case SCANCODE_A:
            _c(self)->runAnimation(self, self->selIdx, self->selIdx + 1.0f);
            break;

        case SCANCODE_D:
            _c(self)->runAnimation(self, self->selIdx, self->selIdx - 1.0f);
            break;
    }

    return 1;
}


static LRESULT mPicker_wndProc(mPicker* self, UINT msg, WPARAM w, LPARAM l)
{
    int count, width;
    static unsigned int lbuttondown = 0;

    switch (msg) {
        case MSG_SETTEXT:
            SetWindowCaption(self->hwnd, (const char*)l);
            resetItemsByStrings(self);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return strlen((const char*)l);

        case MSG_LBUTTONDOWN:
            self->mouseY = -1;
            lbuttondown = 1;
            SetCapture(self->hwnd);
            return 0;

        case MSG_MOUSEMOVE:
            width = 5;

			if (self->mouseY == -1) {
				self->mouseY = HISWORD(l);
				return 0;
			}

            if (lbuttondown && self->mouseY != HISWORD(l)) {
                if (self->mouseY - HISWORD(l) > width) {
                    self->mouseY = HISWORD(l);
                    self->key += 0.25f;
                    g_finalFrame = 1;
                    InvalidateRect(self->hwnd, NULL, TRUE);
                } else if (HISWORD(l) - self->mouseY > width) {
                    self->mouseY = HISWORD(l);
                    self->key -= 0.25f;
                    g_finalFrame = 1;
                    InvalidateRect(self->hwnd, NULL, TRUE);
                }
                if (!(GetWindowStyle(self->hwnd) & NCSS_PCK_LOOP)) {
                    count = _c(self)->getItemCount(self);
                    if (self->key >= count + 1.0)
                        self->key = count + 1.0;
                    if (self->key < -2.0)
                        self->key = -2.0;
                }
            }
            return 0;

        case MSG_LBUTTONUP:
            if (self->key - self->selIdx < 0.5) {
                _c(self)->runAnimation(self, self->key, self->selIdx);
            } else {
                _c(self)->runAnimation(self, self->key, self->selIdx + 1.0f);
            }

            lbuttondown = 0;
            ReleaseCapture();
            return 0;

        default:
            break;
    }

    return Class(mWidget).wndProc((mWidget*)self, msg, w, l);
}


static BOOL mPicker_setProperty(mPicker* self, int id, DWORD value)
{
    if (id > NCSP_PCK_MAX)
        return FALSE;

    switch (id) {
        case NCSP_WIDGET_RDR:
            if (strcmp((char*)value, NCS4TOUCH_RENDERER) == 0)
                break;
            else
                return FALSE;

        case NCSP_PCK_ALIGN:
            self->itemAlign = value;
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_PCK_LINES:
            if ((int)value % 2 == 0)
                return FALSE;
            self->itemVisible = (int)value;
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_PCK_COUNT:
            return FALSE;

        case NCSP_PCK_SELIDX:
            self->selIdx = value;
            self->key = (float)(int)value;
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_PCK_HMARGIN:
            self->hMargin = (unsigned int)value;
            prepareBkDC(self);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_PCK_VMARGIN:
            self->vMargin = (unsigned int)value;
            prepareBkDC(self);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_PCK_NUMBERBITS:
            self->numBits = (unsigned int)value;
            resetItemsByDigits(self);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        case NCSP_PCK_SHOWBERBITS:
            self->showBits = (unsigned int)value;
            resetItemsByDigits(self);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        default:
            break;
    }

    return Class(mAnimation).setProperty((mAnimation *)self, id, value);
}


static DWORD mPicker_getProperty(mPicker* self, int id)
{
    if (id > NCSP_PCK_MAX)
        return 0;

    switch (id) {
        case NCSP_PCK_ALIGN:
            return self->itemAlign;

        case NCSP_PCK_LINES:
            return self->itemVisible;

        case NCSP_PCK_COUNT:
            return _M(self, getItemCount);

        case NCSP_PCK_SELIDX:
            return self->selIdx;

        case NCSP_PCK_HMARGIN:
            return self->hMargin;

        case NCSP_PCK_VMARGIN:
            return self->vMargin;

        case NCSP_PCK_NUMBERBITS:
            return self->numBits;

        case NCSP_PCK_SHOWBERBITS:
            return self->showBits;

        default:
            break;
    }

    return Class(mAnimation).getProperty((mAnimation *)self, id);
}


static void mPicker_endAnimation(mPicker* self)
{
    int old = 0;
    int count = _c(self)->getItemCount(self);
    DWORD dwStyle = GetWindowStyle(self->hwnd);

    old = self->selIdx;
    if (dwStyle & NCSS_PCK_LOOP) {
        while (self->key >= count)
            self->key -= count;
        while (self->key < 0)
            self->key += count;
        self->selIdx = self->key;
    } else {
        if (self->key > count - 1) {
            self->selIdx = count - 2;
        } else if (self->key < 0.0) {
            self->selIdx = 0;
        } else {
            self->selIdx = self->key;
        }
    }

    _c(self)->hilight(self, _c(self)->getItem(self, self->selIdx));

    if (old != self->selIdx) {
        ncsNotifyParentEx((mWidget*)self, NCSN_PCK_SELCHANGED, (DWORD)self->selIdx);
        if (self->selIdx == 0)
            ncsNotifyParentEx((mWidget *)self, NCSN_PCK_REACHMIN, 0);
        else if (self->selIdx == count-1)
            ncsNotifyParentEx((mWidget *)self, NCSN_PCK_REACHMAX, 0);
    }
}


static void _finished_cb(MGEFF_ANIMATION animation)
{
    g_finalFrame = 1;
}


static void mPicker_runAnimation(mPicker* self, float startV, float endV)
{
    //static MGEFF_ANIMATION anim = NULL;
    int duration = 500;
    float distance = startV - endV;

    if (self->handle) {
        mGEffAnimationStop(self->handle);
        mGEffAnimationDelete(self->handle);
        self->handle = NULL;
    }

    if ((distance <= PCK_EPSINON) && (distance >= -PCK_EPSINON))
        return;

    if (distance < 0)
        distance =  -distance;

    if (distance >= 1)
        duration /= distance;
    else
        duration *= distance;

    if (duration <= 0) {
        fprintf(stderr, "%s:duration < 0!!\n", __FUNCTION__);
        return;
    }

    {
	    EffPropertyAnimationSetting settings[] = {
		    {
			    &self->key, MGEFF_FLOAT,
			    (void*)&startV, (void*)&endV,
			    InOutCubic, duration, _finished_cb
		    },
		    {NULL, 0, NULL, NULL, 0, 0, NULL}
	    };

	    self->handle = _M(self, createAnimation, settings);
    }
    mGEffAnimationAsyncRun(self->handle);
    mGEffAnimationSetProperty(self->handle, MGEFF_PROP_KEEPALIVE, 1);
}


static BOOL mPicker_addItem(mPicker* self, int index, const char* string)
{
    if (NULL == string)
        return FALSE;

    if (!(GetWindowStyle(self->hwnd) & NCSS_PCK_STRINGS))
        return FALSE;

    if (0 == _c(self)->createItem(self, 0, 0, index, -1, string, 0, NULL, FALSE))
        return FALSE;

    _c(self)->refreshRect(self, NULL);

    return TRUE;
}


static BOOL mPicker_addItems(mPicker* self, const char** strings, int len)
{
    int i = 0;

    if (NULL == strings)
        return FALSE;

    if (!(GetWindowStyle(self->hwnd) & NCSS_PCK_STRINGS))
        return FALSE;

    for (i = 0; i < len; i++) {
        if (NULL == strings[i])
            return FALSE;

        if (!_c(self)->addItem(self, -1, strings[i]))
            return FALSE;
    }

    _c(self)->refreshRect(self, NULL);

    return TRUE;
}


static BOOL mPicker_rmItem(mPicker* self, int index)
{
    HITEM hitem = 0;

    if (index >= _c(self)->getItemCount(self))
        return FALSE;

    hitem = _c(self)->getItem(self, index);

    if (-1 == _c(self)->removeItem(self, hitem))
        return FALSE;

    return TRUE;
}


static BOOL mPicker_setRange(mPicker* self, int min, int max)
{
    self->minValue = min;
    self->maxValue = max;
    resetItemsByDigits(self);

    return TRUE;
}


static BOOL mPicker_setSpan(mPicker* self, int span)
{
    self->span = span;
    resetItemsByDigits(self);

    return TRUE;
}


static int mPicker_onSizeChanged(mPicker *self, RECT *rtClient)
{
    Class(mAnimation).onSizeChanged((mAnimation *)self, rtClient);
    prepareBkDC(self);

    return 0;
}


static void mPicker_enableItems(mPicker *self, int idxFrom, int count)
{
    int i = 0;
    HITEM itm = 0;

    if (count < 0)
        count = _M(self, getItemCount) - idxFrom;

    for (i = 0; i < count; i++) {
        itm = _M(self, getItem, idxFrom + i);
        if (itm) {
            _M(self, enable, itm, TRUE);
        }
    }

    _c(self)->refreshRect(self, NULL);
}


static void mPicker_disableItems(mPicker *self, int idxFrom, int count)
{
    int i = 0;
    HITEM itm = 0;

    if (count < 0)
        count = _M(self, getItemCount) - idxFrom;

    for (i = 0; i < count; i++) {
        itm = _M(self, getItem, idxFrom + i);
        if (itm) {
            _M(self, enable, itm, FALSE);
        }
    }

    _c(self)->refreshRect(self, NULL);
}


static void _call_draw(MGEFF_ANIMATION animation, void* target, intptr_t id, void* value)
{
    UpdateWindow((HWND)target, FALSE);
}


static MGEFF_ANIMATION mPicker_createAnimation(mPicker* self,
        const EffPropertyAnimationSetting* settings)
{
    MGEFF_ANIMATION group;
    MGEFF_ANIMATION drawAnim;

    if (!settings || !settings->property) {
        return NULL;
    }

    group = mGEffCreatePropertyAnimation(settings);
    if (!group) {
        return NULL;
    }

    drawAnim = mGEffAnimationCreate((void*)self->hwnd,
            _call_draw, 0, MGEFF_INT);
    if (!group) {
        return NULL;
    }
    mGEffAnimationSetDuration(drawAnim, settings->duration);

    mGEffAnimationAddToGroup(group, drawAnim);

    return group;
}

#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
static BOOL mPicker_refresh(mPicker* self)
{
    DWORD dwStyle = GetWindowStyle(self->hwnd);

    if (dwStyle & NCSS_PCK_NUMBER) {
        resetItemsByDigits(self);
    } else if (dwStyle & NCSS_PCK_STRINGS) {
        resetItemsByStrings(self);
    }

    InvalidateRect(self->hwnd, NULL, TRUE);

    return TRUE;
}
#endif


BEGIN_CMPT_CLASS(mPicker, mAnimation)
    CLASS_METHOD_MAP(mPicker, construct)
    CLASS_METHOD_MAP(mPicker, destroy)
    CLASS_METHOD_MAP(mPicker, onCreate)
    CLASS_METHOD_MAP(mPicker, onPaint)
    CLASS_METHOD_MAP(mPicker, onEraseBkgnd)
    CLASS_METHOD_MAP(mPicker, onKeyDown)
    CLASS_METHOD_MAP(mPicker, onSizeChanged)
    CLASS_METHOD_MAP(mPicker, wndProc)
    CLASS_METHOD_MAP(mPicker, setProperty)
    CLASS_METHOD_MAP(mPicker, getProperty)
    CLASS_METHOD_MAP(mPicker, endAnimation)
    CLASS_METHOD_MAP(mPicker, runAnimation)
    CLASS_METHOD_MAP(mPicker, createAnimation)
    CLASS_METHOD_MAP(mPicker, addItem)
    CLASS_METHOD_MAP(mPicker, addItems)
    CLASS_METHOD_MAP(mPicker, rmItem)
    CLASS_METHOD_MAP(mPicker, setRange)
    CLASS_METHOD_MAP(mPicker, setSpan)
    CLASS_METHOD_MAP(mPicker, enableItems)
    CLASS_METHOD_MAP(mPicker, disableItems)
#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
    CLASS_METHOD_MAP(mPicker, refresh)
#endif
END_CMPT_CLASS

