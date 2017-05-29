/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Beijing FMSoft Technology Co., Ltd.
 * Room 902, Floor 9, Taixing, No.11, Huayuan East Road, Haidian
 * District, Beijing, P. R. CHINA 100191.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Beijing FMSoft Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance you entered into with FMSoft.
 *
 *          http://www.minigui.com
 *
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
		DWORD tcolor = padGetElement(owner, NCS4PAD_FGC_BNB_BUTTON);
		_M((mColorableLabelPiece *)self->content, setProperty, NCSP_CLP_PIECE_COLOR , tcolor);
	}
	Class(mPushButtonPiece).paint((mPushButtonPiece *)self, hdc, owner, add_data);
}

BEGIN_MINI_CLASS(mShapePushButtonPiece, mPushButtonPiece)
	CLASS_METHOD_MAP(mShapePushButtonPiece, construct)
	CLASS_METHOD_MAP(mShapePushButtonPiece, paint)
	CLASS_METHOD_MAP(mShapePushButtonPiece, setRect)
	CLASS_METHOD_MAP(mShapePushButtonPiece, getContentRect)
	CLASS_METHOD_MAP(mShapePushButtonPiece, setProperty)
	CLASS_METHOD_MAP(mShapePushButtonPiece, getProperty)
END_MINI_CLASS


