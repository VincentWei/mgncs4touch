TOP_DIR = ../..

AM_CPPFLAGS = -D__NCS4PAD_LIB__ -I$(TOP_DIR)/include -I$(TOP_DIR)/include/ -I. -I$(TOP_DIR) -I..

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

