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
    if $WITH_DEVTOOLS; then # Scilab thirparties
        UMFPACK_CFLAGS="-I$DEVTOOLS_INCDIR"
    else
        AC_CHECK_HEADER([suitesparse/umfpack.h],
            [SUITESPARSE=yes],
            [AC_CHECK_HEADER(
                [umfpack.h],
                [SUITESPARSE=no],
                [AC_MSG_ERROR([Cannot find headers (umfpack.h) of the library UMFPACK. Please install the dev package (Debian : libsuitesparse-dev)])
                ])
            ])
    fi
fi

# --with-umfpack-library set then check in this dir
if test "x$with_umfpack_library" != "xyes"; then
	AC_MSG_CHECKING([for umfpack_di_solve in $with_umfpack_library])
	save_LIBS="$LIBS"
	LIBS="$BLAS_LIBS -L$with_umfpack_library -lm $LIBS"
	# We need -lm because sometimes (ubuntu 7.10 for example) does not link libamd against lib math

	AC_CHECK_LIB([umfpack], [umfpack_di_solve],
			[UMFPACK_LIB="-L$with_umfpack_library -lumfpack $UMFPACK_LIB"; UMFPACK_OK=yes],
            [AC_MSG_ERROR([libumfpack : Library missing. (Cannot find umfpack_di_solve). Check if libumfpack is installed and if the version is correct (also called lib suitesparse)])]
			)

#	AC_TRY_LINK_FUNC(umfpack_di_solve, [UMFPACK_OK=yes; BLAS_TYPE="Using BLAS_LIBS environment variable"], [UMFPACK_LIBS=""])
	AC_MSG_RESULT($UMFPACK_OK)
	LIBS="$save_LIBS"
fi

# check in the default path
if test $UMFPACK_OK = no; then
    if $WITH_DEVTOOLS; then # Scilab thirparties
        UMFPACK_LIB="-L$DEVTOOLS_LIBDIR -lumfpack -lamd"
    else
        save_LIBS="$LIBS"
        LIBS="$BLAS_LIBS $LIBS -lm" # libamd* is mandatory to link umfpack
        # We need -lm because sometimes (ubuntu 7.10 for example) does not link libamd against lib math

        AC_CHECK_LIB([amd], [amd_info],
            [UMFPACK_LIB="-lamd"],
            [AC_MSG_ERROR([libamd: Library missing (Cannot find symbol amd_info). Check if libamd (sparse matrix minimum degree ordering) is installed and if the version is correct])]
            )
        LIBS="$UMFPACK_LIB $LIBS"
        AC_CHECK_LIB([umfpack], [umfpack_di_solve],
            [UMFPACK_LIB="-lumfpack $UMFPACK_LIB"; UMFPACK_OK=yes],
            [AC_MSG_ERROR([libumfpack: Library missing. (Cannot find symbol umfpack_di_solve). Check if libumfpack is installed and if the version is correct (also called lib suitesparse)])]
            )
        LIBS="$save_LIBS"
    fi
fi

AC_SUBST(UMFPACK_LIB)
AC_SUBST(UMFPACK_CFLAGS)
if test $SUITESPARSE = yes; then
   AC_DEFINE_UNQUOTED([UMFPACK_SUITESPARSE],[] , [If it is UMFPACK/Suitesparse or UMFPACK standalone])
fi

AC_DEFINE([WITH_UMFPACK], [], [With the UMFPACK library])

])
