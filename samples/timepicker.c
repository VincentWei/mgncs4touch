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
 * \file timerpicker.c
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

#define ID_TPICKER     101

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	mTimePicker* pick = NULL;
	
    if (NULL != (pick = (mTimePicker*)ncsGetChildObj(self->hwnd, ID_TPICKER))) {
        _M(pick, setTime, 10, 54);
	} else {
	    LOGE("ERROR ---- Get mTimePicker Error. \n");
	}

    if (NULL != (pick = (mTimePicker*)ncsGetChildObj(self->hwnd, ID_TPICKER+1))) {
        //_M(pick, setTime, 14, 54);
        _M(pick, setProperty, NCSP_TPCK_AMPMSTRINGS, (DWORD)"Forenoon\nAfternoon");
	} else {
	    LOGE("ERROR ---- Get mTimePicker Error. \n");
	}
	return TRUE;
}


static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static void timechanged_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    int h, m;
    _M((mTimePicker *)self, getTime, &h, &m);
	LOGE("NCSN_TPCK_TIMECHANGED:  %d : %d \n", h, m);
}

static NCS_EVENT_HANDLER multi_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_TPCK_TIMECHANGED, timechanged_notify),
	{0, NULL}
};

static NCS_RDR_ELEMENT _rdr_elements[] = 
{
	{NCS4TOUCH_BGC_PCK_MAIN, 0xFF44DD44},
	{NCS4TOUCH_BGC_PCK_SELECT, 0xFFDD2222},
	{ -1, 0 }
};

static NCS_RDR_INFO rdr_info =
{
    NCS4TOUCH_RENDERER, NCS4TOUCH_RENDERER, _rdr_elements
};

static NCS_PROP_ENTRY _props [] = {
	{NCSP_CMBPCK_HMARGIN, 10},
	{NCSP_CMBPCK_VMARGIN, 10},
	{0, 0}
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_TIMEPICKER,
		ID_TPICKER,
		10, 20, 240, 200,
		WS_VISIBLE | NCSS_TPCK_24H | NCSS_NOTIFY,
		WS_EX_NONE,
		"timepicker",
		_props,
		&rdr_info,
		multi_handlers, //handlers,
		0, //controls
		0,
		0 //add data
	},
	{
		NCSCTRL_TIMEPICKER,
		ID_TPICKER+1,
		10, 240, 240, 200,
		WS_VISIBLE | NCSS_TPCK_12H | NCSS_NOTIFY,
		WS_EX_NONE,
		"timepicker",
		_props,
		&rdr_info,
		multi_handlers, //handlers,
		0, //controls
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
	264, 60, 272, 480,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
	"mTimePicker Test ...",
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

