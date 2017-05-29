/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Feynman Software Technology Co., Ltd.
 * Room 508B-C, Floor 5, Citic Guoan Shumagang, No.32, Haidian South
 * Road, Haidian District, Beijing, P. R. CHINA 100080.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Feynman Software Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall use
 * it only in accordance you entered into with Feynman Software.
 *          http://www.minigui.com
 *
 *    FileName : mitembar.h
 *      Author : <wangxin@minigui.org>
 * Create Date : Monday September 25, 2010
 *     Version : 0.0.0.1
 *********************************************************************/

#ifndef __NCS4PAD_ITEM_BAR_H__
#define __NCS4PAD_ITEM_BAR_H__

/**
 * \defgroup ControlItemBar mItemBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_ITEMBAR
 * \brief the name of itembar control
 */
#define NCSCTRL_ITEMBAR     NCSCLASSNAME("itembar")

typedef struct _mItemBar         mItemBar;
typedef struct _mItemBarClass    mItemBarClass;
typedef struct _mItemBarRenderer mItemBarRenderer;

#define mItemBarHeader(clsName) \
    mWidgetHeader(clsName)      \
    BOOL dotHit;                \
    int tickSize;               \
    int imageSize;              \
    int ltextSize;              \
    int rtextSize;              \
    int angleSize;

/**
 * \struct mItemBar
 * \brief ItemBar class, derived from \ref mWidget
 *
 * \sa \ref mWidget
 */
struct _mItemBar {
    mItemBarHeader(mItemBar)
};

#define mItemBarClassHeader(clsName, parentClass) \
    mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mItemBarClass
 * \brief itembar struct of itembar control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */
struct _mItemBarClass {
    mItemBarClassHeader(mItemBar, mWidget)
};

#define mItemBarRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mItemBarRenderer
 * \brief ItemBar class renderer interface, derived from \ref mWidgetRenderer
 *
 * \sa mItemBar, mItemBarClass, mWidgetRenderer
 */
struct _mItemBarRenderer {
    mItemBarRendererHeader(mItemBar, mWidget)
};

/**
 * \var g_stmItemBarCls
 * \brief global mItemBarClass
 */
MPAD_EXPORT extern mItemBarClass g_stmItemBarCls;

/**
 * \enum mItemBarProp
 * \brief the properties id of \ref mItemBar
 *
 * \var NCSP_ITEMBAR_CHECKED
 * \brief Identify with NCSS_ITEMBAR_CHECKABLE style controls, is selecte.
 *
 * \var NCSP_ITEMBAR_IMG
 * \brief Displayed as icons with a picture.
 *
 * \var NCSP_ITEMBAR_MARGIN
 * \brief Both ends of the blank size.
 */
enum mItemBarProp {
    NCSP_ITEMBAR_CHECKED = NCSP_WIDGET_MAX + 1,
    NCSP_ITEMBAR_IMG,
    NCSP_ITEMBAR_MARGIN,
    NCSP_ITEMBAR_MAX
};

/**
 * \enum eItemBarNotify
 * \brief the notification code of \ref mItemBar
 *
 * \var NCSN_ITEMBAR_CHECKED
 * \brief Identifies the control with NCSS_ITEMBAR_CHECKABLE style, selection status changes.
 */
enum mItemBarNotify {
    NCSN_ITEMBAR_CHECKED = NCSN_WIDGET_MAX + 1,
    NCSN_ITEMBAR_MAX
};

/**
 * \define mItemBarStyle
 * \brief the style id of \ref mItemBar
 *
 * \var NCSS_ITEMBAR_CHECKABLE
 * \brief Click Check whether there is support for the control.
 *
 * \var NCSS_ITEMBAR_HASCHILD
 * \brief Whether child controls identified.
 */
#define NCSS_ITEMBAR_CHECKABLE      (1 << NCSS_WIDGET_SHIFT)
#define NCSS_ITEMBAR_HASCHILD       (2 << NCSS_WIDGET_SHIFT)
#define NCSS_ITEMBAR_SHIFT          (NCSS_WIDGET_SHIFT + 2)

/**
 * @} end of ControlItemBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __NCS4PAD_ITEM_BAR_H__ */

