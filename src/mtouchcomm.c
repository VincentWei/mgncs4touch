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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>

#include "mtouchcomm.h"

// #define NDEBUG   1
#include "mtouchdebug.h"

/*
 * context define for CircleArcGenerator Using.
 */
typedef struct _RBCONTEXT {
    HDC   hdc;
    RECT  rc;
    unsigned char rS, gS, bS;
    float  rDelta, gDelta, bDelta;
} RBC;


/*
 * Call back function used by CircleArcGenerator.
 *
 * this function will :
 *     1. calculate a gradient color ,
 *     2. draw a line with the color.
 *
 * And this function be called every line when draw the corner.
 */
static void _cb(void* context, int x, int y)
{
    int r, g, b;
    RBC *stct = (RBC*)context;

    r = stct->rS + (y - stct->rc.top) * stct->rDelta; 
    g = stct->gS + (y - stct->rc.top) * stct->gDelta; 
    b = stct->bS + (y - stct->rc.top) * stct->bDelta; 
    SetPenColor(stct->hdc, RGB2Pixel(stct->hdc, r, g, b));
    
    LineEx(stct->hdc, x, y, stct->rc.right - (x - stct->rc.left), y);
}


/*
 *   ___________________
 *  /                   \
 * |                     |
 * |                     |
 *  \___________________/
 */
void DrawGradientRroundRect(HDC hdc, DWORD color, PRECT pRc, unsigned int r, BOOL isConvex)
{
    RBC stct;
    int cw, ch, a;
    int x0 = pRc->left;
    int y0 = pRc->top;
    int x1 = pRc->right - 1;
    int y1 = pRc->bottom - 1;

    DWORD dark  = ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER);
    DWORD light = ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_LIGHTER);

    cw = r * 2;
    ch = r * 2;

    if (x0 >= x1 || y0 >= y1 || cw > (x1 - x0) || ch > (y1 - y0) || cw < 0 || ch < 0)
        return ;

    a = 90 * 64;    // Right Angle

    if (r > 0) {
        // draw top
        stct.hdc = hdc;
        SetRect(&stct.rc, x0, y0, x1, y0 + r);
        stct.rS = GetRValue(dark);
        stct.gS = GetGValue(dark);
        stct.bS = GetBValue(dark);
        stct.rDelta = (GetRValue(color) - stct.rS) * 1.0 / r; 
        stct.gDelta = (GetGValue(color) - stct.gS) * 1.0 / r; 
        stct.bDelta = (GetBValue(color) - stct.bS) * 1.0 / r; 
        CircleArcGenerator((void*)&stct, x0 + r, y0 + r, r, a, a, _cb);
        // draw bottom
        SetRect(&stct.rc, x0, y1 - r, x1, y1);
        stct.rS = GetRValue(color);
        stct.gS = GetGValue(color);
        stct.bS = GetBValue(color);
        stct.rDelta = (GetRValue(light) - stct.rS) * 1.0 / r; 
        stct.gDelta = (GetGValue(light) - stct.gS) * 1.0 / r; 
        stct.bDelta = (GetBValue(light) - stct.bS) * 1.0 / r; 
        CircleArcGenerator((void*)&stct, x0 + r, y1 - r, r, -a, -a, _cb);
    }
    
    // draw main body
    if (y1 - y0 > ch) {
        SetBrushColor(hdc, ncsColor2Pixel(hdc, color));
        FillBox(hdc, x0, y0 + r, x1 - x0, (y1 - y0) - ch);
    }
    
    // draw thin frame
    SetPenColor(hdc, ncsColor2Pixel(hdc, dark));
    if (r > 0) {
        ArcEx(hdc, x0, y0, cw, ch, a, a);
        ArcEx(hdc, x1 - cw, y0, cw, ch, 0, a);
        ArcEx(hdc, x1 - cw, y1 - ch, cw, ch, -a, a);
        ArcEx(hdc, x0, y1 - ch, cw, ch, -a, -a);
    }
    LineEx(hdc, x0 + r, y0, x1 - r, y0);
    LineEx(hdc, x0 + r, y1, x1 - r, y1);
    LineEx(hdc, x0, y0 + r, x0, y1 - r);
    LineEx(hdc, x1, y0 + r, x1, y1 - r);

    // draw convex frame
    if (isConvex) {
        x0++, x1--;
        y0++, y1--;
        SetPenColor(hdc, ncsColor2Pixel(hdc, light));
        if (r > 0) {
            ArcEx(hdc, x0, y0, cw, ch, a, a);
            ArcEx(hdc, x1 - cw, y0, cw, ch, 0, a);
            ArcEx(hdc, x1 - cw, y1 - ch, cw, ch, -a, a);
            ArcEx(hdc, x0, y1 - ch, cw, ch, -a, -a);
        }
        LineEx(hdc, x0 + r, y0, x1 - r, y0);
        LineEx(hdc, x0 + r, y1, x1 - r, y1);
        LineEx(hdc, x0, y0 + r, x0, y1 - r);
        LineEx(hdc, x1, y0 + r, x1, y1 - r);
    }
}


int DrawRoundRectButton(HDC hdc, RECT *prc, DWORD color, unsigned int r)
{
    HPATH path;
    HBRUSH brush;
    HPEN pen;
    HGRAPHICS graphic;
    DWORD p[2];
    POINT pt[8];
    int i;
    RECT rcC = {1, 1, RECTWP(prc) - 1, RECTHP(prc) - 1};

    pt[0].x = rcC.right - r; pt[0].y = 0; 
    pt[1].x = rcC.left + r; pt[1].y = 0; 
    pt[2].x = 0; pt[2].y = r; 
    pt[3].x = 0; pt[3].y = RECTH(rcC) - r; 
    pt[4].x = r; pt[4].y = RECTH(rcC);
    pt[5].x = rcC.right - r; pt[5].y = RECTH(rcC);
    pt[6].x = rcC.right; pt[6].y = RECTH(rcC) - r; 
    pt[7].x = rcC.right; pt[7].y = r; 

    p[0] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_LIGHTER));
    p[1] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER));

    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic) {
        return -1; 
    }

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (MP_INV_HANDLE == brush) {
        MGPlusGraphicDelete(graphic);
        return -1; 
    }   

    pen = MGPlusPenCreate(1, p[1]);
    if (MP_INV_HANDLE == pen) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        return -1; 
    }

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        MGPlusPenDelete(pen);
        return -1; 
    }

    MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);

    MGPlusSetLinearGradientBrushRect(brush, &rcC);
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)p, 2);

    MGPlusPathMoveto(path, pt[0].x, pt[0].y);
    for (i = 1; i < 8; i++) {
        MGPlusPathLinetoI(path, pt[i].x, pt[i].y);
    }
    MGPlusPathLinetoI(path, pt[0].x, pt[0].y);
    MGPlusFillPath(graphic, brush, path);
    MGPlusDrawPath(graphic, pen, path);

    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0, hdc, prc->left, prc->top, 0);

    MGPlusPenDelete(pen);
    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
    MGPlusGraphicDelete(graphic);

    return 0;
}


int DrawPickerRect(HDC hdc, RECT *prc, DWORD edgeColor, DWORD mainColor, int corner)
{
#if 1
	int r = 3;
	HPATH path;
    HBRUSH brush;
    HPEN pen;
    HGRAPHICS graphic;
    RECT rcC = {0, 0, RECTWP(prc), RECTHP(prc)};
	ARGB color[4];
    
    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic) {
        return -1; 
    }

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (MP_INV_HANDLE == brush) {
        MGPlusGraphicDelete(graphic);
        return -1; 
    }   
    
    pen = MGPlusPenCreate(1, 
			ABGR2ARGB(ncsCommRDRCalc3dboxColor(edgeColor, NCSR_COLOR_DARKER)));
    if (MP_INV_HANDLE == pen) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        return -1; 
    }

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        MGPlusPenDelete(pen);
        return -1; 
    }
	
	switch (corner) {
		case ECT_BOTH :// both corner round
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top + r);
				
				MGPlusPathMovetoI(path, rcC.right - r, rcC.top);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.right, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
				break;
			}
		case ECT_LEFTR: //left corner round
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top + r);
				
				MGPlusPathMovetoI(path, rcC.right - r, rcC.top);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.top);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
				break;
			}
		case ECT_RIGHTR : //right corner round
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top);
				
				MGPlusPathMovetoI(path, rcC.right - r, rcC.top);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.right, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
				break;
			}
		default : // none corner round
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top);
				
				MGPlusPathMovetoI(path, rcC.right - r, rcC.top);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.top);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
				break;
			}
	}
	MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);
    MGPlusSetLinearGradientBrushRect(brush, &rcC);
	
	color[0] = ABGR2ARGB(edgeColor);
    color[1] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(edgeColor, NCSR_COLOR_LIGHTER));
	color[2] = color[0];
    MGPlusSetLinearGradientBrushColors(brush, color, 3);
    
    MGPlusFillPath(graphic, brush, path);
    
	MGPlusPathReset(path);
	MGPlusPathMovetoI(path, rcC.left + r, rcC.top);
	MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
	MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
	MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
	MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
    
	color[0] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(mainColor, NCSR_COLOR_DARKER));
    color[1] = ABGR2ARGB(mainColor);
    color[2] = color[1];
	color[3] = color[0];
    MGPlusSetLinearGradientBrushColors(brush, color, 4);
	MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);
    MGPlusSetLinearGradientBrushRect(brush, &rcC);
	
    MGPlusFillPath(graphic, brush, path);
	
	MGPlusPathReset(path);
	rcC.right--;
	rcC.bottom--;
	switch (corner) {
		case ECT_BOTH :// both corner round
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.right, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top + r);
				break;
			}
		case ECT_LEFTR:
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.top);
				MGPlusPathLinetoI(path, rcC.left + r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top + r);
				break;
			}
		case ECT_RIGHTR:
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom - r);
				MGPlusPathLinetoI(path, rcC.right, rcC.top + r);
				MGPlusPathLinetoI(path, rcC.right - r, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top);
				break;
			}
		default:
		   	{
				MGPlusPathMovetoI(path, rcC.left, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.bottom);
				MGPlusPathLinetoI(path, rcC.right, rcC.top);
				MGPlusPathLinetoI(path, rcC.left, rcC.top);
				break;
			}
	}

	MGPlusDrawPath(graphic, pen, path);
    
    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0, hdc, prc->left, prc->top, 0);

	MGPlusPenDelete(pen);
    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
    MGPlusGraphicDelete(graphic);

    return 0;
#else
    int line, gh;
    int x0, y0, x1, y1;
    int r0, g0, b0, r1, g1, b1, r, g, b;

    x0 = prc->left;
    y0 = prc->top;
    x1 = prc->right - 1;
    y1 = prc->bottom - 1;

    r0 = GetRValue(edgeColor);
    g0 = GetGValue(edgeColor);
    b0 = GetBValue(edgeColor);
    r1 = GetRValue(mainColor);
    g1 = GetGValue(mainColor);
    b1 = GetBValue(mainColor);

    gh = RECTHP(prc) / 3;

    for (line = 0; line < gh; line++) {
        r = r0 + line * (r1 - r0) / gh;
        g = g0 + line * (g1 - g0) / gh;
        b = b0 + line * (b1 - b0) / gh;
        SetPenColor(hdc, RGB2Pixel(hdc, r, g, b));
        LineEx(hdc, x0, y0 + line, x1, y0 + line);
        LineEx(hdc, x0, y1 - line, x1, y1 - line);
    }

    SetBrushColor(hdc, RGB2Pixel(hdc, r1, g1, b1));
    FillBox(hdc, x0, y0 + gh, RECTWP(prc), RECTHP(prc) - (gh << 1));

    SetPenColor(hdc, RGB2Pixel(hdc, r0, g0, b0));
    Rectangle(hdc, x0, y0, x1, y1);

    return 0;
#endif
}


/*       
 *    /-------------+    +------------\
 *   /              |    |             \
 *  X               |    |              X
 *   \              |    |             /
 *    \-------------+    +------------/
 */
int DrawSharpButton(HDC hdc, RECT *prc, DWORD color, BOOL left)
{
    HPATH path;
    HBRUSH brush;
    HPEN pen;
    HGRAPHICS graphic;
    DWORD p[2];
    POINT pt[7];
    int i, r = 3;
    RECT rcC = {1, 1, RECTWP(prc) - 1, RECTHP(prc) - 1};

    int half = RECTH(rcC) >> 1;

    if (left) {
        pt[0].x = rcC.right - r; pt[0].y = 0;
		pt[1].x = half; pt[1].y = 0;
		pt[2].x = 0; pt[2].y = half;
		pt[3].x = half; pt[3].y = RECTH(rcC);
		pt[4].x = rcC.right - r; pt[4].y = RECTH(rcC);
		pt[5].x = rcC.right; pt[5].y = RECTH(rcC) - r;
		pt[6].x = rcC.right; pt[6].y = r;
    } else {
		pt[0].x = r; pt[0].y = 0;
		pt[1].x = rcC.right - half; pt[1].y = 0;
		pt[2].x = rcC.right; pt[2].y = half;
		pt[3].x = rcC.right - half; pt[3].y = RECTH(rcC);
		pt[4].x = r; pt[4].y = RECTH(rcC);
		pt[5].x = 0; pt[5].y = RECTH(rcC) - r;
		pt[6].x = 0; pt[6].y = r;
    }

    p[0] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_LIGHTER));
    p[1] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER));

    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic)
        return -1;

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (MP_INV_HANDLE == brush) {
        MGPlusGraphicDelete(graphic);
        return -1;
    }

    pen = MGPlusPenCreate(1, p[1]);
    if (MP_INV_HANDLE == pen) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        return -1; 
    }

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        MGPlusPenDelete(pen);
        return -1;
    }

    MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);

    MGPlusSetLinearGradientBrushRect(brush, &rcC);
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)p, 2);

    MGPlusPathMoveto(path, pt[0].x, pt[0].y);
    for (i = 1; i < 7; i++) {
        MGPlusPathLinetoI(path, pt[i].x, pt[i].y);
    }
    MGPlusPathLinetoI(path, pt[0].x, pt[0].y);
    MGPlusFillPath(graphic, brush, path);
    MGPlusDrawPath(graphic, pen, path);

    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0, hdc, prc->left, prc->top, 0);

    MGPlusPenDelete(pen);
    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
    MGPlusGraphicDelete(graphic);
    return 0;
}


int DrawTouchTick(HDC hdc, RECT* prc, DWORD color)
{
    HPATH path;
    HPEN pen;
    HGRAPHICS graphic;
    POINT pt[3];
    int marg = RECTWP(prc) / 5;
    RECT rcC = { 0, 0, RECTWP(prc) - marg * 2, RECTHP(prc) - marg * 2};

    rcC.top = (RECTHP(prc) - RECTWP(prc)) / 2;
    rcC.bottom = rcC.top + RECTWP(prc);

    pt[0].x = RECTW(rcC) / 4; pt[0].y = RECTH(rcC) * 2 / 4; 
    pt[1].x = RECTW(rcC) * 1 / 2; pt[1].y = RECTH(rcC) * 2 / 3; 
    pt[2].x = RECTW(rcC); pt[2].y = RECTH(rcC) / 3; 

    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic)
        return -1; 

    pen = MGPlusPenCreate(3, ABGR2ARGB(color));
    if (MP_INV_HANDLE == pen) {
        MGPlusGraphicDelete(graphic);
        return -1; 
    }

    MGPlusPenSetJoinStyle(pen, JOIN_MILTER_ROUND);
    MGPlusPenSetCapStyle(pen, CAP_ROUND);

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusPenDelete(pen);
        return -1; 
    }

    MGPlusPathMoveto(path, pt[0].x, pt[0].y);
    MGPlusPathLinetoI(path, pt[1].x, pt[1].y);
    MGPlusPathLinetoI(path, pt[2].x, pt[2].y);
    MGPlusDrawPath(graphic, pen, path);

    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0,
            hdc, prc->left + marg, prc->top + (RECTHP(prc) - RECTH(rcC)) / 2, 0);

    MGPlusPenDelete(pen);
    MGPlusPathDelete(path);
    MGPlusGraphicDelete(graphic);

    return 0;
}


int DrawTouchAngle(HDC hdc, RECT* prc, DWORD color)
{
    HPATH path;
    HPEN pen;
    HGRAPHICS graphic;
    POINT pt[3];
    RECT rcC = { 1, 1, RECTWP(prc) - 1, RECTHP(prc) - 1};

    rcC.top = (RECTHP(prc) - RECTWP(prc)) / 2;
    rcC.bottom = rcC.top + RECTWP(prc);

    pt[0].x = RECTW(rcC) * 5 / 12; pt[0].y = RECTH(rcC) * 1 / 3; 
    pt[1].x = RECTW(rcC) * 7 / 12; pt[1].y = RECTH(rcC) * 1 / 2; 
    pt[2].x = RECTW(rcC) * 5 / 12; pt[2].y = RECTH(rcC) * 2 / 3; 

    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic) {
        return -1;
    } 

    pen = MGPlusPenCreate(3, ABGR2ARGB(color));
    if (MP_INV_HANDLE == pen) {
        MGPlusGraphicDelete(graphic);
        return -1; 
    }

    MGPlusPenSetJoinStyle(pen, JOIN_MILTER_ROUND);
    MGPlusPenSetCapStyle(pen, CAP_ROUND);

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusPenDelete(pen);
        return -1; 
    }

    MGPlusPathMoveto(path, pt[0].x, pt[0].y);
    MGPlusPathLinetoI(path, pt[1].x, pt[1].y);
    MGPlusPathLinetoI(path, pt[2].x, pt[2].y);
    MGPlusDrawPath(graphic, pen, path);

    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0,
            hdc, prc->left, prc->top + (RECTHP(prc) - RECTH(rcC)) / 2, 0);

    MGPlusPenDelete(pen);
    MGPlusPathDelete(path);
    MGPlusGraphicDelete(graphic);

    return 0;
}


int Draw3DCircle(HDC hdc, PRECT prc, DWORD color)
{
    HPATH path;
    HBRUSH brush;
    HGRAPHICS graphic;
    DWORD p[2];
    RECT rcC = { 0, 0, RECTWP(prc), RECTHP(prc)};

    int rx = RECTWP(prc) >> 1;
    int ry = RECTHP(prc) >> 1;

    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic)
        return -1;

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (MP_INV_HANDLE == brush) {
        MGPlusGraphicDelete(graphic);
        return -1;
    }

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_WINDING);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        return -1;
    }

    MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);
    MGPlusSetLinearGradientBrushRect(brush, &rcC);
    p[0] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER));
    p[1] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_LIGHTER));
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)p, 2);
    MGPlusPathAddArcI(path, rx, ry, rx, ry, 0, 360);
    MGPlusFillPath(graphic, brush, path);

    MGPlusPathReset(path);
    MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);
    MGPlusSetLinearGradientBrushRect(brush, &rcC);
    p[0] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_LIGHTER));
    p[1] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER));
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)p, 2);
    MGPlusPathAddArcI(path, rx, ry, rx - 1, ry - 1, 0, 360);
    MGPlusFillPath(graphic, brush, path);

    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0, hdc, prc->left, prc->top, 0);

    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
    MGPlusGraphicDelete(graphic);

    return 0;
}


BOOL DrawVariableRoundRect(HDC hdc, PRECT prc, DWORD color, int r, ECONER con)
{
    int c;
    HPEN pen;
    HPATH path;
    HBRUSH brush;
    HGRAPHICS graphic;
    ARGB penColor;
    POINT pc[4];
    RECT rcC = {1, 1, RECTWP(prc) - 1, RECTHP(prc) - 1};

    c = r << 1;

	pc[0].x = c; pc[0].y = c; 
	pc[1].x = c; pc[1].y = rcC.bottom - c; 
	pc[2].x = rcC.right - c; pc[2].y = rcC.bottom - c; 
	pc[3].x = rcC.right - c; pc[3].y = c; 

    penColor = ABGR2ARGB(ncsCommRDRCalc3dboxColor(color, NCSR_COLOR_DARKER));

    graphic = MGPlusGraphicCreate(RECTWP(prc), RECTHP(prc));
    if (MP_INV_HANDLE == graphic)
        return -1; 

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_SOLIDCOLOR);
    if (MP_INV_HANDLE == brush) {
        MGPlusGraphicDelete(graphic);
        return -1; 
    }   

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_ALTERNATE);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        return -1; 
    }

    pen = MGPlusPenCreate(1, penColor);
    if (MP_INV_HANDLE == pen) {
        MGPlusGraphicDelete(graphic);
        MGPlusBrushDelete(brush);
        MGPlusPathDelete(path);
        return -1; 
    }

    MGPlusSetSolidBrushColor(brush, ABGR2ARGB(color));

    switch (con) {
        case ECT_BOTH:
            MGPlusPathAddRoundRect(path, 0, 0, RECTW(rcC) + 1, RECTH(rcC) + 1, c);
            break;

        case ECT_TOPR:
            MGPlusPathMovetoI(path, r + 1, 0);
            MGPlusPathLinetoI(path, r + 1, 0);
            MGPlusPathLinetoI(path, rcC.right - r - 2, 0);
            MGPlusPathAddArcI(path, pc[0].x, pc[0].y, c, c, 90, -90);
            MGPlusPathLinetoI(path, 1, rcC.bottom);
            MGPlusPathLinetoI(path, rcC.right, rcC.bottom);
            MGPlusPathAddArcI(path, pc[3].x, pc[3].y, c, c, -180, -90);
            break;

        case ECT_BOTTOMR:
            MGPlusPathMovetoI(path, 0, 0);
            MGPlusPathAddArcI(path, pc[1].x, pc[1].y, c, c, 0, -90);
            MGPlusPathAddArcI(path, pc[2].x, pc[2].y, c, c, -90, -90);
            MGPlusPathLinetoI(path, rcC.right + 1, 0);
            MGPlusPathLinetoI(path, 0, 0);
            break;

        default:
            MGPlusPathAddRectangleI(path, 0, 0, RECTW(rcC) + 1, RECTH(rcC) + 1);
            break;
    }

    MGPlusFillPath(graphic, brush, path);
    MGPlusDrawPath(graphic, pen, path);

    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0, hdc, prc->left, prc->top, 0);

    MGPlusPenDelete(pen);
    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
    MGPlusGraphicDelete(graphic);

    return TRUE;
}


int DrawRectRing(HDC hdc, PRECT pRcOutside, PRECT pRcInside, DWORD c)
{
    HPATH path;
    HBRUSH brush;
    HGRAPHICS graphic;
    ARGB p[3];
    RECT rcG = {0, 0, RECTWP(pRcOutside), RECTHP(pRcOutside)}; 

    graphic = MGPlusGraphicCreate(RECTWP(pRcOutside), RECTHP(pRcOutside));
    if (MP_INV_HANDLE == graphic)
        return -1; 

    path = MGPlusPathCreate(MP_PATH_FILL_MODE_ALTERNATE);
    if (MP_INV_HANDLE == path) {
        MGPlusGraphicDelete(graphic);
        return -1; 
    }

    MGPlusPathAddRectangleI(path, 0, 0, RECTWP(pRcOutside), RECTHP(pRcOutside));
    //MGPlusPathAddRectangleI(path, pRcInside->left - pRcOutside->left, pRcInside->top - pRcOutside->top, 
    //        RECTWP(pRcInside), RECTHP(pRcInside));

    brush = MGPlusBrushCreate(MP_BRUSH_TYPE_LINEARGRADIENT);
    if (MP_INV_HANDLE == brush) {
        MGPlusGraphicDelete(graphic);
        MGPlusPathDelete(path);
        return -1; 
    }   

    MGPlusSetLinearGradientBrushMode(brush, MP_LINEAR_GRADIENT_MODE_VERTICAL);
    MGPlusSetLinearGradientBrushRect(brush, &rcG);

    p[1] = ABGR2ARGB(c);
    p[0] = p[2] = ABGR2ARGB(ncsCommRDRCalc3dboxColor(c, NCSR_COLOR_LIGHTER));
    MGPlusSetLinearGradientBrushColors(brush, (ARGB*)p, 3);

    MGPlusFillPath(graphic, brush, path);
    BitBlt(MGPlusGetGraphicDC(graphic), 0, 0, 0, 0, hdc, pRcOutside->left, pRcOutside->top, 0);

    MGPlusPathDelete(path);
    MGPlusBrushDelete(brush);
    MGPlusGraphicDelete(graphic);

    return 0;
}

