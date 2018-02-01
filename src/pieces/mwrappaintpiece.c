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
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"

static void mWrapPaintPiece_construct(mWrapPaintPiece *self, DWORD add_data)
{
	Class(mLabelPiece).construct((mLabelPiece*)self, add_data);
    self->effect = NCS_WRAPPAINT_ZOOM;
    self->wrappiece = (mHotPiece*)add_data;
    self->parent = self->wrappiece->parent;
}

void wrappaint_effect(mWrapPaintPiece *self, HDC hdc, HDC effect_dc)
{
    RECT rc;
    _c(self)->getRect(self, &rc);
    if (RECTH(rc) != 0) {
        StretchBlt(effect_dc, 0, 0, 0, 0,
                hdc, 0, 0, RECTW(rc), RECTH(rc), 0);
    }
}

static void mWrapPaintPiece_paint(mWrapPaintPiece *self, HDC hdc, mWidget *owner, DWORD add_data)
{
    /* need rotate */
    HDC effect_dc;
    RECT rc;
    _c(self->wrappiece)->getRect(self->wrappiece, &rc);
    effect_dc = CreateCompatibleDCEx(hdc, RECTW(rc), RECTH(rc));
    _c(self->wrappiece)->paint(self->wrappiece, effect_dc, (mObject*)owner, add_data);

    wrappaint_effect(self, hdc, effect_dc);

    DeleteMemDC(effect_dc);
}

static BOOL mWrapPaintPiece_setProperty(mWrapPaintPiece *self, int id, DWORD value)
{
    if (id == NCSP_WRAPPAINT_EFFECT) {
        self->effect = value;
    } else {
        return Class(mLabelPiece).setProperty((mLabelPiece*)self, id, value);
    }
    return TRUE;
}

static DWORD mWrapPaintPiece_getProperty(mWrapPaintPiece* self, int id)
{
    if (id == NCSP_WRAPPAINT_EFFECT) {
        return (DWORD)self->effect;
    } else {
        return Class(mLabelPiece).getProperty((mLabelPiece*)self, id);
    }
}


BEGIN_MINI_CLASS(mWrapPaintPiece, mStaticPiece)
	CLASS_METHOD_MAP(mWrapPaintPiece, construct)
	CLASS_METHOD_MAP(mWrapPaintPiece, paint)
	CLASS_METHOD_MAP(mWrapPaintPiece, setProperty)
	CLASS_METHOD_MAP(mWrapPaintPiece, getProperty)
END_MINI_CLASS

