dnl Because this macro is used by AC_PROG_GCC_TRADITIONAL, which must
dnl come early, it is not included in AC_BEFORE checks.
dnl AC_GREP_CPP(PATTERN, PROGRAM, [ACTION-IF-FOUND [,
dnl              ACTION-IF-NOT-FOUND]])
AC_DEFUN([AC_GREP_CPP],
[AC_REQUIRE_CPP()dnl
cat > conftest.$ac_ext <<EOF
[#]line __oline__ "configure"
#include "confdefs.h"
[$2]
EOF
dnl eval is necessary to expand ac_cpp.
dnl Ultrix and Pyramid sh refuse to redirect output of eval, so use subshell.
if (eval "$ac_cpp conftest.$ac_ext") 2>&AC_FD_CC |
dnl Prevent m4 from eating character classes:
changequote(, )dnl
  grep "$1" >/dev/null 2>&1; then
changequote([, ])dnl
  ifelse([$3], , :, [rm -rf conftest*
  $3])
ifelse([$4], , , [else
  rm -rf conftest*
  $4
])dnl
fi
rm -f conftest*
])

dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2008 - Sylvestre Ledru
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
