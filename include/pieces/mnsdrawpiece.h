/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Feynman Software Technology Co., Ltd.
 * Room 508B-C, Floor 5, Citic Guoan Shumagang, No.32, Haidian South
 * Road, Haidian District, Beijing, P. R. CHINA 100080.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Feynman Software Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall use
 * it only in accordance you entered into with Feynman Software.
 *          http://www.minigui.com
 *
 *    FileName : mnsdrawpiece.h
 *      Author : <wangxin@minigui.org>
 * Create Date : Monday September 25, 2010
 *     Version : 0.0.0.1
 *********************************************************************/

#ifndef __MGNCS4PAD_NSDRAWPIECE_H__
#define __MGNCS4PAD_NSDRAWPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SetNSDrawPieceRect(self, prc) do {            \
	(self)->left   = (short)((prc->left)   & 0xFFFF); \
	(self)->top    = (short)((prc->top)    & 0xFFFF); \
	(self)->right  = (short)((prc->right)  & 0xFFFF); \
	(self)->bottom = (short)((prc->bottom) & 0xFFFF); \
} while (0)

typedef struct _mNSDrawPiece      mNSDrawPiece;
typedef struct _mNSDrawPieceClass mNSDrawPieceClass;

#define mNSDrawPieceHeader(clss) \
	mHotPieceHeader(clss)        \
	DWORD shape;                 \
	DWORD dotHit;                \
	int left:16;                 \
	int top:16;                  \
	int right:16;                \
	int bottom:16;

struct _mNSDrawPiece {
	mNSDrawPieceHeader(mNSDrawPiece)
};

#define mNSDrawPieceClassHeader(clss, superCls) \
	mHotPieceClassHeader(clss, superCls)

struct _mNSDrawPieceClass {
	mNSDrawPieceClassHeader(mNSDrawPiece, mHotPiece)
};

MGNCS_EXPORT extern mNSDrawPieceClass g_stmNSDrawPieceCls;

enum mNSDrawPieceProp {
	NCSP_NSDRAW_PIECE_TICK = USER_PIECE_PROP_BEGIN + 1,
	NCSP_NSDRAW_PIECE_ANGLE,
	NCSP_NSDRAW_PIECE_DOTHIT,
	NCSP_NSDRAW_PIECE_MAX,
};

enum mNSDrawPieceShape {
	NSDRAW_SHAPE_TICK = NCSP_NSDRAW_PIECE_MAX + 1,
	NSDRAW_SHAPE_ANGLE,
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4PAD_NSDRAWPIECE_H__

