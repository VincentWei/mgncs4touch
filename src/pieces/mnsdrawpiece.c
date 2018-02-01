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
#include <mgncs/mgncs.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "pieces/mnsdrawpiece.h"

#define NDEBUG	1
#include "mtouchdebug.h"


static void mNSDrawPiece_construct(mNSDrawPiece* self, DWORD add_data)
{
	Class(mHotPiece).construct((mHotPiece*)self, add_data);

	self->shape   = -1;
	self->dotHit  = FALSE;
	self->left    = 0;
	self->top     = 0;
	self->right   = 0;
	self->bottom  = 0;
}


static void mNSDrawPiece_paint(mNSDrawPiece* self, HDC hdc, mObject* owner, DWORD add_data)
{
	RECT rc;
	DWORD color = 0;

	_c(self)->getRect(self, &rc);

	if (self->shape == NSDRAW_SHAPE_TICK) {
		if (self->dotHit)
			color = ncsGetElement((mWidget*)owner, NCS4TOUCH_FGC_ITB_TICK_LIGHT);
		else
			color = ncsGetElement((mWidget*)owner, NCS4TOUCH_FGC_ITB_TICK);
		DrawTouchTick(hdc, &rc, color);
	} else if (self->shape == NSDRAW_SHAPE_ANGLE) {
		if (self->dotHit)
			color = ncsGetElement((mWidget*)owner, NCS4TOUCH_FGC_ITB_ANGLE_LIGHT);
		else
			color = ncsGetElement((mWidget*)owner, NCS4TOUCH_FGC_ITB_ANGLE);
		DrawTouchAngle(hdc, &rc, color);
	}
}


static BOOL mNSDrawPiece_setProperty(mNSDrawPiece* self, int id, DWORD value)
{
	if (id > NCSP_NSDRAW_PIECE_MAX)
		return FALSE;

	switch (id) {
		case NCSP_NSDRAW_PIECE_TICK:
			self->shape = value;
			return TRUE;

		case NCSP_NSDRAW_PIECE_ANGLE:
			self->shape = value;
			return TRUE;

		case NCSP_NSDRAW_PIECE_DOTHIT:
			self->dotHit = value;
			return TRUE;

		default:
			break;
	}

	return Class(mHotPiece).setProperty((mHotPiece*)self, id, value);
}


static DWORD mNSDrawPiece_getProperty(mNSDrawPiece* self, int id)
{
	if (id > NCSP_NSDRAW_PIECE_MAX)
		return (DWORD)-1;

	switch (id) {
		case NCSP_NSDRAW_PIECE_TICK:
			return self->shape;

		case NCSP_NSDRAW_PIECE_ANGLE:
			return self->shape;

		case NCSP_NSDRAW_PIECE_DOTHIT:
			return self->dotHit;

		default:
			break;
	}

	return Class(mHotPiece).getProperty((mHotPiece*)self, id);
}


static BOOL mNSDrawPiece_setRect(mNSDrawPiece* self, RECT* prc)
{
	if (!prc)
		return FALSE;

	SetNSDrawPieceRect(self, prc);

	return TRUE;
}


static BOOL mNSDrawPiece_getRect(mNSDrawPiece* self, RECT* prc)
{
	if (!prc)
		return FALSE;

	SetRect(prc, self->left, self->top, self->right, self->bottom);

	return TRUE;
}


static mHotPiece* mNSDrawPiece_hitTest(mNSDrawPiece* self, int x, int y)
{
	if (x > self->left && x < self->right
		&& y > self->top && y < self->bottom)
		return (mHotPiece*)self;

	return NULL;
}


BEGIN_MINI_CLASS(mNSDrawPiece, mHotPiece)
	CLASS_METHOD_MAP(mNSDrawPiece, construct)
	CLASS_METHOD_MAP(mNSDrawPiece, paint)
	CLASS_METHOD_MAP(mNSDrawPiece, setProperty)
	CLASS_METHOD_MAP(mNSDrawPiece, getProperty)
	CLASS_METHOD_MAP(mNSDrawPiece, setRect)
	CLASS_METHOD_MAP(mNSDrawPiece, getRect)
	CLASS_METHOD_MAP(mNSDrawPiece, hitTest)
END_MINI_CLASS

