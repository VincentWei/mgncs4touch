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
#include "mexlist.h"

// #define NDEBUG    1
#include "mtouchdebug.h"


#define DEFAULT_DURATION      300


static BOOL animation_doing = FALSE;


static void mExList_construct(mExList* self, DWORD add)
{
    Class(mList).construct((mList*)self, add);
    mGEffInit();

    self->effector = MGEFF_EFFECTOR_PUSH;
    //self->effector = MGEFF_EFFECTOR_ALPHA;
    //self->effector = MGEFF_EFFECTOR_CUBIC_ROTATE;
    self->duration = DEFAULT_DURATION;
    self->key = 0;
}


static void mExList_destroy(mExList* self)
{
    mGEffDeinit();
    Class(mList).destroy((mList*)self);
}


static int runChgRootAnimation(mExList* self, BOOL toChild)
{
    HDC dst_dc, src1_dc, src2_dc;
    MGEFF_EFFECTOR effector;
    MGEFF_SOURCE source1, source2;
    MGEFF_SINK sink;
    MGEFF_ANIMATION handle;

    dst_dc  = GetClientDC(self->hwnd);
    src1_dc = CreateCompatibleDC(dst_dc);
    src2_dc = CreateCompatibleDC(dst_dc);

    BitBlt(dst_dc, 0, 0, 0, 0, src1_dc, 0, 0, 0);
    _c(self)->onEraseBkgnd(self, src2_dc, NULL);
    _c(self)->onPaint(self, src2_dc, NULL);

    effector = mGEffEffectorCreate(self->effector);
    source1  = mGEffCreateSource(src1_dc);
    source2  = mGEffCreateSource(src2_dc);
    sink     = mGEffCreateHDCSink(dst_dc);

    mGEffEffectorAppendSource(effector, source1);
    mGEffEffectorAppendSource(effector, source2);
    mGEffSetBufferSink(sink, src1_dc);
    mGEffEffectorSetSink(effector, sink);

    mGEffEffectorSetProperty(effector, MGEFF_PROPERTY_DIRECTION,
            toChild ? MGEFF_DIRECTION_RIGHT2LEFT : MGEFF_DIRECTION_LEFT2RIGHT);

    handle = mGEffAnimationCreateWithEffector(effector);

    mGEffAnimationSetDuration(handle, self->duration);
    mGEffAnimationSyncRun(handle);
    mGEffAnimationSetProperty(handle, MGEFF_PROP_KEEPALIVE, 0);

    mGEffEffectorDelete(effector);
    DeleteCompatibleDC(src1_dc);
    DeleteCompatibleDC(src2_dc);
    ReleaseDC(dst_dc);

    return 0;
}


static mNode* ChangeRoot(mExList* self, mNode* newRoot)
{
    mNode* oldRoot = NULL;
    mNode* node = NULL;

    if (!self || !newRoot || self->root == newRoot)
        return NULL;

    if ((INSTANCEOF(newRoot->parent, mList) && newRoot->parent != (mObject*)self)
        || (INSTANCEOF(newRoot->parent, mNode)
            && mAbstractList_getControl(newRoot) != (mAbstractList*)self))
        return NULL;

    _c(self)->freeze(self, TRUE);
    oldRoot = self->root;

    _c(self->layout)->nodeEvent(self->layout, NCSE_LIST_ROOTCHANGING, (DWORD)newRoot);

    self->contX = 0;
    self->contY = 0;
    self->root = newRoot;
    self->hiliteNode = NULL;

    node = _c(self)->getNode(self, 0);
    while (node) {
        if (!self->hiliteNode && _c(node)->isSelected(node)) {
            self->hiliteNode = node;
            break;
        }
        node = _c(node)->getNext(node);
    }

    _c(self)->freeze(self, FALSE);

    _c(self->layout)->nodeEvent(self->layout, NCSE_LIST_ROOTCHANGED, (DWORD)newRoot);

    if (!self->hiliteNode)
        _c(self)->setCurSel(self, 0);
    else
        _c(self)->showNode(self, self->hiliteNode);

    ncsNotifyParentEx((mWidget*)self, NCSN_LIST_ROOTNODECHANGED, (DWORD)newRoot);

    return oldRoot;
}


static void mExList_enterSubList(mExList* self, mNode* selNode)
{
    if (!self || !selNode)
        return;

    if (selNode && _c(selNode)->canAsRoot(selNode)) {
        //ncsNotifyParentEx((mWidget*)self, NCSN_LIST_ENTERSUBLIST, (DWORD)selNode);
        ChangeRoot(self, selNode);
        runChgRootAnimation(self, TRUE);
    }
}


static void mExList_backUpList(mExList* self, mNode* selNode)
{
    mNode* newRoot = NULL;

    if (NULL == self)
        return;

    if (NULL == selNode)
        newRoot = (mNode*)_M(self, getProperty, NCSP_ASTLST_ROOTNODE);
    else
        newRoot = (mNode*)_M(selNode, getProperty, NCSP_NODE_PARENTNODE);

    if (newRoot && INSTANCEOF(newRoot->parent, mNode)) {
        newRoot = (mNode*)newRoot->parent;

        while (!_c(newRoot)->canAsRoot(newRoot)) {
            if (INSTANCEOF(newRoot->parent, mNode)) {
                newRoot = (mNode*)newRoot->parent;
            }
        }
        ncsNotifyParentEx((mWidget*)self, NCSN_LIST_BACKUPLIST, (DWORD)selNode);
        ChangeRoot(self, newRoot);
        runChgRootAnimation(self, FALSE);
    }
}


static LRESULT mExList_wndProc(mExList* self, UINT msg, WPARAM w, LPARAM l)
{
    static unsigned int lbuttondown = 0;
    int _x = 0, _y = 0;

    switch (msg) {
        case MSG_LBUTTONDOWN:
            _x = LOSWORD(l);
            _y = HISWORD(l);
            _c(self)->onLButtonDown(self, _x, _y, 0L);
            ClientToScreen(self->hwnd, &_x, &_y);
            self->mouseY = _y;
            lbuttondown = 1;
            SetCapture(self->hwnd);
            return 0;

        case MSG_MOUSEMOVE:
            if (lbuttondown && self->mouseY != HISWORD(l)) {
                if (self->mouseY - HISWORD(l) > 3) {
                    self->mouseY = HISWORD(l);
                    self->contY += 3;
                    InvalidateRect(self->hwnd, NULL, TRUE);
                } else if (HISWORD(l) - self->mouseY > 3) {
                    self->mouseY = HISWORD(l);
                    self->contY -= 3;
                    InvalidateRect(self->hwnd, NULL, TRUE);
                }
            }
            return 0;

        case MSG_LBUTTONUP: {
            RECT rcNode;
            mNode* node = NULL;
            mNode* firstVisNode = NULL;
            mNode* firstNode = NULL;

            firstNode = _M(self, getNode, 0);
            _c(self)->getRect(self, firstNode, &rcNode, FALSE);

            if (rcNode.top >= self->contY){
                self->key = rcNode.top;
            } else {
                for (node = firstNode; node; node = _c(node)->getNext(node)) {
                    _c(self)->getRect(self, node, &rcNode, FALSE);
                    if (firstVisNode == NULL && rcNode.top >= self->contY) {
                        firstVisNode = node;
                        self->key = rcNode.top;
                    }
                }

                if (rcNode.bottom < self->contY + self->visHeight) {
                    self->key = rcNode.bottom - self->visHeight;
                }
            }
            animation_doing = TRUE;
            _c(self)->runAnimation(self, self->contY, self->key);

            lbuttondown = 0;
            ReleaseCapture();
        }
        return 0;
    }

    return Class(mList).wndProc((mList*)self, msg, w, l);
}


static void mExList_onPaint(mExList* self, HDC hdc, const PCLIPRGN pinv_clip)
{
    RECT rcVis, rcCont;
    mNode* node = NULL;
    int height = 0;

    if (_M(self, isFrozen))
        return;

    if (animation_doing)
        self->contY = self->key;

    _M(self, getContRect, &rcCont);
    if (self->visWidth > self->realContWidth)
        rcCont.right = rcCont.left + self->visWidth;
    else
        rcCont.right = rcCont.left + self->contWidth;

    _M(self, getVisRect, &rcVis);
    ClipRectIntersect(hdc, &rcVis);

    if (RECTW(rcCont) <= 0)
        return;

    height = 0;
    node = _M(self, getNode, 0);

    while (node) {
        rcCont.top += height;
        height = _c(node)->getProperty(node, NCSP_NODE_HEIGHT);
        rcCont.bottom = rcCont.top + height;
        if (rcCont.bottom < rcVis.top) {
            node = _c(node)->getNext(node);
            continue;
        }

        if (rcCont.top > rcVis.bottom)
            break;

        if (_c(node)->paint && RECTH(rcCont) > 0) {
            _c(node)->paint(node, hdc, &rcCont, (mObject*)self);
        }

        node = _c(node)->getNext(node);
    }
}


static void _finished_cb(MGEFF_ANIMATION animation)
{
    animation_doing = FALSE;
}


static void _call_draw(MGEFF_ANIMATION animation, void* target, intptr_t id, void* value)
{
    UpdateWindow((HWND)target, TRUE);
}


static void mExList_runAnimation(mExList* self, int startV, int endV)
{
    MGEFF_ANIMATION anim, drawAnim;

    EffPropertyAnimationSetting settings[] = {
        {
            &self->key, MGEFF_INT,
            (void*)&startV, (void*)&endV,
            InOutCubic, self->duration, _finished_cb
        },
        {NULL, 0, NULL, NULL, 0, 0, NULL}
    };

    if (NULL == (anim = mGEffCreatePropertyAnimation(settings))
        || NULL == (drawAnim = mGEffAnimationCreate((void*)self->hwnd,
            _call_draw, 0, MGEFF_INT))){
        return;
    }
    mGEffAnimationSetDuration(drawAnim, settings->duration);
    mGEffAnimationAddToGroup(anim, drawAnim);

    mGEffAnimationAsyncRun(anim);
    mGEffAnimationSetProperty(anim, MGEFF_PROP_KEEPALIVE, 0);
}


static BOOL mExList_setProperty(mExList* self, int id, DWORD value)
{
    if (id > NCSP_EXLIST_MAX)
        return FALSE;

    if (id == NCSP_EXLIST_EFFECTOR) {
        self->effector = (unsigned long)value;
        return TRUE;
    }

    return Class(mList).setProperty((mList*)self, id, value);
}


static DWORD mExList_getProperty(mExList* self, int id)
{
    if (id > NCSP_EXLIST_MAX)
        return (-1);

    if (id == NCSP_EXLIST_EFFECTOR) {
        return self->effector;
    }

    return Class(mList).getProperty((mList*)self, id);
}


BEGIN_CMPT_CLASS(mExList, mList)
    CLASS_METHOD_MAP(mExList, construct)
    CLASS_METHOD_MAP(mExList, destroy)
    CLASS_METHOD_MAP(mExList, wndProc)
    CLASS_METHOD_MAP(mExList, onPaint)
    CLASS_METHOD_MAP(mExList, runAnimation)
    CLASS_METHOD_MAP(mExList, enterSubList)
    CLASS_METHOD_MAP(mExList, backUpList)
    CLASS_METHOD_MAP(mExList, setProperty)
    CLASS_METHOD_MAP(mExList, getProperty)
END_CMPT_CLASS

