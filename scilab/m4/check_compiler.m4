dnl Macros which process ./configure arguments
dnl Copyright INRIA
dnl Sylvestre Ledru - June 2006


AC_DEFUN([AC_CHECK_PROG_GCC],[

	# Look for GCC
	AC_CHECK_PROGS(GCC,gcc,no)
	if test "$GCC" = no; then
		AC_MSG_ERROR([Unable to configure: gcc compiler not found])
	fi
])

AC_DEFUN([AC_CHECK_PROG_G77],[
	# Look for G77
	STOP=$1
	AC_PROG_FC(g77)
	if test "$STOP" = ""; then
		if test "$FC" != "g77"; then
			AC_MSG_ERROR([Unable to configure: g77 compiler not found])
		fi
	fi

])

AC_DEFUN([AC_CHECK_PROG_GFORTRAN],[
	# Look for GFortran
	STOP=$1
	AC_PROG_FC(gfortran)
	if test "$STOP" = ""; then
		if test "$FC" != "gfortran"; then
			AC_MSG_ERROR([Unable to configure: gfortran compiler not found])
		fi
	fi
])

AC_DEFUN([AC_CHECK_PROG_GNU],[
	# GNU C and Fortran compilers required
	AC_CHECK_PROG_GCC()
	AC_CHECK_PROG_GFORTRAN(no)
	if test "$WITH_GFORTRAN" = no; then
		AC_CHECK_PROG_G77(no)
		if test "$WITH_G77" = no; then
			AC_MSG_ERROR([Unable to configure: g77 or gfortran compiler not found])
		fi
	fi
])

AC_DEFUN([AC_CHECK_PROG_INTEL],[
	# Look for Intel Compiler
	case "$host" in
	i*86-*-linux-gnu | *-pc-linux-gnu | x86_64-*-linux-gnu | ia64-*-linux-gnu) 
		AC_CHECK_PROGS(ICC,icc ecc,no)
		if test "$ICC" = no; then
			AC_MSG_ERROR([Unable to configure: icc nor ecc compiler found])
		fi
		AC_CHECK_PROGS(IFC,ifort ifc efc,no)
		AC_PROG_FC(ifort ifc efc)
		if test -z "$FC" ; then
			AC_MSG_ERROR([Unable to configure: ifort, ifc nor efc compiler found])
		fi
		;;
	*) 
		AC_MSG_ERROR([Intel compiler not supported for the architecture $host])
		;;
	esac
])