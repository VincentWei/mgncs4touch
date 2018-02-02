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

static void mListItemManager_construct(mListItemManager *self, DWORD add_data)
{
	Class(mAbstractItemManager).construct((mAbstractItemManager*)self, add_data);
    INIT_LIST_HEAD(&self->queue);
}

static void mListItemManager_destroy(mListItemManager *self)
{
    _c(self)->clear(self);
    Class(mAbstractItemManager).destroy((mAbstractItemManager*)self);
}


int  mListItemManager_addItem(mListItemManager* self, mPieceItem* item)
{
    if (NULL != item) {
        list_add_tail(&(item->list), &self->queue);
        return 0;
    }
    return -1;
}

void mListItemManager_removeItem(mListItemManager* self, mPieceItem* item)
{
    if (NULL != item) {
        list_t *pos = NULL, *n = NULL;
        list_for_each_safe(pos, n, &self->queue) {
            if (pos == &item->list) {
                list_del(pos);
            }
        }
    }
}

void mListItemManager_clear(mListItemManager* self)
{
    list_t *pos = NULL, *n = NULL;
    list_for_each_safe(pos, n, &self->queue) {
        list_del(pos);
    }
}

mItemIterator* mListItemManager_createItemIterator(mListItemManager* self)
{
    mListItemIterator* iter = NEW(mListItemIterator);
    if (NULL != iter) {
        iter->current = iter->head = &self->queue;
        iter->manager = (mAbstractItemManager*)self;
        return (mItemIterator*)iter;
    }
    return NULL;
}

BEGIN_MINI_CLASS(mListItemManager, mAbstractItemManager)
	CLASS_METHOD_MAP(mListItemManager, construct)
	CLASS_METHOD_MAP(mListItemManager, destroy)
	CLASS_METHOD_MAP(mListItemManager, addItem)
	CLASS_METHOD_MAP(mListItemManager, removeItem)
	CLASS_METHOD_MAP(mListItemManager, clear)
	CLASS_METHOD_MAP(mListItemManager, createItemIterator)
END_MINI_CLASS

