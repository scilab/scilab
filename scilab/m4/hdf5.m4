dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) DIGITEO - 2009 - Bruno JOFRET
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
AC_DEFUN([AC_HDF5], [


AC_ARG_WITH(hdf5_include,
        AC_HELP_STRING([--with-hdf5-include=DIR],[Set the path to the HDF5 headers]),
        [with_hdf5_include=$withval],
        [with_hdf5_include='yes']
        )

AC_ARG_WITH(hdf5_library,
        AC_HELP_STRING([--with-hdf5-library=DIR],[Set the path to the HDF5 libraries]),
        [with_hdf5_library=$withval],
        [with_hdf5_library='yes']
        )
        
if test "x$with_hdf5_include" != "xyes"; then
    save_CFLAGS="$CFLAGS"
    CFLAGS="-I$with_hdf5_include"
    AC_CHECK_HEADER([hdf5.h],
        [HDF5_CFLAGS="$CFLAGS"],
        [AC_MSG_ERROR([Cannot find headers (hdf5.h) of the library HDF5 in $with_hdf5_include. Please install the dev package])]
    )
    CFLAGS="$save_CFLAGS"
else
    HDF5_CFLAGS=""
    AC_CHECK_HEADER([hdf5.h],
        [HDF5_CFLAGS=""],
        [AC_MSG_ERROR([Cannot find headers (hdf5.h) of the library HDF5. Please install the dev package])])
fi

save_LIBS="$LIBS"

# --with-hdf5-library set then check in this dir
if test "x$with_hdf5_library" != "xyes"; then
    HDF5_LIBS="-L$with_hdf5_library -lhdf5"
    LIBS="$LIBS $HDF5_LIBS"
    AC_CHECK_LIB([hdf5], [H5Fopen],
            [],
            [AC_MSG_ERROR([libhdf5 : library missing. (Cannot find symbol H5Fopen) in $with_hdf5_library. Check if libhdf5 is installed and if the version is correct])]
            )
    
else
    HDF5_LIBS="-lhdf5"
    LIBS="$LIBS $HDF5_LIBS"
    AC_CHECK_LIB([hdf5], [H5Fopen],
            [],
            [AC_MSG_ERROR([libhdf5 : library missing. (Cannot find symbol H5Fopen). Check if libhdf5 is installed and if the version is correct])]
            )
fi

LIBS="$save_LIBS"

AC_SUBST(HDF5_LIBS)
AC_SUBST(HDF5_CFLAGS)

AC_DEFINE([WITH_HDF5], [], [With the HDF5 library])

# Gets compilation and library flags
])


AC_DEFUN([AC_JAVA_HDF5], [

     # HDF5 java lib
     AC_JAVA_CHECK_PACKAGE([jhdf5],[ncsa.hdf.hdf5lib.HDF5Constants],[HDF5 Java library])
     JHDF5=$PACKAGE_JAR_FILE
     AC_SUBST(JHDF5)
])

AC_DEFUN([AC_JNI_HDF5], [
     LDFLAGS_save=$LDFLAGS
   # Provide known paths where distribs/OS can store JNI libs
        LDFLAGS="-L/usr/lib/jni -L/usr/lib64/jni/ -L$SCI_SRCDIR/bin/ -L$SCI_SRCDIR/thirdparty/  -lpthread $HDF5_LIBS"
   # -lpthread because of packaging bug in jhdf5
      AC_CHECK_LIB([jhdf5], [h5JNIFatalError], [JHDF5_LIBS="-ljhdf5"],
           [AC_MSG_ERROR([libjhdf5: Library missing (Cannot find symbol h5JNIFatalError). Check if libjhdf5 - C/Java (JNI) interface for HDF5 - is installed and if the version is correct. Note that you might have to update etc/librarypath.xml to provide the actual path the the JNI libraries.])])
      LDFLAGS=$LDFLAGS_save

      LD_LIBRARY_PATH_save=$LD_LIBRARY_PATH
      LD_LIBRARY_PATH="/usr/lib/jni:/usr/lib64/jni/:$SCI_SRCDIR/bin/:$SCI_SRCDIR/thirdparty/:$with_hdf5_library"
      export LD_LIBRARY_PATH
      AC_JAVA_CHECK_VERSION_PACKAGE([hdf5],[import ncsa.hdf.hdf5lib.H5;],$JHDF5,[1.8.4],[ver],[int[] vers = new int[3];
                try { H5.H5get_libversion(vers); }
                catch (Throwable ex) {System.exit(-1);}
                String ver = vers[0] + "."+ vers[1] +"."+vers[2];
      ])
      unset LD_LIBRARY_PATH
      LD_LIBRARY_PATH=$LD_LIBRARY_PATH_save
])
