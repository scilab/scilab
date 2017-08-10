dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) DIGITEO - 2009 - Bruno JOFRET
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
    if $WITH_DEVTOOLS; then # Scilab thirdparties
        HDF5_CFLAGS="-I$DEVTOOLS_INCDIR"
    else
        if test -d /usr/include/hdf5/serial; then # New Debian packaging layout since hdf5-1.8.13
            AC_CHECK_HEADER([hdf5/serial/hdf5.h],
                [HDF5_CFLAGS="-I/usr/include/hdf5/serial"],
                [AC_MSG_ERROR([Cannot find headers (hdf5.h) of the library HDF5. Please install the dev package])])
        else
            AC_CHECK_HEADER([hdf5.h],
                [HDF5_CFLAGS=""],
                [AC_MSG_ERROR([Cannot find headers (hdf5.h) of the library HDF5. Please install the dev package])])
        fi
    fi
fi

save_LIBS="$LIBS"

# --with-hdf5-library set then check in this dir
if test "x$with_hdf5_library" != "xyes"; then
    HDF5_LIBS="-L$with_hdf5_library -lhdf5 -lhdf5_hl"
    LIBS="$LIBS $HDF5_LIBS"
    AC_CHECK_LIB([hdf5], [H5Fopen],
            [],
            [AC_MSG_ERROR([libhdf5 or libhdf5_hl: library missing. (Cannot find symbol H5Fopen) in $with_hdf5_library. Check if libhdf5 is installed and if the version is correct])],
            [-lz]
            )
else
    if $WITH_DEVTOOLS; then # Scilab thirparties
        HDF5_LIBS="-L$DEVTOOLS_LIBDIR -lhdf5 -lhdf5_hl"
    else
        if test -d /usr/include/hdf5/serial; then # New Debian packaging layout since hdf5-1.8.13
            HDF5_LIBS="-lhdf5_serial -lhdf5_serial_hl"
            LIBS="$LIBS $HDF5_LIBS"
            AC_CHECK_LIB([hdf5_serial], [H5Fopen],
                [],
                [AC_MSG_ERROR([libhdf5_serial or libhdf5_serial_hl: library missing. (Cannot find symbol H5Fopen). Check if libhdf5 is installed and if the version is correct])],
                [-lz]
                )
        else
            HDF5_LIBS="-lhdf5 -lhdf5_hl"
            LIBS="$LIBS $HDF5_LIBS"
            AC_CHECK_LIB([hdf5], [H5Fopen],
                [],
                [AC_MSG_ERROR([libhdf5 or libhdf5_hl: library missing. (Cannot find symbol H5Fopen). Check if libhdf5 is installed and if the version is correct])],
                [-lz]
                )
        fi
    fi
fi

LIBS="$save_LIBS"

AC_SUBST(HDF5_LIBS)
AC_SUBST(HDF5_CFLAGS)

AC_DEFINE([WITH_HDF5], [], [With the HDF5 library])

# Gets compilation and library flags
])
