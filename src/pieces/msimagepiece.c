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

#include "mpcomm.h"
#include "pieces/msimagepiece.h"

static int mSImagePiece_processMessage(mSImagePiece* self, int message, WPARAM wParam, LPARAM lParam, mWidget * owner)
{
	if (message == MSG_LBUTTONDOWN) {
		mWidget_captureHotPiece(owner, (mObject*)self);
	}else if (message == MSG_LBUTTONUP)
	{
		mWidget_releaseCapturedHotPiece();
		ncsRaiseEvent((mObject *)self, NCSN_ABP_CLICKED, 0);
	}
	return 0;
}

BEGIN_MINI_CLASS(mSImagePiece, mImagePiece)
	CLASS_METHOD_MAP(mSImagePiece, processMessage)
END_MINI_CLASS

