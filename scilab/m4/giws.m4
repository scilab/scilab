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
dnl AC_GIWS([required-version = N[.N[.N]]])

AC_DEFUN([AC_GIWS], [
# Checking Python in the current path... As Giws is not at all mandatory
# Python is not mandatory and we don't want to provide an additionnal option
# to the ./configure
   AC_CHECK_PROGS(PYTHON,python,no)
   if test "x$PYTHON" = "xno"; then
		AC_MSG_ERROR([Giws needs Python])
   fi

## If the user is providing a path to the option
if test "$enable_build_giws" != 'yes' -a "$enable_build_giws" != 'no'; then

   AC_MSG_CHECKING([giws])
   GIWS_BIN="$enable_build_giws/giws" 
   if test -f "$GIWS_BIN"; then
                AC_MSG_RESULT([$GIWS_BIN])
        else
                AC_MSG_ERROR([Unable to find $GIWS_BIN. Please check the path you provided])
		fi
else
# Looks for it in the path
   AC_CHECK_PROGS(GIWS_BIN,giws,no)
   if test "x$GIWS_BIN" = "xno"; then
		AC_MSG_ERROR([Could find giws in the PATH])
   fi
fi

if test -f $GIWS_BIN; then
   chmod +x $GIWS_BIN
fi

AC_MSG_CHECKING([for GIWS version])
[giws_version=`$GIWS_BIN --version 2>&1 | grep 'GIWS' | sed 's/.*\([0-9]\+\.[0-9]\+\.[0-9]\+\).*/\1/g'`]
AC_MSG_RESULT([$giws_version])
if test -n "$giws_version" ; then
	# Calculate the required version number
    [giws_tmp=`echo $1 | sed 's/[^0-9]\+/ /g'`]
    [giws_required_version=`echo $giws_tmp | awk '{print  1000000 * $ 1 +  1000 * $ 2 + $ 3}'`]

	# Calculate the available version number
    [giws_tmp=`echo $giws_version | sed 's/[^0-9]\+/ /g'`]
    [giws_tmp=`echo $giws_tmp | awk '{print  1000000 * $ 1 +  1000 * $ 2 + $ 3}'`]

	if test $giws_required_version -gt $giws_tmp ; then
       AC_MSG_ERROR([GIWS version $1 is required, $giws_version detected])
    fi
else
	AC_MSG_ERROR([cannot determine GIWS version])
fi

AC_SUBST(GIWS_BIN)

])

