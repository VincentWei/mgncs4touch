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
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"

static void mContainerCtrl_setBody(mContainerCtrl* self, mHotPiece* body)
{
    self->body = (mObject*)body;

    assert(INSTANCEOF(body, mPanelPiece));

    // set owner for body
    _c((mPanelPiece*)body)->setOwner((mPanelPiece*)body, (mWidget*)self);

    return;
}

#ifdef ENABLE_ANIM_FPS_TEST
#include <sys/times.h>
#include <unistd.h>

#define TCK_PER_SEC sysconf(_SC_CLK_TCK)
#define ANIM_TICKS_MAX	(2 * TCK_PER_SEC)

volatile int anim_fps_test_status = 0; /* 0:ready, 1:running */
static int anim_frames = 0;
static clock_t anim_start_time = 0;

static void anim_fps_test_tellspeed (int ticks)
{
    unsigned int fps = (int) ( (double) anim_frames / (ticks / TCK_PER_SEC) );

    /* print result */
    fprintf (stderr, "**** anim fps: n=%d, time=%d, speed=%ufps\n", anim_frames, (int) ticks, fps);
}

void anim_fps_test_start (int new_status)
{
    if (1 == new_status) {
        printf ("**** anim test start ...\n");
    }
    else {
        printf ("**** anim test end.\n");
    }
        
    if (anim_fps_test_status == 1) { /* print result of previous test */
        int ticks = times (NULL) - anim_start_time;
        anim_fps_test_tellspeed (ticks);
    }

    /* set status */
    anim_fps_test_status = new_status;

    anim_frames = 0;
    anim_start_time = times (NULL);
}
#endif

static void mContainerCtrl_onPaint(mContainerCtrl *self, HDC hdc, const PCLIPRGN pclip) {
    if (self->body) {
        _c((mHotPiece*)(self->body))->paint(
                (mHotPiece*)self->body, hdc, (mObject*)self, (DWORD)NULL);

#ifdef ENABLE_ANIM_FPS_TEST

        if (anim_fps_test_status == 1) { /* running */

            /* printf ("+++++++++ %d\n", random ()); */

            if ( (times (NULL) - anim_start_time) >= ANIM_TICKS_MAX ) { /* end test */
                printf ("**** anim test timeout.\n");
                anim_fps_test_start (0);
            }
            else {
                ++ anim_frames;
            }
        }
#endif
    }
}

/*
static BOOL mContainerCtrl_onEraseBkgnd(mContainerCtrl *self, HDC hdc, const RECT *pinv)
{
    return TRUE;
}
*/

#ifdef DEBUG
static LRESULT mContainerCtrl_wndProc(mWidget* self,  UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == MSG_PAINT) {
        mPanelPiece* topPanel = (mPanelPiece*)self->body;
        if ( NULL != topPanel) {
            RECT* invalidRect = &topPanel->invalidRect;
            GetUpdateRect(self->hwnd, invalidRect);
            _MG_PRINTF ("piece is %p, invalidRect is (%d,%d,%d,%d).\n", topPanel, invalidRect->left, 
                    invalidRect->top, invalidRect->right, invalidRect->bottom);
        }
    }

    return Class(mWidget).wndProc((mWidget*)self, message, wParam, lParam);
}
#endif

BEGIN_CMPT_CLASS(mContainerCtrl, mWidget)
    CLASS_METHOD_MAP(mContainerCtrl, setBody)
#ifdef DEBUG
    CLASS_METHOD_MAP(mContainerCtrl, wndProc)
#endif
    CLASS_METHOD_MAP(mContainerCtrl, onPaint)
END_CMPT_CLASS
