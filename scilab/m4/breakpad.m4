dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) DIGITEO - 2010 - Sylvestre Ledru
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dnl
dnl breakpad
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_BREAKPAD], [


AC_ARG_WITH(breakpad_include,
		AC_HELP_STRING([--with-breakpad-include=DIR],[Set the path to the BREAKPAD headers]),
		[with_breakpad_include=$withval],
		[with_breakpad_include='yes']
		)

AC_ARG_WITH(breakpad_library,
		AC_HELP_STRING([--with-breakpad-library=DIR],[Set the path to the BREAKPAD libraries]),
		[with_breakpad_library=$withval],
		[with_breakpad_library='yes']
		)
AC_LANG([C++]) 		
if test "x$with_breakpad_include" != "xyes"; then
	save_CXXFLAGS="$CXXFLAGS"
	CXXFLAGS="-I$with_breakpad_include"
	BREAKPAD_CXXFLAGS="-I$with_breakpad_include"
    AC_CHECK_HEADER([exception_handler.h],
		[BREAKPAD_CXXFLAGS="$CXXFLAGS"],
		[AC_MSG_ERROR([Cannot find headers (exception_handler.h) of the library breakpad in $with_breakpad_include. Please install the dev package])]
	)
	CXXFLAGS="$save_CXXFLAGS"
else
	BREAKPAD_CXXFLAGS=""
    AC_CHECK_HEADER([exception_handler.h],
		[BREAKPAD_CXXFLAGS="$CXXFLAGS"],
		[AC_MSG_ERROR([Cannot find headers (exception_handler.h) of the library breakpad. Please install the dev package])])
fi

save_LIBS="$LIBS"

# --with-breakpad-library set then check in this dir
if test "x$with_breakpad_library" != "xyes"; then
    BREAKPAD_LIBS="-L$with_breakpad_library -lbreakpad -lpthread"
    LIBS="$LIBS $BREAKPAD_LIBS"
	AC_CHECK_LIB([breakpad], [isLegalUTF8Sequence],
			[],
            [AC_MSG_ERROR([libbreakpad : library missing. (Cannot find symbol isLegalUTF8Sequence) in $with_breakpad_library. Check if libbreakpad is installed and if the version is correct])]
			)
	
else
    BREAKPAD_LIBS=" -lbreakpad -lpthread"
    LIBS="$LIBS $BREAKPAD_LIBS"
    AC_CHECK_LIB([breakpad], [isLegalUTF8Sequence],
			[],
            [AC_MSG_ERROR([libbreakpad : library missing. (Cannot find symbol isLegalUTF8Sequence). Check if libbreakpad is installed and if the version is correct])]
			)
fi

LIBS="$save_LIBS"

AC_SUBST(BREAKPAD_CXXFLAGS)
AC_SUBST(BREAKPAD_LIBS)

AC_DEFINE_UNQUOTED([BREAKPAD_CXXFLAGS],["$BREAKPAD_CXXFLAGS"],[BREAKPAD cflags])
AC_DEFINE_UNQUOTED([BREAKPAD_LIBS],["$BREAKPAD_LIBS"],[BREAKPAD library])

AC_DEFINE([WITH_BREAKPAD], [], [With the BREAKPAD library])

# Gets compilation and library flags
])

