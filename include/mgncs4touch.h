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

#ifndef __MGNCS4TOUCH_H__
#define __MGNCS4TOUCH_H__

#define MGNCS4TOUCH_ETCFILE "mgncs4touch.cfg"

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "manimation.h"
#include "mswitchbutton.h"
#include "mnewtrackbar.h"
#include "mpicker.h"
#include "mcombopicker.h"
#include "mtimepicker.h"
#include "mdatepicker.h"
#include "mbtnnavbar.h"
#include "mimgnavbar.h"
#include "pieces/mshapepushbuttonpiece.h"
#include "pieces/mshapeboxpiece.h"
#include "pieces/msimagepiece.h"
#include "pieces/mcolorablelabelpiece.h"
#include "mexlist.h"
#include "pieces/mnsdrawpiece.h"
#include "mitembar.h"

#ifdef __cplusplus
extern "C" {
#endif

MPAD_EXPORT extern BOOL ncs4PadInitialize(void);

MPAD_EXPORT extern void ncs4PadUninitialize(void);

#ifdef __cplusplus
}
#endif

#endif //__MGNCS4TOUCH_H__
