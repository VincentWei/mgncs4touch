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

#ifndef __MGNCS4TOUCH_DPCK_H__
#define __MGNCS4TOUCH_DPCK_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_DATEPICKER          NCSCLASSNAME("datepicker")

typedef struct _mDatePicker         mDatePicker;
typedef struct _mDatePickerClass    mDatePickerClass;
typedef struct _mDatePickerRenderer mDatePickerRenderer;

#define mDatePickerHeader(clsName) \
    mComboPickerHeader(clsName)    \
    PUBLIC int idYear;             \
    PUBLIC int idMonth;            \
    PUBLIC int idDay ;             \
    PUBLIC int year;               \
    PUBLIC int month;              \
    PUBLIC int day;

struct _mDatePicker {
    mDatePickerHeader(mDatePicker)
};

#define mDatePickerClassHeader(clsName, parentClass)                 \
    mComboPickerClassHeader(clsName, parentClass)                    \
    PUBLIC void (*setDate)(clsName*, int year, int month, int day);  \
    PUBLIC BOOL (*getDate)(clsName*, int* year, int* month, int* day);

struct _mDatePickerClass {
    mDatePickerClassHeader(mDatePicker, mComboPicker)
};

#define mDatePickerRendererHeader(clsName, parentClass) \
    mComboPickerRendererHeader(clsName, parentClass)

struct _mDatePickerRenderer {
    mDatePickerRendererHeader(mDatePicker, mComboPicker)
};

#define NCSS_DPCK_YYYYMMDD  (0 << NCSS_CMBPCK_SHIFT)
#define NCSS_DPCK_MMDDYYYY  (1 << NCSS_CMBPCK_SHIFT)
#define NCSS_DPCK_YYMMDD    (2 << NCSS_CMBPCK_SHIFT)
#define NCSS_DPCK_MMDDYY    (4 << NCSS_CMBPCK_SHIFT)

#define NCSS_DPCK_SHIFT     (3 + NCSS_CMBPCK_SHIFT)

enum mDatePickerProp {
    NCSP_DPCK_YEAR = NCSP_CMBPCK_MAX + 1,
    NCSP_DPCK_MONTH,
    NCSP_DPCK_DAY,
    NCSP_DPCK_MAX
};

enum mDatePickerNotify {
    NCSN_DPCK_DATECHANGED = NCSN_CMBPCK_MAX + 1,
    NCSN_DPCK_MAX
};

MPAD_EXPORT extern mDatePickerClass g_stmDatePickerCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_DPCK_H__ */

