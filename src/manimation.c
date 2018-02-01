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

#include "manimation.h"

// #define NDEBUG	1
#include "mtouchdebug.h"


static void mAnimation_construct(mAnimation* self, DWORD addData)
{
    Class(mItemView).construct((mItemView*)self, addData);
    // construct myself
    mGEffInit();
}


static void mAnimation_destroy(mAnimation* self)
{
    Class(mItemView).destroy((mItemView*)self);
    // destroy myself
    mGEffDeinit();
}


static void _call_draw(MGEFF_ANIMATION animation, void* target, intptr_t id, void* value)
{
    UpdateWindow((HWND)target, TRUE);
}


static MGEFF_ANIMATION mAnimation_createAnimation(mAnimation* self,
        const EffPropertyAnimationSetting* settings)
{
    MGEFF_ANIMATION group;
    MGEFF_ANIMATION drawAnim;

    if (!settings || !settings->property) {
        assert(0);
        return NULL;
    }

    group = mGEffCreatePropertyAnimation(settings);
    if (!group) {
        assert(0);
        return NULL;
    }

    drawAnim = mGEffAnimationCreate((void*)self->hwnd,
            _call_draw, 0, MGEFF_INT);
    if (!group) {
        assert(0);
        return NULL;
    }
    mGEffAnimationSetDuration(drawAnim, settings->duration);

    mGEffAnimationAddToGroup(group, drawAnim);

    return group;
}


BEGIN_CMPT_CLASS(mAnimation, mItemView)
    CLASS_METHOD_MAP(mAnimation, construct)
    CLASS_METHOD_MAP(mAnimation, destroy)
    CLASS_METHOD_MAP(mAnimation, createAnimation)
END_CMPT_CLASS

