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

#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
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
	
    bgcolor = ncsGetElement(owner, NCS4TOUCH_BGC_BNB_BUTTON); 
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
