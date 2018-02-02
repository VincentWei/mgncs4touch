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

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"

static void mAbstractItemManager_construct(mAbstractItemManager *self, DWORD add_data)
{
	Class(mObject).construct((mObject*)self, add_data);
}

static void mAbstractItemManager_destroy(mAbstractItemManager *self)
{
    Class(mObject).destroy((mObject*)self);
}

static int  mAbstractItemManager_addItem(mAbstractItemManager* self, mPieceItem* item)
{
    return -1;
}

static void mAbstractItemManager_removeItem(mAbstractItemManager* self, mPieceItem* item)
{

}

static mItemIterator* mAbstractItemManager_createItemIterator(mAbstractItemManager* self)
{
    return NULL;
}

BEGIN_MINI_CLASS(mAbstractItemManager, mObject)
	CLASS_METHOD_MAP(mAbstractItemManager, construct)
	CLASS_METHOD_MAP(mAbstractItemManager, destroy)
	CLASS_METHOD_MAP(mAbstractItemManager, addItem)
	CLASS_METHOD_MAP(mAbstractItemManager, removeItem)
	CLASS_METHOD_MAP(mAbstractItemManager, createItemIterator)
END_MINI_CLASS

