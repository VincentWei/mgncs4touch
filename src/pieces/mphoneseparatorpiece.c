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

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

static void mPhoneSeparatorPiece_construct(mPhoneSeparatorPiece *self, DWORD add_data)
{
    Class(mStaticPiece).construct((mStaticPiece*)self, add_data);

    self->darkercolor  = 0x0;
    self->lightercolor = 0x0;

    self->direct_mode = PHONESEPARATORPIECE_HERT;

    return;
}

static void mPhoneSeparatorPiece_destroy(mPhoneSeparatorPiece *self)
{
    Class(mStaticPiece).destroy((mStaticPiece*)self);

    return;
}

static inline void line(HDC hdc, int x0,int y0, int x1, int y1, gal_pixel color)
{
    gal_pixel old = SetPenColor(hdc, color);

    MoveTo(hdc, x0, y0);

    LineTo(hdc, x1, y1);

    SetPenColor(hdc, old);

    return;
}

static void mPhoneSeparatorPiece_paint(mPhoneSeparatorPiece *self,
                            HDC hdc, mWidget *owner, DWORD add_data)
{
    RECT rect;
    int middle;

    if ((self->darkercolor == self->lightercolor)
            && (self->darkercolor == 0))
        return;

    _c(self)->getRect(self, &rect);

    if(self->direct_mode == PHONESEPARATORPIECE_HERT){
        int x;

        middle = (rect.top + rect.bottom) / 2;

        for (x = rect.top; x < middle; x++){
            line (hdc, rect.left, x, rect.right, x, ncsColor2Pixel(hdc, self->darkercolor));
        }

        for (x = middle; x < rect.bottom; x++){
            line (hdc, rect.left, x, rect.right, x, ncsColor2Pixel(hdc, self->lightercolor));
        }
    }
    else {
        int y;

        middle = (rect.left + rect.right) / 2;

        for (y = rect.left; y < middle; y++){
            line (hdc, y, rect.top, y, rect.bottom, ncsColor2Pixel(hdc, self->darkercolor));
        }

        for (y = middle; y < rect.right; y++) {
            line (hdc, y, rect.top, y, rect.bottom, ncsColor2Pixel(hdc, self->lightercolor));
        }
    }
}

static BOOL mPhoneSeparatorPiece_setProperty(mPhoneSeparatorPiece *self, int id, DWORD value)
{
    switch(id)
    {
        case NCSP_PHONESEPARATORPIECE_DARKER_COLOR:
            self->darkercolor = value;
            break;
        case NCSP_PHONESEPARATORPIECE_LIGHTER_COLOR:
            self->lightercolor = value;
            break;
        case NCSP_PHONESEPARATORPIECE_DIRECT_MODE:
            self->direct_mode = (PHONESEPARATORPIECE_DIRECT_MODE_E)value;
            break;
        default:
            return Class(mStaticPiece).setProperty((mStaticPiece*)self, id, value);
    }

    return TRUE;
}

static DWORD mPhoneSeparatorPiece_getProperty(mPhoneSeparatorPiece* self, int id)
{
    switch(id)
    {
        case NCSP_PHONESEPARATORPIECE_DARKER_COLOR:
            return (DWORD)self->darkercolor;
        case NCSP_PHONESEPARATORPIECE_LIGHTER_COLOR:
            return (DWORD)self->lightercolor;
        case NCSP_PHONESEPARATORPIECE_DIRECT_MODE:
            return (PHONESEPARATORPIECE_DIRECT_MODE_E)self->direct_mode;
    }

    return Class(mStaticPiece).getProperty((mStaticPiece*)self, id);
}

BEGIN_MINI_CLASS(mPhoneSeparatorPiece, mStaticPiece)
    CLASS_METHOD_MAP( mPhoneSeparatorPiece, construct     )
    CLASS_METHOD_MAP( mPhoneSeparatorPiece, destroy       )
    CLASS_METHOD_MAP( mPhoneSeparatorPiece, paint         )
    CLASS_METHOD_MAP( mPhoneSeparatorPiece, setProperty   )
    CLASS_METHOD_MAP( mPhoneSeparatorPiece, getProperty   )
END_MINI_CLASS

