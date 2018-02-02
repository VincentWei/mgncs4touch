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

static void mPieceItem_construct(mPieceItem *self, DWORD add_data)
{
	Class(mObject).construct((mObject*)self, add_data);
    self->x = self->y = 0;
    self->piece = NULL;
    self->wscalefactor = 1.0;
    self->hscalefactor = 1.0;
    self->alpha = 255;
    self->normalVector.angle = 0.0;
    self->normalVector.x = 0;
    self->normalVector.y = 0;
    self->normalVector.z = 0;
    self->underLayout = FALSE;
    self->type = 0;

    self->isEnableCache = FALSE;
    self->cacheDC = HDC_INVALID;
}

static void mPieceItem_destroy(mPieceItem *self)
{
    if (self->cacheDC != HDC_INVALID) {
        DeleteMemDC(self->cacheDC);
        self->cacheDC = HDC_INVALID;
    }

    Class(mObject).destroy((mObject*)self);
}

static void mPieceItem_setPiece(mPieceItem *self, mHotPiece* piece)
{
    self->piece = piece;
}

static mHotPiece* mPieceItem_getPiece(mPieceItem* self)
{
    return self->piece;
}

static int mPieceItem_getType(mPieceItem* self)
{
    return self->type;
}

static void mPieceItem_setType(mPieceItem* self, int type)
{
    self->type  = type;
}


static void mPieceItem_setX(mPieceItem* self, int x)
{
    self->x = x;
}

static void mPieceItem_setY(mPieceItem* self, int y)
{
    self->y = y;
}

static int  mPieceItem_getX(mPieceItem* self)
{
    return self->x;
}

static int  mPieceItem_getY(mPieceItem* self)
{
    return self->y;
}

BEGIN_MINI_CLASS(mPieceItem, mObject)
	CLASS_METHOD_MAP(mPieceItem, construct)
	CLASS_METHOD_MAP(mPieceItem, destroy)
	CLASS_METHOD_MAP(mPieceItem, setPiece)
	CLASS_METHOD_MAP(mPieceItem, getPiece)
	CLASS_METHOD_MAP(mPieceItem, setX)
	CLASS_METHOD_MAP(mPieceItem, setY)
	CLASS_METHOD_MAP(mPieceItem, getX)
	CLASS_METHOD_MAP(mPieceItem, getY)
	CLASS_METHOD_MAP(mPieceItem, setType)
	CLASS_METHOD_MAP(mPieceItem, getType)
END_MINI_CLASS

