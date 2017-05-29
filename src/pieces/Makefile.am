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

TOP_DIR = ../..

INCLUDES = -I$(TOP_DIR)/include -I$(TOP_DIR)/include/ -I. -I$(TOP_DIR) -I..

noinst_LTLIBRARIES = libpieces.la

COMMON_SRCS = mshapeboxpiece.c  \
		mshapepushbuttonpiece.c \
		msimagepiece.c          \
		mnsdrawpiece.c			\
		mcolorablelabelpiece.c

libpieces_la_SOURCES = $(COMMON_SRCS)
libpieces_la_LIBADD =
libpieces_la_LDFLAGS =

EXTRA_DIST =

CLEANFILES = *.a

