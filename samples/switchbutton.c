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
 * \file switchbutton.c
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

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include <mgncs4touch/mgncs4touch.h>
// #define NDEBUG	1
#include <mgncs4touch/mtouchdebug.h>

#define ID_BTN1     101

static BOOL update_time(mSwitchButton *listener, 
        mTimer* sender, int id, DWORD total_count)
{
    static int s = 0;
    DWORD c = random() | 0xFF000000;
    ncsSetElement(listener, NCS4TOUCH_BGC_BLOCK, c);
    LOGE("NCS4TOUCH_BGC_BLOCK :: %d\n", NCS4TOUCH_BGC_BLOCK);
    _M(listener, setProperty, NCSP_SWB_STATUS, s = (s == 0 ? 1 : 0));
    InvalidateRect(listener->hwnd, NULL, TRUE);
    return TRUE;
}

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	mSwitchButton *msb = NULL;
    mTimer * timer = SAFE_CAST(mTimer, _c(self)->getChild(self, 700));
    
    if (NULL != (msb = (mSwitchButton *)ncsGetChildObj(self->hwnd, ID_BTN1))) {
        _M(msb, setProperty, NCSP_SWB_STATUS, NCS_SWB_OFF);
    } else {
        LOGE("---- Get Switch Button Error. \n");
    }
    
    if (timer) {
		ncsAddEventListener((mObject*)timer, 
                (mObject*)ncsGetChildObj(self->hwnd, 101), 
                (NCS_CB_ONPIECEEVENT)update_time, MSG_TIMER);
		//_c(timer)->start(timer);
	}
    srand((int)time(0));

	return TRUE;
}

static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static void btn_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    printf ("switch button (%d, %d) notify : %s\n", id, nc, add_data == 0 ? "OFF" : "ON");
}

static NCS_EVENT_HANDLER btn_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_SWB_STATUSCHANGED, btn_notify),
	{0, NULL}	
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = 
{
	{
		NCSCTRL_SWBUTTON, 
		ID_BTN1,
		20, 20, 80, 30,
		WS_VISIBLE,
		WS_EX_NONE,
		"I\nO",
		NULL, //props,
		NULL, //rdr_info
		btn_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},	
    {
		NCSCTRL_SWBUTTON, 
		ID_BTN1+1,
		20, 70, 100, 50,
		WS_VISIBLE,
		WS_EX_NONE,
		"ON\nOFF",
		NULL, //props,
		NULL, //rdr_info
		btn_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
    {
		NCSCTRL_SWBUTTON, 
		ID_BTN1+2,
		20, 140, 150, 50,
		WS_VISIBLE,
		WS_EX_NONE,
		"1\n0",
		NULL, //props,
		NULL, //rdr_info
		btn_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
    {
		NCSCTRL_SWBUTTON, 
		ID_BTN1+3,
		20, 210, 150, 50,
		WS_VISIBLE,
		WS_EX_NONE,
		"OPEN\nCLOSE",
		NULL, //props,
		NULL, //rdr_info
		btn_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},	
    {
		NCSCTRL_TIMER, 
		700,
		10, 10, 0, 0,
		WS_BORDER | WS_VISIBLE,
		WS_EX_NONE,
		"",
		NULL, //props,
		NULL, //rdr_info
		NULL, //timer_props, //handlers,
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
	160, 164, 480, 320,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "SwitchButton Test ......",
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

