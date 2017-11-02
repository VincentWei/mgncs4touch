#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(mgncs4pad-test,1.0.0,-)
AC_CONFIG_SRCDIR([picker.c])

AC_CANONICAL_SYSTEM

dnl ========================================================================
# Checks for programs.
AC_PROG_CC
AC_PROG_LD
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl ========================================================================
dnl Put the name and version of the package here
AM_INIT_AUTOMAKE

# Checks for libraries.

# Checks for header files.
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
AC_CHECK_FUNCS([bzero dup2 getcwd memmove memset putenv strchr strdup strerror strstr strndup ftruncate mmap])

dnl ========================================================================
dnl check for dependencies
dnl ========================================================================

PKG_CHECK_MODULES([MGNCS], [mgncs >= 1.0.6])
APP_LIBS="$APP_LIBS $MGNCS_LIBS"

PKG_CHECK_MODULES([MGPLUS], [mgplus >= 1.2.0])
APP_LIBS="$APP_LIBS $MGPLUS_LIBS"

PKG_CHECK_MODULES([MINIGUI], [minigui >= 3.0.10])
APP_LIBS="$APP_LIBS $MINIGUI_LIBS"

if test "$ac_cv_prog_gcc" = "yes"; then
    CFLAGS="$CFLAGS -Wall -Wstrict-prototypes -pipe"
fi

AC_SUBST(APP_LIBS)

AC_CONFIG_FILES([
    Makefile
    res/Makefile
])

AC_OUTPUT

