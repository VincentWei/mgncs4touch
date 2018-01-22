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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/fixedmath.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>

#include "mgncs4padconfig.h"
#include "mpadrdr.h"

// #define NDEBUG	1
#include "mpaddebug.h"


#ifndef __NOUNIX__
#    include <unistd.h>
#    include <pwd.h>
#endif

#ifdef WIN32
#include <sys/stat.h>
#endif


static NCS_RDR_ID_NAME config_str[] = {
    {"roundrect_radius",        NCS4PAD_RRECT_RADIUS        },
    {"bgc_block",               NCS4PAD_BGC_BLOCK           },
    {"fgc_swb_on",              NCS4PAD_FGC_SWBON           },
    {"bgc_swb_on",              NCS4PAD_BGC_SWBON           },
    {"fgc_swb_off",             NCS4PAD_FGC_SWBOFF          },
    {"bgc_swb_off",             NCS4PAD_BGC_SWBOFF          },
    {"bgc_ntb_hitem",           NCS4PAD_BGC_NTB_HITEM       },
    {"bgc_ntb_ditem",           NCS4PAD_BGC_NTB_DITEM       },
    {"bgc_ntb_block",           NCS4PAD_BGC_NTB_BLOCK       },
    {"bgc_bnb_button",          NCS4PAD_BGC_BNB_BUTTON      },
    {"fgc_bnb_button",          NCS4PAD_FGC_BNB_BUTTON      },
    {"fgc_bnb_text",            NCS4PAD_FGC_BNB_TEXT        },
    {"fgc_inb_text",            NCS4PAD_FGC_INB_TEXT        },
    {"bgc_pck_main",            NCS4PAD_BGC_PCK_MAIN        },
    {"bgc_pck_picker",          NCS4PAD_BGC_PCK_PICKER      },
    {"fgc_pck_picker",          NCS4PAD_FGC_PCK_PICKER      },
    {"bgc_pck_select",          NCS4PAD_BGC_PCK_SELECT      },
    {"fgc_pck_disable",         NCS4PAD_FGC_PCK_DISABLE     },
    {"bgc_itb",                 NCS4PAD_BGC_ITB             },
    {"bgc_itb_light",           NCS4PAD_BGC_ITB_LIGHT       },
    {"fgc_itb_tick",            NCS4PAD_FGC_ITB_TICK        },
    {"fgc_itb_tick_light",      NCS4PAD_FGC_ITB_TICK_LIGHT  },
    {"fgc_itb_angle",           NCS4PAD_FGC_ITB_ANGLE       },
    {"fgc_itb_angle_light",     NCS4PAD_FGC_ITB_ANGLE_LIGHT },
    {"fgc_itb_ltext",           NCS4PAD_FGC_ITB_LTEXT       },
    {"fgc_itb_ltext_light",     NCS4PAD_FGC_ITB_LTEXT_LIGHT },
    {"fgc_itb_rtext",           NCS4PAD_FGC_ITB_RTEXT       },
    {"fgc_itb_rtext_light",     NCS4PAD_FGC_ITB_RTEXT_LIGHT },
    {NULL, 0}
};

static char ncsPadRdrEtcFile[MAX_PATH + 1];

extern mWidgetRenderer pad_widget_renderer;

extern BOOL pad_init_piece_renderer(void);

#define RDR_ENTRY(CLASSNAME, classname) \
    { NCSCTRL_##CLASSNAME, (mWidgetRenderer*)(void*)(&(pad_##classname##_renderer))}


static BOOL _lookup_etcfile(const char* filename)
{
    char etcfile[MAX_PATH + 1] = { 0 };
    FILE* fp = NULL;
    char* env_path = NULL;
    struct passwd *pwd = NULL;

    //try from NCS_CFG_PATH
    if ((env_path = getenv("NCS4PAD_CFG_PATH"))) {
        sprintf(etcfile, "%s/%s", env_path,filename);
        fp = fopen(etcfile, "r");
        if (NULL != fp) {
            fclose(fp);
            strcpy(ncsPadRdrEtcFile, etcfile);
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
        strcpy(ncsPadRdrEtcFile, etcfile);
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
            strcpy(ncsPadRdrEtcFile, etcfile);
            return TRUE;
        }
    }

    sprintf(etcfile, "/usr/local/etc/%s", filename);

    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsPadRdrEtcFile, etcfile);
        return TRUE;
    }

    sprintf(etcfile, "/etc/%s", filename);
    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsPadRdrEtcFile, etcfile);
        return TRUE;
    }

    return FALSE;

#elif defined(WIN32)
    fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsPadRdrEtcFile, etcfile);
        return TRUE;
    }

    sprintf(etcfile, "c:\\%s", filename);
     fp = fopen(etcfile, "r");
    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsPadRdrEtcFile, etcfile);
        return TRUE;
    }
    return FALSE;
#endif

    if (NULL != fp) {
        fclose(fp);
        strcpy(ncsPadRdrEtcFile, etcfile);
        return TRUE;
    }

    return FALSE;
}


BOOL ncsInitPadRenderers(void)
{
    int i = 0;
    char *padname[] = {NCS4PAD_RENDERER, };

    NCS_RDR_ENTRY entries[] = {
        RDR_ENTRY(WIDGET, widget),
        //TODO other render
    };

    if (!_lookup_etcfile(MGNCS4PAD_ETCFILE)) {
        LOGE("File [mgncs4pad.cfg] not found !\n");
        return FALSE;
    }

    if (!ncsLoadRdrElementsFromEtcFile(ncsPadRdrEtcFile,
                &padname, 1, config_str)) {
        LOGE("Load Pad renderer Error!\n");
        return FALSE;
    }

    for (i = 0; i < sizeof(entries) / sizeof(NCS_RDR_ENTRY); i++) {
        entries[i].renderer->class_init(entries[i].renderer);
        if (entries[i].renderer->init_self)
            entries[i].renderer->init_self(entries[i].renderer);
    }

    pad_init_piece_renderer();

    return ncsRegisterCtrlRDRs(NCS4PAD_RENDERER,
            entries,
            sizeof(entries) / sizeof(NCS_RDR_ENTRY)
            );
}

