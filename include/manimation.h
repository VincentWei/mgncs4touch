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

#ifndef __MGNCS4PAD_ANIMATION_H__
#define __MGNCS4PAD_ANIMATION_H__

/**
 * \defgroup Ctrl_Aniamtion mAnimation
 * @{
*/
#ifdef  __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define NCSCTRL_ANIMATION          NCSCLASSNAME("animation")

typedef struct _mAnimation         mAnimation;
typedef struct _mAnimationClass    mAnimationClass;
typedef struct _mAnimationRenderer mAnimationRenderer;

#define mAnimationHeader(className) \
    mItemViewHeader(className)

/**
 * \struct mAnimation
 * \brief
 *
 *  - property\n
 *
 *  - varianttype\n
 *
 *  - start_value\n
 *
 *  - end_value\n
 *
 *  - curve\n
 *
 *  - duration\n
 *
 */
struct _mAnimation {
    mAnimationHeader(mAnimation)
};

#define mAnimationClassHeader(clsName, parentClass)             \
    mItemViewClassHeader(clsName, parentClass)                  \
    MGEFF_ANIMATION (*createAnimation)(clsName*, const EffPropertyAnimationSetting*);

/**
 * \struct mAnimationClass
 * \brief The virtual function table of mAnimation,
 *        which derived from mItemView.
 *
 */
struct _mAnimationClass {
    mAnimationClassHeader(mAnimation, mItemView)
};

#define mAnimationRendererHeader(clsName, parentClass) \
    mItemViewRendererHeader(clsName, parentClass)

struct _mAnimationRenderer {
    mAnimationRendererHeader(mAnimation, mItemView)
};

/**
 * \var g_stmAnimationCls
 * \brief The instance of mAnimationClass.
 *
 * \sa mAnimationClass
 */
MGNCS_EXPORT  extern mAnimationClass g_stmAnimationCls;

#ifdef  __cplusplus
}
#endif  /* __cplusplus */

#endif /* __MGNCS4PAD_ANIMATION_H__ */

