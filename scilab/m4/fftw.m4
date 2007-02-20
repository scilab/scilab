dnl AC_FFTW
dnl ------------------------------------------------------
dnl Check if FFTW is usable and working
dnl
AC_DEFUN([AC_FFTW], [

       AC_CHECK_HEADER([fftw3.h],[],[AC_MSG_ERROR([Cannot find headers (fftw3.h) of the library fftw. Please install the dev package (Debian : fftw3-dev)])])
	   AC_CHECK_LIB([fftw3], [fftw_plan_dft_r2c],
			[FFTW3_LIB=-lfftw3],
            [AC_MSG_ERROR([libfftw3 : library missing. (Cannot find symbol fftw_plan_dft_r2c). Check if libfftw3 is installed and if the version is correct])]
			)
       AC_SUBST(FFTW3_LIB)
	   AC_DEFINE([WITH_FFTW], [], [With FFTW])
])


