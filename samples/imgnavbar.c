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


static BITMAP bmpLimg;
static BITMAP bmpRimg;

static BOOL mymain_onCreate(mWidget* self, DWORD add_data)
{
	if (LoadBitmapFromFile(HDC_SCREEN, &bmpLimg, "res/bmpLimg.png") != 0) {
		LOGE("cannot load image file \"bmpLimg.png\"\n");
	}
	if (LoadBitmapFromFile(HDC_SCREEN, &bmpRimg, "res/bmpRimg.png") != 0) {
		LOGE("cannot load image file \"bmpRimg.png\"\n");
	}
	
	mImgNavBar* tl1 = (mImgNavBar *)ncsGetChildObj(self->hwnd, ID_TLC1);
	if (tl1) {
		_c(tl1)->setProperty(tl1, NCSP_INB_LBITMAP,  (DWORD)&bmpLimg);
		_c(tl1)->setProperty(tl1, NCSP_INB_RBITMAP,  (DWORD)&bmpRimg);
		_c(tl1)->setProperty(tl1, NCSP_INB_MARGIN,   (DWORD)4);
		_c(tl1)->setProperty(tl1, NCSP_INB_IMG_H,    (DWORD)25);
		_c(tl1)->setProperty(tl1, NCSP_INB_DRAWMODE, (DWORD)NCS_DM_SCALED);
	}
	
	mImgNavBar* tl2 = (mImgNavBar *)ncsGetChildObj(self->hwnd, ID_TLC2);
	if (tl2) {
		_c(tl2)->setProperty(tl2, NCSP_INB_LBITMAP,  (DWORD)&bmpLimg);
		_c(tl2)->setProperty(tl2, NCSP_INB_RBITMAP,  (DWORD)&bmpRimg);
		_c(tl2)->setProperty(tl2, NCSP_INB_DRAWMODE, (DWORD)NCS_DM_NORMAL);
	}
	
	mImgNavBar* tl3 = (mImgNavBar *)ncsGetChildObj(self->hwnd, ID_TLC3);
	if (tl3) {
		_c(tl3)->setProperty(tl3, NCSP_INB_LBITMAP,  (DWORD)&bmpLimg);
		_c(tl3)->setProperty(tl3, NCSP_INB_RBITMAP,  (DWORD)&bmpRimg);
		_c(tl3)->setProperty(tl3, NCSP_INB_DRAWMODE, (DWORD)NCS_DM_TILED);
	}
	
	return TRUE;
}

static void mymain_onDestroy(mWidget* self, int message)
{
    UnloadBitmap(&bmpLimg);
    UnloadBitmap(&bmpRimg);
}


static void mymain_onClose(mWidget* self, int message)
{
	DestroyMainWindow(self->hwnd);
	PostQuitMessage(0);
}

static void imgclick_notify(mWidget *self, int id, int nc, DWORD add_data)
{
	LOGE("NCSN_INB_IMGCLICK:add_data is %d \n", (int)add_data);
}

static NCS_EVENT_HANDLER multi_handlers [] = {
    NCS_MAP_NOTIFY(NCSN_INB_IMGCLICK, imgclick_notify),
	{0, NULL}
};

//Controls
static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_IMGNAVBAR,
		ID_TLC1,
		12, 10, 240, 40,
		WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"scaled",
		NULL,
		NULL,//rdr_info
		multi_handlers,//handlers,
		NULL,
		0,
		0	//add data
	},
    {
		NCSCTRL_IMGNAVBAR,
		ID_TLC2,
		12, 70, 240, 40,
		WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"normal",
		NULL,
		NULL,//rdr_info
		multi_handlers,//handlers,
		NULL,
		0,
		0	//add data
	}, 
    {
		NCSCTRL_IMGNAVBAR,
		ID_TLC3,
		12, 130, 240, 40,
		WS_VISIBLE | NCSS_NOTIFY,
		WS_EX_NONE,
		"tiled",
		NULL,
		NULL,//rdr_info
		multi_handlers,//handlers,
		NULL,
		0,
		0	//add data
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
	264, 60, 272, 480,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
	"ImgNavBar Test ....",
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

