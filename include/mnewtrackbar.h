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

#ifndef __MGNCS4TOUCH_NTRKBAR_H__
#define __MGNCS4TOUCH_NTRKBAR_H__

/**
 * \defgroup ControlNewTrackBar mNewTrackBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_NEWTRACKBAR
 * \brief the name of newtrackbar control
 */
#define NCSCTRL_NEWTRACKBAR          NCSCLASSNAME("newtrackbar")

typedef struct _mNewTrackBar         mNewTrackBar;
typedef struct _mNewTrackBarClass    mNewTrackBarClass;
typedef struct _mNewTrackBarRenderer mNewTrackBarRenderer;

#define mNewTrackBarHeader(clsName) \
    mTrackBarHeader(clsName)

/**
 * \struct mNewTrackBar
 * \brief NewTrakBar class, derived from \ref mTrackBar
 *
 * \sa \ref mTrackBar
 */
struct _mNewTrackBar {
    mNewTrackBarHeader(mNewTrackBar)
};

#define mNewTrackBarClassHeader(clsName, parentClass) \
    mTrackBarClassHeader(clsName, parentClass)

/**
 * \struct mNewTrackBarClass
 * \brief  newtrackbar struct of newtrackbar control, derived from \ref mTrackBarClass
 *
 * \sa \ref mTrackBarClass
 */
struct _mNewTrackBarClass {
    mNewTrackBarClassHeader(mNewTrackBar, mTrackBar)
};

#define mNewTrackBarRendererHeader(clsName, parentClass) \
    mTrackBarRendererHeader(clsName, parentClass)

/**
 * \struct mNewTrackBarRenderer
 * \brief NewTrakBar class renderer interface, derived from \ref mTrackBarRenderer
 *
 * \sa mNewTrackBar, mNewTrackBarClass, mTrackBarRenderer
 */
struct _mNewTrackBarRenderer {
    mNewTrackBarRendererHeader(mNewTrackBar, mTrackBar)
};

/**
 * \var g_stmNewTrackBarCls
 * \brief global mNewTrackBarClass
 */
MPAD_EXPORT extern mNewTrackBarClass g_stmNewTrackBarCls;

/**
 * \define mNewTrackBarStyle
 * \brief the style id of \ref mNewTrackBar
 *
 * \var NCSS_NTRKBAR_CIRCLE
 * \brief Identified as a circular slider.
 */
#define NCSS_NTRKBAR_CIRCLE       (1 << (NCSS_TRKBAR_SHIFT))
#define NCSS_NTRKBAR_SHIFT        (1 + NCSS_TRKBAR_SHIFT)

/**
 * \enum mNewTrackBarProp
 * \brief the properties id of \ref mNewTrackBar
 */
enum mNewTrackBarProp {
    NCSP_NTRKBAR_MAX = NCSP_TRKBAR_MAX + 1,
};

#define HORZ_NTRKBAR_SLIDER_W        16
#define HORZ_NTRKBAR_SLIDER_H        16
#define VERT_NTRKBAR_SLIDER_W        16
#define VERT_NTRKBAR_SLIDER_H        16

/**
 * @} end of ControlNewTrakBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4TOUCH_NTRKBAR_H__ */

