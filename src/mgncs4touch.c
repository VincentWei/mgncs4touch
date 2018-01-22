/*
 * \file mgncs4touch.c
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
    <http://www.minigui.com/en/about/licensing-policy/>.

 \endverbatim
*/

#include <stdio.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"
#include "mtouchrdr.h"

#define NDEBUG    1
#include "mtouchdebug.h"


#define InitPieceClass(className) \
        Class(className).classConstructor((mObjectClass*)(void*)(&(Class(className))))


extern BOOL RegisterPadRDR(void);


void init_mgncs4pad_pieces_classes(void)
{
    InitPieceClass(mShapePushButtonPiece);
    InitPieceClass(mShapeBoxPiece);
    InitPieceClass(mSImagePiece);
    InitPieceClass(mNSDrawPiece);
    InitPieceClass(mColorableLabelPiece);
}


BOOL ncs4PadInitialize(void)
{
    if (!ncsInitPadRenderers()) {
        LOGE("Init Pad renderer Error.\n");
        return FALSE;
    }
    if (!RegisterPadRDR()) {
        LOGE("Init system renderer [pad] Error.\n");
        return FALSE;
    }

    init_mgncs4pad_pieces_classes();
    // TODO
    MGNCS_REGISTER_COMPONENT(mAnimation);
    MGNCS_REGISTER_COMPONENT(mSwitchButton);
    MGNCS_REGISTER_COMPONENT(mNewTrackBar);
    MGNCS_REGISTER_COMPONENT(mPicker);
    MGNCS_REGISTER_COMPONENT(mComboPicker);
    MGNCS_REGISTER_COMPONENT(mDatePicker);
    MGNCS_REGISTER_COMPONENT(mTimePicker);
    MGNCS_REGISTER_COMPONENT(mBtnNavBar);
    MGNCS_REGISTER_COMPONENT(mImgNavBar);
    MGNCS_REGISTER_COMPONENT(mExList);
    MGNCS_REGISTER_COMPONENT(mItemBar);

    return TRUE;
}


void ncs4PadUninitialize(void)
{
    // TODO
    MGNCS_UNREG_COMPONENT(mAnimation);
    MGNCS_UNREG_COMPONENT(mSwitchButton);
    MGNCS_UNREG_COMPONENT(mNewTrackBar);
    MGNCS_UNREG_COMPONENT(mPicker);
    MGNCS_UNREG_COMPONENT(mComboPicker);
    MGNCS_UNREG_COMPONENT(mDatePicker);
    MGNCS_UNREG_COMPONENT(mTimePicker);
    MGNCS_UNREG_COMPONENT(mBtnNavBar);
    MGNCS_UNREG_COMPONENT(mImgNavBar);
    MGNCS_UNREG_COMPONENT(mExList);
    MGNCS_UNREG_COMPONENT(mItemBar);
}

