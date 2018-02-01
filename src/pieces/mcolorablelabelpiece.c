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

#include "pieces/mcolorablelabelpiece.h"

static void mColorableLabelPiece_construct(mColorableLabelPiece *self, DWORD add_data)
{
	Class(mLabelPiece).construct((mLabelPiece *)self, add_data);
	self->txtColor = ncsGetElement(NULL, NCS_FGC_3DBODY);
}


static void mColorableLabelPiece_paint(mColorableLabelPiece *self, HDC hdc, mWidget *owner, DWORD add_data)
{
    RECT rcClient;
    DWORD uFormat = DT_WORDBREAK;
	//gal_pixel old_color;
    const char* str = self->str;

    if(owner == NULL || str == NULL)
        return ;

    SelectFont(hdc, GetWindowFont(owner->hwnd));
	_c(self)->getRect(self, &rcClient);

	/*old_color = */SetTextColor(hdc, ncsColor2Pixel(hdc, self->txtColor));

    if(self->align == NCS_ALIGN_CENTER)
        uFormat |= DT_CENTER ;
    else if(self->align == NCS_ALIGN_RIGHT)
        uFormat |= DT_RIGHT ;
    else
        uFormat |= DT_LEFT;

    if(self->valign == NCS_VALIGN_CENTER)
        uFormat |= DT_VCENTER ;
    else if(self->valign == NCS_VALIGN_BOTTOM)
        uFormat |= DT_BOTTOM ;
    else
        uFormat |= DT_TOP;

    if(!mLabelPiece_isAutoWrap(self))
        uFormat |= DT_SINGLELINE;

    if(!mLabelPiece_isPrefix(self))
        uFormat |= DT_NOPREFIX;
/*
 * for vertical text
 */	
	if(mLabelPiece_isWordBreak(self))
		uFormat |= DT_WORDBREAK;

    SetBkMode(hdc, BM_TRANSPARENT);
    
	DrawText(hdc, str, -1, &rcClient, uFormat);
}

static BOOL mColorableLabelPiece_setProperty(mColorableLabelPiece *self, int id, DWORD value)
{
	if(id == NCSP_CLP_PIECE_COLOR) {
		self->txtColor = value;
		return TRUE;
	}
	return Class(mLabelPiece).setProperty((mLabelPiece *)self, id, value);
}

static DWORD mColorableLabelPiece_getProperty(mColorableLabelPiece *self, int id)
{
	if(id == NCSP_CLP_PIECE_COLOR) {
		return self->txtColor;
	}
	return Class(mLabelPiece).getProperty((mLabelPiece *)self, id);
}



BEGIN_MINI_CLASS(mColorableLabelPiece, mLabelPiece)
	CLASS_METHOD_MAP(mColorableLabelPiece, construct)
	CLASS_METHOD_MAP(mColorableLabelPiece, paint)
	CLASS_METHOD_MAP(mColorableLabelPiece, setProperty)
	CLASS_METHOD_MAP(mColorableLabelPiece, getProperty)
END_MINI_CLASS
