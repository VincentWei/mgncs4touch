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

#include "mtouchcomm.h"
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

