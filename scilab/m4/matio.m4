# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
# 
# This file is distributed under the same license as the Scilab package.

dnl AC_MATIO
dnl ------------------------------------------------------
dnl Check if the library MATIO (access to matlab files) is usable and working
dnl
AC_DEFUN([AC_MATIO], [

AC_ARG_WITH(matio_include,
		AC_HELP_STRING([--with-matio-include=DIR],[Set the path to the MATIO headers]),
		[with_matio_include=$withval],
		[with_matio_include='yes']
		)

AC_ARG_WITH(matio_library,
		AC_HELP_STRING([--with-matio-library=DIR],[Set the path to the MATIO libraries]),
		[with_matio_library=$withval],
		[with_matio_library='yes']
		)

# Check of zlib
save_LIBS="$LIBS"
AC_CHECK_LIB([z], [inflateCopy],
		[ZLIB="-lz"],
        [AC_MSG_ERROR([zlib: library missing (Cannot find symbol inflateCopy). Check if zlib is installed and if the version is correct])]
	)
LIBS="$save_LIBS"


# Mat I/O
if test "x$with_matio_include" != "xyes"; then
	save_CFLAGS="$CFLAGS"
	CFLAGS="-I$with_matio_include"
	AC_CHECK_HEADERS([matio.h matioConfig.h],
		[MATIO_CFLAGS="$CFLAGS"],
		[AC_MSG_ERROR([Cannot find headers (matio.h) of the library Matio in $with_matio_include. Please install the dev package (Debian : libmatio-dev)])]
	)
	CFLAGS="$save_CFLAGS"
else
	AC_CHECK_HEADERS([matio.h matioConfig.h],
		[MATIO_CFLAGS=""],
		[AC_MSG_ERROR([Cannot find headers (matio.h) of the library matio. Please install the dev package (Debian : libmatio-dev)])])
fi


# --with-matio-library set then check in this dir
if test "x$with_matio_library" != "xyes"; then
	save_LIBS="$LIBS"
	# -lm to allow static linking (floor, which is needed by matio, is in libm)
	LIBS="-L$with_matio_library -lm -lmatio $ZLIB"
	AC_CHECK_LIB([matio], [Mat_Open],
			[MATIO_LIB="-L$with_matio_library -lm -lmatio $ZLIB"],
            [AC_MSG_ERROR([libmatio : library missing. (Cannot find symbol Mat_Open) in $with_matio_library. Check if libmatio is installed and if the version is correct])]
			)
	LIBS="$save_LIBS"
else
	save_LIBS="$LIBS"
	LIBS="$ZLIB -lm"
	AC_CHECK_LIB([matio], [Mat_Open],
			[MATIO_LIB="-lmatio  -lm $ZLIB"],
            [AC_MSG_ERROR([libmatio : library missing. (Cannot Mat_Open). Check if libmatio is installed and if the version is correct])]
			)
	LIBS="$save_LIBS"
fi
AC_SUBST(MATIO_LIB)
AC_SUBST(MATIO_CFLAGS)
AC_DEFINE([WITH_MATIO], [], [With the MATIO library])

])
