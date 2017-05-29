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
 *    FileName : switchbutton.c
 *      Author : <wangxin@minigui.org>
 * Create Date : Monday, 23 August 2010
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

// #define NDEBUG	1
#include <../include/mpaddebug.h>


#define ID_BTN1     101

static BOOL update_time(mSwitchButton *listener, 
        mTimer* sender, int id, DWORD total_count)
{
    static int s = 0;
    DWORD c = random() | 0xFF000000;
    ncsSetElement(listener, NCS4PAD_BGC_BLOCK, c);
    LOGE("NCS4PAD_BGC_BLOCK :: %d\n", NCS4PAD_BGC_BLOCK);
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
    LOGE("switch button notify : %s\n", add_data == 0 ? "OFF" : "ON");
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

