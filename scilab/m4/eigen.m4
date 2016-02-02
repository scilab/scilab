dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2008 - Sylvestre Ledru
dnl
dnl Copyright (C) 2012 - 2016 - Scilab Enterprises
dnl
dnl This file is hereby licensed under the terms of the GNU GPL v2.0,
dnl pursuant to article 5.3.4 of the CeCILL v.2.1.
dnl This file was originally licensed under the terms of the CeCILL v2.1,
dnl and continues to be available under such terms.
dnl For more information, see the COPYING file which you should have received
dnl along with this program.
dnl
dnl AC_EIGEN
dnl ------------------------------------------------------
dnl Check if EIGEN is usable and working
dnl
AC_DEFUN([AC_EIGEN], [
AC_LANG_PUSH([C++])

AC_ARG_WITH(eigen_include,
        AC_HELP_STRING([--with-eigen-include=DIR],[Set the path to the EIGEN headers]),
        [with_eigen_include=$withval],
        [with_eigen_include='yes']
        )


save_CPPFLAGS="$CPPFLAGS"

if test "x$with_eigen_include" != "xyes"; then
    CPPFLAGS="-I$with_eigen_include"
    AC_CHECK_HEADER([Eigen/Sparse],
        [EIGEN_CPPFLAGS="$CPPFLAGS"],
        [AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library EIGEN in $with_eigen_include. Please install the dev package (Debian : libeigen3-dev)])]
    )
    PATH_TO_EIGEN=$with_eigen_include
else
    if $WITH_DEVTOOLS; then # Scilab thirparties
        CPPFLAGS="-I$DEVTOOLS_INCDIR"
        AC_CHECK_HEADER([Eigen/Sparse],
            [EIGEN_CPPFLAGS="$CPPFLAGS"],
            [AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library EIGEN in $with_eigen_include. Please install the dev package (Debian : libeigen3-dev)])]
        )
        PATH_TO_EIGEN=$DEVTOOLS_INCDIR
    else
        AC_CHECK_HEADER([Eigen/Sparse],
            [EIGEN_CPPFLAGS=""],
            [       $as_unset ac_cv_header_Eigen_Sparse
                    CPPFLAGS="-I/usr/include/eigen3/"
                    AC_CHECK_HEADER([Eigen/Sparse],
                                    [EIGEN_CPPFLAGS=$CPPFLAGS],
                                    AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library eigen. Please install the dev package (Debian : libeigen3-dev)])
                    )
             ])
        PATH_TO_EIGEN="/usr/include/eigen3/"
    fi
fi
CPPFLAGS="$save_CPPFLAGS"

CHK_EIGEN_WORLD=3
CHK_EIGEN_MAJOR=1
CHK_EIGEN_MINOR=0

AC_MSG_CHECKING([if Eigen is version $CHK_EIGEN_WORLD.$CHK_EIGEN_MAJOR.$CHK_EIGEN_MINOR or later])
AC_GREP_CPP(EIGEN_VERSION_OK,
[
#include "$PATH_TO_EIGEN/Eigen/Sparse"
#if EIGEN_VERSION_AT_LEAST(3,1,0)
EIGEN_VERSION_OK
#endif
],\
EIGEN_VERSION_OK=1,\
EIGEN_VERSION_OK=0 )
if test $EIGEN_VERSION_OK = 0; then
   AC_MSG_ERROR([Version $CHK_EIGEN_WORLD.$CHK_EIGEN_MAJOR.$CHK_EIGEN_MINOR of Eigen expected (at least)])
else
   AC_MSG_RESULT([yes])
fi
AC_SUBST(EIGEN_CPPFLAGS)

AC_DEFINE([WITH_EIGEN], [], [With the EIGEN library])
AC_LANG_POP([C++])
])
