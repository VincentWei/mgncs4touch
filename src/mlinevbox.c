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
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

static void mLineVBox_reLayout(mLineVBox *self, mItemIterator* iter, int w, int h)
{
    RECT rc;
    int y = 0;
    mPieceItem* item = NULL;
    while((item = _c(iter)->next(iter))) {
        mHotPiece* piece = _c(item)->getPiece(item);
        _c(item)->setY(item, y);
        _c(piece)->getRect(piece, &rc);
        y += (RECTH(rc) + self->gap);
    }

    return;
}

static void mLineVBox_setGap(mLineVBox *self, int gap)
{
    self->gap = gap;
}

static void mLineVBox_getLayoutRect(mLineVBox *self, mItemIterator* iter, RECT* rect)
{
    RECT rc;
    int h = 0;
    int w = 0;
    mPieceItem* item = NULL;
    while((item = _c(iter)->next(iter))) {
        mHotPiece* piece = _c(item)->getPiece(item);
        _c(piece)->getRect(piece, &rc);
        h += RECTH(rc);
        if (RECTW(rc) > w) {
            w = RECTW(rc);
        }
    }
    rect->top = rect->left = 0;
    rect->right = w;
    rect->bottom = h;
    return;
}

BEGIN_MINI_CLASS(mLineVBox, mLayoutManager)
    CLASS_METHOD_MAP(mLineVBox, reLayout)
    CLASS_METHOD_MAP(mLineVBox, setGap)
    CLASS_METHOD_MAP(mLineVBox, getLayoutRect)
END_MINI_CLASS
