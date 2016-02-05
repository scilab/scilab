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
dnl AC_FFTW
dnl ------------------------------------------------------
dnl Check if FFTW is usable and working
dnl
AC_DEFUN([AC_FFTW], [

AC_ARG_WITH(fftw_include,
		AC_HELP_STRING([--with-fftw-include=DIR],[Set the path to the FFTW headers]),
		[with_fftw_include=$withval],
		[with_fftw_include='yes']
		)

AC_ARG_WITH(fftw_library,
		AC_HELP_STRING([--with-fftw-library=DIR],[Set the path to the FFTW libraries]),
		[with_fftw_library=$withval],
		[with_fftw_library='yes']
		)

if test "x$with_fftw_include" != "xyes"; then
	save_CFLAGS="$CFLAGS"
	CFLAGS="-I$with_fftw_include"
	AC_CHECK_HEADER([fftw3.h],
		[FFTW3_CFLAGS="$CFLAGS"],
		[AC_MSG_ERROR([Cannot find headers (fftw3.h) of the library FFTW in $with_fftw_include. Please install the dev package (Debian : libfftw3-dev)])]
	)
	CFLAGS="$save_CFLAGS"
else
    if $WITH_DEVTOOLS; then # Scilab thirdparties
        FFTW3_CFLAGS="-I$DEVTOOLS_INCDIR"
    else
        AC_CHECK_HEADER([fftw3.h],
        [FFTW3_CFLAGS=""],
        [AC_MSG_ERROR([Cannot find headers (fftw3.h) of the library fftw. Please install the dev package (Debian : libfftw3-dev)])])
    fi
fi


# --with-fftw-library set then check in this dir
if test "x$with_fftw_library" != "xyes"; then
    save_LIBS="$LIBS"
    LIBS="-L$with_fftw_library -lfftw3"
    AC_CHECK_LIB([fftw3], [fftw_plan_dft_r2c],
        [FFTW3_LIB="-L$with_fftw_library -lfftw3"],
        [AC_MSG_ERROR([libfftw3 : library missing. (Cannot find symbol fftw_plan_dft_r2c) in $with_fftw_library. Check if libfftw3 is installed and if the version is correct])]
        )
    LIBS="$save_LIBS"
else
    save_LIBS="$LIBS"
    if $WITH_DEVTOOLS; then # Scilab thirdparties
        FFTW3_LIB="-L$DEVTOOLS_LIBDIR -lfftw3"
    else
        AC_CHECK_LIB([fftw3], [fftw_plan_dft_r2c],
            [FFTW3_LIB="-lfftw3"],
            [AC_MSG_ERROR([libfftw3 : library missing. (Cannot find symbol fftw_plan_dft_r2c). Check if libfftw3 is installed and if the version is correct])]
            )
    fi
    LIBS="$save_LIBS"
fi
AC_SUBST(FFTW3_LIB)
AC_DEFINE([WITH_FFTW], [], [With the FFTW library])

])
