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

#ifndef __MGNCS4TOUCH_PCK_H__
#define __MGNCS4TOUCH_PCK_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_PICKER          NCSCLASSNAME("picker")

typedef struct _mPicker         mPicker;
typedef struct _mPickerClass    mPickerClass;
typedef struct _mPickerRenderer mPickerRenderer;

#define mPickerHeader(clsName)       \
    mAnimationHeader(clsName)        \
    PRIVATE MGEFF_ANIMATION handle;  \
    PRIVATE HDC bkDC;                \
    PRIVATE float key;               \
    PRIVATE int mouseY;              \
    PRIVATE unsigned char itemAlign; \
    PRIVATE unsigned int hMargin;    \
    PRIVATE unsigned int vMargin;    \
    PUBLIC unsigned int itemVisible; \
    PUBLIC int maxValue;             \
    PUBLIC int minValue;             \
    PUBLIC unsigned int span;        \
    PUBLIC unsigned int numBits;     \
    PUBLIC unsigned int showBits;    \
    PUBLIC int selIdx;

struct _mPicker {
    mPickerHeader(mPicker)
};

#define mPickerClassHeader(clsName, parentClass)                              \
    mAnimationClassHeader(clsName, parentClass)                               \
    PRIVATE void (*runAnimation)(clsName*, float startvalue, float endvalue); \
    PRIVATE void (*endAnimation)(clsName*);                                   \
    PUBLIC BOOL (*addItem)(clsName*, int index, const char* string);          \
    PUBLIC BOOL (*addItems)(clsName*, const char** strings, int len);         \
    PUBLIC BOOL (*rmItem)(clsName*, int index);                               \
    PUBLIC BOOL (*setRange)(clsName*, int min, int max);                      \
    PUBLIC BOOL (*setSpan)(clsName*, int span);                               \
    PUBLIC void (*enableItems)(clsName*, int idxFrom, int count);             \
    PUBLIC void (*disableItems)(clsName*, int idxFrom, int count);

struct _mPickerClass {
    mPickerClassHeader(mPicker, mAnimation)
};

#define mPickerRendererHeader(clsName, parentClass) \
    mAnimationRendererHeader(clsName, parentClass)

struct _mPickerRenderer {
    mPickerRendererHeader(mPicker, mAnimation)
};

#define NCSS_PCK_NUMBER     (0x01 << NCSS_ICONV_SHIFT)
#define NCSS_PCK_STRINGS    (0x02 << NCSS_ICONV_SHIFT)
#define NCSS_PCK_LOOP       (NCSS_ICONV_LOOP)
// left round corner
#define NCSS_PCK_LRC    	(0x04 << NCSS_ICONV_SHIFT)
// right round corner
#define NCSS_PCK_RRC    	(0x08 << NCSS_ICONV_SHIFT)
// both round corner
#define NCSS_PCK_BRC    	(0x10 << NCSS_ICONV_SHIFT)

#define NCSS_PCK_SHIFT      (NCSS_ICONV_SHIFT + 5)

enum mPickerProp {
    NCSP_PCK_ALIGN = NCSP_ICONV_MAX + 1,
    NCSP_PCK_LINES,
    NCSP_PCK_COUNT,
    NCSP_PCK_SELIDX,
    NCSP_PCK_HMARGIN,
    NCSP_PCK_VMARGIN,
    NCSP_PCK_NUMBERBITS,
    NCSP_PCK_SHOWBERBITS,
    NCSP_PCK_MAX
};

enum mPickerNotify {
    NCSN_PCK_SELCHANGED = NCSN_ICONV_MAX + 1,
    NCSN_PCK_REACHMIN,
    NCSN_PCK_REACHMAX,
    NCSN_PCK_MAX
};

MPAD_EXPORT extern mPickerClass g_stmPickerCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_PCK_H__ */

