#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT([libmgncs4touch], [1.5.0])
AC_CONFIG_SRCDIR([src/mgncs4touch.c])

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
MGNCS4TOUCH_MAJOR_VERSION=1
MGNCS4TOUCH_MINOR_VERSION=5
MGNCS4TOUCH_MICRO_VERSION=0
MGNCS4TOUCH_INTERFACE_AGE=0
MGNCS4TOUCH_BINARY_AGE=0
MGNCS4TOUCH_VERSION=$MGNCS4TOUCH_MAJOR_VERSION.$MGNCS4TOUCH_MINOR_VERSION.$MGNCS4TOUCH_MICRO_VERSION

AC_SUBST(MGNCS4TOUCH_MAJOR_VERSION)
AC_SUBST(MGNCS4TOUCH_MINOR_VERSION)
AC_SUBST(MGNCS4TOUCH_MICRO_VERSION)
AC_SUBST(MGNCS4TOUCH_INTERFACE_AGE)
AC_SUBST(MGNCS4TOUCH_BINARY_AGE)
AC_SUBST(MGNCS4TOUCH_VERSION)

AC_DEFINE_UNQUOTED(MGNCS4TOUCH_MAJOR_VERSION,
                   $MGNCS4TOUCH_MAJOR_VERSION, [Major version of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4TOUCH_MINOR_VERSION,
                   $MGNCS4TOUCH_MINOR_VERSION, [Minor version of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4TOUCH_MICRO_VERSION,
                   $MGNCS4TOUCH_MICRO_VERSION, [Micro version of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4TOUCH_INTERFACE_AGE,
                   $MGNCS4TOUCH_INTERFACE_AGE, [Interface age of mgncs4touch])
AC_DEFINE_UNQUOTED(MGNCS4TOUCH_BINARY_AGE,
                   $MGNCS4TOUCH_BINARY_AGE, [Binary age of mgncs4touch])

# libtool versioning
LT_RELEASE=$MGNCS4TOUCH_MAJOR_VERSION.$MGNCS4TOUCH_MINOR_VERSION
LT_CURRENT=`expr $MGNCS4TOUCH_MICRO_VERSION - $MGNCS4TOUCH_INTERFACE_AGE`
LT_REVISION=$MGNCS4TOUCH_INTERFACE_AGE
LT_AGE=`expr $MGNCS4TOUCH_BINARY_AGE - $MGNCS4TOUCH_INTERFACE_AGE`
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
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([bzero dup2 getcwd memmove memset putenv strchr strdup strerror strstr])

AC_CONFIG_HEADER(mgncs4touchconfig.h)

devel_mode="no"
AC_ARG_ENABLE(develmode,
[  --enable-develmode       developer mode <default=no>],
devel_mode=$enableval)

detail_debug="no"
AC_ARG_ENABLE(detail_debug,
[  --enable-detaildebug     detailed debug information <default=no>],
detail_debug=$enableval)

AC_ARG_WITH(libsuffix,
  [  --with-libsuffix=ARG   configure the suffix of the library name.])

case "$with_libsuffix" in
    "")
        NCS4TOUCH_LIBSUFFIX=""
        have_libsuffix="no"
    ;;
    *)
        NCS4TOUCH_LIBSUFFIX="_$withval"
        have_libsuffix="yes"
    ;;
esac

guibuilder_support="yes"
AC_ARG_ENABLE(guibuildersupport,
[ --enable-guibuildersupport   definitions for plugin of guibuilder <default=no>],
guibuilder_support=$enableval)

if test "x$guibuilder_support" = "xyes"; then
    AC_DEFINE(_MGNCS4TOUCH_GUIBUILDER_SUPPORT, 1, [Define if support guibuilder])
fi

AM_CONDITIONAL(NCS4TOUCH_HAVE_LIBSUFFIX, test "x$have_libsuffix" = "xyes")
AC_SUBST(NCS4TOUCH_LIBSUFFIX)

dnl ========================================================================
dnl check for dependencies
dnl ========================================================================

PKG_CHECK_MODULES([MGNCS], [mgncs >= 1.2.0])
DEP_LIBS="$DEP_LIBS $MGNCS_LIBS"

PKG_CHECK_MODULES([MGEFF], [mgeff >= 1.2.0])
DEP_LIBS="$DEP_LIBS $MGEFF_LIBS"

PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.4.0])
DEP_LIBS="$DEP_LIBS $MGPLUS_LIBS"

PKG_CHECK_MODULES([MINIGUI], [minigui >= 3.2.0])
DEP_LIBS="$DEP_LIBS $MINIGUI_LIBS"

PKG_CHECK_MODULES([CHIPMUNK], [chipmunk >= 5.3.1])
DEP_LIBS="$DEP_LIBS $CHIPMUNK_LIBS"

APP_LIBS="$DEP_LIBS"
PC_LIBS="-lmgncs4touch $DEP_LIBS"

PC_CFLAGS=""
AC_SUBST(DEP_LIBS)
AC_SUBST(APP_LIBS)
AC_SUBST(PC_LIBS)
AC_SUBST(PC_CFLAGS)

CFLAGS="$CFLAGS -D__MGNCS4TOUCH_LIB__"
if test "x$devel_mode" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -D_DEBUG"
    if test "x$detail_debug" = "xyes"; then
        CPPFLAGS="$CPPFLAGS -DDEBUG"
    fi

    if test "$ac_cv_prog_gcc" = "yes"; then
        CPPFLAGS="$CPPFLAGS -Wall -Werror"
    fi
else
    CPPFLAGS="$CPPFLAGS -DNDEBUG"
fi

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wstrict-prototypes -pipe"
fi

AC_OUTPUT([
    Makefile
    mgncs4touch.pc
    m4/Makefile
    include/Makefile
    include/pieces/Makefile
    include/physics-animation/Makefile
    src/Makefile
    src/pieces/Makefile
    src/physics-animation/Makefile
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

echo
echo "Creating include directory for samples"
cd samples/
if test ! -h mgncs4touch; then
    ln -s ../include mgncs4touch
fi
cd ..
echo "Done."

