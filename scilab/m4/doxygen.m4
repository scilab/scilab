dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) Scilab Enterprises - 2012 - Clement David
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dnl
dnl Doxugen detection
dnl
dnl ------------------------------------------------------
dnl Check if doxygen is usable and working
dnl
AC_DEFUN([AC_DOXYGEN], [
    ## If the user is providing a path to the option
    if test "$enable_build_doxygen" != 'yes' -a "$enable_build_doxygen" != 'no'; then
        AC_MSG_CHECKING([doxygen])
        DOXYGEN_BIN="$enable_build_doxygen/doxygen" 
        if test -f "$DOXYGEN_BIN"; then
            AC_MSG_RESULT([$DOXYGEN_BIN])
        else
            AC_MSG_ERROR([Unable to find $DOXYGEN_BIN. Please check the path you provided])
        fi
    else
        # Looks for it in the path
        AC_CHECK_PROGS(DOXYGEN_BIN,doxygen,no)
        if test "x$DOXYGEN_BIN" = "xno"; then
            AC_MSG_ERROR([Could find doxygen in the PATH])
        fi
    fi
	AC_SUBST([DOXYGEN_BIN])
])

