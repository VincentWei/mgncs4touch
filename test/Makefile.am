######################################################################
# Copyright (C) 2002 ~ 2010, Feynman Software Technology Co., Ltd.   #
# Room 902, Floor 9, Taixing, No.11, Huayuan East Road, Haidian      #
# District, Beijing, P. R. CHINA 100191.                             #
# All rights reserved.                                               #
#                                                                    #
# This software is the confidential and proprietary information of   #
# Feynman Software Technology Co., Ltd. ("Confidential Information").#
# You shall not disclose such Confidential Information and shall use #
# it only in accordance you entered into with Feynman Software.      #
#                                                                    #
#          http://www.minigui.com                                    #
#                                                                    #
######################################################################

AUTOMAKE_OPTIONS = foreign

TOP_DIR = ..

SUBDIRS = res

INCLUDES = -I$(TOP_DIR) -I$(TOP_DIR)/include -I.

COMMON_LADD = $(TOP_DIR)/src/.libs/libmgncs4pad.so -lmgutils -lmgplus -lmgeff -lmgncs @APP_LIBS@ 

noinst_PROGRAMS = switchbutton picker newtrackbar commdraw datepicker \
				  timepicker exlist btnnavbar imgnavbar name2value itembar \
				  balloontip

switchbutton_SOURCES = switchbutton.c
switchbutton_LDADD = $(COMMON_LADD) @APP_LIBS@

picker_SOURCES = picker.c
picker_LDADD = $(COMMON_LADD) @APP_LIBS@

newtrackbar_SOURCES = newtrackbar.c
newtrackbar_LDADD = $(COMMON_LADD) @APP_LIBS@

commdraw_SOURCES = comm_draw.c
commdraw_LDADD = $(COMMON_LADD) $(APPLIBS)

datepicker_SOURCES = datepicker.c
datepicker_LDADD = $(COMMON_LADD) @APP_LIBS@

timepicker_SOURCES = timepicker.c
timepicker_LDADD = $(COMMON_LADD) @APP_LIBS@

btnnavbar_SOURCES = btnnavbar.c
btnnavbar_LDADD = $(COMMON_LADD) @APP_LIBS@

imgnavbar_SOURCES = imgnavbar.c
imgnavbar_LDADD = $(COMMON_LADD) @APP_LIBS@

exlist_SOURCES = exlist.c
exlist_LDADD = $(COMMON_LADD) @APP_LIBS@

name2value_SOURCES = name2value.c
name2value_LDADD = $(COMMON_LADD) @APP_LIBS@

itembar_SOURCES = itembar.c
itembar_LDADD = $(COMMON_LADD) @APP_LIBS@

balloontip_SOURCES = balloontip.c
balloontip_LDADD = ${COMMON_LADD} ${APP_LIBS}

EXTRA_DIST =

