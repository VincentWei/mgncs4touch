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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "manimation.h"
#include "mpicker.h"
#include "mcombopicker.h"

// #define NDEBUG	1
#include "mtouchdebug.h"


static inline CMBPICKITM *getEntryPickerItem(list_t *entry)
{
	return list_entry(entry, CMBPICKITM, list);
}

static void mComboPicker_construct(mComboPicker* self, DWORD addData)
{
	Class(mPanel).construct((mPanel *)self, addData);

	INIT_LIST_HEAD(&(self)->pickerList);
	self->pickerCount = 0;

	self->hMargin = 0;
	self->vMargin = 0;
	self->visLine = 5;
}

static void mComboPicker_destroy (mComboPicker *self)
{
    _M(self, removeAllPickers);
	Class(mPanel).destroy((mPanel *)self);
}

static BOOL mComboPicker_onCreate(mComboPicker* self, DWORD add)
{
    _c(self)->setProperty(self, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

	return TRUE;
}

static void mComboPicker_onPaint(mComboPicker* self, HDC hdc, const PCLIPRGN _clip)
{
	DWORD c;
	RECT rcI, rcO;
  
	GetClientRect (self->hwnd, &rcO);
	_M(self, getClientArea, &rcI);
	rcI.left += 3;
	rcI.right -= 3;
   
	c = ncsGetElement(self, NCS4TOUCH_BGC_PCK_MAIN);
   
	DrawRectRing (hdc, &rcO, &rcI, c);

	return;
}

static BOOL mComboPicker_addPicker(mComboPicker* self,
		NCS_WND_TEMPLATE* pickTplt, int idx)
{
	mPicker *child = NULL;
	CMBPICKITM *newitm = NULL;

	pickTplt->ex_style |= WS_EX_USEPARENTRDR; 

	if (NULL == (newitm = (CMBPICKITM *)calloc (1, sizeof(CMBPICKITM)))) {
		return FALSE;
	}

	if (NULL == (child = (mPicker *)ncsCreateWindowIndirect(pickTplt, self->hwnd))) {
        free (newitm);
		return FALSE;
	}

#if 0
	_M(child, setProperty, NCSP_PCK_HMARGIN, 0);
	_M(child, setProperty, NCSP_PCK_VMARGIN, 0);
#endif

	newitm->picker = child;
	newitm->pickerID = pickTplt->id;

	if (idx < 0) {
		list_add_tail(&newitm->list, &self->pickerList);
	} else if (idx == 0) {
		list_add(&newitm->list, &self->pickerList);
	} else {
		list_add_by_index(&newitm->list, &self->pickerList, idx);
	}
	self->pickerCount++;

	return TRUE;
}


static BOOL mComboPicker_removeAllPickers(mComboPicker* self)
{
	list_t *me = NULL, *n = NULL;
	CMBPICKITM *itm = NULL;

	if (self->pickerCount <= 0)
		return FALSE;

	list_for_each_safe(me, n, &self->pickerList) {
		itm = getEntryPickerItem(me);
		ncsDeleteWidget((mWidget *)itm->picker);
		list_del(&itm->list);
        free (itm);
		self->pickerCount--;
		if (self->pickerCount <= 0)
			return TRUE;
	}

	return FALSE;
}


static BOOL mComboPicker_removePicker(mComboPicker* self, int id)
{
	list_t *me = NULL, *n = NULL;
	CMBPICKITM *itm = NULL;

	if (self->pickerCount <= 0)
		return FALSE;

	list_for_each_safe(me, n, &self->pickerList) {
		itm = getEntryPickerItem(me);
		if (itm->pickerID == id) {
			ncsDeleteWidget((mWidget *)itm->picker);
			list_del(&itm->list);
            free (itm);
			self->pickerCount--;
			return TRUE;
		}
	}

	return FALSE;
}

static mComponent *mComboPicker_getChild (mComboPicker *self, int id)
{
	list_t *me = NULL;
	CMBPICKITM *itm = NULL;

	list_for_each(me, &self->pickerList) {
		itm = getEntryPickerItem(me);
		if (itm->pickerID == id) {
			return (mComponent *)itm->picker;
		}
	}
    
    return Class(mPanel).getChild((mPanel *)self, id);
}

static BOOL mComboPicker_removePickerByIdx(mComboPicker* self, int idx)
{
	int i = 0;
	list_t *me = NULL, *n = NULL;
	CMBPICKITM *itm = NULL;

	if (self->pickerCount <= 0 || self->pickerCount < idx)
		return FALSE;

	list_for_each_safe(me, n, &self->pickerList) {
		itm = getEntryPickerItem(me);
		if (i == idx)
		{
			ncsDeleteWidget((mWidget *)itm->picker);
			list_del(&itm->list);
            free (itm);
			self->pickerCount--;
			return TRUE;
		}
		i++;
	}

	return FALSE;
}

static void mComboPicker_getClientArea(mComboPicker* self, PRECT prc)
{
	GetClientRect (self->hwnd, prc);
	prc->left   += self->hMargin;
	prc->right  -= self->hMargin;
	prc->top    += self->vMargin;
	prc->bottom -= self->vMargin;
}

static BOOL mComboPicker_setProperty(mComboPicker* self, int id, DWORD value)
{
    if (id > NCSP_CMBPCK_MAX)
        return FALSE;
	
    switch (id)
    {
        case NCSP_WIDGET_RDR:
           if (strcmp((char*)value, NCS4TOUCH_RENDERER) != 0)
               return FALSE;
           else 
               break;
	
        case NCSP_CMBPCK_HMARGIN:
			self->hMargin = (int)value;
            InvalidateRect(self->hwnd, NULL, TRUE);
			return TRUE;

		case NCSP_CMBPCK_VMARGIN:
			self->vMargin = (int)value;
            InvalidateRect(self->hwnd, NULL, TRUE);
			return TRUE;

		case NCSP_CMBPCK_VISLINE:
			{
				list_t *me = NULL;
				CMBPICKITM *pickitm = NULL;
				
				if ((int)value % 2 == 0)
					 return FALSE;

				self->visLine = (int)value;

				list_for_each(me, &self->pickerList) {
					 pickitm = getEntryPickerItem(me);
					 if (self->visLine <= 
					         _M(pickitm->picker, getProperty, NCSP_PCK_COUNT))
					 {
					     _M(pickitm->picker, setProperty, NCSP_PCK_LINES, self->visLine);
					 }
				}

				return TRUE;
			}
		default:
			break;
	}

	return Class(mPanel).setProperty((mPanel *)self, id, value);
}

static DWORD mComboPicker_getProperty (mComboPicker *self , int id)
{    
	if (id > NCSP_CMBPCK_MAX)
		return FALSE;

	switch (id) {
		case NCSP_CMBPCK_HMARGIN:
			return (DWORD)self->hMargin;
		case NCSP_CMBPCK_VMARGIN:
			return (DWORD)self->vMargin;
		case NCSP_CMBPCK_VISLINE:
			return (DWORD)self->visLine;
	}

	return Class(mPanel).getProperty((mPanel *)self, id);
}

static BOOL mComboPicker_listenerCallBack(mObject* self, 
		mObject* sender, int eventId, DWORD param)
{
	return FALSE;
}

#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
static BOOL mComboPicker_refresh(mComboPicker* self)
{
	list_t *me = NULL;
	CMBPICKITM *pickitm = NULL;

	if (self->pickerCount <= 0)
		return TRUE;

	list_for_each(me, &self->pickerList) {
		pickitm = getEntryPickerItem(me);
		_M(pickitm->picker, refresh);
	}
	return TRUE;
}
#endif

BEGIN_CMPT_CLASS(mComboPicker, mPanel)
	CLASS_METHOD_MAP(mComboPicker, construct)
	CLASS_METHOD_MAP(mComboPicker, destroy)
	CLASS_METHOD_MAP(mComboPicker, onCreate)
	CLASS_METHOD_MAP(mComboPicker, onPaint)
	CLASS_METHOD_MAP(mComboPicker, addPicker)
	CLASS_METHOD_MAP(mComboPicker, removePicker)
	CLASS_METHOD_MAP(mComboPicker, removePickerByIdx)
	CLASS_METHOD_MAP(mComboPicker, removeAllPickers)
	CLASS_METHOD_MAP(mComboPicker, getChild)
	CLASS_METHOD_MAP(mComboPicker, setProperty)
	CLASS_METHOD_MAP(mComboPicker, getProperty)
	CLASS_METHOD_MAP(mComboPicker, getClientArea)
	CLASS_METHOD_MAP(mComboPicker, listenerCallBack)
#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
	CLASS_METHOD_MAP(mComboPicker, refresh)
#endif
END_CMPT_CLASS

