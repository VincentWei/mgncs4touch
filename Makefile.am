AUTOMAKE_OPTIONS=foreign

if NCS4TOUCH_HAVE_LIBSUFFIX
SAMPLES=
else
SAMPLES=samples
endif

SUBDIRS=m4 include src etc $(SAMPLES)
DIST_SUBDIRS=m4 include src etc samples plugin 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=mgncs4touch.pc

EXTRA_DIST=README.md RELEASE-NOTES.md LICENSE
