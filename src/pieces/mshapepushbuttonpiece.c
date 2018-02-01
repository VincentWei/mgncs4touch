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
#include "pieces/mshapepushbuttonpiece.h"
#include "pieces/mshapeboxpiece.h"
#include "pieces/mcolorablelabelpiece.h"

static void mShapePushButtonPiece_construct(mShapePushButtonPiece *self, DWORD add_data)
{
	if(add_data){
		self->body = (mHotPiece*)add_data;
    } else {
		self->body = (mHotPiece*)NEWPIECE(mShapeBoxPiece);  
		self->content = (mHotPiece*)NEWPIECE(mColorableLabelPiece);
	}
}

static void mShapePushButtonPiece_getContentRect(mShapePushButtonPiece * self,  RECT *prc)
{
	RECT _rc;
	int round = 5;  
    
    Class(mShapePushButtonPiece).getRect((mShapePushButtonPiece*)self, &_rc);

	if(self->shape == SHAPE_LEFT)
	{
		prc->left	= _rc.left + RECTH(_rc)/2;
		prc->right	= _rc.right -round;
		prc->top	= _rc.top +round;
		prc->bottom	= _rc.bottom -round; 
	} 
    else if(self->shape == SHAPE_RIGHT) {
		prc->left	= _rc.left + round;
		prc->right	= _rc.right -round - RECTH(_rc)/2;
		prc->top	= _rc.top +round;
		prc->bottom	= _rc.bottom -round; 
	}
    else if(self->shape == SHAPE_RECT) {
		prc->left	= _rc.left + round;
		prc->right	= _rc.right -round;
		prc->top	= _rc.top +round;
		prc->bottom	= _rc.bottom -round; 
	}
}

static BOOL mShapePushButtonPiece_setRect(mShapePushButtonPiece *self, const RECT *prc)
{
    Class(mPushButtonPiece).setRect((mPushButtonPiece*)self, prc);

	if(self->content)
	{
		RECT rc;
		_c(self)->getContentRect(self, &rc);  
		_c(self->content)->setRect(self->content, &rc); 
	}																					
	return TRUE;
}

static BOOL mShapePushButtonPiece_setProperty(mShapePushButtonPiece *self, int id, DWORD value)
{
	if (id == NCSP_SPBP_SHAPE)
	{
		self->shape = (int)value;
		return TRUE;
	}
	return Class(mPushButtonPiece).setProperty ((mPushButtonPiece *)self, id, value);
}

static DWORD mShapePushButtonPiece_getProperty(mShapePushButtonPiece *self, int id)
{
	if (id == NCSP_SPBP_SHAPE)
	{
		return (DWORD)self->shape;
	}
	return Class(mPushButtonPiece).getProperty ((mPushButtonPiece *)self, id);
}

static void mShapePushButtonPiece_paint(mShapePushButtonPiece *self, HDC hdc, mWidget * owner, DWORD add_data)
{
	if (self->content){
		DWORD tcolor = ncsGetElement(owner, NCS4TOUCH_FGC_BNB_BUTTON);
		_M((mColorableLabelPiece *)self->content, setProperty, NCSP_CLP_PIECE_COLOR , tcolor);
	}
	Class(mPushButtonPiece).paint((mPushButtonPiece *)self, hdc, (mObject *)owner, add_data);
}

BEGIN_MINI_CLASS(mShapePushButtonPiece, mPushButtonPiece)
	CLASS_METHOD_MAP(mShapePushButtonPiece, construct)
	CLASS_METHOD_MAP(mShapePushButtonPiece, paint)
	CLASS_METHOD_MAP(mShapePushButtonPiece, setRect)
	CLASS_METHOD_MAP(mShapePushButtonPiece, getContentRect)
	CLASS_METHOD_MAP(mShapePushButtonPiece, setProperty)
	CLASS_METHOD_MAP(mShapePushButtonPiece, getProperty)
END_MINI_CLASS


