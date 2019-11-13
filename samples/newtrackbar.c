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
 * \file newtrackbar.c
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
#define NDEBUG	1
#include <mgncs4touch/mtouchdebug.h>

#define ID_TRB1	 101
#define ID_TRB2	 102
#define ID_TRB3	 103
#define ID_TRB4	 104

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	mNewTrackBar* tb = (mNewTrackBar*)ncsGetChildObj(self->hwnd, ID_TRB1);
	int max = _c(tb)->getProperty(tb, NCSP_SLIDER_MAXPOS);
	int min = _c(tb)->getProperty(tb, NCSP_SLIDER_MINPOS);
	int cur = _c(tb)->getProperty(tb, NCSP_SLIDER_CURPOS);
	LOGE("tb = %p, max : %d, min : %d, cur : %d\n", tb, max, min, cur);

	return TRUE;
}

static void mymain_onDestroy(mWidget* self, int message)
{
}


static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}


static NCS_PROP_ENTRY trk_props [] = {
	{NCSP_TRKBAR_MINPOS, 0},
	{NCSP_TRKBAR_MAXPOS, 25},
	{NCSP_TRKBAR_CURPOS, 0},
	{NCSP_TRKBAR_LINESTEP, 5},
	{NCSP_TRKBAR_PAGESTEP, 5},
	{NCSP_TRKBAR_THUMB_WIDTH,  20},
	{NCSP_TRKBAR_THUMB_HEIGHT, 20},
	{0, 0}
};

static NCS_RDR_INFO _rdr_info[] =
{
    {NCS4TOUCH_RENDERER, NCS4TOUCH_RENDERER, NULL}
};


// Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_NEWTRACKBAR,
		ID_TRB1,
		2, 10, 250, 40,
		WS_VISIBLE | NCSS_TRKBAR_HORIZONTAL | NCSS_NOTIFY,
		WS_EX_TRANSPARENT | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS,
		"",
		trk_props,  //props,
	    _rdr_info,
		NULL,	    //trk1_handlers,  //handlers,
		NULL,       //controls
		0,
		0			//add data
	},
	{
		NCSCTRL_NEWTRACKBAR,
		ID_TRB2,
		2, 70, 250, 40,
		WS_VISIBLE | NCSS_NOTIFY | NCSS_NTRKBAR_CIRCLE,
		WS_EX_TRANSPARENT | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS,
		"",
		trk_props, //props,
		NULL, //track_rdr_info, //rdr_info
		NULL, //trk2_handlers,  //handlers,
		NULL, //controls
		0,
		0     //add data
	},
	{
		NCSCTRL_NEWTRACKBAR,
		ID_TRB3,
		16, 130, 40, 260,
		WS_VISIBLE | NCSS_TRKBAR_VERTICAL | NCSS_NOTIFY,
		WS_EX_TRANSPARENT | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS,
		"",
		trk_props, //props,
		NULL, //track_rdr_info, //rdr_info
		NULL, //trk2_handlers,  //handlers,
		NULL, //controls
		0,
		0     //add data
	},
	{
		NCSCTRL_NEWTRACKBAR,
		ID_TRB4,
		116, 130, 40, 260,
		WS_VISIBLE | NCSS_TRKBAR_VERTICAL | NCSS_NOTIFY | NCSS_NTRKBAR_CIRCLE,
		WS_EX_TRANSPARENT | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS,
		"",
		trk_props, //props,
		NULL, //track_rdr_info, //rdr_info
		NULL, //trk2_handlers,  //handlers,
		NULL, //controls
		0,
		0     //add data
	},
};


static NCS_EVENT_HANDLER mymain_handlers[] = {
	{MSG_CREATE, mymain_onCreate},
	{MSG_CLOSE, mymain_onClose},
	{MSG_DESTROY, mymain_onDestroy},
	{0, NULL }
};


// define the main window template
static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_MAINWND, 
	1,
	264, 60, 272, 480,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
	"NewTrackbar Test ....",
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
    SetDefaultWindowElementRenderer ("classic");

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

