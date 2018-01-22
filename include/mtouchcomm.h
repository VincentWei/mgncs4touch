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

#ifndef __MGNCS4TOUCH_COMMON_H__
#define __MGNCS4TOUCH_COMMON_H__

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
#   ifdef __MGNCS4TOUCH_LIB__
#       define MPAD_EXPORT       __declspec(dllexport)
#   else
#       define MPAD_EXPORT       __declspec(dllimport)
#   endif
#else
#   define MPAD_EXPORT
#endif

#define ABGR2ARGB(c) (((c) & 0xFF00FF00) | (((c) & 0x000000FF)<<16) | (((c) & 0x00FF0000)>>16))

typedef enum _eCornerType {
    ECT_NORMAL = 0,
    ECT_TOPR,
    ECT_BOTTOMR,
    ECT_LEFTR,
    ECT_RIGHTR,
    ECT_BOTH,
} ECONER;


MPAD_EXPORT void DrawGradientRroundRect(HDC hdc,
        DWORD color , PRECT pRc, unsigned int r, BOOL isConvex);

MPAD_EXPORT int DrawPickerRect(HDC hdc, RECT *prc, DWORD beginColor, DWORD endColor, int corner);

MPAD_EXPORT int DrawSharpButton(HDC hdc, RECT *prc, DWORD color, BOOL left);

MPAD_EXPORT int DrawRoundRectButton(HDC hdc, RECT *prc, DWORD color, unsigned int r);

MPAD_EXPORT int DrawPadTick(HDC hdc, RECT* prc, DWORD color);

MPAD_EXPORT int DrawPadAngle(HDC hdc, RECT* prc, DWORD color);

MPAD_EXPORT int Draw3DCircle(HDC hdc, PRECT prc, DWORD color);

MPAD_EXPORT BOOL DrawVariableRoundRect(HDC hdc, PRECT prc, DWORD color, int r, ECONER con);

MPAD_EXPORT int DrawRectRing(HDC hdc, PRECT pRcOutside, PRECT pRcInside, DWORD color);

#ifdef __cplusplus
}
#endif

#endif //__MGNCS4TOUCH_COMMON_H__

