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
 *		  http://www.minigui.com
 *
 *	FileName : switchbutton.c
 *	  Author : <wangxin@minigui.org>
 * Create Date : Monday, 23 August 2010
 *	 Version : 0.0.0.1
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
    {NCS4PAD_RENDERER, NCS4PAD_RENDERER, NULL}
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
	ncs4PadInitialize();
    SetDefaultWindowElementRenderer ("classic");

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

