dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2007 - Sylvestre Ledru
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
