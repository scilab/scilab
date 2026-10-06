dnl
dnl Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) 2023 - Dassault Systèmes S.E. - Clément DAVID
dnl
dnl
dnl libarchive  - https://www.libarchive.org detection
dnl it is mandatory in Scilab
dnl

AC_DEFUN([AC_LIBARCHIVE], [

AC_ARG_WITH(libarchive_include,
        AS_HELP_STRING([--with-libarchive-include=DIR],[Set the path to the libarchive headers]),
        [with_libarchive_include="-I$withval"], [])

AC_ARG_WITH(libarchive_library,
        AS_HELP_STRING([--with-libarchive-library=DIR],[Set the path to the libarchive libraries]),
        [with_libarchive_library="-L$withval"], [])

# enforce a specific version
if test "$with_libarchive_include" != '' -a "$libarchive_library" != 'no'; then
    LIBARCHIVE_CFLAGS="$with_libarchive_include"
    LIBARCHIVE_LIBS="$with_libarchive_library"
    LIBARCHIVE_VERSION=""
elif $WITH_DEVTOOLS; then # Scilab thirdparties
    LIBARCHIVE_CFLAGS="-I$DEVTOOLS_INCDIR"
    LIBARCHIVE_LIBS="-L$DEVTOOLS_LIBDIR -larchive -lcrypto -lscixml2 -lscilzma -lsciz -lscizstd"
    LIBARCHIVE_VERSION=""
fi

saved_LIBS="$LIBS"
saved_CFLAGS="$CFLAGS"
LIBS="$LIBARCHIVE_LIBS $LIBS"
CFLAGS="$LIBARCHIVE_CFLAGS $CFLAGS"

AC_CHECK_HEADERS([archive.h])
AC_CHECK_LIB([archive], [archive_read_new])

# try with pkg-config on preset failure
if test "$ac_cv_header_archive_h" != yes && test "$ac_cv_search_archive_read_new" == no; then
    PKG_CHECK_MODULES(LIBARCHIVE, libarchive >= 3.1)

    LIBS="$LIBARCHIVE_LIBS $LIBS"
    CFLAGS="$LIBARCHIVE_CFLAGS $CFLAGS"

    AC_CHECK_HEADERS([archive.h])
    AC_CHECK_LIB([archive], [archive_read_new])
fi

AC_RUN_IFELSE([AC_LANG_PROGRAM([
#include <archive.h>
#include <stdio.h>
], [
int major = ARCHIVE_VERSION_NUMBER / 1000000;
int minor = (ARCHIVE_VERSION_NUMBER % 1000000) / 1000;
int rev = ARCHIVE_VERSION_NUMBER % 1000;
printf("%d.%d.%d\n", major, minor, rev);
return 0;
])], [ LIBARCHIVE_VERSION=$(./conftest$EXEEXT) ], [ AC_MSG_FAILURE("Unable to detect libarchive") ] )

LIBARCHIVE_LIBS="$LIBS"
LIBARCHIVE_CFLAGS="$CFLAGS"

LIBS="$saved_LIBS"
CFLAGS="$saved_CFLAGS"

AC_SUBST(LIBARCHIVE_CFLAGS)
AC_SUBST(LIBARCHIVE_LIBS)
AC_SUBST(LIBARCHIVE_VERSION)

AC_DEFINE_UNQUOTED([LIBARCHIVE_CFLAGS],["$LIBARCHIVE_CFLAGS"],[LIBARCHIVE flags])
AC_DEFINE_UNQUOTED([LIBARCHIVE_LIBS],["$LIBARCHIVE_LIBS"],[LIBARCHIVE library])
AC_DEFINE_UNQUOTED([LIBARCHIVE_VERSION],["$LIBARCHIVE_VERSION"],[LIBARCHIVE version])

])
