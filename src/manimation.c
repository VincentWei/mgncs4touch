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
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "manimation.h"

// #define NDEBUG	1
#include "mpaddebug.h"


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


static void _call_draw(MGEFF_ANIMATION animation, void* target, int id, void* value)
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

