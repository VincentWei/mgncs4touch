/*!============================================================================
 * @file balloon_tip_maker.h 
 * @Synopsis  
 * @version 1.0
 *  Company: Beijing Feynman Software Technology Co., Ltd.
 */

#ifndef  BALLOON_TIP_MAKER_INC
#define  BALLOON_TIP_MAKER_INC

enum _TRIANGLE_DIRECTION {
    TRIANGLE_UP = 1,
    TRIANGLE_DOWN,
    TRIANGLE_LEFT,
    TRIANGLE_RIGHT,
}typedef TRIANGLE_DIRECTION;

struct _BALLOONTIP_SHAPE {
    int main_x, main_y, main_w, main_h;

    int triangle_height;
    TRIANGLE_DIRECTION triangle_direction;
}typedef BALLOONTIP_SHAPE;

int SetBalloonTipStyle (HWND hWnd, unsigned short triangle_offset, TRIANGLE_DIRECTION triangle_direction);
#endif   /* ----- #ifndef BALLOON_TIP_MAKER_INC  ----- */

