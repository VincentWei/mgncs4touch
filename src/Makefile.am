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

SUBDIRS = pieces 

TOP_DIR = ..

INCLUDES = -I$(TOP_DIR)/include -I. -I$(TOP_DIR)

lib_LTLIBRARIES = libmgncs4pad@NCS4PAD_LIBSUFFIX@.la

COMMON_SRCS = mpad.c mpcomm.c lf_pad.c mpadrdr.c pad_pieces.c         \
			pad_widget_rdr.c manimation.c mswitchbutton.c mpicker.c   \
			mnewtrackbar.c mcombopicker.c mtimepicker.c mdatepicker.c \
			mexlist.c mbtnnavbar.c mimgnavbar.c mitembar.c            \
			balloon_tip_maker.c

libmgncs4pad@NCS4PAD_LIBSUFFIX@_la_LIBADD  = pieces/libpieces.la 
libmgncs4pad@NCS4PAD_LIBSUFFIX@_la_SOURCES = $(COMMON_SRCS) 
libmgncs4pad@NCS4PAD_LIBSUFFIX@_la_LDFLAGS =

EXTRA_DIST =
CLEANFILES = *.a

