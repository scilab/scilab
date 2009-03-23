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
dnl UMFPACK detection
dnl ------------------------------------------------------
dnl Check if UMFPACK is usable and working
dnl
AC_DEFUN([AC_UMFPACK], [

BLAS_LIBS="$1"

UMFPACK_OK=no
SUITESPARSE=no

AC_ARG_WITH(umfpack_library,
		AC_HELP_STRING([--with-umfpack-library=DIR],[Set the path to the UMFPACK libraries]),
		[with_umfpack_library=$withval],
		[with_umfpack_library='yes']
		)

AC_ARG_WITH(umfpack_include,
		AC_HELP_STRING([--with-umfpack-include=DIR],[Set the path to the UMFPACK headers]),
		[with_umfpack_include=$withval],
		[with_umfpack_include='yes']
		)

# Include provided... check if you set it as -I/path/ if it can find the header
if test "x$with_umfpack_include" != "xyes"; then
	save_CFLAGS="$CFLAGS"
	CFLAGS="-I$with_umfpack_include"
	AC_CHECK_HEADER([suitesparse/umfpack.h],
	[UMFPACK_CFLAGS="$CFLAGS"; SUITESPARSE=yes],
	[AC_CHECK_HEADER(
				[umfpack.h],
				[UMFPACK_CFLAGS="$CFLAGS"; SUITESPARSE=no],
				[AC_MSG_ERROR([Cannot find headers (umfpack.h) of the library UMFPACK. Please install the dev package (Debian : libsuitesparse-dev)])
	])
	])
	CFLAGS="$save_CFLAGS"
fi

# Look in the default paths
if test "x$UMFPACK_INCLUDE" = "x" ; then 
AC_CHECK_HEADER([suitesparse/umfpack.h],
	[SUITESPARSE=yes],
	[AC_CHECK_HEADER(
				[umfpack.h],
				[SUITESPARSE=no],
				[AC_MSG_ERROR([Cannot find headers (umfpack.h) of the library UMFPACK. Please install the dev package (Debian : libsuitesparse-dev)])
	])
	])
fi

# --with-umfpack-library set then check in this dir
if test "x$with_umfpack_library" != "xyes"; then
	AC_MSG_CHECKING([for umf_divcomplex in $with_umfpack_library])
	save_LIBS="$LIBS"
	LIBS="$BLAS_LIBS -L$with_umfpack_library -lm $LIBS"
	# We need -lm because sometime (ubuntu 7.10 for example) does not link libamd against lib math

	AC_CHECK_LIB([amd], [amd_info],
			[UMFPACK_LIB="-lamd"],
            [AC_MSG_ERROR([libamd: Library missing (Cannot find symbol amd_info). Check if libamd (sparse matrix minimum degree ordering) is installed and if the version is correct])]
			)
	AC_CHECK_LIB([umfpack], [umf_divcomplex],
			[UMFPACK_LIB="-L$with_umfpack_library -lumfpack $UMFPACK_LIB"; UMFPACK_OK=yes],
            [AC_MSG_ERROR([libumfpack : Library missing. (Cannot find umf_divcomplex). Check if libumfpack is installed and if the version is correct (also called lib suitesparse)])]
			)

#	AC_TRY_LINK_FUNC(umf_divcomplex, [UMFPACK_OK=yes; BLAS_TYPE="Using BLAS_LIBS environment variable"], [UMFPACK_LIBS=""])
	AC_MSG_RESULT($UMFPACK_OK)
	LIBS="$save_LIBS"
fi

# check in the default path
if test $UMFPACK_OK = no; then
	save_LIBS="$LIBS"
	LIBS="$BLAS_LIBS $LIBS -lm" # libamd* is mandatory to link umfpack
	# We need -lm because sometime (ubuntu 7.10 for example) does not link libamd against lib math

	AC_CHECK_LIB([amd], [amd_info],
			[UMFPACK_LIB="-lamd"],
            [AC_MSG_ERROR([libamd: Library missing (Cannot find symbol amd_info). Check if libamd (sparse matrix minimum degree ordering) is installed and if the version is correct])]
			)
	LIBS="$UMFPACK_LIB $LIBS"
	AC_CHECK_LIB([umfpack], [umf_divcomplex],
			[UMFPACK_LIB="-lumfpack $UMFPACK_LIB"; UMFPACK_OK=yes],
            [AC_MSG_ERROR([libumfpack: Library missing. (Cannot find symbol umf_divcomplex). Check if libumfpack is installed and if the version is correct (also called lib suitesparse)])]
			)
		LIBS="$save_LIBS"
fi

AC_SUBST(UMFPACK_LIB)
if test $SUITESPARSE = yes; then
	AC_DEFINE_UNQUOTED([UMFPACK_SUITESPARSE],[] , [If it is UMFPACK/Suitesparse or UMFPACK standalone])
fi

AC_DEFINE([WITH_UMFPACK], [], [With the UMFPACK library])

])
