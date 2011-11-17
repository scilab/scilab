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
    EIGENPATH="$with_eigen_include"
	CPPFLAGS="-I$EIGENPATH"
	AC_CHECK_HEADER([Eigen/Sparse],
		[EIGEN_CPPFLAGS="$CPPFLAGS"],
		[AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library EIGEN in $with_eigen_include. Please install the dev package (Debian : libeigen3-dev)])]
	)
else
	AC_CHECK_HEADER([Eigen/Sparse],
		[EIGEN_CPPFLAGS=""],
		[       $as_unset ac_cv_header_Eigen_Sparse 
                saved_cppflags="$CPPFLAGS"
                EIGENPATH="/usr/include/eigen3/"
                CPPFLAGS="$CPPFLAGS -I$EIGENPATH  -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET"
                AC_CHECK_HEADER([Eigen/Sparse],
                                [EIGEN_CPPFLAGS="-I$EIGENPATH  -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET"],
                                AC_MSG_ERROR([Cannot find headers (Eigen/Sparse) of the library eigen. Please install the dev package (Debian : libeigen3-dev)])
                )
         ])
fi
CPPFLAGS="$save_CPPFLAGS"

# Check in the packaged version of eigen where is this stuff
AC_CHECK_EIGEN_VERSION($EIGENPATH,3,0,Eigen/src/Core/util/Macros.h)

AC_SUBST(EIGEN_CPPFLAGS)
AC_DEFINE([WITH_EIGEN], [], [With the EIGEN library])
AC_LANG_POP([C++])
])



AC_DEFUN([AC_CHECK_EIGEN_VERSION], [
dnl INPUTS :
dnl  $1 : Path where to find the include file (/home/xxx/Eigen f. ex.)
dnl  $2 : Major version number (8 f. ex)
dnl  $3 : Minor version number (0 f. ex.)
dnl  $4 : include file name (Core/util/Macros.h f. ex.)
dnl
dnl OUTPUTS
dnl  EIGEN_VERSION_OK : 1 if OK, 0 otherwise
dnl  EIGEN_INC_PATH : include path flag for tcl.h (-I/usr/include f. ex.)
dnl  EIGEN_VERSION : ( 8.1 f. ex.)
dnl  EIGEN_MAJOR_VERSION: 
dnl  EIGEN_MAJOR_VERSION: 
dnl  Check the version of tcl associated to header file tcl.h 

CHK_EIGEN_INCLUDE_PATH=$1
CHK_EIGEN_WORLD=$2
CHK_EIGEN_MAJOR=$3
CHK_EIGEN_INC_NAME=$4
saved_cflags="$CFLAGS"
saved_cppflags="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I$CHK_EIGEN_INCLUDE_PATH"

AC_MSG_CHECKING([if Eigen is version $CHK_EIGEN_WORLD.$CHK_EIGEN_MAJOR or later])
AC_GREP_CPP(EIGEN_VERSION_OK,
[
#include "$CHK_EIGEN_INCLUDE_PATH/$CHK_EIGEN_INC_NAME"
#if (EIGEN_WORLD_VERSION > $CHK_EIGEN_WORLD)
EIGEN_VERSION_OK
#else
#if ((EIGEN_WORLD_VERSION == $CHK_EIGEN_WORLD) && (EIGEN_MAJOR_VERSION >= $CHK_EIGEN_MAJOR))
EIGEN_VERSION_OK
#endif
#endif
],\
EIGEN_VERSION_OK=1,\
EIGEN_VERSION_OK=0 )

AC_RUN_IFELSE(
    [AC_LANG_SOURCE([dnl
#include "confdefs.h"
#include <stdio.h>
#include <$CHK_EIGEN_INC_NAME>
int main(void) {
        FILE *world = fopen("eigenworld","w");
        FILE *major = fopen("eigenmajor","w");
        FILE *minor = fopen("eigenminor","w");
        fprintf(world,"%d",EIGEN_WORLD_VERSION);
        fprintf(major,"%d",EIGEN_MAJOR_VERSION);
        fprintf(minor,"%d",EIGEN_MINOR_VERSION);
        fclose(world);
        fclose(major);
        fclose(minor);
        return 0;
}
])],
       [
  EIGEN_WORLD_VERSION=`cat eigenworld`
  EIGEN_MAJOR_VERSION=`cat eigenmajor`
  EIGEN_MINOR_VERSION=`cat eigenminor`
  EIGEN_VERSION=$EIGEN_WORLD_VERSION.$EIGEN_MAJOR_VERSION
  rm -f eigenworld eigenmajor eigenminor
],
[AC_MSG_FAILURE([The EIGEN detection of the version failed. 
If you are using Intel Compiler, check if Intel Library (ex : libimf.so) is available (in the LD_LIBRARY_PATH for example)])])

case $host_os in
  darwin* | rhapsody*) 
# already added to CFLAGS 
  ;;
  *)
 EIGEN_INC_PATH=-I$i 
  ;;
esac

if test $EIGEN_VERSION_OK = 1; then 
	AC_MSG_RESULT([($EIGEN_VERSION) yes])
else 
	AC_MSG_RESULT([($EIGEN_VERSION) no])
	if test "$EIGEN_VERSION" = "can't happen"; then
		AC_MSG_ERROR([can't happen])
        else 
	   AC_MSG_ERROR([You need at least version $CHK_EIGEN_WORLD.$CHK_EIGEN_MAJOR of Eigen])
	fi
fi
CPPFLAGS=$saved_cppflags
]) dnl End of AC_CHECK_EIGEN_VERSION
