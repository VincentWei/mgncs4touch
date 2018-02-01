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

static void mCenterHBox_reLayout(mCenterHBox *self, mItemIterator* iter, int w, int h)
{
    mItemIterator *iter_duplicate = _c(iter)->duplicate(iter);
    mPieceItem *item = NULL;
    int offset = 0;
    int w_total = 0;
    RECT rc;
    while ((item = _c(iter)->next(iter))) {
        if (item->underLayout) {
            _c(item->piece)->getRect(item->piece, &rc);
            w_total += RECTW(rc);
        }
    }

    offset = (w - w_total) / 2;

    while ((item = _c(iter_duplicate)->next(iter_duplicate))) {
        if (item->underLayout) {
            _c(item->piece)->getRect(item->piece, &rc);
            item->x = offset;
            item->y = (h - RECTH(rc)) / 2;
            offset += RECTW(rc);
        }
    }

    DELETE(iter_duplicate);

    return;
}

BEGIN_MINI_CLASS(mCenterHBox, mLayoutManager)
    CLASS_METHOD_MAP(mCenterHBox, reLayout)
END_MINI_CLASS
