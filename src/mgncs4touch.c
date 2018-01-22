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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4padconfig.h"
#include "mpad.h"
#include "mpadrdr.h"

#define NDEBUG    1
#include "mpaddebug.h"


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

