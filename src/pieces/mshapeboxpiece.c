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

#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>

#include "mpcomm.h"
#include "mpadrdr.h"
#include "pieces/mshapeboxpiece.h"
#include "mbtnnavbar.h"
#include "pieces/mshapepushbuttonpiece.h"


static void mShapeBoxPiece_paint(mShapeBoxPiece *self, 
        HDC hdc, mWidget *owner, DWORD add_data)
{
	RECT rc;
	DWORD bgcolor;
    mShapePushButtonPiece *shapepiece = NULL, *p = NULL; 
	int r = 3;
	
    bgcolor = padGetElement(owner, NCS4PAD_BGC_BNB_BUTTON); 
	if (add_data == NCS_ABP_PUSHED) {
        bgcolor = ncsCommRDRCalc3dboxColor(bgcolor, NCSR_COLOR_DARKER); 
	}
    else if (add_data == NCS_ABP_HILIGHT) {
        bgcolor = ncsCommRDRCalc3dboxColor(bgcolor, NCSR_COLOR_LIGHTER); 
	}

	rc.left	    = self->left;
	rc.top		= self->top;
	rc.right	= self->right;
	rc.bottom	= self->bottom;

	if(NULL != (p = (mShapePushButtonPiece *)GetLeftBtnPiece(owner)) 
            && (mShapeBoxPiece *)p->body == self) {
        shapepiece = p;
	}
    else if(NULL != (p = (mShapePushButtonPiece *)GetRightBtnPiece(owner)) 
            && (mShapeBoxPiece *)p->body == self) {
        shapepiece = p;
	} else {
        return;
    }
    
    if(shapepiece->shape == SHAPE_RECT)
        DrawRoundRectButton(hdc, &rc, bgcolor, r);
    else if(shapepiece->shape == SHAPE_LEFT)
        DrawSharpButton(hdc, &rc, bgcolor, TRUE);
    else if(shapepiece->shape == SHAPE_RIGHT)
        DrawSharpButton(hdc, &rc, bgcolor, FALSE);
    
    return ;
}

BEGIN_MINI_CLASS(mShapeBoxPiece, mStaticPiece)
	CLASS_METHOD_MAP(mShapeBoxPiece, paint)
END_MINI_CLASS
