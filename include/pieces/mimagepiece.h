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

#ifndef __MGNCS4PAD_IMAGEPIECE_H__
#define __MGNCS4PAD_IMAGEPIECE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mImagePiece      mImagePiece;
typedef struct _mImagePieceClass mImagePieceClass;

#define mImagePieceClassHeader(clss, superCls) \
	mStaticPieceClassHeader(clss, superCls)

struct _mImagePieceClass {
	mImagePieceClassHeader(mImagePiece, mStaticPiece)
};

MGNCS_EXPORT extern mImagePieceClass g_stmImagePieceCls;

#define mImagePieceHeader(clss) \
	mStaticPieceHeader(clss)    \
	ImageDrawInfo img;

struct _mImagePiece {
	mImagePieceHeader(mImagePiece)
};

#define mImagePiece_setAlign(self, align) ((self)->img.revert=((((self)->img.revert)&0xF0)|((align)&0xF)))
#define mImagePiece_getAlign(self)   ((self)->img.revert&0xF)

#define mImagePiece_setVAlign(self, valign) ((self)->img.revert=(((self)->img.revert&0x0F)|(((valign)&0xF)<<4)))
#define mImagePiece_getVAlign(self)  (((self)->img.revert&0xF0)>>4)

enum mImagePieceProps {
	NCSP_IMAGEPIECE_IMAGE = IMAGEPIECE_PROP_BEGIN,
	NCSP_IMAGEPIECE_ICON,
	NCSP_IMAGEPIECE_MYBITMAP,
	NCSP_IMAGEPIECE_IMAGEFILE,
	NCSP_IMAGEPIECE_ALIGN    = PIECECOMM_PROP_ALIGN,
	NCSP_IMAGEPIECE_VALIGN   = PIECECOMM_PROP_VALIGN,
	NCSP_IMAGEPIECE_DRAWMODE = PIECECOMM_PROP_DRAWMOD
};

#ifdef __cplusplus
}
#endif

#endif	// __MGNCS4PAD_IMAGEPIECE_H__

