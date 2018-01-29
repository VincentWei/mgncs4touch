#ifndef __MGUI_NCSCTRL_HSCROLLVIEWPIECE_H
#define __MGUI_NCSCTRL_HSCROLLVIEWPIECE_H

typedef struct _mHScrollViewPieceClass mHScrollViewPieceClass;
typedef struct _mHScrollViewPiece mHScrollViewPiece;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define mHScrollViewPieceClassHeader(clss, superCls) \
    mPanelPieceClassHeader(clss, superCls) \
    void (*moveViewport)(clss*, int x, int y); \
    void (*getViewport)(clss*, RECT *rc); \
    void (*showScrollBar)(clss*, BOOL show); \
    void (*enableCache)(clss*, BOOL cachable);

struct _mHScrollViewPieceClass
{
    mHScrollViewPieceClassHeader(mHScrollViewPiece, mPanelPiece)
};

MGNCS_EXPORT extern mHScrollViewPieceClass g_stmHScrollViewPieceCls;

#define mHScrollViewPieceHeader(clss) \
    mPanelPieceHeader(clss) \
    unsigned int m_timePressed; \
    int m_bScrollbarAutoHided; \
    BOOL m_bNeedScrollBar; \
    BOOL m_bPressed; \
    BOOL m_bMouseMoved; \
    unsigned int m_mouseFlag; /* 1: move, 2: cancel animation */ \
    BOOL m_bTimedout; \
    POINT m_pressMousePos; \
    POINT m_oldMousePos; \
    float m_ratioX; \
    float m_ratioY; \
    HDC m_cache; \
    BOOL m_cachable; \
    RECT m_contentDirtyRect; \
    RECT m_cachedViewport; \
    mPieceItem *m_content; \
    mPieceItem *m_scrollbar; \
    MGEFF_ANIMATION m_animation; \
    int m_movingStatus; /* -1, 0, 1 */ \
    void *m_phy_ctx;

struct _mHScrollViewPiece
{
    mHScrollViewPieceHeader(mHScrollViewPiece)
};

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __MGUI_NCSCTRL_HSCROLLVIEWPIECE_H */
