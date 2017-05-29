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
 *    FileName : mcombopicker.h
 *      Author : <wangxin@minigui.org>
 * Create Date : Tuesday September 07, 2010
 *     Version : 0.0.0.1
 *********************************************************************/

#ifndef __MGNCS4PAD_COMBOPICKER_H__
#define __MGNCS4PAD_COMBOPICKER_H__

/**
 * \defgroup Control_ComboPicker mComboPicker
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct _PICKITM {
    list_t   list;
    mPicker* picker;
    int      pickerID;
} CMBPICKITM, *PCMBPICKITM;

/**
 * \def NCSCTRL_CMBPICKER
 * \brief The control class name of combopicker.
 */
#define NCSCTRL_CMBPICKER            NCSCLASSNAME("combopicker")

typedef struct _mComboPicker         mComboPicker;
typedef struct _mComboPickerClass    mComboPickerClass;
typedef struct _mComboPickerRenderer mComboPickerRenderer;

#define mComboPickerHeader(clsName) \
    mPanelHeader(clsName)           \
    list_t pickerList;              \
    int pickerCount;                \
    int hMargin;                    \
    int vMargin;                    \
    int visLine;

struct _mComboPicker {
    mComboPickerHeader(mComboPicker)
};

#define mComboPickerClassHeader(clsName, parentClass)                   \
    mPanelClassHeader(clsName, parentClass)                             \
    BOOL (*addPicker)(clsName*, const NCS_WND_TEMPLATE* pick, int idx); \
    BOOL (*removePicker)(clsName*, int id);                             \
    BOOL (*removePickerByIdx)(clsName*, int idx);                       \
    BOOL (*removeAllPickers)(clsName*);                                 \
    void (*getClientArea)(clsName*, PRECT prc);                         \
    BOOL (*listenerCallBack)(mObject* self,                             \
            mObject* sender, int eventId, DWORD param); 

struct _mComboPickerClass {
    mComboPickerClassHeader(mComboPicker, mPanel)
};

#define mComboPickerRendererHeader(clsName, parentClass) \
    mPanelRendererHeader(clsName, parentClass)

/**
 * \struct mComboPickerRenderer
 * \brief The renderer structure of mComboPicker, which inheried from
 *        mComboPickerRenderer.
 */
struct _mComboPickerRenderer {
    mComboPickerRendererHeader(mComboPicker, mPanel)
};

/**
 * \var g_stmComboPickerCls
 * \brief The instance of mComboPickerClass.
 *
 * \sa mComboPickerClass
 */
MPAD_EXPORT extern mComboPickerClass g_stmComboPickerCls;

/**
 * \def NCSS_CMBPCK_SHIFT
 * \brief The bits used by mComboPicker in style.
 */
#define NCSS_CMBPCK_SHIFT        NCSS_PANEL_SHIFT

/**
 * \enum mComboPickerProperty
 * \brief the properties id of \ref mComboPicker
 *
 * \var NCSP_CMBPCK_HMARGIN
 * \brief Left and right ends of the blank reserved.
 *
 * \var NCSP_CMBPCK_VMARGIN
 * \brief Reserved for the upper and lower ends of the blank.
 *
 * \var NCSP_CMBPCK_VISLINE
 * \brief Information on the number of rows to display.
 */
enum mComboPickerProperty {
    NCSP_CMBPCK_HMARGIN = NCSP_PANEL_MAX + 1,
    NCSP_CMBPCK_VMARGIN,
    NCSP_CMBPCK_VISLINE,
    NCSP_CMBPCK_MAX,
};

/**
 * \enum mComboPickerNotify
 * \brief The notification code id of mComboPicker.
 */
enum mComboPickerNotify {
    /**
     * The maximum value of mComboPicker notification code id.
     */
    NCSN_CMBPCK_MAX = NCSN_PANEL_MAX + 1
};

/**
 * @} end of Control_ComboPicker
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4PAD_COMBOPICKER_H__ */

