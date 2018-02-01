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

#include "mtouchrdr.h"

static const char* touch_name = NCS4TOUCH_RENDERER;

static void touch_uninit(mWidgetRenderer* rdr)
{
	if (NULL != rdr->prv_data) {
		free(rdr->prv_data);
		rdr->prv_data = NULL;
	}
}


static void touch_drawFocusFrame(mWidget* self, HDC hdc, const RECT* rc)
{
	ncsCommRDRDrawFocusFrame(self->hwnd, hdc, rc);
}


static BOOL touch_drawBkground(mWidget* self, HDC hdc, const RECT* inv_rc)
{
	return ncsCommRDRDrawBkgnd(self->hwnd, hdc, inv_rc);
}


static BOOL touch_init(mWidgetRenderer* rdr)
{
	return TRUE;
}


static void touch_draw3dbox(mWidget* self, HDC hdc, const RECT* rc, DWORD color, DWORD flag)
{
	ncsCommRDRDraw3dbox(hdc, rc, color, flag);
}


static void touch_drawCheckbox(mWidget* self, HDC hdc, const RECT* rc, DWORD flag)
{
	DWORD fgcolor = ncsGetElement(self, NCS_FGC_WINDOW);
	DWORD bgcolor = ncsGetElement(self, NCS_BGC_WINDOW);
	DWORD discolor = ncsGetElement(self, NCS_FGC_DISABLED_ITEM);

	ncsCommRDRDrawCheckbox(hdc, rc, fgcolor, bgcolor, discolor, flag);
}


static void touch_drawRadio(mWidget* self, HDC hdc, const RECT* rc, DWORD flag)
{
	DWORD fgcolor = ncsGetElement(self, NCS_FGC_WINDOW);
	DWORD bgcolor = ncsGetElement(self, NCS_BGC_WINDOW);
	DWORD discolor = ncsGetElement(self, NCS_FGC_DISABLED_ITEM);

	ncsCommRDRDrawRadio(hdc, rc, fgcolor, bgcolor, discolor, flag);
}


static void touch_drawArrow(mWidget* self, HDC hdc,
        const RECT* rc, int arrow,DWORD basiccolor, DWORD flag)
{
	ncsCommRDRDrawArrow(hdc, rc, arrow, basiccolor, flag&NCSRF_FILL);
}


static void touch_drawItem(mWidget* self, HDC hdc, const RECT* rc, DWORD flag)
{
	int id = 0;
	DWORD color = 0;

	switch (NCSR_STATUS(flag)) {
		case NCSRS_HIGHLIGHT:
			id = NCS_BGC_HILIGHT_ITEM;
			break;

		case NCSRS_SELECTED:
			id = NCS_BGC_SELECTED_ITEM;
			break;

		case NCSRS_DISABLE:
			id = NCS_BGC_DISABLED_ITEM;
			break;

		case NCSRS_SIGNIFICANT:
		default:
			id = NCS_BGC_WINDOW;
	}

	color = ncsGetElement(self, id);

	ncsCommRDRDrawItem(hdc, rc, color);
}


static void touch_class_init(mWidgetRenderer* rdr)
{
	rdr->rdr_name       = touch_name;
	rdr->drawFocusFrame = touch_drawFocusFrame;
	rdr->drawBkground   = touch_drawBkground;
	rdr->draw3dbox      = touch_draw3dbox;
	rdr->drawCheckbox   = touch_drawCheckbox;
	rdr->drawRadio      = touch_drawRadio;
	rdr->drawArrow      = touch_drawArrow;
	rdr->drawItem       = touch_drawItem;
}


mWidgetRenderer _ncs_touch_widget_renderer = {
	"",
	touch_class_init,
	NULL,
	touch_init,
	touch_uninit,
};

