dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) S/E - 2013/2013 - Sylvestre Ledru
dnl Copyright (C) Scilab Enterprises - 2014 - Clement DAVID
dnl
dnl Copyright (C) 2012 - 2016 - Scilab Enterprises
dnl
dnl This file is hereby licensed under the terms of the GNU GPL v2.0,
dnl pursuant to article 5.3.4 of the CeCILL v.2.1.
dnl This file was originally licensed under the terms of the CeCILL v2.1,
dnl and continues to be available under such terms.
dnl For more information, see the COPYING file which you should have received
dnl along with this program.

dnl CHECK_COMPILER_ARG(LANG, ARG, ACTION-IF-NOT_FOUND)
AC_DEFUN([CHECK_COMPILER_ARG],[
    AC_LANG_PUSH([$1])
    AC_MSG_CHECKING([if the $1 compiler accepts $2])
    ac_save_$3="$$3"
    $3="$$3 $$2"

    AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
       [AC_MSG_RESULT([yes])
       $4="$2"
       ],
       [AC_MSG_RESULT([no])]
    )
    $3="$ac_save_$3";
    AC_LANG_POP([$1])

])


AC_DEFUN([AC_CHECK_STDCPP_STATIC],[

AC_ARG_ENABLE(static-system-lib,
    AC_HELP_STRING([--enable-static-system-lib],[Enable the usage of the static stdc++ and libgcc libs]),
)

# Check if -static-libstdc++ and -static-libgcc are supported.
saved_LDFLAGS="$LDFLAGS"

compiler_manage_static_libs=no
USE_STATIC_SYSTEM_LIB=no

if test "x$enable_static_system_lib" == "xyes"; then
# The user ask for an explicit enable.
   USE_STATIC_SYSTEM_LIB=yes
fi


LDFLAGS="$LDFLAGS -static-libstdc++ -static-libgcc"
AC_MSG_CHECKING([whether g++ accepts -static-libstdc++ -static-libgcc])
AC_LANG_PUSH(C++)
AC_LINK_IFELSE([AC_LANG_SOURCE([
#if (__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 5)
#error -static-libstdc++ not implemented
#endif
int main() {return 0;}])],
  [AC_MSG_RESULT([yes]); compiler_manage_static_libs=yes],
  [AC_MSG_RESULT([no])])
AC_LANG_POP(C++)

LDFLAGS="$saved_LDFLAGS"
if test "x$compiler_manage_static_libs" == "xno" -a "x$enable_static_system_lib" == "xyes"; then
   AC_MSG_ERROR([The $CC compiler does not support the options -static-libstdc++/-static-libgcc. Update your compiler or use the option --disable-static-system-lib])
fi

if test "x$compiler_manage_static_libs" == "xno"; then
   echo "-static-libstdc++ -static-libgcc unsupported by the compiler. Skipped"
   USE_STATIC_SYSTEM_LIB=no
fi

AM_CONDITIONAL(USE_STATIC_SYSTEM_LIB, test "$USE_STATIC_SYSTEM_LIB" == yes)

])
