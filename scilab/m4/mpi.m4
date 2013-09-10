dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2007 - Sylvestre Ledru
dnl
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at
dnl http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
dnl
dnl Detection of openmpi
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags
dnl * what are linking flags
AC_DEFUN([AC_OPENMPI], [
		AC_CHECK_PROGS(OPENMPI_CC,mpicc,no)
		if test "x$MPICC" = "xno"; then
				AC_MSG_ERROR([Unable to find mpicc in the path. Please check your installation of openmpi (example : openmpi & openmpi-dev with Debian)])
		fi
		AC_CHECK_PROGS(OPENMPI_CXX,mpicxx,no)
		if test "x$MPICXX" = "xno"; then
				AC_MSG_ERROR([Unable to find mpicxx in the path. Please check your installation of openmpi (example : openmpi & openmpi-dev with Debian)])
		fi
saved_cflags=$CFLAGS
saved_cxxflags=$CXXFLAGS
saved_LIBS="$LIBS"

CFLAGS=`$OPENMPI_CC --showme:compile`
CXXFLAGS=`$OPENMPI_CXX --showme:compile`
LIBS=`$OPENMPI_CC --showme:link`

AC_CHECK_HEADER([mpi.h],
	[OPENMPI_CFLAGS=$CFLAGS; OPENMPI_CXXFLAGS=$CXXFLAGS],
	[AC_MSG_ERROR([Cannot find headers of the library OpenMPI. Please install the dev package (Debian : openmpi-dev)])])

AC_CHECK_LIB([mpi], [MPI_Init],
               [OPENMPI_LIBS=$LIBS],
               [AC_MSG_ERROR([openmpi : library missing. (Cannot find symbol MPI_Init in -lmpi). Check if OpenMPI is installed])]
               )

AC_DEFINE([ENABLE_MPI],[],[Have MPI features])

LIBS="$saved_LIBS"
CFLAGS=$saved_cflags
CXXFLAGS=$saved_cppflags

AC_SUBST(OPENMPI_CFLAGS)
AC_SUBST(OPENMPI_CXXFLAGS)
AC_SUBST(OPENMPI_LIBS)

])
