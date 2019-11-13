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
 * \file picker.c
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

#define ID_PICKER     101
#define ID_PICKER1    102

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	mPicker* pick = NULL;
	const char* str[] = {"MiniGUI", "MGUtils","MGPlus","MGNCS","MStduio",};

    if (NULL != (pick = (mPicker*)ncsGetChildObj(self->hwnd, ID_PICKER))) {
        _M(pick, setProperty, NCSP_PCK_NUMBERBITS, 3);
		_M(pick, disableItems, 6, -1);
    } else {
        LOGE("---- Get Switch Button Error. \n");
    }

    if (NULL != (pick = (mPicker*)ncsGetChildObj(self->hwnd, ID_PICKER1))) {
        SetWindowText (pick->hwnd, "M1\nM2\nM3\n\n\n\nM4\n");
		//_c(pick)->removeAll(pick);
		_M(pick, addItems, str, TABLESIZE(str));
		_M(pick, disableItems, 3, 2);

		_M(pick, setProperty, NCSP_PCK_HMARGIN, (DWORD)30);
		_M(pick, setProperty, NCSP_PCK_VMARGIN, (DWORD)20);
    } else {
        LOGE("---- Get Switch Button Error. \n");
    }

	return TRUE;
}

static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static NCS_RDR_INFO _rdr_info[] = {
    {NCS4TOUCH_RENDERER, NCS4TOUCH_RENDERER, NULL}
};

static void selchanged_notify(mWidget* self, int id, int nc, DWORD add_data)
{
	LOGE("NCSN_PCK_SELCHANGED: selected index [%d] -- TEXT[%s]\n",
            (int)add_data, _M((mPicker*)self, getTextByIdx, (int)add_data));
}

static void reachmin_notify(mWidget* self, int id, int nc, DWORD add_data)
{
	LOGE("NCSN_PCK_REACHMIN:add_data is %d \n", (int)add_data);
}

static void reachmax_notify(mWidget* self, int id, int nc, DWORD add_data)
{
	LOGE("NCSN_PCK_REACHMAX:add_data is %d \n", (int)add_data);
}

static NCS_EVENT_HANDLER multi_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_PCK_SELCHANGED, selchanged_notify),
    NCS_MAP_NOTIFY(NCSN_PCK_REACHMIN, reachmin_notify),
    NCS_MAP_NOTIFY(NCSN_PCK_REACHMAX, reachmax_notify),
	{0, NULL}
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_PICKER,
		ID_PICKER,
		40, 20, 100, 200,
		WS_VISIBLE | NCSS_PCK_NUMBER | NCSS_PCK_BRC | NCSS_NOTIFY,
		WS_EX_TRANSPARENT,
		"picker",
		NULL, //props,
		_rdr_info, //rdr_info
		multi_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
    {
		NCSCTRL_PICKER,
		ID_PICKER1,
		150, 20, 150, 200,
		WS_VISIBLE | NCSS_PCK_STRINGS | NCSS_PCK_BRC | NCSS_NOTIFY,
		WS_EX_TRANSPARENT,
		"picker\np0\np1\np2\np3",
		NULL, //props,
		_rdr_info, //rdr_info
		multi_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
};

static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{0, NULL}
};

//define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_MAINWND,
	1,
	160, 164, 400, 400,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "mPicker Test ......",
	NULL,
	NULL,
	mymain_handlers,
	_ctrl_templ,
	sizeof(_ctrl_templ) / sizeof(NCS_WND_TEMPLATE),
	0,
	0,
	0,
};

int MiniGUIMain(int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer (NAME_DEF_LAYER, NULL, 0, 0);
#endif
	printf(" sytle = %x\n", NCSS_PCK_BRC );
	
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

