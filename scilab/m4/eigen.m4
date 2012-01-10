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
	CPPFLAGS="-I$with_eigen_include -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET"
	AC_CHECK_HEADER([Eigen/Sparse],
		[EIGEN_CPPFLAGS="$CPPFLAGS"],
		[AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library EIGEN in $with_eigen_include. Please install the dev package (Debian : libeigen3-dev)])]
	)
else
	AC_CHECK_HEADER([Eigen/Sparse],
		[EIGEN_CPPFLAGS=""],
		[       $as_unset ac_cv_header_Eigen_Sparse 
                CPPFLAGS="-I/usr/include/eigen3/ -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET"
                AC_CHECK_HEADER([Eigen/Sparse],
                                [EIGEN_CPPFLAGS=$CPPFLAGS],
                                AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library eigen. Please install the dev package (Debian : libeigen3-dev)])
                )
         ])
fi
CPPFLAGS="$save_CPPFLAGS"

AC_SUBST(EIGEN_CPPFLAGS)
AC_DEFINE([WITH_EIGEN], [], [With the EIGEN library])
AC_LANG_POP([C++])
])
