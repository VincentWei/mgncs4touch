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
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgeff/mgeff.h>
#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

static void setState (mIndicatorButtonPiece *self, enum mIndicatorButtonPieceState state)
{
    if (self->state == state) return;

    self->state = state;

    if (self->images) {
        _c(self->imagePiece)->setProperty(self->imagePiece,
                NCSP_IMAGEPIECE_IMAGE, (DWORD)self->images->bmp[state]);
    }

    PanelPiece_update ((mHotPiece *)self, TRUE);
}

static int s_onMouseEvent (mHotPiece *_self, int message, WPARAM wParam, LPARAM lParam, mObject *owner)
{
    mIndicatorButtonPiece *self = (mIndicatorButtonPiece *)_self;

    enum mIndicatorButtonPieceState state = self->state;

    if (message == MSG_LBUTTONDOWN) {
        if (state != NCS_IBP_PUSHED)
            setState (self, NCS_IBP_PUSHED);
    }
    else if (message == MSG_LBUTTONUP) {
        if (state != NCS_IBP_CLICKED) {
            setState (self, NCS_IBP_CLICKED);
            ncsRaiseEvent ((mObject*)self, NCSN_ABP_CLICKED, ((mPanelPiece *)self)->addData);
        }
    }
    else if (message == MSG_MOUSEMOVEIN) {
        if (!(BOOL)wParam && state != NCS_IBP_CLICKED) {
            setState (self, NCS_IBP_CLICKED);
        }
    }
    else {
        return -1;
    }

    return 0;
}

static void mIndicatorButtonPiece_construct (mIndicatorButtonPiece *self, DWORD add_data)
{
    mImagePiece *img;

    Class(mPanelPiece).construct ((mPanelPiece *)self, add_data);

    img = NEWPIECE(mImagePiece);
    self->imagePiece = img;
    self->images = NULL;

    {
        self->colors[0][0] = 0xFFFFFFFF;
        self->colors[0][1] = 0xFFFFFFFF;
        self->colors[0][2] = 0xFFFFFFFF;
        self->colors[0][3] = 0xFFFFFFFF;

        self->colors[1][0] = 0xFFDEDEDE;
        self->colors[1][1] = 0xFFDEDEDE;
        self->colors[1][2] = 0xFFDEDEDE;
        self->colors[1][3] = 0xFFDEDEDE;

        self->colors[2][0] = 0xFFC4C7C9;
        self->colors[2][1] = 0xFFC4C7C9;
        self->colors[2][2] = 0xFFC4C7C9;
        self->colors[2][3] = 0xFFC4C7C9;

        self->colors[3][0] = 0xFFD0D3D4;
        self->colors[3][1] = 0xFFD0D3D4;
        self->colors[3][2] = 0xFFD0D3D4;
        self->colors[3][3] = 0xFFD0D3D4;

        self->colorsPos[0] = 0.07;
        self->colorsPos[1] = 0.15;
        self->colorsPos[2] = 0.32;
        self->colorsPos[3] = 1.0;

        self->state = NCS_IBP_CLICKED;

        self->hgs = MP_INV_HANDLE;

        self->brushSolid    = MGPlusBrushCreate (MP_BRUSH_TYPE_SOLIDCOLOR);
        self->brushGradient = MGPlusBrushCreate (MP_BRUSH_TYPE_LINEARGRADIENT);

        MGPlusSetLinearGradientBrushMode (self->brushGradient, MP_LINEAR_GRADIENT_MODE_VERTICAL);
    }

    _c(self)->appendEventHandler (self, MSG_LBUTTONUP, s_onMouseEvent);
    _c(self)->appendEventHandler (self, MSG_LBUTTONDOWN, s_onMouseEvent);
    _c(self)->appendEventHandler (self, MSG_MOUSEMOVEIN, s_onMouseEvent);
}

static void mIndicatorButtonPiece_destroy (mIndicatorButtonPiece *self)
{
    {
        MGPlusBrushDelete (self->brushSolid);
        MGPlusBrushDelete (self->brushGradient);
        MGPlusGraphicDelete (self->hgs);
    }

    DELPIECE(self->imagePiece);
    Class(mPanelPiece).destroy ((mPanelPiece *)self);
}

static void paintWithMGPlus(mIndicatorButtonPiece *self, HDC hdc)
{
    HGRAPHICS hgs = self->hgs;

    HBRUSH brushSolid = self->brushSolid;
    HBRUSH brushGradient = self->brushGradient;

    int w = self->hgs_width;
    int h = self->hgs_height;
   
    MGPlusGraphicClear (hgs, 0);

    {
        MGPlusSetSolidBrushColor (brushSolid, 0xFFB4B4B4);
        MGPlusFillEllipse (hgs, brushSolid, w / 2, h / 2,
                w / 3 + 1, h / 3 + 1);
    }

    {
        RECT rc;

        HPATH path = MGPlusPathCreate (MP_PATH_FILL_MODE_WINDING);

        _c(self)->getRect (self, &rc);

        MGPlusPathAddArc (path, w / 2, h / 2, w / 3, h / 3, 0, 360);

        MGPlusSetLinearGradientBrushColorsEx (brushGradient, self->colors[self->state],
                TABLESIZE (self->colors[self->state]), self->colorsPos);

        MGPlusSetLinearGradientBrushRect (brushGradient, &rc);

        MGPlusFillPath (self->hgs, brushGradient, path);

        MGPlusPathDelete (path);
    }

    {
        HPEN pen = MGPlusPenCreate (2, 0xFF1882BD);

        MGPlusDrawLine (hgs, pen, w * 5 / 12, h * 5 / 16, w * 2 / 3, h / 2);
        MGPlusDrawLine (hgs, pen, w * 2 / 3, h / 2, w * 5 / 12, h * 11 / 16);

        MGPlusPenDelete (pen);
    }

    BitBlt (MGPlusGetGraphicDC(hgs), 0, 0, 0, 0, hdc, 0, 0, 0);
}

static void mIndicatorButtonPiece_paint (mIndicatorButtonPiece *self, HDC hdc, mObject *owner, DWORD add_data)
{
    if (self->images)
        _c(self->imagePiece)->paint(self->imagePiece, hdc, owner, add_data);
    else
        paintWithMGPlus(self, hdc);
}

static BOOL mIndicatorButtonPiece_setRect (mIndicatorButtonPiece *self, const RECT *prc)
{
    BOOL ret = Class (mPanelPiece).setRect ((mPanelPiece *)self, prc);

    if (ret) {
        {
            self->hgs_width  = RECTWP (prc);
            self->hgs_height = RECTHP (prc);

            if (self->hgs != MP_INV_HANDLE) {
                MGPlusGraphicDelete (self->hgs);
            }

            self->hgs = MGPlusGraphicCreate (self->hgs_width, self->hgs_height);
        }
        return _c(self->imagePiece)->setRect(self->imagePiece, prc);
    }
    return ret;
}

static BOOL mIndicatorButtonPiece_setProperty(mIndicatorButtonPiece *self, int id, DWORD value)
{
    switch (id) {
        case NCSP_INDICATORBUTTON_IMAGE:
            if (value) {
                self->images = (mIndicatorButtonPieceImages*)value;
                _c(self->imagePiece)->setProperty(self->imagePiece,
                        NCSP_IMAGEPIECE_IMAGE, (DWORD)self->images->bmp[self->state]);
            }
            break;
        default:
            return Class(mPanelPiece).setProperty((mPanelPiece*)self, id, value);
    }
    return TRUE;
}

static DWORD mIndicatorButtonPiece_getProperty(mIndicatorButtonPiece *self, int id)
{
    switch (id) {
        case NCSP_INDICATORBUTTON_IMAGE:
            return (DWORD)self->images;
        default:
            return Class(mPanelPiece).getProperty((mPanelPiece*)self, id);
    }
}

BEGIN_MINI_CLASS (mIndicatorButtonPiece, mPanelPiece)
    CLASS_METHOD_MAP (mIndicatorButtonPiece, construct   )
    CLASS_METHOD_MAP (mIndicatorButtonPiece, destroy     )
    CLASS_METHOD_MAP (mIndicatorButtonPiece, paint       )
    CLASS_METHOD_MAP (mIndicatorButtonPiece, setRect     )
    CLASS_METHOD_MAP (mIndicatorButtonPiece, setProperty )
    CLASS_METHOD_MAP (mIndicatorButtonPiece, getProperty )
END_MINI_CLASS
