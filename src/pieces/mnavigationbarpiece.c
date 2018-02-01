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

#include <mgncs/mgncs.h>
#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"

static void mNavigationBarPiece_construct (mNavigationBarPiece *self, DWORD param)
{
	Class(mPanelPiece).construct((mPanelPiece*)self, param);

    self->left_button = NULL;
    self->title_button = NULL;
    self->right_button = NULL;
}


static void mNavigationBarPiece_destroy (mNavigationBarPiece* self, DWORD param)
{
    Class(mPanelPiece).destroy((mPanelPiece*)self);
}


static BOOL mNavigationBarPiece_setProperty (mNavigationBarPiece* self, int id, DWORD value)
{
    if ( NULL == self )
        return FALSE;

    switch (id) {
        case NCSP_NAVIGATIONBARPIECE_BACKGROUND:
            if ( NULL == (mShapeTransRoundPiece*)value ) 
                return FALSE;
            self->background = (mShapeTransRoundPiece*)value;
            break;

        case NCSP_NAVIGATIONBARPIECE_BKG:
            break;

        case NCSP_NAVIGATIONBARPIECE_LEFT_BUTTON:
            self->left_button = (mButtonPanelPiece*)value;
            break;

        case NCSP_NAVIGATIONBARPIECE_TITLE_BUTTON:
            self->title_button = (mTextPiece*)value;
            break;

        case NCSP_NAVIGATIONBARPIECE_RIGHT_BUTTON:
            self->right_button = (mButtonPanelPiece*)value;
            break;

        default:
            return Class(mPanelPiece).setProperty((mPanelPiece*)self, id, value);
    }

    return TRUE;
}


static DWORD mNavigationBarPiece_getProperty (mNavigationBarPiece* self, int id)
{
    if ( NULL == self )
        return (DWORD)NULL;

	switch (id) {
        case NCSP_NAVIGATIONBARPIECE_BACKGROUND:
            return (DWORD)self->background;

        case NCSP_NAVIGATIONBARPIECE_BKG:
            return (DWORD)NULL;

        case NCSP_NAVIGATIONBARPIECE_LEFT_BUTTON:
            return (DWORD)self->left_button;

        case NCSP_NAVIGATIONBARPIECE_TITLE_BUTTON:
            return (DWORD)self->title_button;

        case NCSP_NAVIGATIONBARPIECE_RIGHT_BUTTON:
            return (DWORD)self->right_button;

        default:
            break;
	}

	return Class(mPanelPiece).getProperty((mPanelPiece*)self, id);
}


BEGIN_MINI_CLASS(mNavigationBarPiece, mPanelPiece)
	CLASS_METHOD_MAP(mNavigationBarPiece, construct)
	CLASS_METHOD_MAP(mNavigationBarPiece, destroy)
	CLASS_METHOD_MAP(mNavigationBarPiece, setProperty)
	CLASS_METHOD_MAP(mNavigationBarPiece, getProperty)
END_MINI_CLASS

