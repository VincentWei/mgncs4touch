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

#ifndef __MGNCS4PAD_SHAPEBOXPIECE_H__
#define __MGNCS4PAD_SHAPEBOXPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mShapeBoxPieceClass mShapeBoxPieceClass;
typedef struct _mShapeBoxPiece      mShapeBoxPiece;

#define mShapeBoxPieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mShapeBoxPieceClass {
	mShapeBoxPieceClassHeader(mShapeBoxPiece, mStaticPiece)
};

MGNCS_EXPORT extern mShapeBoxPieceClass g_stmShapeBoxPieceCls;

#define mShapeBoxPieceHeader(clss) \
	mStaticPieceHeader(clss)
	
struct _mShapeBoxPiece {
	mShapeBoxPieceHeader(mShapeBoxPiece)
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4PAD_SHAPEBOXPIECE_H__

