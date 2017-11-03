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
    { NCS4PAD_BGC_BNB_BUTTON, 0xFF0000FF},
    { NCS4PAD_FGC_BNB_BUTTON, 0xFFFF0000},
    { NCS4PAD_FGC_BNB_TEXT, 0xFF00FF00},
    { -1, 0 }
};

static NCS_RDR_INFO rdr_info =
{
    NCS4PAD_RENDERER, NCS4PAD_RENDERER, _rdr_elements
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

