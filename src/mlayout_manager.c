#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

static void mLayoutManager_reLayout(mLayoutManager *self, mItemIterator* iter, int w, int h)
{
    return;
}

BEGIN_MINI_CLASS(mLayoutManager, mObject)
    CLASS_METHOD_MAP(mLayoutManager, reLayout)
END_MINI_CLASS
