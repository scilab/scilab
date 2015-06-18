dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) Scilab Enterprises - 2015 - Vincent COUVERT
dnl
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at
dnl http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
dnl
dnl libarchive is mandatory in Scilab
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags
dnl * what are linking flags

AC_DEFUN([AC_LIBARCHIVE], [

AC_ARG_WITH(libarchive_include,
        AC_HELP_STRING([--with-libarchive-include=DIR],[Set the path to the libarchive headers]),
        [with_libarchive_include=$withval],
        [with_libarchive_include='yes']
        )

AC_ARG_WITH(libarchive_library,
        AC_HELP_STRING([--with-libarchive-library=DIR],[Set the path to the libarchive libraries]),
        [with_libarchive_library=$withval],
        [with_libarchive_library='yes']
        )

if test "x$with_libarchive_include" != "xyes"; then
    save_CFLAGS="$CFLAGS"
    CFLAGS="-I$with_libarchive_include"
    AC_CHECK_HEADERS([archive.h archive_entry.h],
        [LIBARCHIVE_CFLAGS="$CFLAGS"],
        [AC_MSG_ERROR([Cannot find headers (archive.h, archive_entry.h) of the library libarchive in $with_libarchive_include. Please install the dev package])])
    CFLAGS="$save_CFLAGS"
else
    LIBARCHIVE_CFLAGS=""
    #if $WITH_DEVTOOLS; then # Scilab thirdparties
    #    LIBARCHIVE_CFLAGS="-I$DEVTOOLS_INCDIR"
    #else
        AC_CHECK_HEADERS([archive.h archive_entry.h],
            [LIBARCHIVE_CFLAGS=""],
            [AC_MSG_ERROR([Cannot find headers (archive.h, archive_entry.h) of the library libarchive. Please install the dev package])])
    #fi
fi

save_LIBS="$LIBS"

# --with-libarchive-library set then check in this dir
if test "x$with_libarchive_library" != "xyes"; then
    LIBARCHIVE_LIBS="-L$with_libarchive_library -larchive"
    LIBS="$LIBS $LIBARCHIVE_LIBS"
    AC_CHECK_LIB([archive], [archive_read_new],
        [],
        [AC_MSG_ERROR([libarchive: library missing. (Cannot find symbol archive_read_new) in $with_libarchive_library. Check if libarchive is installed and if the version is correct])],
        []
        )
else
    #if $WITH_DEVTOOLS; then # Scilab thirparties
    #    LIBARCHIVE_LIBS="-L$DEVTOOLS_LIBDIR -larchive"
    #else
        LIBARCHIVE_LIBS="-larchive"
        LIBS="$LIBS $LIBARCHIVE_LIBS"
        AC_CHECK_LIB([archive], [archive_read_new],
            [],
            [AC_MSG_ERROR([libarchive: library missing. (Cannot find symbol archive_read_new) in $with_libarchive_library. Check if libarchive is installed and if the version is correct])],
            []
            )
    #fi
fi

LIBS="$save_LIBS"

AC_SUBST(LIBARCHIVE_LIBS)
AC_SUBST(LIBARCHIVE_CFLAGS)

AC_DEFINE([WITH_LIBARCHIVE], [], [With the libarchive library])

# Gets compilation and library flags
])
