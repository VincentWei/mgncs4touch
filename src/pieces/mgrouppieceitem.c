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

static void mGroupPieceItem_construct(mGroupPieceItem *self, DWORD add_data)
{
    Class(mPieceItem).construct((mPieceItem*)self, add_data);
    _c(self)->setKey(self, (char*)add_data);
}

static void mGroupPieceItem_destroy(mGroupPieceItem *self)
{
    if (self->key) {
        free(self->key);
    }
    Class(mPieceItem).destroy((mPieceItem*)self);
}

static void mGroupPieceItem_setKey(mGroupPieceItem *self, const char* key)
{
    if (self->key) {
        free(self->key);
    }
    self->key = malloc(sizeof(char)*(strlen(key)+1));
    strcpy(self->key, key);
    self->key[strlen(key)] = '\0';
}

static char* mGroupPieceItem_getKey(mGroupPieceItem *self)
{
    return self->key;
}

BEGIN_MINI_CLASS(mGroupPieceItem, mPieceItem)
	CLASS_METHOD_MAP(mGroupPieceItem, construct)
	CLASS_METHOD_MAP(mGroupPieceItem, destroy)
    CLASS_METHOD_MAP(mGroupPieceItem, getKey)
    CLASS_METHOD_MAP(mGroupPieceItem, setKey)
END_MINI_CLASS

