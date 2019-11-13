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
 * \file exlist.c
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

#define IDL_DIR    100
#define IDL_FILE   110
#define INFO_NUM   3

static NCS_MENU_DATA child[] = {
    {0, NULL, NCSS_NODE_LTEXTRS, "home:  100", NULL, -1, 0, NULL},
    {0, NULL, NCSS_NODE_LTEXTRS, "office:101", NULL, -1, 0, NULL},
    {0, NULL, NCSS_NODE_LTEXTRS, "phone: 102", NULL, -1, 0, NULL},
};

//nr_child, child, style, text, image, height, addData, parent
static NCS_MENU_DATA data[] = {
    {3, child, NCSS_NODE_LTEXTRS, "Zeus",     NULL, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Pan",      NULL, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Apollo",   NULL, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Heracles", NULL, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Achilles", NULL, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Jason",    NULL, -1, 0, NULL},
    {3, child, NCSS_NODE_LTEXTRS, "Theseus",  NULL, -1, 0, NULL},
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
    {NCS4TOUCH_RENDERER, NCS4TOUCH_RENDERER, NULL}
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

	ncsInitialize();
	ncs4TouchInitialize();

	mDialogBox* mydlg = (mDialogBox*)ncsCreateMainWindowIndirect
                                (&mymain_templ, HWND_DESKTOP);
    initListData (mydlg, NULL);
	_c(mydlg)->doModal(mydlg, TRUE);

	ncs4TouchUninitialize();
	ncsUninitialize();
	return 0;
}

