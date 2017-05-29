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
 *    FileName : newstatic.c
 *      Author : <wangxin@minigui.org>
 * Create Date : Saturday September 25, 2010
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


#define NST_ID1	 101
#define NST_ID2	 102
#define NST_ID3	 103
#define NST_ID4	 104


static BITMAP bmpLimg;

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	if (LoadBitmapFromFile(HDC_SCREEN, &bmpLimg, "res/bmpLimg.png") != 0) {
		LOGE("cannot load image file \"bmpLimg.png\"\n");
	}

	LOGELINE();
	mItemBar* tb1 = (mItemBar*)ncsGetChildObj(self->hwnd, NST_ID1);
	if (tb1) {
		_c(tb1)->setProperty(tb1, NCSP_ITEMBAR_IMG, (DWORD)&bmpLimg);
	}

	mItemBar* tb2 = (mItemBar*)ncsGetChildObj(self->hwnd, NST_ID2);
	if (tb2) {
		_c(tb2)->setProperty(tb2, NCSP_ITEMBAR_IMG, (DWORD)&bmpLimg);
	}

	mItemBar* tb3 = (mItemBar*)ncsGetChildObj(self->hwnd, NST_ID3);
	if (tb3) {
		_c(tb3)->setProperty(tb3, NCSP_ITEMBAR_IMG, (DWORD)&bmpLimg);
		_c(tb3)->setProperty(tb3, NCSP_ITEMBAR_MARGIN, (DWORD)6);
	}

	return TRUE;
}

static void mymain_onDestroy(mWidget* self, int message)
{
    UnloadBitmap(&bmpLimg);
}


static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static NCS_RDR_ELEMENT _rdr_elements[] = 
{
	{ WE_FGC_ACTIVE_WND_BORDER, 0xFFDDFF00},
	{ WE_FGC_INACTIVE_WND_BORDER, 0xFFFF00DD},
	{ WE_METRICS_WND_BORDER, 3},
	{ NCS4PAD_FGC_ITB_LTEXT, 0xFF0000FF},
	{ NCS4PAD_FGC_ITB_RTEXT, 0xFFFF0000},
	{ -1, 0 }
};

static NCS_RDR_INFO rdr_info =
{
    NCS4PAD_RENDERER, NCS4PAD_RENDERER, _rdr_elements
};

static void clicked_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    LOGE("NCSN_ITEMBAR_CLICK: add_data is %d \n", (int)add_data);
}

static void checked_notify(mWidget *self, int id, int nc, DWORD add_data)
{
    LOGE("NCSN_ITEMBAR_CHECKED: add_data is %d \n", (int)add_data);
}

static NCS_EVENT_HANDLER _handlers [] = {
    NCS_MAP_NOTIFY(NCSN_WIDGET_CLICKED, clicked_notify),
    NCS_MAP_NOTIFY(NCSN_ITEMBAR_CHECKED, checked_notify),
	{0, NULL}
};


//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_ITEMBAR,
		NST_ID1,
		80, 10, 320, 40,
		WS_VISIBLE | NCSS_NOTIFY | NCSS_ITEMBAR_CHECKABLE | NCSS_ITEMBAR_HASCHILD,
		WS_EX_TROUNDCNS | WS_EX_BROUNDCNS | WS_EX_TRANSPARENT,
		"Left\nRight",
		NULL,      //props,
		&rdr_info,
		_handlers,	   //trk1_handlers,  //handlers,
		NULL,	   //controls
		0,
		0	 	   //add data
	},
	{
		NCSCTRL_ITEMBAR,
		NST_ID2,
		80, 70, 320, 40,
		WS_VISIBLE | NCSS_NOTIFY | NCSS_ITEMBAR_HASCHILD,
		WS_EX_TROUNDCNS | WS_EX_TRANSPARENT,
		"MainTitle\nSubTitle",
		NULL, //props,
		&rdr_info,
		_handlers, //trk2_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
	{
		NCSCTRL_ITEMBAR,
		NST_ID3,
		80, 110, 320, 40,
		WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"Title\nDetail",
		NULL, //props,
		&rdr_info,
		_handlers, //trk2_handlers, //handlers,
		NULL, //controls
		0,
		0 //add data
	},
	{
		NCSCTRL_ITEMBAR,
		NST_ID4,
		80, 150, 320, 40,
		WS_VISIBLE | NCSS_NOTIFY | NCSS_ITEMBAR_HASCHILD,
		WS_EX_BROUNDCNS | WS_EX_TRANSPARENT,
		"Title",
		NULL, //props,
		&rdr_info,
		_handlers, //trk2_handlers, //handlers,
		NULL, //controls
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
	160, 60, 480, 272,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
	"ItemBar Test ....",
	NULL,
	NULL,
	mymain_handlers,
	_ctrl_templ,
	sizeof(_ctrl_templ) / sizeof(NCS_WND_TEMPLATE),
	0,
	0xFFF43F33,
	0,
};


int MiniGUIMain(int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
	JoinLayer (NAME_DEF_LAYER, NULL, 0, 0);
#endif
	
	ncsInitialize();
	ncs4PadInitialize();
    //SetDefaultWindowElementRenderer ("pad");

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

