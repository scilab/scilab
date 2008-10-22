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
	AC_CHECK_HEADER([fftw3.h],
		[FFTW3_CFLAGS=""],
		[AC_MSG_ERROR([Cannot find headers (fftw3.h) of the library fftw. Please install the dev package (Debian : libfftw3-dev)])])
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
	AC_CHECK_LIB([fftw3], [fftw_plan_dft_r2c],
			[FFTW3_LIB="-lfftw3"],
            [AC_MSG_ERROR([libfftw3 : library missing. (Cannot find symbol fftw_plan_dft_r2c). Check if libfftw3 is installed and if the version is correct])]
			)
	LIBS="$save_LIBS"
fi
AC_SUBST(FFTW3_LIB)
AC_DEFINE([WITH_FFTW], [], [With the FFTW library])

])
