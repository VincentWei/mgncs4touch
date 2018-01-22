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

#ifndef __MGNCS4TOUCH_PUSHBUTTONPIECE_H__
#define __MGNCS4TOUCH_PUSHBUTTONPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mPushButtonPiece      mPushButtonPiece;
typedef struct _mPushButtonPieceClass mPushButtonPieceClass;

#define mPushButtonPieceClassHeader(clss, superCls) \
	mAbstractButtonPieceClassHeader(clss, superCls)

struct _mPushButtonPieceClass {
	mPushButtonPieceClassHeader(mPushButtonPiece, mAbstractButtonPiece)
};

MGNCS_EXPORT extern mPushButtonPieceClass g_stmPushButtonPieceCls;

#define mPushButtonPieceHeader(clss) \
	mAbstractButtonPieceHeader(clss) \
	mHotPiece* content;

struct _mPushButtonPiece {
	mPushButtonPieceHeader(mPushButtonPiece)
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4TOUCH_PUSHBUTTONPIECE_H__

