/*
 * \file name2value.c
 * \author FMSoft
 * \date 2010/10/09
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/en/about/licensing-policy/>.

 \endverbatim
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include <mgncs4touch/mgncs4touch.h>
// #define NDEBUG	1
#include <mgncs4touch/mtouchdebug.h>

#define _(name)  {#name, (const char*)(name)}

static const char * name_values[][2] = {
    {"mGNCS4Touch  ==== renderer ====", 0},
	_(NCS4TOUCH_RRECT_RADIUS),
	_(NCS4TOUCH_BGC_BLOCK ),
	_(NCS4TOUCH_FGC_SWBON ),
	_(NCS4TOUCH_BGC_SWBON ),
	_(NCS4TOUCH_FGC_SWBOFF),
	_(NCS4TOUCH_BGC_SWBOFF),
	_(NCS4TOUCH_BGC_NTB_HITEM),
	_(NCS4TOUCH_BGC_NTB_DITEM),
	_(NCS4TOUCH_BGC_NTB_BLOCK),
	_(NCS4TOUCH_BGC_BNB_BUTTON),
	_(NCS4TOUCH_FGC_BNB_BUTTON),
	_(NCS4TOUCH_FGC_BNB_TEXT),
	_(NCS4TOUCH_FGC_INB_TEXT),
	_(NCS4TOUCH_BGC_PCK_MAIN  ),
	_(NCS4TOUCH_BGC_PCK_PICKER),
	_(NCS4TOUCH_FGC_PCK_PICKER),
	_(NCS4TOUCH_BGC_PCK_SELECT),
	_(NCS4TOUCH_FGC_PCK_DISABLE),
	_(NCS4TOUCH_BGC_ITB            ),
	_(NCS4TOUCH_BGC_ITB_LIGHT      ),
	_(NCS4TOUCH_FGC_ITB_TICK       ),
	_(NCS4TOUCH_FGC_ITB_TICK_LIGHT ),
	_(NCS4TOUCH_FGC_ITB_ANGLE      ),
	_(NCS4TOUCH_FGC_ITB_ANGLE_LIGHT),
	_(NCS4TOUCH_FGC_ITB_LTEXT      ),
	_(NCS4TOUCH_FGC_ITB_LTEXT_LIGHT),
	_(NCS4TOUCH_FGC_ITB_RTEXT      ),
	_(NCS4TOUCH_FGC_ITB_RTEXT_LIGHT),
    // switch button
    {"mGNCS4Touch  ==== switch button ====", 0},
	_(NCSP_SWB_STATUS),
	_(NCSP_SWB_MAX),
	_(NCSN_SWB_STATUSCHANGED),
	_(NCSN_SWB_MAX),
    // new track bar
    {"mGNCS4Touch  ==== new trackbar ====", 0},
    _(NCSS_NTRKBAR_CIRCLE),
    _(NCSP_NTRKBAR_MAX),
    // tool caption
    {"mGNCS4Touch  ==== btnnavbar(button navigation bar) ====", 0},
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
    {"mGNCS4Touch  ==== imgnavbar(image navigation bar) ====", 0},
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
    {"mGNCS4Touch  ==== picker ====", 0 },
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
    {"mGNCS4Touch  ==== combo picker ====", 0},
    _(NCSP_CMBPCK_HMARGIN),
    _(NCSP_CMBPCK_VMARGIN),
    _(NCSP_CMBPCK_VISLINE),
    _(NCSP_CMBPCK_MAX    ),
    // time picker
    {"mGNCS4Touch  ==== time picker ====", 0},
    _(NCSS_TPCK_24H         ),
    _(NCSS_TPCK_12H         ),
    _(NCSP_TPCK_HOUR        ),
    _(NCSP_TPCK_MINUTE      ),
    _(NCSP_TPCK_AMPMSTRINGS ),
    _(NCSN_TPCK_TIMECHANGED ),
    //date picker
    {"mGNCS4Touch  ==== date picker ====", 0},
    _(NCSS_DPCK_YYYYMMDD    ),
    _(NCSS_DPCK_MMDDYYYY    ),
    _(NCSS_DPCK_YYMMDD      ),
    _(NCSS_DPCK_MMDDYY      ),
    _(NCSP_DPCK_YEAR        ),
    _(NCSP_DPCK_MONTH       ),
    _(NCSP_DPCK_DAY         ),
    _(NCSN_DPCK_DATECHANGED ),
    //itembar
    {"mGNCS4Touch  ==== itembar ====", 0},
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
				intptr_t v = strtol(argv[j], NULL, 0);
				for (i=0;i<sizeof(name_values)/sizeof(char*[2]);i++) {
					if ((intptr_t)name_values[i][1] == v) {
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
						LOGE("%s=%p\n", name_values[i][0], name_values[i][1]);
                    } else {
						LOGE("%s=%p\n", name_values[i][0], name_values[i][1]);
                    }
					break;
				}
			}
		}
		return 0;
	}

	for (i = 0; i < sizeof(name_values) / sizeof(char*[2]); i++) {
			if (strncmp(name_values[i][0], "NCSS", 4) == 0) {
				LOGE("%s=%p\n", name_values[i][0], name_values[i][1]);
			} else {
				LOGE("%s=%p\n", name_values[i][0], name_values[i][1]);
			}
	}

	return 0;
}

