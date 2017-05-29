/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Feynman Software Technology Co., Ltd.
 * Room 508B-C, Floor 5, Citic Guoan Shumagang, No.32, Haidian South
 * Road, Haidian District, Beijing, P. R. CHINA 100080.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Feynman Software Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall use
 * it only in accordance you entered into with Feynman Software.
 *          http://www.minigui.com
 *
 *    FileName : datepicker.c
 *      Author : <wangxin@minigui.org>
 * Create Date : Thursday September 09, 2010
 *     Version : 0.0.0.1
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include <../include/mpad.h>

#define NDEBUG	1
#include <../include/mpaddebug.h>


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
	{NCS4PAD_BGC_PCK_MAIN, 0xFF44DD44},
	{NCS4PAD_BGC_PCK_SELECT, 0xFFDD2222},
	{ -1, 0 }
};

static NCS_RDR_INFO rdr_info =
{
    NCS4PAD_RENDERER, NCS4PAD_RENDERER, _rdr_elements
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
		NULL, //controls
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
		NULL, &rdr_info,
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
	ncs4PadInitialize();

	mDialogBox* mydlg = (mDialogBox*)ncsCreateMainWindowIndirect
									(&mymain_templ, HWND_DESKTOP);
	_c(mydlg)->doModal(mydlg, TRUE);

	ncs4PadUninitialize();
	ncsUninitialize();

	MainWindowThreadCleanup(mydlg->hwnd);
	return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

