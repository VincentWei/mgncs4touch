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

#ifndef __MGNCS4TOUCH_RENDER_H__
#define __MGNCS4TOUCH_RENDER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NCS_TYPE4TOUCH                0xA000

#define NCS4TOUCH_RRECT_RADIUS        (NCS_TYPE4TOUCH + 0x01)
#define NCS4TOUCH_BGC_BLOCK           (NCS_TYPE4TOUCH + 0x11)
#define NCS4TOUCH_FGC_SWBON           (NCS_TYPE4TOUCH + 0x12)
#define NCS4TOUCH_BGC_SWBON           (NCS_TYPE4TOUCH + 0x13)
#define NCS4TOUCH_FGC_SWBOFF          (NCS_TYPE4TOUCH + 0x14)
#define NCS4TOUCH_BGC_SWBOFF          (NCS_TYPE4TOUCH + 0x15)

#define NCS4TOUCH_BGC_NTB_HITEM       (NCS_TYPE4TOUCH + 0x21)
#define NCS4TOUCH_BGC_NTB_DITEM       (NCS_TYPE4TOUCH + 0x22)
#define NCS4TOUCH_BGC_NTB_BLOCK       (NCS_TYPE4TOUCH + 0x23)

#define NCS4TOUCH_BGC_BNB_BUTTON      (NCS_TYPE4TOUCH + 0x26)
#define NCS4TOUCH_FGC_BNB_BUTTON      (NCS_TYPE4TOUCH + 0x27)
#define NCS4TOUCH_FGC_BNB_TEXT        (NCS_TYPE4TOUCH + 0x28)
#define NCS4TOUCH_FGC_INB_TEXT        (NCS_TYPE4TOUCH + 0x29)

#define NCS4TOUCH_BGC_PCK_MAIN        (NCS_TYPE4TOUCH + 0x30)
#define NCS4TOUCH_BGC_PCK_PICKER      (NCS_TYPE4TOUCH + 0x31)
#define NCS4TOUCH_FGC_PCK_PICKER      (NCS_TYPE4TOUCH + 0x32)
#define NCS4TOUCH_BGC_PCK_SELECT      (NCS_TYPE4TOUCH + 0x33)
#define NCS4TOUCH_FGC_PCK_DISABLE     (NCS_TYPE4TOUCH + 0x34)

#define NCS4TOUCH_BGC_ITB                 (NCS_TYPE4TOUCH + 0x41)
#define NCS4TOUCH_BGC_ITB_LIGHT           (NCS_TYPE4TOUCH + 0x42)
#define NCS4TOUCH_FGC_ITB_TICK            (NCS_TYPE4TOUCH + 0x43)
#define NCS4TOUCH_FGC_ITB_TICK_LIGHT      (NCS_TYPE4TOUCH + 0x44)
#define NCS4TOUCH_FGC_ITB_ANGLE           (NCS_TYPE4TOUCH + 0x45)
#define NCS4TOUCH_FGC_ITB_ANGLE_LIGHT     (NCS_TYPE4TOUCH + 0x46)
#define NCS4TOUCH_FGC_ITB_LTEXT           (NCS_TYPE4TOUCH + 0x47)
#define NCS4TOUCH_FGC_ITB_LTEXT_LIGHT     (NCS_TYPE4TOUCH + 0x48)
#define NCS4TOUCH_FGC_ITB_RTEXT           (NCS_TYPE4TOUCH + 0x49)
#define NCS4TOUCH_FGC_ITB_RTEXT_LIGHT     (NCS_TYPE4TOUCH + 0x4a)

#define padGetElement   ncsGetElement
#define padSetElement   ncsSetElement

#define NCS4TOUCH_RENDERER       "fashion"

BOOL ncsInitPadRenderers(void);

#ifdef __cplusplus
}
#endif

#endif

