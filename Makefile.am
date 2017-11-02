######################################################################
# Copyright (C) 2002 ~ 2010, Beijing FMSoft Technology Co., Ltd.     #
# Room 902, Floor 9, Taixing, No.11, Huayuan East Road, Haidian      #
# District, Beijing, P. R. CHINA 100191.                             #
# All rights reserved.                                               #
#                                                                    #
# This software is the confidential and proprietary information of   #
# Beijing FMSoft Technology Co., Ltd. ("Confidential Information").  #
# You shall not disclose such Confidential Information and shall     #
# use it only in accordance you entered into with FMSoft.            #
#                                                                    #
#          http://www.minigui.com                                    #
#                                                                    #
######################################################################

AUTOMAKE_OPTIONS = foreign

if NCS4PAD_HAVE_LIBSUFFIX
SUBDIRS = include src etc plugin
else
SUBDIRS = include src etc
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=mgncs4pad.pc
