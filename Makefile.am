AUTOMAKE_OPTIONS=foreign

if NCS4TOUCH_HAVE_LIBSUFFIX
SAMPLES=
else
SAMPLES=samples
endif

SUBDIRS=include src etc $(SAMPLES)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=mgncs4touch.pc

EXTRA_DIST=README.md RELEASE-NOTES.md LICENSE
DIST_SUBDIRS=plugin
