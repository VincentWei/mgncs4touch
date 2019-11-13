///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
 * \file comm_draw.c
 * \author FMSoft
 * \date 2010/10/09
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/blog/minigui-licensing-policy/>.

 \endverbatim
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include <mgncs4touch/mgncs4touch.h>
// #define NDEBUG	1
#include <mgncs4touch/mtouchdebug.h>

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
    srand((int)time(0));
	return TRUE;
}


static void mymain_onPaint(mWidget* self, HDC hdc, const PCLIPRGN clip_rgn)
{
	RECT rc;
	DWORD c = 0;
    int i = 0;
	int r = 3;

    ///////////////////////////////////////////////////////////
    TextOut(hdc, 20, 40, "DrawGradientRroundRect(...)");
	rc = (RECT){20, 70, 180, 100};

	for (i = 0; i < 7; i++) {
		c = random() | 0xFF000000;
		DrawGradientRroundRect (hdc, c, &rc, random()%10, random()%1);
		rc.top += 40;
		rc.bottom += 40;
	}

    ///////////////////////////////////////////////////////////
    TextOut(hdc, 250, 40, "DrawPickerRect(...)");
	rc = (RECT){250, 70, 295, 300};

	for (i = 0; i < 3; i++) {
		c = random() | 0xFF000000;
		DrawPickerRect (hdc, &rc, c, 0xFFFFFFFF, ECT_BOTH);
		rc.left += 50;
		rc.right += 50;
	}

    ///////////////////////////////////////////////////////////
    TextOut(hdc, 450, 40, "DrawSharpButton(...)");
	rc = (RECT){450, 70, 550, 100};

	for (i = 0; i < 7; i++) {
		c = random() | 0xFF000000;
		DrawSharpButton (hdc, &rc, c, TRUE);
		rc.top += 40;
		rc.bottom += 40;
	}

    ///////////////////////////////////////////////////////////
    TextOut(hdc, 250, 320, "TickAndAngle(...)");
	rc = (RECT){260, 360, 260+50, 360+50};
	c = 0xff991f22;
	DrawTouchTick(hdc, &rc, c);
	rc = (RECT){320, 360, 320+50, 360+50};
	// c = random() | 0xFF000000;
	c = 0xff991f22;
	DrawTouchAngle(hdc, &rc, c);

	// draw circle
    TextOut(hdc, 380, 360, "Draw3DCircle(...)");
	rc = (RECT){380, 380, 380+32, 380+32};
	c = random() | 0xFF000000;
	Draw3DCircle(hdc, &rc, c);

	// draw circle
    TextOut(hdc, 20, 400, "DrawRoundRectButton(...)");
	c = random() | 0xFF000000;
	rc = (RECT){20, 430, 20 + 80, 430 + 30};
	DrawRoundRectButton(hdc, &rc, c, r);
}


static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}


static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{MSG_PAINT, mymain_onPaint},
	{0, NULL }
};


//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_MAINWND,
	1,
	0, 0, 800, 600,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
	"Pcomm Test ....",
	NULL,
	NULL,
	mymain_handlers,
	NULL,
	0,
	0,
	0,
	0,
};


int MiniGUIMain(int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
	JoinLayer (NAME_DEF_LAYER, NULL, 0, 0);
#endif
	
	ncsInitialize();
	ncs4TouchInitialize();

	mDialogBox* mydlg = (mDialogBox*)ncsCreateMainWindowIndirect
									(&mymain_templ, HWND_DESKTOP);
	_c(mydlg)->doModal(mydlg, TRUE);

	ncs4TouchUninitialize();
	ncsUninitialize();

	return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

