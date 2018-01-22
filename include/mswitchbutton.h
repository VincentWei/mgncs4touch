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

#ifndef __MGNCS4TOUCH_SWB_H__
#define __MGNCS4TOUCH_SWB_H__

/**
 * \defgroup ControlSwitchButton mSwitchButton
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_SWBUTTON
 * \brief the name of switchbutton control
 */
#define NCSCTRL_SWBUTTON              NCSCLASSNAME("switchbutton")

typedef struct _mSwitchButton         mSwitchButton;
typedef struct _mSwitchButtonClass    mSwitchButtonClass;
typedef struct _mSwitchButtonRenderer mSwitchButtonRenderer;

/**
 * \enum eSwitchButtonCheckState
 * \brief define the check state of switchbutton
 *
 * \var NCS_SWB_OFF
 * \brief SwitchButton status, OFF
 *
 * \var NCS_SWB_ON
 * \brief SwitchButton status, ON
 */
enum mSwitchButtonStatus {
    NCS_SWB_OFF = 0,
    NCS_SWB_ON
};

#define mSwitchButtonHeader(clsName) \
    mWidgetHeader(clsName)           \
    DWORD status;                    \
    BOOL  pressOnBlock;              \
    int   radius;                    \
    int   mouseX;                    \
    int   moveFrom;                  \
    int   dist;                      \
    int   step;

/**
 * \struct mSwitchButton
 * \brief SwitchButton class, derived from \ref mWidget
 *
 * -int \b status - the switch status, 0-OFF, 1-ON
 *
 * \sa \ref mWidget
 */
struct _mSwitchButton {
    mSwitchButtonHeader(mSwitchButton)
};

#define mSwitchButtonClassHeader(clsName, parentClass)  \
    mWidgetClassHeader(clsName, parentClass)            \
    void (*runAnimation)(clsName*, int start, int end, int duration);

/**
 * \struct mSwitchButtonClass
 * \brief  switchbutton struct of switchbutton control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */
struct _mSwitchButtonClass {
    mSwitchButtonClassHeader(mSwitchButton, mWidget)
};

#define mSwitchButtonRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mSwitchButtonRenderer
 * \brief SwitchButton class renderer interface, derived from \ref mWidgetRenderer
 *
 * \sa mSwitchButton, mSwitchButtonClass, mWidgetRenderer
 */
struct _mSwitchButtonRenderer {
    mSwitchButtonRendererHeader(mSwitchButton, mWidget)
};

/**
 * \var g_stmSwitchButtonCls
 * \brief global mSwitchButtonClass
 */
MPAD_EXPORT extern mSwitchButtonClass g_stmSwitchButtonCls;

/**
 * \define mSwitchButtonStyle
 * \brief the style id of \ref mSwitchButton
 */
#define NCSS_SWB_SHIFT      (NCSS_WIDGET_SHIFT)

/**
 * \enum mSwitchButtonProp
 * \brief the properties id of \ref mSwitchButton
 *
 * \var NCSP_SWB_STATUS
 * \brief Label control displays the switch status.
 */
enum mSwitchButtonProp {
    NCSP_SWB_STATUS = NCSP_WIDGET_MAX + 1,
    NCSP_SWB_MAX
};

/**
 * \enum eSwitchButtonNotify
 * \brief the notification code of \ref mSwitchButton
 *
 * \var NCSN_SWB_STATUSCHANGED
 * \brief Identify switch control choice status changes.
 */
enum mSwitchButtonNotify {
    NCSN_SWB_STATUSCHANGED = NCSN_WIDGET_MAX + 1,
    NCSN_SWB_MAX
};

/**
 * @} end of ControlSwitchButton
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_SWB_H__ */

