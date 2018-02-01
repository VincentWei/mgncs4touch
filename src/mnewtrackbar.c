/*
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
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

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "mnewtrackbar.h"

#define NDEBUG   1
#include "mtouchdebug.h"


static BOOL mNewTrackBar_onCreate(mNewTrackBar* self, DWORD addData)
{
    Class(mTrackBar).onCreate((mTrackBar*)self, addData);
    _c(self)->setProperty(self, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

    return TRUE;
}


static BOOL mNewTrackBar_setProperty(mNewTrackBar* self, int id, DWORD value)
{
    if (id >= NCSP_NTRKBAR_MAX)
        return FALSE;

    if (id == NCSP_WIDGET_RDR && strcmp((char*)value, NCS4TOUCH_RENDERER) != 0)
        return FALSE;

    return Class(mTrackBar).setProperty((mTrackBar*)self, id, value);
}


#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
static BOOL mNewTrackBar_refresh(mNewTrackBar* self)
{
    _c(self)->setProperty(self, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);
    InvalidateRect(self->hwnd, NULL, TRUE);

    return TRUE;
}
#endif


BEGIN_CMPT_CLASS(mNewTrackBar, mTrackBar)
    CLASS_METHOD_MAP(mNewTrackBar, onCreate)
    CLASS_METHOD_MAP(mNewTrackBar, setProperty)
#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
    CLASS_METHOD_MAP(mNewTrackBar, refresh)
#endif
END_CMPT_CLASS

