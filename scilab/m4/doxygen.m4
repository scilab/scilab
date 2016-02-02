dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) Scilab Enterprises - 2012 - Clement David
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

