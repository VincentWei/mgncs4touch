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

#define ITEM_HEAD(ptr) list_entry(ptr, mPieceItem, list)

static void mListItemIterator_construct(mListItemIterator *self, DWORD add_data)
{
	Class(mItemIterator).construct((mItemIterator*)self, add_data);
    self->current = self->head = NULL;
    self->manager = NULL;
}

static void mListItemIterator_destroy(mListItemIterator *self)
{
    Class(mItemIterator).destroy((mItemIterator*)self);
}

static mPieceItem*  mListItemIterator_next(mListItemIterator* self)
{
    if (self->current->next != self->head) {
        self->current = self->current->next;
        return (mPieceItem*)ITEM_HEAD(self->current);
    }
    return NULL;
}

static mPieceItem*  mListItemIterator_prev(mListItemIterator* self)
{
    if (self->current->prev != self->head) {
        self->current = self->current->prev;
        return (mPieceItem*)ITEM_HEAD(self->current);
    }
    return NULL;
}

static mListItemIterator* mListItemIterator_duplicate(mListItemIterator* self)
{
    mListItemIterator* iter = NEW(mListItemIterator);
    if (NULL != iter) {
        iter->head = self->head;
        iter->current = self->current;
        iter->manager = self->manager;
        return iter;
    }
    return NULL;
}

BEGIN_MINI_CLASS(mListItemIterator, mItemIterator)
	CLASS_METHOD_MAP(mListItemIterator, construct)
	CLASS_METHOD_MAP(mListItemIterator, destroy)
	CLASS_METHOD_MAP(mListItemIterator, next)
	CLASS_METHOD_MAP(mListItemIterator, prev)
	CLASS_METHOD_MAP(mListItemIterator, duplicate)
END_MINI_CLASS
