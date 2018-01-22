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
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
//#include <minigui/fixedmath.h>
#include <mgncs/mgncs.h>
//#include <mgeff/mgeff.h>

#include "mgncs4padconfig.h"
#include "mpcomm.h"
#include "mpadrdr.h"
#include "mnewtrackbar.h"

#define NDEBUG    1
#include "mpaddebug.h"

#define NTB_TRIP_L    1
#define NTB_TRIP_R    1


// thumbbox
static void pad_thumbbox_paint(mThumbBoxPiece* self, HDC hdc,
		mWidget* owner, DWORD add_data)
{
    RECT rc;
    DWORD dwStyle = 0;
    DWORD thumbColor = 0;
    mNewTrackBar* newTrackBar = (mNewTrackBar*)owner;
    int r = 0;

    if (!_c(self)->getRect(self, &rc))
        return;

    dwStyle = GetWindowStyle(newTrackBar->hwnd);
    thumbColor = ncsGetElement(owner, NCS4PAD_BGC_NTB_BLOCK);

    if (dwStyle & NCSS_NTRKBAR_CIRCLE) {
        Draw3DCircle(hdc, &rc, thumbColor);
        return;
    }

    rc.right--;
    rc.bottom--;

    if (!(NCS_PIECE_PAINT_VERT & add_data))
        r = RECTH(rc) / 5;
    else
        r = RECTW(rc) / 5;

    thumbColor = ncsGetElement(owner, NCS4PAD_BGC_NTB_BLOCK);
    DrawGradientRroundRect(hdc, thumbColor, &rc, r, TRUE);
}


static void pad_trackbar_set_rect(mWidget* owner, DWORD add_data,
        PRECT prc, PRECT prcl, PRECT prcr, int* r)
{
    RECT rc_thumb;
    mSliderPiece* sliderPiece = NULL;
    mThumbBoxPiece* thumb = NULL;
    int cur_v = 0;

    sliderPiece = (mSliderPiece*)owner->body;
    thumb = (mThumbBoxPiece*)sliderPiece->thumb;
    if (!_c(thumb)->getRect(thumb, &rc_thumb))
        return;

    *r = 3;
    if (!(NCS_PIECE_PAINT_VERT & add_data)) {	// horz
        cur_v = (rc_thumb.left + rc_thumb.right) / 2;
        prcl->left   = prcr->left   = prc->left + NTB_TRIP_L;
        prcl->right  = prcr->right  = prc->right - NTB_TRIP_R;
        prcl->top    = prcr->top    = (prc->top + prc->bottom) / 2 - (*r);
        prcl->bottom = prcr->bottom = prcl->top + 2 * (*r) + 1;
        prcl->right  = prcr->left   = cur_v;
    } else {	// vert
        cur_v = (rc_thumb.top + rc_thumb.bottom) / 2;
        prcl->top    = prcr->top    = prc->top + NTB_TRIP_L;
        prcl->bottom = prcr->bottom = prc->bottom - NTB_TRIP_R;
        prcl->left   = prcr->left   = (prc->left + prc->right) / 2 - (*r);
        prcl->right  = prcr->right  = prcl->left + 2 * (*r) + 1;
        prcl->bottom = prcr->top    = cur_v;
    }

	NormalizeRect(prcl);
	NormalizeRect(prcr);
	
	return ;
}


// draw trackbar
static void pad_trackbar_paint(mTrackBarPiece* self, HDC hdc,
        mWidget* owner, DWORD add_data)
{
    /*
     * the data struct of add_data 
     *
     * bits 0x0000FFFF  : the tick
     * bits 0x00FF0000  : horz or vert
     * bits 0xFF000000  : enable or disable
     */
    RECT  rc, rcl, rcr;
    DWORD lbgcolor = 0;
    DWORD rbgcolor = 0;
    int   r = 0;

    if (!_c(self)->getRect(self, &rc))
        return;

    lbgcolor = ncsGetElement(owner, NCS4PAD_BGC_NTB_HITEM);
    rbgcolor = ncsGetElement(owner, NCS4PAD_BGC_NTB_DITEM);

    pad_trackbar_set_rect(owner, add_data, &rc, &rcl, &rcr, &r);
	
    if (!(NCS_PIECE_PAINT_VERT & add_data)) {	//horzontal
        RECT rcLeftT, rcRightT;
        memcpy (&rcLeftT,  &rcl, sizeof(RECT));
        rcLeftT.right = rcLeftT.left + RECTH(rcLeftT);
        memcpy (&rcRightT, &rcr, sizeof(RECT));
        rcRightT.left = rcRightT.right - RECTH(rcRightT);
		if (RECTW(rcl) > r * 2)
		{
			DrawGradientRroundRect(hdc, lbgcolor, &rcLeftT, r, FALSE);
			StretchBlt(hdc, (rcLeftT.left + rcLeftT.right) / 2, rcLeftT.top, 1, RECTH(rcLeftT),
					hdc, (rcLeftT.left + rcLeftT.right) / 2, rcLeftT.top, 
					rcl.right - (rcLeftT.left + rcLeftT.right) / 2,RECTH(rcLeftT), 0 );  
		}
		if (RECTW(rcr) > r * 2)
		{
			DrawGradientRroundRect(hdc, rbgcolor, &rcRightT, r, FALSE);
			StretchBlt(hdc, (rcRightT.left + rcRightT.right) / 2, rcRightT.top, 1, RECTH(rcRightT),
					hdc, rcr.left, rcRightT.top, 
					(rcRightT.left + rcRightT.right) / 2 - rcr.left, RECTH(rcRightT), 0 );  
		}
    } else { //vertical
        // FIXME, to optimize it
        DrawGradientRroundRect(hdc, lbgcolor, &rcl, r, FALSE);
        DrawGradientRroundRect(hdc, rbgcolor, &rcr, r, FALSE);
    }
}


// init boxpiece
void pad_init_piece_renderer(void)
{
    NCS_RDR_ENTRY entries [] = {
        {Class(mThumbBoxPiece).typeName, (mWidgetRenderer*)(void*)pad_thumbbox_paint},
        {Class(mTrackBarPiece).typeName, (mWidgetRenderer*)(void*)pad_trackbar_paint},
    };

    ncsRegisterCtrlRDRs(NCS4PAD_RENDERER, entries, sizeof(entries) / sizeof(NCS_RDR_ENTRY));
}

