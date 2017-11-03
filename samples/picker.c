/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Beijing FMSoft Technology Co., Ltd.
 * Room 902, Floor 9, Taixing, No.11, Huayuan East Road, Haidian
 * District, Beijing, P. R. CHINA 100191.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Beijing FMSoft Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance you entered into with FMSoft.
 *
 *          http://www.minigui.com
 *
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
    {NCS4PAD_RENDERER, NCS4PAD_RENDERER, NULL}
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

