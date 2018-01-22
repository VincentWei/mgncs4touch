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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>

#include "mpadrdr.h"


static const char* pad_name = NCS4PAD_RENDERER;


static void pad_uninit(mWidgetRenderer* rdr)
{
	if (NULL != rdr->prv_data) {
		free(rdr->prv_data);
		rdr->prv_data = NULL;
	}
}


static void pad_drawFocusFrame(mWidget* self, HDC hdc, const RECT* rc)
{
	ncsCommRDRDrawFocusFrame(self->hwnd, hdc, rc);
}


static BOOL pad_drawBkground(mWidget* self, HDC hdc, const RECT* inv_rc)
{
	return ncsCommRDRDrawBkgnd(self->hwnd, hdc, inv_rc);
}


static BOOL pad_init(mWidgetRenderer* rdr)
{
	return TRUE;
}


static void pad_draw3dbox(mWidget* self, HDC hdc, const RECT* rc, DWORD color, DWORD flag)
{
	ncsCommRDRDraw3dbox(hdc, rc, color, flag);
}


static void pad_drawCheckbox(mWidget* self, HDC hdc, const RECT* rc, DWORD flag)
{
	DWORD fgcolor = ncsGetElement(self, NCS_FGC_WINDOW);
	DWORD bgcolor = ncsGetElement(self, NCS_BGC_WINDOW);
	DWORD discolor = ncsGetElement(self, NCS_FGC_DISABLED_ITEM);

	ncsCommRDRDrawCheckbox(hdc, rc, fgcolor, bgcolor, discolor, flag);
}


static void pad_drawRadio(mWidget* self, HDC hdc, const RECT* rc, DWORD flag)
{
	DWORD fgcolor = ncsGetElement(self, NCS_FGC_WINDOW);
	DWORD bgcolor = ncsGetElement(self, NCS_BGC_WINDOW);
	DWORD discolor = ncsGetElement(self, NCS_FGC_DISABLED_ITEM);

	ncsCommRDRDrawRadio(hdc, rc, fgcolor, bgcolor, discolor, flag);
}


static void pad_drawArrow(mWidget* self, HDC hdc,
        const RECT* rc, int arrow,DWORD basiccolor, DWORD flag)
{
	ncsCommRDRDrawArrow(hdc, rc, arrow, basiccolor, flag&NCSRF_FILL);
}


static void pad_drawItem(mWidget* self, HDC hdc, const RECT* rc, DWORD flag)
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


static void pad_class_init(mWidgetRenderer* rdr)
{
	rdr->rdr_name       = pad_name;
	rdr->drawFocusFrame = pad_drawFocusFrame;
	rdr->drawBkground   = pad_drawBkground;
	rdr->draw3dbox      = pad_draw3dbox;
	rdr->drawCheckbox   = pad_drawCheckbox;
	rdr->drawRadio      = pad_drawRadio;
	rdr->drawArrow      = pad_drawArrow;
	rdr->drawItem       = pad_drawItem;
}


mWidgetRenderer pad_widget_renderer = {
	"",
	pad_class_init,
	NULL,
	pad_init,
	pad_uninit,
};

