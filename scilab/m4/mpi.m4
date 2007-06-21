dnl
dnl Sylvestre Ledru <sylvestre.ledru@inria.fr>
dnl INRIA - Scilab 2007
dnl

dnl Detection of openmpi
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_OPENMPI], [

if test "$with_openmpi" != 'yes' -a "$with_openmpi" != 'no'; then
   # Look if mpicc (which provides cflags and ldflags) is available
   AC_MSG_CHECKING([openmpi, for mpicc])
   OPENMPI_FOUND=0
   MPICC="$with_openmpi/bin/mpicc" 
        if test -x "$MPICC"; then
                AC_MSG_RESULT([$MPICC])
				OPENMPI_CC=$MPICC
				OPENMPI_FOUND=1
		fi
   if test $OPENMPI_FOUND -eq 0; then
   MPICC="$with_openmpi/mpicc" 
        if test -x "$MPICC"; then
				OPENMPI_FOUND=1
				OPENMPI_CC=$MPICC
                AC_MSG_RESULT([$MPICC])
        fi
   fi
		if test $OPENMPI_FOUND -eq 0; then
                AC_MSG_ERROR([Unable to find $MPICC. Please check the path you provided])
		else
			unset OPENMPI_FOUND
		fi
else
		AC_CHECK_PROGS(OPENMPI_CC,mpicc,no)
		if test "x$MPICC" = "xno"; then
				AC_MSG_ERROR([Unable to find mpicc in the path. Please check your installation of openmpi (example : openmpi & openmpi-dev with Debian)])
		fi
fi
saved_cflags=$CFLAGS
saved_LIBS="$LIBS"
AC_CHECK_HEADER([mpi.h],
	[],
	[AC_MSG_ERROR([Cannot find headers of the library OpenMPI. Please install the dev package (Debian : openmpi-dev)])])

AC_CHECK_LIB([mpi], [MPI_Init],
               [OPENMPI_LIBS="-lmpi"],
               [AC_MSG_ERROR([openmpi : library missing. (Cannot find symbol MPI_Init in -lmpi). Check if OpenMPI is installed])]
               )

#OPENMPI_CPPFLAGS="-I$openmpi_dir/include"
#OPENMPI_LDFLAGS="-L$openmpi_dir/lib/"

#OPENMPI_HEADER="$openmpi_dir/include/mpi.h"
#OPENMPI_DIR="$openmpi_dir"

LIBS="$saved_LIBS"
CFLAGS=$saved_cflags

AC_SUBST(OPENMPI_FLAGS)
AC_SUBST(OPENMPI_LIBS)

#CFLAGS="$CFLAGS $OPENMPI_FLAGS"
#AC_CHECK_LIB(xml2,xmlInitParserCtxt,,[AC_MSG_ERROR([pcre : library missing])])


#AC_CHECK_HEADERS([pcre/tree.h],,[AC_MSG_ERROR([pcre : library missing missing])])	

# Gets compilation and library flags
])
