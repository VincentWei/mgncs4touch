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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

static void mItemIterator_construct(mItemIterator *self, DWORD add_data)
{
	Class(mObject).construct((mObject*)self, add_data);
    self->manager = NULL;
}

static void mItemIterator_destroy(mItemIterator *self)
{
    Class(mObject).destroy((mObject*)self);
}

static mPieceItem*  mItemIterator_next(mItemIterator* self)
{
    return NULL;
}

static mPieceItem*  mItemIterator_prev(mItemIterator* self)
{
    return NULL;
}

static mItemIterator* mItemIterator_duplicate(mItemIterator* self)
{
    mItemIterator* iter = NEW(mItemIterator);
    if (NULL != iter) {
        iter->manager = self->manager;
        return iter;
    }
    return NULL;
}

BEGIN_MINI_CLASS(mItemIterator, mObject)
	CLASS_METHOD_MAP(mItemIterator, construct)
	CLASS_METHOD_MAP(mItemIterator, destroy)
	CLASS_METHOD_MAP(mItemIterator, next)
	CLASS_METHOD_MAP(mItemIterator, prev)
	CLASS_METHOD_MAP(mItemIterator, duplicate)
END_MINI_CLASS

