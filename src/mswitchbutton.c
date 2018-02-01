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
#include "mswitchbutton.h"

#ifndef WIN32
#   include <unistd.h>
#endif

#define NDEBUG	1
#include "mtouchdebug.h"


#define	SWB_TEXT_LEN	32
#define	SWB_STEP		5


static int ParseString(const char* strSrc, char* str1, char* str2,
		int str1_len, int str2_len)
{
	char* p1 = NULL;
	char* p2 = NULL;

	if (NULL == strSrc)
		return -1;

	p1 = (char*)strSrc;
	p2 = strchr(strSrc, '\n');

	if (p2 == NULL) {
		// there is no '\n', so the second section is NULL
		snprintf(str1, str1_len, "%s", strSrc);
		memset(str2, 0, str2_len);
		return 0;
	}

	p2++;

	str1_len = MIN(str1_len, (p2 - p1));
	str2_len = MIN(str2_len, (strlen(p2) + 1));

	snprintf(str1, str1_len, "%s", p1);
	snprintf(str2, str2_len, "%s", p2);

	return 0;
}


static void DivideRect(mSwitchButton* self, PRECT pRc,
        PRECT leftTxtRc, PRECT rightTxtRc, PRECT blockRc, PRECT onRc, PRECT offRc)
{
    int r = self->radius;
    int right = pRc->right;
    int txt_w = RECTWP(pRc) * 3 / 5;
    int blk_w = RECTWP(pRc) - txt_w;

    if (leftTxtRc) {
        leftTxtRc->top    = pRc->top;
        leftTxtRc->bottom = pRc->bottom;
    }
    if (rightTxtRc) {
        rightTxtRc->top    = pRc->top;
        rightTxtRc->bottom = pRc->bottom;
    }
    if (blockRc) {
        blockRc->top    = pRc->top;
        blockRc->bottom = pRc->bottom + 1;
    }
    if (onRc) {
        onRc->top    = pRc->top;
        onRc->bottom = pRc->bottom;
        onRc->left   = pRc->left;
    }
    if (offRc) {
        offRc->top    = pRc->top;
        offRc->bottom = pRc->bottom;
        offRc->right  = pRc->right;
    }
    if (NCS_SWB_ON == self->status) {
        if (leftTxtRc) {
            leftTxtRc->left = pRc->left - self->dist;
            //leftTxtRc->right = txt_w - self->dist;
            leftTxtRc->right = leftTxtRc->left + txt_w;
        }
        if (rightTxtRc) {
            rightTxtRc->left = right - self->dist;
            rightTxtRc->right = rightTxtRc->left + txt_w;
        }
        if (blockRc) {
            blockRc->left = pRc->left + txt_w - self->dist;
            blockRc->right = blockRc->left + blk_w;
        }
        if (self->dist > 0 && self->dist < txt_w) {
            if (onRc) onRc->right = txt_w - self->dist + 2 * r;
            if (offRc) offRc->left = pRc->right - self->dist - 2 * r;
        } else if (self->dist >= txt_w) {
            if (onRc) onRc->right = -1;
            if (offRc) offRc->left = pRc->right - self->dist - 2 * r;
        } else {
            if (onRc) onRc->right = txt_w - self->dist + 2 * r;
            if (offRc) offRc->left = pRc->right - self->dist;
        }
    } else {
        if (leftTxtRc) {
            leftTxtRc->left = pRc->left - txt_w + self->dist;
            leftTxtRc->right = self->dist - 1;
        }
        if (rightTxtRc) {
            rightTxtRc->left = blk_w + self->dist;
            rightTxtRc->right = rightTxtRc->left + txt_w;
        }
        if (blockRc) {
            blockRc->left = pRc->left + self->dist;
            blockRc->right = blockRc->left + blk_w;
        }
        if (self->dist > 0 && self->dist < txt_w) {
            if (onRc) onRc->right = self->dist + 2 * r;
            if (offRc) offRc->left = blk_w + self->dist - 2 * r;
        } else if (self->dist >= txt_w) {
            if (onRc) onRc->right = self->dist + 2 * r;
            if (offRc) offRc->left = pRc->right;
        } else {
            if (onRc) onRc->right = self->dist;
            if (offRc) offRc->left = blk_w + self->dist - 2 * r - 1;
        }
    }
}


static void SetDist(mSwitchButton* self, PRECT pRc)
{
	int maxLen = 0;

	if (self->pressOnBlock) {
		maxLen = RECTWP(pRc) * 3 / 5;

        if (NCS_SWB_ON == self->status) {
			self->dist = self->moveFrom - self->mouseX;
		} else {
			self->dist = self->mouseX - self->moveFrom;
		}

        if (self->dist > maxLen) {
            self->dist = maxLen;
        } else if (self->dist < 0) {
            self->dist = 0;
        }
	}
}


static void DrawSwitchButton(mSwitchButton* self, HDC hdc, PRECT pRc, int r)
{
	RECT rtTxtOn, rtTxtOff, rtBlock, rtOn, rtOff;
	const char* spCaption = NULL;
	char str1[SWB_TEXT_LEN] = {0};
	char str2[SWB_TEXT_LEN] = {0};
	DWORD blockColorBg = 0;
	DWORD onColorFg = 0, offColorFg = 0;
	DWORD onColorBg = 0, offColorBg = 0;

	onColorBg    = ncsGetElement((mWidget*)self, NCS4TOUCH_BGC_SWBON);
	onColorFg    = ncsGetElement((mWidget*)self, NCS4TOUCH_FGC_SWBON);
	offColorBg   = ncsGetElement((mWidget*)self, NCS4TOUCH_BGC_SWBOFF);
	offColorFg   = ncsGetElement((mWidget*)self, NCS4TOUCH_FGC_SWBOFF);
	blockColorBg = ncsGetElement((mWidget*)self, NCS4TOUCH_BGC_BLOCK);

	DivideRect(self, pRc, &rtTxtOn, &rtTxtOff, &rtBlock, &rtOn, &rtOff);

	DrawGradientRroundRect(hdc, onColorBg, &rtOn, r, FALSE);
	DrawGradientRroundRect(hdc, offColorBg, &rtOff, r, FALSE);

	if (NULL != (spCaption = GetWindowCaption(self->hwnd))) {
		ParseString(spCaption, str1, str2, SWB_TEXT_LEN, SWB_TEXT_LEN);
		SetBkMode(hdc, BM_TRANSPARENT);
		SetTextColor(hdc, ncsColor2Pixel(hdc, onColorFg));
		DrawText(hdc, str1, -1, &rtTxtOn, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		SetTextColor(hdc, ncsColor2Pixel(hdc, offColorFg));
		DrawText(hdc, str2, -1, &rtTxtOff, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	DrawRoundRectButton(hdc, &rtBlock, blockColorBg, r);
}


static void mSwitchButton_construct(mSwitchButton* self, DWORD param)
{
	Class(mWidget).construct((mWidget*)self, param);

	self->status = NCS_SWB_OFF;
	self->pressOnBlock = FALSE;
	self->radius = 3;
	self->mouseX = 0;
	self->moveFrom = 0;
	self->dist = 0;
	self->step = 0;

	IncludeWindowStyle(self->hwnd, NCSS_NOTIFY);
}


static void mSwitchButton_destroy(mSwitchButton* self)
{
	Class(mWidget).destroy((mWidget*)self);
}


static BOOL mSwitchButton_onCreate(mSwitchButton* self, DWORD addData)
{
	Class(mWidget).onCreate((mWidget*)self, addData);
	_M(self, setProperty, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

	return TRUE;
}


static void mSwitchButton_onPaint(mSwitchButton* self, HDC hdc, const PCLIPRGN pClip)
{
	RECT rc;

	GetClientRect(self->hwnd, &rc);
	self->radius = ncsGetElement((mWidget*)self, NCS4TOUCH_RRECT_RADIUS);

    if (0 == self->step)
		SetDist(self, &rc);

    DrawSwitchButton(self, hdc, &rc, self->radius);

    return;
}


static int mSwitchButton_onLButtonDown(mSwitchButton* self, int x, int y, DWORD keyFlags)
{
	RECT rc, rcBlock;

	GetClientRect(self->hwnd, &rc);
    memcpy (&rcBlock, &rc, sizeof (RECT));

    if (self->status == NCS_SWB_ON) {
        rcBlock.left = rcBlock.right - RECTW(rc) * 2 / 5;
    } else {
        rcBlock.right = rcBlock.left + RECTW(rc) * 2 / 5;
    }

	if (PtInRect (&rcBlock, x, y)) {
		self->pressOnBlock = TRUE;
		self->moveFrom = x;
	}

    SetCapture(self->hwnd);

	return 0;
}


static int mSwitchButton_onLButtonUp(mSwitchButton* self, int x, int y, DWORD keyFlags)
{
	RECT rc, rcBlock;
	int txt_w, blk_w, center;
	int x1 = x, y1 = y;

	if (GetCapture() == self->hwnd) {
		ScreenToClient(self->hwnd, &x1, &y1);
		GetClientRect(self->hwnd, &rc);

		txt_w = RECTW(rc) / 5 * 3;
		blk_w = RECTW(rc) - txt_w;

        if (self->pressOnBlock) {
            DivideRect(self, &rc, NULL, NULL, &rcBlock, NULL, NULL);
			center = rcBlock.left + blk_w / 2;
            if (self->status == NCS_SWB_ON
                    && center >= 0 && center < (rc.right + rc.left) / 2) {
                _M(self, runAnimation, self->dist, txt_w, 50 * (txt_w -self->dist) / txt_w);
				self->status = NCS_SWB_OFF;
                ncsNotifyParentEx((mWidget*)self, NCSN_SWB_STATUSCHANGED, self->status);
            }
            else if (self->status == NCS_SWB_OFF
                    && center >= (rc.left + rc.right) / 2 && center < rc.right) {
                _M(self, runAnimation, self->dist, txt_w, 50 * (txt_w -self->dist) / txt_w);
				self->status = NCS_SWB_ON;
                ncsNotifyParentEx((mWidget*)self, NCSN_SWB_STATUSCHANGED, self->status);
            }
		} else {
            if (self->status == NCS_SWB_ON
                    && x1 >= rc.left && x1 < rc.left + RECTW(rc)/2) {
                _M(self, runAnimation, 0, txt_w, 40);
				self->status = NCS_SWB_OFF;
                ncsNotifyParentEx((mWidget*)self, NCSN_SWB_STATUSCHANGED, self->status);
            } else if (self->status == NCS_SWB_OFF
                    && x1 >= rc.left + RECTW(rc)/2 && x1 < rc.right) {
                _M(self, runAnimation, 0, txt_w, 40);
				self->status = NCS_SWB_ON;
                ncsNotifyParentEx((mWidget*)self, NCSN_SWB_STATUSCHANGED, self->status);
            }
		}

		self->pressOnBlock = FALSE;
		self->mouseX = 0;
		self->moveFrom = 0;
		self->dist = 0;
		ReleaseCapture();
        InvalidateRect(self->hwnd, NULL, FALSE);
	}

	return 0;
}


static int mSwitchButton_onMouseMove(mSwitchButton* self, int x, int y, DWORD keyFlags)
{
	int x1 = x;
	int y1 = y;

	if (GetCapture() == self->hwnd) {
		ScreenToClient(self->hwnd, &x1, &y1);
		if (self->pressOnBlock && self->mouseX - x1) {
			self->mouseX = x1;
			InvalidateRect(self->hwnd, NULL, FALSE);
		}
	}

	return 0;
}


static BOOL mSwitchButton_setProperty(mSwitchButton* self, int id, DWORD value)
{
	if (id > NCSP_SWB_MAX)
		return FALSE;

	switch (id) {
		case NCSP_WIDGET_RDR:
			if (strcmp((char*)value, NCS4TOUCH_RENDERER) == 0)
				break;
			else
				return FALSE;

		case NCSP_SWB_STATUS:
			if (self->status != value) {
				self->status = value;
				InvalidateRect(self->hwnd, NULL, FALSE);
			}
			return TRUE;

		default:
			break;
	}

	return Class(mWidget).setProperty((mWidget*)self, id, value);
}


static DWORD mSwitchButton_getProperty(mSwitchButton* self, int id)
{
	if (id >= NCSP_SWB_MAX)
		return (-1);

	if (id == NCSP_SWB_STATUS) {
        return (DWORD)self->status;
	}

	return Class(mWidget).getProperty((mWidget*)self, id);
}

static void mSwitchButton_runAnimation(mSwitchButton* self, int start, int end, int duration)
{
    //FIXME, need to redefine this function use mgeff
	int i = 0;
	int tmp = 0;

    self->step = SWB_STEP;
    tmp = (end - start) / SWB_STEP + 1;

    for (i = 0; i < SWB_STEP; i++) {
        self->dist = start + i * tmp;
        UpdateWindow(self->hwnd, FALSE);
        if (duration < 0)
            duration = 1;
#ifndef WIN32
        usleep(duration * 1000);
#else
	{
		int windows_does_not_have_usleep;
	}
#endif
    }
    self->step = 0;
}


#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
static BOOL mSwitchButton_refresh(mSwitchButton* self)
{
	UpdateWindow(self->hwnd, TRUE);

    return TRUE;
}
#endif


BEGIN_CMPT_CLASS(mSwitchButton, mWidget)
	CLASS_METHOD_MAP(mSwitchButton, construct)
	CLASS_METHOD_MAP(mSwitchButton, destroy)
	CLASS_METHOD_MAP(mSwitchButton, onCreate)
	CLASS_METHOD_MAP(mSwitchButton, onPaint)
	CLASS_METHOD_MAP(mSwitchButton, onLButtonUp)
	CLASS_METHOD_MAP(mSwitchButton, onLButtonDown)
	CLASS_METHOD_MAP(mSwitchButton, onMouseMove)
	CLASS_METHOD_MAP(mSwitchButton, setProperty)
	CLASS_METHOD_MAP(mSwitchButton, getProperty)
	CLASS_METHOD_MAP(mSwitchButton, runAnimation)
#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
	CLASS_METHOD_MAP(mSwitchButton, refresh)
#endif
END_CMPT_CLASS

