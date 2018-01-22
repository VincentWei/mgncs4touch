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

#ifndef __MGNCS4TOUCH_SHAPEPUSHBUTTONPIECE_H__
#define __MGNCS4TOUCH_SHAPEPUSHBUTTONPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mShapePushButtonPieceClass mShapePushButtonPieceClass;
typedef struct _mShapePushButtonPiece      mShapePushButtonPiece;

#define mShapePushButtonPieceClassHeader(clss, superCls) \
	mPushButtonPieceClassHeader(clss, superCls)          \
	void (*getContentRect)(clss* ,  RECT* prc);


struct _mShapePushButtonPieceClass {
	mShapePushButtonPieceClassHeader(mShapePushButtonPiece, mPushButtonPiece)
};

MGNCS_EXPORT extern mShapePushButtonPieceClass g_stmShapePushButtonPieceCls;

#define mShapePushButtonPieceHeader(clss) \
	mPushButtonPieceHeader(clss)          \
	int shape;
	
struct _mShapePushButtonPiece {
	mShapePushButtonPieceHeader(mShapePushButtonPiece)
};

#define SHAPE_RECT      0
#define SHAPE_LEFT      1
#define SHAPE_RIGHT     2

enum mShapePushButtonPieceProps {
	NCSP_SPBP_SHAPE = ABP_PROP_END + 1,
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4TOUCH_SHAPEPUSHBUTTONPIECE_H__

