/*
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/fixedmath.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"

// #define NDEBUG	1
#include "mtouchdebug.h"


#ifndef __NOUNIX__
#    include <unistd.h>
#    include <pwd.h>
#endif

#ifdef WIN32
#include <sys/stat.h>
#endif


static NCS_RDR_ID_NAME config_str[] = {
    {"roundrect_radius",        NCS4TOUCH_RRECT_RADIUS        },
    {"bgc_block",               NCS4TOUCH_BGC_BLOCK           },
    {"fgc_swb_on",              NCS4TOUCH_FGC_SWBON           },
    {"bgc_swb_on",              NCS4TOUCH_BGC_SWBON           },
    {"fgc_swb_off",             NCS4TOUCH_FGC_SWBOFF          },
    {"bgc_swb_off",             NCS4TOUCH_BGC_SWBOFF          },
    {"bgc_ntb_hitem",           NCS4TOUCH_BGC_NTB_HITEM       },
    {"bgc_ntb_ditem",           NCS4TOUCH_BGC_NTB_DITEM       },
    {"bgc_ntb_block",           NCS4TOUCH_BGC_NTB_BLOCK       },
    {"bgc_bnb_button",          NCS4TOUCH_BGC_BNB_BUTTON      },
    {"fgc_bnb_button",          NCS4TOUCH_FGC_BNB_BUTTON      },
    {"fgc_bnb_text",            NCS4TOUCH_FGC_BNB_TEXT        },
    {"fgc_inb_text",            NCS4TOUCH_FGC_INB_TEXT        },
    {"bgc_pck_main",            NCS4TOUCH_BGC_PCK_MAIN        },
    {"bgc_pck_picker",          NCS4TOUCH_BGC_PCK_PICKER      },
    {"fgc_pck_picker",          NCS4TOUCH_FGC_PCK_PICKER      },
    {"bgc_pck_select",          NCS4TOUCH_BGC_PCK_SELECT      },
    {"fgc_pck_disable",         NCS4TOUCH_FGC_PCK_DISABLE     },
    {"bgc_itb",                 NCS4TOUCH_BGC_ITB             },
    {"bgc_itb_light",           NCS4TOUCH_BGC_ITB_LIGHT       },
    {"fgc_itb_tick",            NCS4TOUCH_FGC_ITB_TICK        },
    {"fgc_itb_tick_light",      NCS4TOUCH_FGC_ITB_TICK_LIGHT  },
    {"fgc_itb_angle",           NCS4TOUCH_FGC_ITB_ANGLE       },
    {"fgc_itb_angle_light",     NCS4TOUCH_FGC_ITB_ANGLE_LIGHT },
    {"fgc_itb_ltext",           NCS4TOUCH_FGC_ITB_LTEXT       },
    {"fgc_itb_ltext_light",     NCS4TOUCH_FGC_ITB_LTEXT_LIGHT },
    {"fgc_itb_rtext",           NCS4TOUCH_FGC_ITB_RTEXT       },
    {"fgc_itb_rtext_light",     NCS4TOUCH_FGC_ITB_RTEXT_LIGHT },
    {NULL, 0}
};

static char ncsTouchRdrEtcFile[MAX_PATH + 1];

extern mWidgetRenderer _ncs_touch_widget_renderer;

extern BOOL _ncs_touch_init_piece_renderer(void);

#define RDR_ENTRY(CLASSNAME, classname) \
    { NCSCTRL_##CLASSNAME, (mWidgetRenderer*)(void*)(&(_ncs_touch_##classname##_renderer))}


static BOOL _lookup_etcfile(const char* filename)
{
    char etcfile[MAX_PATH + 1] = { 0 };
    FILE* fp = NULL;
    char* env_path = NULL;
    struct passwd *pwd = NULL;

    //try from NCS_CFG_PATH
    if ((env_path = getenv("NCS4TOUCH_CFG_PATH"))) {
        sprintf(etcfile, "%s/%s", env_path,filename);
        fp = fopen(etcfile, "r");
        if (NULL != fp) {
            fclose(fp);
            strcpy(ncsTouchRdrEtcFile, etcfile);
            return TRUE;
        }
    }

    if (!getcwd(etcfile, MAX_PATH))
        return FALSE;
    strcat(etcfile, "/");
    strcat(etcfile, filename);

    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsTouchRdrEtcFile, etcfile);
        return TRUE;
    }
#ifndef __NOUNIX__
    if ((pwd = getpwuid(geteuid())) != NULL) {
        strcpy(etcfile, pwd->pw_dir);

        if (etcfile[strlen(etcfile) - 1] != '/')
            strcat(etcfile, "/");
        strcat(etcfile, ".");
        strcat(etcfile, filename);

        fp = fopen(etcfile, "r");
        if (NULL != fp) {
            fclose(fp);
            strcpy(ncsTouchRdrEtcFile, etcfile);
            return TRUE;
        }
    }

    sprintf(etcfile, "/usr/local/etc/%s", filename);

    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsTouchRdrEtcFile, etcfile);
        return TRUE;
    }

    sprintf(etcfile, "/etc/%s", filename);
    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsTouchRdrEtcFile, etcfile);
        return TRUE;
    }

    return FALSE;

#elif defined(WIN32)
    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsTouchRdrEtcFile, etcfile);
        return TRUE;
    }

    sprintf(etcfile, "c:\\%s", filename);
     fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsTouchRdrEtcFile, etcfile);
        return TRUE;
    }
    return FALSE;
#endif

    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsTouchRdrEtcFile, etcfile);
        return TRUE;
    }

    return FALSE;
}


BOOL ncsTouchInitRenderers(void)
{
    int i = 0;
    const char *rdrname[] = {NCS4TOUCH_RENDERER, NULL};

    NCS_RDR_ENTRY entries[] = {
        RDR_ENTRY(WIDGET, widget),
        //TODO other render
    };

    if (!_lookup_etcfile(MGNCS4TOUCH_ETCFILE)) {
        LOGE("mGNCS4Touch: File [" MGNCS4TOUCH_ETCFILE "] not found !\n");
        return FALSE;
    }

    if (!ncsLoadRdrElementsFromEtcFile(ncsTouchRdrEtcFile,
                rdrname, 1, config_str)) {
        LOGE("mGNCS4Touch: Load renderer Error!\n");
        return FALSE;
    }

    for (i = 0; i < sizeof(entries) / sizeof(NCS_RDR_ENTRY); i++) {
        entries[i].renderer->class_init(entries[i].renderer);
        if (entries[i].renderer->init_self)
            entries[i].renderer->init_self(entries[i].renderer);
    }

    _ncs_touch_init_piece_renderer();

    return ncsRegisterCtrlRDRs(NCS4TOUCH_RENDERER,
            entries,
            sizeof(entries) / sizeof(NCS_RDR_ENTRY)
            );
}

