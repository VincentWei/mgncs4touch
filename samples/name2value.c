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
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>
#include <../include/mpad.h>

#define NDEBUG	1
#include <../include/mpaddebug.h>

#define _(name)  {#name, (const char*)(name)}


static const char * name_values[][2] = {
    {"mGNCS4Pad  ==== renderer ====", 0},
	_(NCS4PAD_RRECT_RADIUS),
	_(NCS4PAD_BGC_BLOCK ),
	_(NCS4PAD_FGC_SWBON ),
	_(NCS4PAD_BGC_SWBON ),
	_(NCS4PAD_FGC_SWBOFF),
	_(NCS4PAD_BGC_SWBOFF),
	_(NCS4PAD_BGC_NTB_HITEM),
	_(NCS4PAD_BGC_NTB_DITEM),
	_(NCS4PAD_BGC_NTB_BLOCK),
	_(NCS4PAD_BGC_BNB_BUTTON),
	_(NCS4PAD_FGC_BNB_BUTTON),
	_(NCS4PAD_FGC_BNB_TEXT),
	_(NCS4PAD_FGC_INB_TEXT),
	_(NCS4PAD_BGC_PCK_MAIN  ),
	_(NCS4PAD_BGC_PCK_PICKER),
	_(NCS4PAD_FGC_PCK_PICKER),
	_(NCS4PAD_BGC_PCK_SELECT),
	_(NCS4PAD_FGC_PCK_DISABLE),
	_(NCS4PAD_BGC_ITB            ),
	_(NCS4PAD_BGC_ITB_LIGHT      ),
	_(NCS4PAD_FGC_ITB_TICK       ),
	_(NCS4PAD_FGC_ITB_TICK_LIGHT ),
	_(NCS4PAD_FGC_ITB_ANGLE      ),
	_(NCS4PAD_FGC_ITB_ANGLE_LIGHT),
	_(NCS4PAD_FGC_ITB_LTEXT      ),
	_(NCS4PAD_FGC_ITB_LTEXT_LIGHT),
	_(NCS4PAD_FGC_ITB_RTEXT      ),
	_(NCS4PAD_FGC_ITB_RTEXT_LIGHT),
    // switch button
    {"mGNCS4Pad  ==== switch button ====", 0},
	_(NCSP_SWB_STATUS),
	_(NCSP_SWB_MAX),
	_(NCSN_SWB_STATUSCHANGED),
	_(NCSN_SWB_MAX),
    // new track bar
    {"mGNCS4Pad  ==== new trackbar ====", 0},
    _(NCSS_NTRKBAR_CIRCLE),
    _(NCSP_NTRKBAR_MAX),
    // tool caption
    {"mGNCS4Pad  ==== btnnavbar(button navigation bar) ====", 0},
    _(NCSS_BNB_LRECT    ),
    _(NCSS_BNB_LOPT     ),
    _(NCSS_BNB_LNONE    ),
    _(NCSS_BNB_RRECT    ),
    _(NCSS_BNB_ROPT     ),
    _(NCSS_BNB_RNONE    ),
    _(NCSP_BNB_LSTRING  ),
    _(NCSP_BNB_RSTRING  ),
    _(NCSP_BNB_FONT     ),
    _(NCSP_BNB_MARGIN   ),
    _(NCSP_BNB_LEFT_W   ),
    _(NCSP_BNB_RIGHT_W  ),
    _(NCSP_BNB_BTN_H ),
    _(NCSP_BNB_MAX      ),
    _(NCSN_BNB_BTNCLICK ),
    {"mGNCS4Pad  ==== imgnavbar(image navigation bar) ====", 0},
    _(NCSP_INB_LBITMAP  ),
    _(NCSP_INB_RBITMAP  ),
    _(NCSP_INB_FONT     ),
    _(NCSP_INB_MARGIN   ),
    _(NCSP_INB_LEFT_W   ),
    _(NCSP_INB_RIGHT_W  ),
    _(NCSP_INB_IMG_H ),
    _(NCSP_INB_DRAWMODE ),
    _(NCSP_INB_MAX      ),
    _(NCSN_INB_IMGCLICK ),
    // picker
    {"mGNCS4Pad  ==== picker ====", 0 },
    _(NCSS_PCK_NUMBER   ),
    _(NCSS_PCK_STRINGS	),
    _(NCSS_PCK_LOOP		),
    _(NCSS_PCK_SHIFT	),
    _(NCSP_PCK_ALIGN    ),
    _(NCSP_PCK_LINES    ),
    _(NCSP_PCK_COUNT    ),
    _(NCSP_PCK_SELIDX   ),
    _(NCSP_PCK_HMARGIN  ),
    _(NCSP_PCK_VMARGIN  ),
    _(NCSP_PCK_NUMBERBITS),
    _(NCSP_PCK_MAX      ),
    _(NCSN_PCK_SELCHANGED),
    _(NCSN_PCK_REACHMIN),
    _(NCSN_PCK_REACHMAX),
    _(NCSN_PCK_MAX      ),
    // combo picker
    {"mGNCS4Pad  ==== combo picker ====", 0},
    _(NCSP_CMBPCK_HMARGIN),
    _(NCSP_CMBPCK_VMARGIN),
    _(NCSP_CMBPCK_VISLINE),
    _(NCSP_CMBPCK_MAX    ),
    // time picker
    {"mGNCS4Pad  ==== time picker ====", 0},
    _(NCSS_TPCK_24H         ),
    _(NCSS_TPCK_12H         ),
    _(NCSP_TPCK_HOUR        ),
    _(NCSP_TPCK_MINUTE      ),
    _(NCSP_TPCK_AMPMSTRINGS ),
    _(NCSN_TPCK_TIMECHANGED ),
    //date picker
    {"mGNCS4Pad  ==== date picker ====", 0},
    _(NCSS_DPCK_YYYYMMDD    ),
    _(NCSS_DPCK_MMDDYYYY    ),
    _(NCSS_DPCK_YYMMDD      ),
    _(NCSS_DPCK_MMDDYY      ),
    _(NCSP_DPCK_YEAR        ),
    _(NCSP_DPCK_MONTH       ),
    _(NCSP_DPCK_DAY         ),
    _(NCSN_DPCK_DATECHANGED ),
    //itembar
    {"mGNCS4Pad  ==== itembar ====", 0},
    _(NCSS_ITEMBAR_CHECKABLE),
    _(NCSS_ITEMBAR_HASCHILD	),
    _(NCSP_ITEMBAR_CHECKED  ),
    _(NCSP_ITEMBAR_IMG      ),
    _(NCSP_ITEMBAR_MARGIN   ),
    _(NCSN_ITEMBAR_CHECKED  ),
};

int main(int argc, const char* argv[])
{
	int i = 0;

	if (argc > 1) {
		int j = 0;

		if (strcasecmp("-find", argv[1])==0) {
			for (j=2; j<argc;j++) {
				int v = strtol(argv[j], NULL, 0);
				for (i=0;i<sizeof(name_values)/sizeof(char*[2]);i++) {
					if ((int)name_values[i][1] == v) {
						LOGE("%s:%s\n", argv[j], name_values[i][0]);
                    }
				}
			}
			return 0;
		} else if (strcasecmp("-help", argv[1]) == 0 || strcasecmp("-h", argv[1]) == 0
                || strcasecmp("-?", argv[1]) == 0) {
			LOGE("usage %s [options] [name1 [name2 ...]]|[value1 [value2 ...]]\n", argv[0]);
			LOGE("\t%s\t\t\t-- Show All Marco's of NCS\n", argv[0]);
			LOGE("\t%s name1 [name2, [name3 ..]]\t-- Show the specail names\n", argv[0]);
			LOGE("\t%s -find value1 [value2 [value3...]]\t -- Find the name by value\n", argv[0]);
			return 0;
		}

		for (j = 1; j < argc; j++) {
			for (i = 0; i < sizeof(name_values) / sizeof(char*[2]); i++) {
				if (strcasecmp(name_values[i][0], argv[j]) == 0) {
					if (strncmp(name_values[i][0], "NCSS", 4) == 0) {
						LOGE("%s=0x%X\n", name_values[i][0], (int) name_values[i][1]);
                    } else {
						LOGE("%s=%d\n", name_values[i][0], (int) name_values[i][1]);
                    }
					break;
				}
			}
		}
		return 0;
	}

	for (i = 0; i < sizeof(name_values) / sizeof(char*[2]); i++) {
			if (strncmp(name_values[i][0], "NCSS", 4) == 0) {
				LOGE("%s=0x%X\n", name_values[i][0], (int) name_values[i][1]);
			} else {
				LOGE("%s=%d\n", name_values[i][0], (int) name_values[i][1]);
			}
	}

	return 0;
}

