dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) DIGITEO - 2009 - 2009 - Bruno JOFRET
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dnl
dnl libhdf5 is mandatory in Scilab 
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_LIBHDF5], [

AC_ARG_WITH(libhdf5,
		AC_HELP_STRING([--with-libhdf5=PREFIX],[Set the path to your libhdf5 installation]),
		[with_libhdf5=$withval],
		[with_libhdf5='yes']
		)

#if test "$with_libhdf5" != 'yes' -a "$with_libhdf5" != 'no'; then
#fi
saved_cflags=$CFLAGS
saved_LIBS="$LIBS"
		
HDF5_FLAGS="-I/usr/lib/openmpi/include"
HDF5_LIBS="-lhdf5"

CFLAGS="$CFLAGS $HDF5_FLAGS"
LIBS="$LIBS $HDF5_LIBS"

AC_CHECK_LIB([hdf5], [H5Dopen],
               [],
               [AC_MSG_ERROR([libhdf5: library missing. (Cannot find symbol H5Dopen). Check if libhdf5 is installed and if the version is correct])]
               )
#AC_CHECK_HEADERS([libxml/xmlreader.h])
#AC_CHECK_HEADERS([libxml/parser.h])
#AC_CHECK_HEADERS([libxml/tree.h])

#AC_CHECK_HEADERS([libxml/xpath.h])
#AC_CHECK_HEADERS([libxml/xpathInternals.h])

CFLAGS=$saved_cflags
LIBS="$saved_LIBS"

AC_SUBST(HDF5_FLAGS)
AC_SUBST(HDF5_LIBS)

AC_DEFINE_UNQUOTED([LIBHDF5_FLAGS],["$HDF5_FLAGS"],[libHDF5 flags])
AC_DEFINE_UNQUOTED([LIBHDF5_LIBS],["$HDF5_LIBS"],[libHDF5 library])

# Gets compilation and library flags
])
