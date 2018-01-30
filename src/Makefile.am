SUBDIRS = pieces physics-animation

AM_CPPFLAGS = -I../include

lib_LTLIBRARIES = libmgncs4touch@NCS4TOUCH_LIBSUFFIX@.la

COMMON_SRCS = mgncs4touch.c mtouchcomm.c lf_touch.c mtouchrdr.c touch_pieces.c \
    touch_widget_rdr.c manimation.c mswitchbutton.c mpicker.c  \
    mnewtrackbar.c mcombopicker.c mtimepicker.c mdatepicker.c \
    mexlist.c mbtnnavbar.c mimgnavbar.c mitembar.c balloon_tip_maker.c \
    mpieceitem.c mitemiterator.c \
    mcenterhbox.c mfillboxex.c mlayout_manager.c mlinevbox.c \
    miconflow.c mcontainerctrl.c

SUBLIBS = pieces/libpieces.la \
    physics-animation/libphysics-animation.la 

libmgncs4touch@NCS4TOUCH_LIBSUFFIX@_la_LIBADD  = $(SUBLIBS) @DEP_LIBS@ 
libmgncs4touch@NCS4TOUCH_LIBSUFFIX@_la_DEPENDENCIES  = $(SUBLIBS)
libmgncs4touch@NCS4TOUCH_LIBSUFFIX@_la_SOURCES = $(COMMON_SRCS) 
libmgncs4touch@NCS4TOUCH_LIBSUFFIX@_la_LDFLAGS = -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

EXTRA_DIST =
CLEANFILES = *.a

