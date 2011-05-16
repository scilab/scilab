AC_DEFUN([AC_BACKTRACE], [

#------------------------------
# backtrace support (for glibc)
#------------------------------

# Basic backtrace functionnality

have_glibc_backtrace=no

AC_MSG_CHECKING([for glibc backtrace])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#if defined(__GNUC__)
#define _GNU_SOURCE
#include <memory.h>
#include <execinfo.h>
#endif]],
               [[ void *tr_array[10]; backtrace(tr_array, 10); ]])],
               [have_glibc_backtrace=yes],
               [have_glibc_backtrace=no])
AC_MSG_RESULT($have_glibc_backtrace)

if test "x$have_glibc_backtrace" = "xyes"; then
  CFLAGS="$CFLAGS -rdynamic"
  CPPFLAGS="$CPPFLAGS -rdynamic"
  FFLAGS="$FFLAGS -rdynamic"
  AC_DEFINE(HAVE_GLIBC_BACKTRACE, 1, HAVE_GLIBC_BACKTRACE)
fi

# C++ demangling

have_cplus_demangle=no

AC_MSG_CHECKING([for glibc cplus_demangle])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <demangle.h>]],
               [[ const char *s; cplus_demangle(s, auto_demangling); ]])],
               [have_cplus_demangle=yes],
               [have_cplus_demangle=no])
AC_MSG_RESULT($have_cplus_demangle)

if test "x$have_cplus_demangle" = "xyes"; then
  CFLAGS="$CFLAGS -rdynamic"
  CPPFLAGS="$CPPFLAGS -rdynamic"
  FFLAGS="$FFLAGS -rdynamic"

  AC_DEFINE(HAVE_CPLUS_DEMANGLE, 1, HAVE_CPLUS_DEMANGLE)
fi
])
