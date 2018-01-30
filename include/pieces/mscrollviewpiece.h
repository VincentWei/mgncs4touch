#ifndef __MGUI_NCSCTRL_SCROLLVIEWPIECE_H
#define __MGUI_NCSCTRL_SCROLLVIEWPIECE_H

typedef struct _mScrollViewPieceClass mScrollViewPieceClass;
typedef struct _mScrollViewPiece mScrollViewPiece;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define mScrollViewPieceClassHeader(clss, superCls) \
    mPanelPieceClassHeader(clss, superCls) \
    void (*moveViewport)(clss*, int x, int y); \
    void (*getViewport)(clss*, RECT *rc); \
    void (*showScrollBar)(clss*, BOOL show); \
    void (*enableCache)(clss*, BOOL cachable);

struct _mScrollViewPieceClass
{
    mScrollViewPieceClassHeader(mScrollViewPiece, mPanelPiece)
};

MGNCS_EXPORT extern mScrollViewPieceClass g_stmScrollViewPieceCls;

#define mScrollViewPieceHeader(clss) \
    mPanelPieceHeader(clss) \
    DWORD m_timePressed; \
    BOOL m_bScrollbarAutoHided; \
    BOOL m_bNeedScrollBar; \
    BOOL m_bPressed; \
    BOOL m_bMouseMoved; \
    int m_movingStatus; /* -1, 0, 1 */ \
    unsigned int m_mouseFlag; /* 1: move, 2: cancel animation */ \
    BOOL m_bTimedout; \
    BOOL m_cachable; \
    HDC m_cache; \
    POINT m_pressMousePos; \
    POINT m_oldMousePos; \
    float m_ratioX; \
    float m_ratioY; \
    RECT m_contentDirtyRect; \
    RECT m_cachedViewport; \
    mPieceItem *m_content; \
    mPieceItem *m_scrollbar; \
    MGEFF_ANIMATION m_animation; \
    void *m_phy_ctx;

struct _mScrollViewPiece
{
    mScrollViewPieceHeader(mScrollViewPiece)
};

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __MGUI_NCSCTRL_SCROLLVIEWPIECE_H */
