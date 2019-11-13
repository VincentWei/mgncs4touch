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
 * \file imgnavbar.c
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

