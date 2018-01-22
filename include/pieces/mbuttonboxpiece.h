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

#ifndef __MGNCS4TOUCH_BUTTONBOXPIECE_H__
#define __MGNCS4TOUCH_BUTTONBOXPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mButtonBoxPieceClass mButtonBoxPieceClass;
typedef struct _mButtonBoxPiece mButtonBoxPiece;

#define mButtonBoxPieceClassHeader(clss, superCls) \
	mRenderablePieceClassHeader(clss, superCls) 

struct _mButtonBoxPieceClass {
	mButtonBoxPieceClassHeader(mButtonBoxPiece, mRenderablePiece)
};

MGNCS_EXPORT extern mButtonBoxPieceClass g_stmButtonBoxPieceCls;

#define mButtonBoxPieceHeader(clss) \
	mRenderablePieceHeader(clss) 
	
struct _mButtonBoxPiece {
	mButtonBoxPieceHeader(mButtonBoxPiece)
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4TOUCH_BUTTONBOXPIECE_H__

