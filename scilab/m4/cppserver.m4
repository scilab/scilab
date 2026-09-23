dnl
dnl Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) S/E - 2026 - Cédric Delamarre
dnl
dnl Copyright (C) 2012 - 2016 - Scilab Enterprises
dnl
dnl This file is hereby licensed under the terms of the GNU GPL v2.0,
dnl pursuant to article 5.3.4 of the CeCILL v.2.1.
dnl This file was originally licensed under the terms of the CeCILL v.2.1,
dnl and continues to be available under such terms.
dnl For more information, see the COPYING file which you should have received
dnl along with this program.
dnl
dnl CPPServer detection
dnl

dnl CPPServer is optional in Scilab
AC_DEFUN([AC_CPPSERVER], [

AC_ARG_WITH([cppserver],
    AS_HELP_STRING([--with-cppserver=DIR],[Set the path to your CPPServer installation]),
    [with_cppserver=$withval],
    [with_cppserver='yes']
    )

if test "$with_cppserver" != 'no'; then
    CPPSERVER_FOUND=0
    CPPSERVER_CFLAGS=""
    CPPSERVER_LIBS=""
    CPPSERVER_VERSION=""

    # Default path based on WITH_DEVTOOLS
    CPPSERVER_INCDIR="${DEVTOOLS_INCDIR}"
    CPPSERVER_LIBDIR="${DEVTOOLS_LIBDIR}"

    if test "$with_cppserver" != 'yes'; then
        CPPSERVER_INCDIR="$with_cppserver"
    fi

    # Check if cppserver directory exists
    if test -d "${CPPSERVER_INCDIR}/cppserver"; then
        CPPSERVER_FOUND=1
        AC_MSG_RESULT([Found CPPServer at $CPPSERVER_INCDIR])

        # Collect include flags for cppserver and all dependencies
        CPPSERVER_CFLAGS="-I${CPPSERVER_INCDIR}/cppserver"
        CPPSERVER_CFLAGS="$CPPSERVER_CFLAGS -I${CPPSERVER_INCDIR}/cppcommon"
        CPPSERVER_CFLAGS="$CPPSERVER_CFLAGS -I${CPPSERVER_INCDIR}/asio"
        CPPSERVER_CFLAGS="$CPPSERVER_CFLAGS -I${CPPSERVER_INCDIR}/fmt"

        if test -z "${CPPSERVER_LIBDIR}"; then
            CPPSERVER_LIBDIR="${CPPSERVER_INCDIR}/../lib"
        fi
        CPPSERVER_LIBS="-L${CPPSERVER_LIBDIR} -lasio -lfmt -lcppcommon -lcppserver"

        # Check for cppserver library
        save_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="${CPPSERVER_CFLAGS} $CPPFLAGS"
        saved_LIBS="$LIBS"
        LIBS="${CPPSERVER_LIBS} $LIBS"

        AC_LANG_PUSH([C++])
        AC_MSG_CHECKING([for asio::io_context in -lcppserver])
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM([[#include <asio.hpp>]],
                              [[asio::io_context ctx; (void)ctx;]])],
            [AC_MSG_RESULT([yes])],
            [AC_MSG_RESULT([no])
             AC_MSG_ERROR([cppserver: library missing. (Cannot find symbol asio::io_context). Check if cppserver is installed])])

        # Check for cppserver headers
        AC_CHECK_HEADERS([server/version.h], [], [AC_MSG_ERROR([Unable to use server/version.h])])

        AC_LANG_POP([C++])
        LIBS="$saved_LIBS"
        CPPFLAGS="$save_CPPFLAGS"

        # Try to get version from library if possible
        if test -f "${CPPSERVER_LIBDIR}/cppserver.so" || test -f "${CPPSERVER_LIBDIR}/libcppserver.so"; then
            CPPSERVER_VERSION="1.0.0"
        fi

        AC_MSG_CHECKING([CPPServer version])
        AC_MSG_RESULT([$CPPSERVER_VERSION])
    else
        AC_MSG_WARN([CPPServer directory not found at $CPPSERVER_DIR. CPPServer support disabled.])
        CPPSERVER_FOUND=0
    fi

    if test $CPPSERVER_FOUND -eq 1; then
        AC_SUBST(CPPSERVER_CFLAGS)
        AC_SUBST(CPPSERVER_LIBS)
        AC_SUBST(CPPSERVER_VERSION)

        AC_DEFINE_UNQUOTED([CPPSERVER_CFLAGS],["$CPPSERVER_CFLAGS"],[CPPServer flags])
        AC_DEFINE_UNQUOTED([CPPSERVER_LIBS],["$CPPSERVER_LIBS"],[CPPServer library])
        AC_DEFINE_UNQUOTED([CPPSERVER_VERSION],["$CPPSERVER_VERSION"],[CPPServer version])
    fi
fi
])
