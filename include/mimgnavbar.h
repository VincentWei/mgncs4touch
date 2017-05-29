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

#ifndef __MGNCS4PAD_INB_H__
#define __MGNCS4PAD_INB_H__

/**
 * \defgroup ControlImgNavBar mImgNavBar
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \def NCSCTRL_IMGNAVBAR
 * \brief the name of imgnavbar control
 */
#define NCSCTRL_IMGNAVBAR          NCSCLASSNAME("imgnavbar")

typedef struct _mImgNavBar         mImgNavBar;
typedef struct _mImgNavBarClass    mImgNavBarClass;
typedef struct _mImgNavBarRenderer mImgNavBarRenderer;

/**
 * \define mImgNavBarStyle
 * \brief the style id of \ref mImgNavBar
 */
#define NCSS_INB_SHIFT      (NCSS_WIDGET_SHIFT + 0)

/**
 * \enum mImgNavBarProp
 * \brief the properties id of \ref mImgNavBar
 *
 * \var NCSP_INB_LBITMAP
 * \brief Placed on the left of the picture.
 *
 * \var NCSP_INB_RBITMAP
 * \brief Placed on the right of the picture.
 *
 * \var NCSP_INB_FONT
 * \brief Set the font size on the navigation bar.
 *
 * \var NCSP_INB_MARGIN
 * \brief Left and right ends of the blank reserved for control.
 *
 * \var NCSP_INB_LEFT_W
 * \brief The width of the left image.
 *
 * \var NCSP_INB_RIGHT_W
 * \brief The width of the right image.
 *
 * \var NCSP_INB_IMG_H
 * \brief Left and right images of the high.
 *
 * \var NCSP_INB_DRAWMODE
 * \brief About the image display modes.
 */
enum mImgNavBarProp {
    NCSP_INB_LBITMAP = NCSP_WIDGET_MAX + 1,
    NCSP_INB_RBITMAP,
    NCSP_INB_FONT,
    NCSP_INB_MARGIN,
    NCSP_INB_LEFT_W,
    NCSP_INB_RIGHT_W,
    NCSP_INB_IMG_H,
    NCSP_INB_DRAWMODE,
    NCSP_INB_MAX
};

#define mImgNavBarHeader(clsName) \
    mWidgetHeader(clsName)        \
    PLOGFONT caption_font;        \
    int margin_w;                 \
    int leftbt_w;                 \
    int rightbt_w;                \
    int img_h;

/**
 * \struct mImgNavBar
 * \brief ImgNavBar class, derived from \ref mWidget
 *
 * \sa \ref mWidget
 */
struct _mImgNavBar {
    mImgNavBarHeader(mImgNavBar)
};

#define mImgNavBarClassHeader(clsName, parentClass) \
    mWidgetClassHeader(clsName, parentClass)

/**
 * \struct mImgNavBarClass
 * \brief  toolcaption struct of toolcaption control, derived from \ref mWidgetClass
 *
 * \sa \ref mWidgetClass
 */
struct _mImgNavBarClass {
    mImgNavBarClassHeader(mImgNavBar, mWidget)
};

#define mImgNavBarRendererHeader(clsName, parentClass) \
    mWidgetRendererHeader(clsName, parentClass)

/**
 * \struct mImgNavBarRenderer
 * \brief ImgNavBar class renderer interface, derived from \ref mWidgetRenderer
 *
 * \sa mImgNavBar, mImgNavBarClass, mWidgetRenderer
 */
struct _mImgNavBarRenderer {
    mImgNavBarRendererHeader(mImgNavBar, mWidget)
};

/**
 * \var g_stmImgNavBarCls
 * \brief global mImgNavBarClass
 */
MPAD_EXPORT extern mImgNavBarClass g_stmImgNavBarCls;

/**
 * \enum mImgNavBarData
 * \brief define the clicked add
 *
 * \var NCSD_INB_LEFT
 * \brief Left image click event listener param.
 *
 * \var NCSD_INB_RIGHT
 * \brief Right image click event listener param.
 */
enum mImgNavBarData {
    NCSD_INB_LEFT = 0,
    NCSD_INB_RIGHT,
};

/**
 * \enum eImgNavBarNotify
 * \brief the notification code of \ref mImgNavBar
 *
 * \var NCSN_IBN_IMGCLICK
 * \brief Around both ends of the image click event listener.
 */
enum mImgNavBarNotify {
    NCSN_INB_IMGCLICK = NCSN_WIDGET_MAX + 1,
    NCSN_INB_MAX,
};

/**
 * \enum mImgNavBarBoxLayoutPieceIndexID
 * \brief the piece index id of \ref mImgNavBar
 *
 * \var INB_LEFT_PIECE_ID
 * \brief The left piece of the index id.
 *
 * \var INB_RIGHT_PIECE_ID
 * \brief The right piece of the index id.
 */
enum mImgNavBarBoxLayoutPieceIndexID {
    INB_LEFT_PIECE_ID = 0,
    // 1 is the text piece (NULL)
    INB_RIGHT_PIECE_ID = 2,
    INB_MAX_PIECE_ID,
};

#define GetLeftPiece(self)   _M((mHBoxLayoutPiece *)self->body, getCell, INB_LEFT_PIECE_ID)
#define GetRightPiece(self)  _M((mHBoxLayoutPiece *)self->body, getCell, INB_RIGHT_PIECE_ID)

/**
 * @} end of ControlImgNavBar
 */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __MGNCS4PAD_INB_H__ */

