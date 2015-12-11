dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) S/E - 2012 - Sylvestre Ledru
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
dnl
dnl CURL detection
dnl

dnl CURL is mandatory in Scilab 
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_CURL], [


AC_ARG_WITH(curl,
    AC_HELP_STRING([--with-curl=DIR],[Set the path to your CURL installation]),
    [with_curl=$withval],
    [with_curl='yes']
    )

if test "$with_curl" != 'yes' -a "$with_curl" != 'no'; then
    # Look if curl-config (which provides cflags and ldflags) is available
    AC_MSG_CHECKING([curl, for curl-config])
    CURL_FOUND=0
    CURL_CONFIG="$with_curl/bin/curl-config"
    if test -x "$CURL_CONFIG"; then
        AC_MSG_RESULT([$CURL_CONFIG])
        CURL_FOUND=1
    fi
    if test $CURL_FOUND -eq 0; then
        CURL_CONFIG="$with_curl/curl-config"
        if test -x "$CURL_CONFIG"; then
            CURL_FOUND=1
            AC_MSG_RESULT([$CURL_CONFIG])
        fi
    fi
    if test $CURL_FOUND -eq 0; then
        AC_MSG_ERROR([Unable to find $CURL_CONFIG. Please check the path you provided])
    else
       unset CURL_FOUND
    fi
else
    if $WITH_DEVTOOLS; then # Scilab thirdparties
        CURL_CONFIG="$DEVTOOLS_BINDIR/curl-config"
    else
        AC_CHECK_PROGS(CURL_CONFIG,curl-config,no)
        if test "x$CURL_CONFIG" = "xno"; then
            AC_MSG_ERROR([Unable to find curl-config in the path. Please check your installation of curl (example : libcurl3 & libcurl3-dev with Debian)])
        fi
    fi
fi
saved_cflags="$CFLAGS"
saved_LIBS="$LIBS"

CURL_CFLAGS=`$CURL_CONFIG --cflags`
CURL_LIBS=`$CURL_CONFIG --libs`
CURL_VERSION=`$CURL_CONFIG --version`

CURL_REQUIRED_VERSION=7.6
    PKG_CHECK_MODULES(CURL,
                      libcurl >= $CURL_REQUIRED_VERSION)

LIBS="$CURL_LIBS $LIBS"
CFLAGS="$CURL_CFLAGS $CFLAGS"

AC_CHECK_LIB([curl], [curl_easy_setopt],
               [],
               [AC_MSG_ERROR([curl : library missing. (Cannot find symbol curl_easy_setopt). Check if curl is installed and if the version is correct])]
               )

AC_CHECK_HEADERS([curl.h])

LIBS="$saved_LIBS"
CFLAGS="$saved_cflags"

AC_SUBST(CURL_CFLAGS)
AC_SUBST(CURL_LIBS)
AC_SUBST(CURL_VERSION)

AC_DEFINE_UNQUOTED([CURL_CFLAGS],["$CURL_CFLAGS"],[CURL flags])
AC_DEFINE_UNQUOTED([CURL_LIBS],["$CURL_LIBS"],[CURL library])
AC_DEFINE_UNQUOTED([CURL_VERSION],["$CURL_VERSION"],[CURL version])

# Gets compilation and library flags
])
