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


extern BOOL RegisterTouchRDR(void);


static void init_mgncs4touch_pieces_classes(void)
{
    InitPieceClass(mShapePushButtonPiece);
    InitPieceClass(mShapeBoxPiece);
    InitPieceClass(mSImagePiece);
    InitPieceClass(mNSDrawPiece);
    InitPieceClass(mColorableLabelPiece);
}


BOOL ncs4TouchInitialize(void)
{
    if (!ncsTouchInitRenderers()) {
        LOGE("mGNCS4Touch: Init renderer error.\n");
        return FALSE;
    }
    if (!RegisterTouchRDR()) {
        LOGE("mGNCS4Touch: Init system renderer error.\n");
        return FALSE;
    }

    init_mgncs4touch_pieces_classes();

    MGNCS_REGISTER_COMPONENT(mAnimation);
    MGNCS_REGISTER_COMPONENT_EX(mSwitchButton, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mNewTrackBar, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mPicker, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mComboPicker, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mDatePicker, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mTimePicker, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mBtnNavBar, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mImgNavBar, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mExList, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);
    MGNCS_REGISTER_COMPONENT_EX(mItemBar, WS_NONE, WS_EX_NONE, IDC_ARROW, NCS_BGC_3DBODY);

    MGNCS_INIT_CLASS(mPieceItem);
    MGNCS_INIT_CLASS(mAbstractItemManager);
    MGNCS_INIT_CLASS(mItemIterator);
    MGNCS_INIT_CLASS(mListItemManager);
    MGNCS_INIT_CLASS(mListItemIterator);
    MGNCS_INIT_CLASS(mRotateSwitchPiece);
    MGNCS_INIT_CLASS(mPanelPiece);
    MGNCS_INIT_CLASS(mItemPiece);
    MGNCS_INIT_CLASS(mCheckMarkPiece);
    MGNCS_INIT_CLASS(mRadioPiece);
    MGNCS_INIT_CLASS(mIndicatorPiece);
    MGNCS_INIT_CLASS(mIndicatorButtonPiece);
    MGNCS_INIT_CLASS(mSlideSwitchPiece);
    MGNCS_INIT_CLASS(mTextPiece);
    MGNCS_INIT_CLASS(mWrapPaintPiece);
    MGNCS_INIT_CLASS(mShapeTransRoundPiece);
    MGNCS_INIT_CLASS(mAnimationEditPiece);
    MGNCS_INIT_CLASS(mButtonPanelPiece);
    MGNCS_INIT_CLASS(mLayoutManager);
    MGNCS_INIT_CLASS(mCenterHBox);
    MGNCS_INIT_CLASS(mExSeparatorPiece);
    MGNCS_INIT_CLASS(mLineVBox);
    MGNCS_INIT_CLASS(mScrollViewPiece);
    MGNCS_INIT_CLASS(mHScrollViewPiece);
    MGNCS_INIT_CLASS(mTableViewItemPiece);
    MGNCS_INIT_CLASS(mTableViewPiece);
    MGNCS_INIT_CLASS(mIndexLocatePiece);
    MGNCS_INIT_CLASS(mNavigationItem);
    MGNCS_INIT_CLASS(mNavigationBarPiece);
    MGNCS_INIT_CLASS(mNavigationPanelPiece);
    //MGNCS_INIT_CLASS(mWidgetHostPiece);
    MGNCS_INIT_CLASS(m3DButtonPiece);

    MGNCS_REGISTER_COMPONENT(mContainerCtrl);
    MGNCS_REGISTER_COMPONENT_EX(mIconFlow, WS_VSCROLL, WS_EX_NONE, IDC_ARROW, NCS_BGC_WINDOW);

    return TRUE;
}


void ncs4TouchUninitialize(void)
{
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

    MGNCS_UNREG_COMPONENT(mIconFlow);
    MGNCS_UNREG_COMPONENT(mContainerCtrl);
}

