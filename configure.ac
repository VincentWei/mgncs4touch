#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT([libmgncs4touch], [1.0.1])
AC_CONFIG_SRCDIR([src/mpad.c])

dnl Set various version strings
#
# Making releases:
# Change the version, then:
#   _MICRO_VERSION += 1;
#   _INTERFACE_AGE += 1;
#   _BINARY_AGE += 1;
# if any functions have been added, set _INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set _BINARY_AGE and _INTERFACE_AGE to 0.
#
MGNCS4PAD_MAJOR_VERSION=1
MGNCS4PAD_MINOR_VERSION=0
MGNCS4PAD_MICRO_VERSION=1
MGNCS4PAD_INTERFACE_AGE=0
MGNCS4PAD_BINARY_AGE=0
MGNCS4PAD_VERSION=$MGNCS4PAD_MAJOR_VERSION.$MGNCS4PAD_MINOR_VERSION.$MGNCS4PAD_MICRO_VERSION

AC_SUBST(MGNCS4PAD_MAJOR_VERSION)
AC_SUBST(MGNCS4PAD_MINOR_VERSION)
AC_SUBST(MGNCS4PAD_MICRO_VERSION)
AC_SUBST(MGNCS4PAD_INTERFACE_AGE)
AC_SUBST(MGNCS4PAD_BINARY_AGE)
AC_SUBST(MGNCS4PAD_VERSION)

AC_DEFINE_UNQUOTED(MGNCS4PAD_MAJOR_VERSION, 
                   $MGNCS4PAD_MAJOR_VERSION, [Major version of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4PAD_MINOR_VERSION, 
                   $MGNCS4PAD_MINOR_VERSION, [Minor version of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4PAD_MICRO_VERSION, 
                   $MGNCS4PAD_MICRO_VERSION, [Micro version of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4PAD_INTERFACE_AGE, 
                   $MGNCS4PAD_INTERFACE_AGE, [Interface age of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4PAD_BINARY_AGE, 
                   $MGNCS4PAD_BINARY_AGE, [Binary age of mgncs4touch])

# libtool versioning
LT_RELEASE=$MGNCS4PAD_MAJOR_VERSION.$MGNCS4PAD_MINOR_VERSION
LT_CURRENT=`expr $MGNCS4PAD_MICRO_VERSION - $MGNCS4PAD_INTERFACE_AGE`
LT_REVISION=$MGNCS4PAD_INTERFACE_AGE
LT_AGE=`expr $MGNCS4PAD_BINARY_AGE - $MGNCS4PAD_INTERFACE_AGE`
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_CANONICAL_SYSTEM

dnl ========================================================================
# Checks for programs.
AC_PROG_CC
AC_PROG_LD
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AM_INIT_AUTOMAKE

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h sys/time.h termio.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([bzero dup2 getcwd memmove memset putenv strchr strdup strerror strstr])

AC_CONFIG_HEADER(mgncs4touchconfig.h)

AC_ARG_WITH(libsuffix,
  [  --with-libsuffix=ARG    configure the suffix of the library name.])

case "$with_libsuffix" in
    "")
        NCS4PAD_LIBSUFFIX=""
        have_libsuffix="no"
    ;;
    *)
        NCS4PAD_LIBSUFFIX="_$withval"
        have_libsuffix="yes"
    ;;
esac

AC_ARG_ENABLE(guibuilder-support,
[ --enable-guibuilder-support   new control set used to support guibuilder <default=no>],
guibuilder_support=$enableval)

if test "x$guibuilder_support" = "xyes"; then
    AC_DEFINE(_MGNCS4PAD_GUIBUILDER_SUPPORT, 1,
        [Define if support guibuilder])
fi

AM_CONDITIONAL(NCS4PAD_HAVE_LIBSUFFIX, test "x$have_libsuffix" = "xyes")
AC_SUBST(NCS4PAD_LIBSUFFIX)
AC_DEFINE_UNQUOTED(_NCS4PAD_LIBSUFFIX, "${NCS4PAD_LIBSUFFIX}", [MGNCS library suffix])

cfgfile=mgncs4touch.cfg

AC_DEFINE_UNQUOTED(MGNCS4PAD_ETCFILE, "${cfgfile}", [MGNCS4PAD configure file name])

dnl ========================================================================
dnl check for dependencies
dnl ========================================================================

PKG_CHECK_MODULES([MGNCS], [mgncs >= 1.0.6])
APP_LIBS="$APP_LIBS $MGNCS_LIBS"

PKG_CHECK_MODULES([MGEFF], [mgeff >= 1.0.0])
APP_LIBS="$APP_LIBS $MGEFF_LIBS"

PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.2.0])
APP_LIBS="$APP_LIBS $MGPLUS_LIBS"

PKG_CHECK_MODULES([MINIGUI], [minigui >= 3.0.10])
APP_LIBS="$APP_LIBS $MINIGUI_LIBS"

PC_LIBS="-lmgncs4touch$NCS4PAD_LIBSUFFIX"
APP_LIBS="$PC_LIBS $APP_LIBS"

PC_CFLAGS=""
AC_SUBST(APP_LIBS)
AC_SUBST(PC_LIBS)
AC_SUBST(PC_CFLAGS)

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wall -Wstrict-prototypes -pipe"
fi

AC_CONFIG_FILES([
    Makefile
    mgncs4touch.pc
    include/Makefile
    include/pieces/Makefile
    src/Makefile
    src/pieces/Makefile
    etc/Makefile
    samples/Makefile
    samples/res/Makefile
    plugin/Makefile
    plugin/guibuilder/Makefile
    plugin/guibuilder/icon/Makefile
    plugin/guibuilder/renderer/Makefile
    plugin/guibuilder/renderer/fashion/Makefile
    plugin/guibuilder/uieditor/Makefile
    plugin/guibuilder/uieditor/defines/Makefile
])

AC_OUTPUT


