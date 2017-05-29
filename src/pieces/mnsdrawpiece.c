/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Feynman Software Technology Co., Ltd.
 * Room 508B-C, Floor 5, Citic Guoan Shumagang, No.32, Haidian South
 * Road, Haidian District, Beijing, P. R. CHINA 100080.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Feynman Software Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall use
 * it only in accordance you entered into with Feynman Software.
 *          http://www.minigui.com
 *
 *    FileName : mnsdrawpiece.c
 *      Author : <wangxin@minigui.org>
 * Create Date : Monday September 25, 2010
 *     Version : 0.0.0.1
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>

#include "mpcomm.h"
#include "mpadrdr.h"
#include "pieces/mnsdrawpiece.h"

#define NDEBUG	1
#include "mpaddebug.h"


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
			color = ncsGetElement((mWidget*)owner, NCS4PAD_FGC_ITB_TICK_LIGHT);
		else
			color = ncsGetElement((mWidget*)owner, NCS4PAD_FGC_ITB_TICK);
		DrawPadTick(hdc, &rc, color);
	} else if (self->shape == NSDRAW_SHAPE_ANGLE) {
		if (self->dotHit)
			color = ncsGetElement((mWidget*)owner, NCS4PAD_FGC_ITB_ANGLE_LIGHT);
		else
			color = ncsGetElement((mWidget*)owner, NCS4PAD_FGC_ITB_ANGLE);
		DrawPadAngle(hdc, &rc, color);
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

