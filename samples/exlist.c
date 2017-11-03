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


#define IDL_DIR    100
#define IDL_FILE   110
#define INFO_NUM   3


static BITMAP demoBmp;

static NCS_MENU_DATA child[] = {
    {0, NULL, NCSS_NODE_LTEXTRS, "home:  100", NULL, -1, 0, NULL},
    {0, NULL, NCSS_NODE_LTEXTRS, "office:101", NULL, -1, 0, NULL},
    {0, NULL, NCSS_NODE_LTEXTRS, "phone: 102", NULL, -1, 0, NULL},
};

//nr_child, child, style, text, image, height, addData, parent
static NCS_MENU_DATA data[] = {
    {3, child, NCSS_NODE_LTEXTRS, "Zeus",     &demoBmp, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Pan",      &demoBmp, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Apollo",   &demoBmp, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Heracles", &demoBmp, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Achilles", &demoBmp, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Jason",    &demoBmp, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Theseus",  &demoBmp, -1, 0, NULL},
};

static void initListData(mDialogBox* dialog, PBITMAP bmp)
{
    mExList* dirObj = NULL;

    dirObj = (mExList*)ncsGetChildObj(dialog->hwnd, IDL_DIR);
    if (NULL == dirObj)
        return;

    _M(dirObj, freeze, TRUE);
    _M(dirObj, setProperty, NCSP_LIST_LAYOUT, NCS_LIST_LLIST);
    _M(dirObj, setNodeDefSize, 52, 52);

    ncsSetMenuData((mObject*)dirObj, data, sizeof(data) / sizeof(NCS_MENU_DATA));

    _M(dirObj, setCurSel, 0);
    _M(dirObj, freeze, FALSE);
}

static NCS_RDR_INFO lb_rdr_info[] = {
    {NCS4PAD_RENDERER, NCS4PAD_RENDERER, NULL}
};

static NCS_PROP_ENTRY static_props [] = {
	{NCSP_STATIC_ALIGN, NCS_ALIGN_LEFT},
	{0, 0}
};

static NCS_WND_TEMPLATE _ctrl_templ[] = {
	{
		NCSCTRL_STATIC,
		IDC_STATIC,
		20, 10, 180, 30,
		WS_VISIBLE,
		WS_EX_NONE, //TRANSPARENT | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS,
		"address",
		static_props,
		NULL, //lb_rdr_info,
		NULL,
		NULL,
		0,
		0
	},
	{
		NCSCTRL_EXLIST,
		IDL_DIR,
		20, 50, 180, 200,
		WS_BORDER | WS_TABSTOP | WS_VISIBLE
            | NCSS_NOTIFY | NCSS_LIST_LOOP | NCSS_ASTLST_AUTOSORT,
		WS_EX_TRANSPARENT | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS,
		"list",
		NULL,
		NULL, //lb_rdr_info,
		NULL, //list_handlers,
		NULL,
		0,
		0
	}
};

static NCS_MNWND_TEMPLATE mymain_templ = {
	NCSCTRL_DIALOGBOX,
	7,
	100, 100, 240, 320,
	WS_CAPTION | WS_BORDER | WS_VISIBLE,
	WS_EX_NONE,
    "List Demo",
	NULL,
	NULL,
	NULL,
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
	if (argc > 1) {
		lb_rdr_info[0].glb_rdr = argv[1];
		lb_rdr_info[0].ctl_rdr = argv[1];
	}

    UnloadBitmap(&demoBmp);
	ncsInitialize();
	ncs4PadInitialize();

	mDialogBox* mydlg = (mDialogBox*)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);

    LoadBitmap(HDC_SCREEN, &demoBmp, "listfolder.png");
    initListData (mydlg, &demoBmp);

	_c(mydlg)->doModal(mydlg, TRUE);

	MainWindowThreadCleanup(mydlg->hwnd);

    UnloadBitmap(&demoBmp);
	ncs4PadUninitialize();
	ncsUninitialize();

    UnloadBitmap(&demoBmp);
	return 0;
}

