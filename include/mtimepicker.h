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

#ifndef __MGNCS4PAD_TPCK_H__
#define __MGNCS4PAD_TPCK_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_TIMEPICKER          NCSCLASSNAME("timepicker")

typedef struct _mTimePicker         mTimePicker;
typedef struct _mTimePickerClass    mTimePickerClass;
typedef struct _mTimePickerRenderer mTimePickerRenderer;

#define mTimePickerHeader(clsName) \
    mComboPickerHeader(clsName)    \
    int idAmpm;                    \
    int idHour;                    \
    int idMinute;                  \
    int hour;                      \
    int minute;                    \
    char *ampmStrs;

struct _mTimePicker {
    mTimePickerHeader(mTimePicker)
};

#define mTimePickerClassHeader(clsName, parentClass)        \
    mComboPickerClassHeader(clsName, parentClass)           \
    PUBLIC void (*setTime)(clsName*, int hour, int minute); \
    PUBLIC BOOL (*getTime)(clsName*, int* hour, int* minute);
    
struct _mTimePickerClass {
    mTimePickerClassHeader(mTimePicker, mComboPicker)
};

#define mTimePickerRendererHeader(clsName, parentClass) \
    mComboPickerRendererHeader(clsName, parentClass)

struct _mTimePickerRenderer {
    mTimePickerRendererHeader(mTimePicker, mComboPicker)
};

#define NCSS_TPCK_24H        (0 << NCSS_CMBPCK_SHIFT)
#define NCSS_TPCK_12H        (1 << NCSS_CMBPCK_SHIFT)
#define NCSS_TPCK_SHIFT        (1 + NCSS_CMBPCK_SHIFT)

enum mTimePickerProp {
    NCSP_TPCK_HOUR = NCSP_CMBPCK_MAX + 1,
    NCSP_TPCK_MINUTE,
    NCSP_TPCK_AMPMSTRINGS,
    NCSP_TPCK_MAX
};

enum mTimePickerNotify {
    NCSN_TPCK_TIMECHANGED = NCSN_CMBPCK_MAX + 1,
    NCSN_TPCK_MAX
};

MPAD_EXPORT extern mTimePickerClass g_stmTimePickerCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4PAD_TPCK_H__ */

