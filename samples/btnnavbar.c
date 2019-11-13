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
 * \file btnnavbar.c
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include <mgncs4touch/mgncs4touch.h>
// #define NDEBUG	1
#include <mgncs4touch/mtouchdebug.h>

#define ID_TLC1	 101
#define ID_TLC2	 102
#define ID_TLC3	 103
#define ID_TLC4	 104
#define ID_TLC5	 105


static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	mBtnNavBar* tl2 = (mBtnNavBar*)ncsGetChildObj(self->hwnd, ID_TLC2);
	if (tl2) {
		_c(tl2)->setProperty(tl2, NCSP_BNB_MARGIN, 20);
		_c(tl2)->setProperty(tl2, NCSP_BNB_BTN_H, 25);
		_c(tl2)->setProperty(tl2, NCSP_BNB_LSTRING, (DWORD)"Backward");
		_c(tl2)->setProperty(tl2, NCSP_BNB_RSTRING, (DWORD)"Forward");
	}

	mBtnNavBar* tl3 = (mBtnNavBar*)ncsGetChildObj(self->hwnd, ID_TLC3);
	if (tl3) {
		_c(tl3)->setProperty(tl3, NCSP_BNB_MARGIN, 6);
		_c(tl3)->setProperty(tl3, NCSP_BNB_LSTRING, (DWORD)"Back");
		_c(tl3)->setProperty(tl3, NCSP_BNB_RSTRING, (DWORD)"Save");
	}

	mBtnNavBar* tl4 = (mBtnNavBar*)ncsGetChildObj(self->hwnd, ID_TLC4);
	if (tl4) {
		_c(tl4)->setProperty(tl4, NCSP_BNB_MARGIN, 6);
		_c(tl4)->setProperty(tl4, NCSP_BNB_LSTRING, (DWORD)"Left");
	}

    mBtnNavBar* tl5 = (mBtnNavBar*)ncsGetChildObj(self->hwnd, ID_TLC5);
	if (tl5) {
		_c(tl5)->setProperty(tl5, NCSP_BNB_MARGIN, 6);
		_c(tl5)->setProperty(tl5, NCSP_BNB_RSTRING, (DWORD)"Right");
	}

	return TRUE;
}


static void mymain_onDestroy(mWidget* self, int message)
{
    //TODO
}


static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}


static void click_notify(mWidget *self, int id, int nc, DWORD add_data)
{
	LOGE("add_data is %d \n", (int)add_data);
}


static NCS_EVENT_HANDLER multi_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_BNB_BTNCLICK, click_notify),
	{0, NULL}
};

static NCS_RDR_ELEMENT _rdr_elements[] =
{
    { WE_FGC_ACTIVE_WND_BORDER, 0xFFDDFF00},
    { WE_FGC_INACTIVE_WND_BORDER, 0xFFFF00DD},
    { NCS4TOUCH_BGC_BNB_BUTTON, 0xFF0000FF},
    { NCS4TOUCH_FGC_BNB_BUTTON, 0xFFFF0000},
    { NCS4TOUCH_FGC_BNB_TEXT, 0xFF00FF00},
    { -1, 0 }
};

static NCS_RDR_INFO rdr_info =
{
    NCS4TOUCH_RENDERER, NCS4TOUCH_RENDERER, _rdr_elements
};


//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_BTNNAVBAR,
		ID_TLC1,
		12, 10, 240, 40,
		WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"navigation",
		NULL,
		NULL,//rdr_info
		multi_handlers,//handlers,
		NULL,
		0,
		0	//add data
	},
	{
		NCSCTRL_BTNNAVBAR,
		ID_TLC2,
		12, 70, 380, 40,
		WS_VISIBLE | NCSS_BNB_LOPT | NCSS_BNB_ROPT | NCSS_NOTIFY,
		WS_EX_NONE,
		"navigation",
		NULL,
		&rdr_info,
		multi_handlers, //handlers,
		NULL,
		0,
		0 //add data
	},
	{
		NCSCTRL_BTNNAVBAR,
		ID_TLC3,
		12, 130, 240, 40,
		WS_VISIBLE | NCSS_BNB_LOPT | NCSS_BNB_RRECT | NCSS_NOTIFY,
		WS_EX_NONE,
		"navigation",
		NULL,
		NULL, //rdr_info
		multi_handlers, //handlers,
		NULL,
		0,
		0 //add data
	},
	{
		NCSCTRL_BTNNAVBAR,
		ID_TLC4,
		12, 190, 240, 40,
		WS_VISIBLE | NCSS_BNB_LOPT | NCSS_NOTIFY,
		WS_EX_NONE,
		"navigation",
		NULL, //props,
		NULL, //rdr_info
		multi_handlers, //handlers,
		NULL,
		0,
		0 //add data
	},
	{
		NCSCTRL_BTNNAVBAR,
		ID_TLC5,
		12, 250, 240, 40,
		WS_VISIBLE | NCSS_BNB_ROPT | NCSS_NOTIFY,
		WS_EX_NONE,
		"navigation",
		NULL, //props,
		NULL, //rdr_info
		multi_handlers, //handlers,
		NULL,
		0,
		0 //add data
	},
};


static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{MSG_DESTROY, mymain_onDestroy},
	{0, NULL }
};


//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_MAINWND,
	1,
	264, 60, 400, 480,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
	"BtnNavBar Test ....",
	NULL,
	NULL,
	mymain_handlers,
	_ctrl_templ,
	sizeof(_ctrl_templ) / sizeof(NCS_WND_TEMPLATE),
	0,
	0xFF3DFFDD,
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

