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
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include "balloon_tip_maker.h"

#define IDC_ADDITIONAL (-1)
#define DATA_SHIFT 16

static int balloon_calculate_coordinate (BALLOONTIP_SHAPE *balloontip_shape, RECT winrc, 
        int triangle_height, TRIANGLE_DIRECTION triangle_direction)
{
    int win_h = RECTH (winrc);
    int win_w = RECTW (winrc);

    balloontip_shape->triangle_height = triangle_height;
    balloontip_shape->triangle_direction = triangle_direction;

    switch (triangle_direction) {
        case TRIANGLE_DOWN:
            balloontip_shape->main_x = 0; 
            balloontip_shape->main_y = 0;
            balloontip_shape->main_w = win_w;
            balloontip_shape->main_h = win_h - triangle_height;
            break;
        case TRIANGLE_UP:
            balloontip_shape->main_x = 0; 
            balloontip_shape->main_y = triangle_height;
            balloontip_shape->main_w = win_w;
            balloontip_shape->main_h = win_h - triangle_height;
            break;
        default : 
            return -1;
    }
    return 0;
}

#define MASK_CORNER_BMP "./res/mask_corner.bmp"
#define MASK_TRIANGLE_BMP "./res/mask_triangle_down.bmp"
#define CORNER_PNG "./res/corner.png"
#define TRIANGLE_PNG "./res/triangle_down.png"

static int balloon_draw_edge (HWND hWnd, unsigned short triangle_offset, TRIANGLE_DIRECTION triangle_direction)
{
    int ret;
    BITMAP bmp_corner, bmp_triangle;
    HDC win_dc;
    RECT winrc;
    BALLOONTIP_SHAPE balloontip_shape = {};
    int triangle_height;
    int radius;
    int i, j, x, y;

    GetWindowRect (hWnd, &winrc);

    win_dc = GetDC (hWnd);
    if (win_dc == HDC_INVALID) {
        _ERR_PRINTF ("mGNCS4Touch>balloon_draw_edge: Failed to get dc.\n");
        return -1;
    }

    memset (&bmp_triangle, 0, sizeof(bmp_triangle));
    ret = LoadBitmapFromFile (win_dc, &bmp_triangle, TRIANGLE_PNG);
    if (ret != 0) {
        _ERR_PRINTF ("mGNCS4Touch>balloon_draw_edge: Failed to load triangle bmp!\n");
        goto error2;
    }

    triangle_height = bmp_triangle.bmHeight;
    balloon_calculate_coordinate (&balloontip_shape, winrc, triangle_height, triangle_direction);

    /*draw triangle*/
    if (triangle_direction == TRIANGLE_UP) {
        RotateBitmapVFlip (win_dc, &bmp_triangle, triangle_offset, 0, 180);
    } else {
        FillBoxWithBitmap (win_dc, triangle_offset, balloontip_shape.main_h, 0, 0, &bmp_triangle);
    }

    /*draw edge*/
    memset (&bmp_corner, 0, sizeof(bmp_corner));
    ret = LoadBitmapFromFile (win_dc, &bmp_corner, CORNER_PNG);
    if (ret != 0) {
        _ERR_PRINTF ("mGNCS4Touch>balloon_draw_edge: Failed to load corner bmp!\n");
        goto error1;
    }

    radius = bmp_corner.bmHeight/2;

    /* draw corner */
    FillBoxWithBitmapPart (win_dc, 
            balloontip_shape.main_x, balloontip_shape.main_y, radius, radius, 
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, 0, 0);/*left top*/
    FillBoxWithBitmapPart (win_dc, 
            balloontip_shape.main_w - radius, balloontip_shape.main_y, radius, radius,  
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, radius, 0);/*right top*/
    FillBoxWithBitmapPart (win_dc, 
            balloontip_shape.main_x, balloontip_shape.main_h-radius+balloontip_shape.main_y, radius, radius,  
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, 0, radius);/*left buttom*/
    FillBoxWithBitmapPart (win_dc, 
            balloontip_shape.main_w-radius, balloontip_shape.main_h-radius+balloontip_shape.main_y, radius, radius,  
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, radius, radius);/*right buttom*/

    for (i=0; i<2; ++i) {
        /*up, down*/
        for (j=0; j<RECTW(winrc)-2*radius; ++j) {
            x = balloontip_shape.main_x + radius + j;
            y = i?(balloontip_shape.main_y+balloontip_shape.main_h - radius):balloontip_shape.main_y;
            FillBoxWithBitmapPart (win_dc, x, y, 1, radius, 
                    bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, radius, i?radius:0);
        }

        /*left right*/
        for (j=0; j<RECTH(winrc)-2*radius; ++j) {
            x = i?(balloontip_shape.main_w - radius):0;
            y = balloontip_shape.main_y + radius + j;
            FillBoxWithBitmapPart (win_dc, x, y, radius, 1, 
                    bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, i?radius:0, radius);
        }
    }

    ReleaseDC (win_dc);
    UnloadBitmap (&bmp_corner);
    UnloadBitmap (&bmp_triangle);

    return 0;
error1:
    UnloadBitmap (&bmp_triangle);
error2:
    ReleaseDC (win_dc);
    return -1;
}

static int put_frame_to_window(HWND hwnd, HDC secondary_dc, 
        HDC real_dc, const RECT* secondary_rc, const RECT* real_rc,
        const RECT* main_update_rc)
{
    if (IsControl(hwnd)) {
        hwnd = GetMainWindowHandle(hwnd);
    }
    if (IsMainWindow(hwnd)) {
        HWND add_ctrl = GetDlgItem (hwnd, IDC_ADDITIONAL);
        BitBlt(secondary_dc, secondary_rc->left, secondary_rc->top, 
            RECTWP(secondary_rc), RECTHP(secondary_rc), real_dc,
            real_rc->left, real_rc->top, -1);
            if (HWND_INVALID != add_ctrl) {
                DWORD addition = GetWindowAdditionalData (add_ctrl);
                if (0 != addition) { /* addition data should be not equal to 0 */
                    unsigned short tri_offset = 0xffff & addition;
                    TRIANGLE_DIRECTION direction = (TRIANGLE_DIRECTION)(addition >> DATA_SHIFT);
                    balloon_draw_edge(hwnd, tri_offset, direction);
            }
        }

        //SetMemDCAlpha (secondary_dc, MEMDC_FLAG_SRCALPHA, 100);
    }
    return 0;
}

static int set_new_update_behavior(HWND hWnd, unsigned short tri_offset, TRIANGLE_DIRECTION direction)
{
    HDC sec_dc = CreateSecondaryDC(hWnd);
    if (HDC_INVALID != sec_dc) {
	DWORD addition;
        SetSecondaryDC (hWnd, sec_dc, put_frame_to_window);
        addition = ((direction << DATA_SHIFT) | tri_offset);
        if (HWND_INVALID != 
                CreateWindow(CTRL_STATIC, "", WS_CHILD, IDC_ADDITIONAL, 0, 0, 1, 1, hWnd, addition)) {
            return 0;
        }
    }
    return -1;
}

int ncsSetBalloonTipStyle (HWND hWnd, unsigned short triangle_offset, TRIANGLE_DIRECTION triangle_direction)
{
    int win_height, win_width, ret;
    MYBITMAP mybmp_mask;
    BITMAP bmp_corner, bmp_triangle;
    HDC mask_hdc;
    RECT winrc;
    BALLOONTIP_SHAPE balloontip_shape = {};
    int radius;
    int triangle_height;
    BYTE *bits;
    int pitch;

    if (IsControl(hWnd)) {
        hWnd = GetMainWindowHandle(hWnd);
    }

    set_new_update_behavior(hWnd, triangle_offset, triangle_direction);
    GetWindowRect (hWnd, &winrc);

    win_height = RECTH (winrc);
    win_width = RECTW (winrc);

#define MASK_DC_BPP 8
    mask_hdc = CreateMemDC (win_width, win_height, MASK_DC_BPP, MEMDC_FLAG_SWSURFACE, 0, 0, 0, 0);
    if (mask_hdc == HDC_INVALID) {
        _ERR_PRINTF ("mGNCS4Touch>ncsSetBalloonTipStyle: Failed to create mem dc!\n");
        return -1;
    }
    SetColorfulPalette (mask_hdc);

    SetBrushColor (mask_hdc,  RGB2Pixel(mask_hdc, 0, 0, 0));
    FillBox (mask_hdc, 0, 0, win_width, win_height);

    memset (&bmp_corner, 0, sizeof(bmp_corner));
    ret = LoadBitmapFromFile (mask_hdc, &bmp_corner, MASK_CORNER_BMP);
    if (ret != 0) {
        _ERR_PRINTF ("mGNCS4Touch>ncsSetBalloonTipStyle: Failed to load MASK_CORNER_BMP!\n");
        goto error2;
    }

    radius = bmp_corner.bmHeight/2;

    memset (&bmp_triangle, 0, sizeof(bmp_triangle));
    ret = LoadBitmapFromFile (mask_hdc, &bmp_triangle, MASK_TRIANGLE_BMP);
    if (ret != 0) {
        _ERR_PRINTF ("mGNCS4Touch>ncsSetBalloonTipStyle: Failed to load MASK_TRIANGLE_BMP!\n");
        goto error1;
    }
    triangle_height = bmp_triangle.bmHeight;

    balloon_calculate_coordinate (&balloontip_shape, winrc, triangle_height, triangle_direction);

    /*draw triangle*/
    if (triangle_direction == TRIANGLE_UP) {
        RotateBitmapVFlip (mask_hdc, &bmp_triangle, triangle_offset, 0, 180);
    } else {
        FillBoxWithBitmap (mask_hdc, triangle_offset, balloontip_shape.main_h, 0, 0, &bmp_triangle);
    }

    SetBrushColor (mask_hdc,  RGB2Pixel(mask_hdc, 0xff, 0xff, 0xff));
    FillBox (mask_hdc, balloontip_shape.main_x, balloontip_shape.main_y, balloontip_shape.main_w, balloontip_shape.main_h);

    /* draw corner */
    FillBoxWithBitmapPart (mask_hdc, 
            balloontip_shape.main_x, balloontip_shape.main_y, radius, radius, 
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, 0, 0);/*left top*/
    FillBoxWithBitmapPart (mask_hdc, 
            balloontip_shape.main_w - radius, balloontip_shape.main_y, radius, radius,  
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, radius, 0);/*right top*/
    FillBoxWithBitmapPart (mask_hdc, 
            balloontip_shape.main_x, balloontip_shape.main_h-radius+balloontip_shape.main_y, radius, radius,  
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, 0, radius);/*left buttom*/
    FillBoxWithBitmapPart (mask_hdc, 
            balloontip_shape.main_w-radius, balloontip_shape.main_h-radius+balloontip_shape.main_y, radius, radius,  
            bmp_corner.bmWidth, bmp_corner.bmHeight, &bmp_corner, radius, radius);/*right buttom*/

    /*make mybitmp*/
    bits = LockDC (mask_hdc, NULL, &win_width, &win_height, &pitch);
    UnlockDC (mask_hdc);
    memset (&mybmp_mask, 0, sizeof(mybmp_mask));
    mybmp_mask.flags = MYBMP_TYPE_NORMAL;
    mybmp_mask.frames = 1;
    mybmp_mask.depth = MASK_DC_BPP;
    mybmp_mask.alpha = 0;
    mybmp_mask.transparent = 0x0;

    mybmp_mask.w = win_width;
    mybmp_mask.h = win_height;
    mybmp_mask.pitch = pitch;
    mybmp_mask.bits = bits;
    /* Uint32 size; */
    SetWindowMask(hWnd, &mybmp_mask);

    UnloadBitmap (&bmp_corner);
    UnloadBitmap (&bmp_triangle);
    return 0;

error1:
    UnloadBitmap (&bmp_corner);
error2:
    DeleteMemDC (mask_hdc);
    return -1;
}
