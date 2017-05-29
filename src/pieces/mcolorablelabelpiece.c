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
	gal_pixel old_color;
    const char* str = self->str;

    if(owner == NULL || str == NULL)
        return ;

    SelectFont(hdc, GetWindowFont(owner->hwnd));
	_c(self)->getRect(self, &rcClient);

	old_color = SetTextColor(hdc, ncsColor2Pixel(hdc, self->txtColor));

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
