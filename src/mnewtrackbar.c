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

#include "mgncs4padconfig.h"
#include "mpcomm.h"
#include "mpadrdr.h"
#include "mnewtrackbar.h"

#define NDEBUG   1
#include "mpaddebug.h"


static BOOL mNewTrackBar_onCreate(mNewTrackBar* self, DWORD addData)
{
    Class(mTrackBar).onCreate((mTrackBar*)self, addData);
    _c(self)->setProperty(self, NCSP_WIDGET_RDR, (DWORD)NCS4PAD_RENDERER);

    return TRUE;
}


static BOOL mNewTrackBar_setProperty(mNewTrackBar* self, int id, DWORD value)
{
    if (id >= NCSP_NTRKBAR_MAX)
        return FALSE;

    if (id == NCSP_WIDGET_RDR && strcmp((char*)value, NCS4PAD_RENDERER) != 0)
        return FALSE;

    return Class(mTrackBar).setProperty((mTrackBar*)self, id, value);
}


#ifdef _MGNCS4PAD_GUIBUILDER_SUPPORT
static BOOL mNewTrackBar_refresh(mNewTrackBar* self)
{
    _c(self)->setProperty(self, NCSP_WIDGET_RDR, (DWORD)NCS4PAD_RENDERER);
    InvalidateRect(self->hwnd, NULL, TRUE);

    return TRUE;
}
#endif


BEGIN_CMPT_CLASS(mNewTrackBar, mTrackBar)
    CLASS_METHOD_MAP(mNewTrackBar, onCreate)
    CLASS_METHOD_MAP(mNewTrackBar, setProperty)
#ifdef _MGNCS4PAD_GUIBUILDER_SUPPORT
    CLASS_METHOD_MAP(mNewTrackBar, refresh)
#endif
END_CMPT_CLASS

