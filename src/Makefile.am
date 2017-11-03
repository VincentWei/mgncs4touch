SUBDIRS = pieces 

TOP_DIR = ..

AM_CPPFLAGS = -D__NCS4PAD_LIB__ -I$(TOP_DIR)/include -I. -I$(TOP_DIR)

lib_LTLIBRARIES = libmgncs4pad@NCS4PAD_LIBSUFFIX@.la

COMMON_SRCS = mpad.c mpcomm.c lf_pad.c mpadrdr.c pad_pieces.c         \
			pad_widget_rdr.c manimation.c mswitchbutton.c mpicker.c   \
			mnewtrackbar.c mcombopicker.c mtimepicker.c mdatepicker.c \
			mexlist.c mbtnnavbar.c mimgnavbar.c mitembar.c            \
			balloon_tip_maker.c

libmgncs4pad@NCS4PAD_LIBSUFFIX@_la_LIBADD  = pieces/libpieces.la 
libmgncs4pad@NCS4PAD_LIBSUFFIX@_la_SOURCES = $(COMMON_SRCS) 
libmgncs4pad@NCS4PAD_LIBSUFFIX@_la_LDFLAGS = -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

EXTRA_DIST =
CLEANFILES = *.a

