dnl
dnl Sylvestre Ledru <sylvestre.ledru@inria.fr>
dnl INRIA - Scilab 2007
dnl

AC_DEFUN([AC_GIWS], [
# Checking Python in the current path... As Giws is not at all mandatory
# Python is not mandatory and we don't want to provide an additionnal option
# to the ./configure
   AC_CHECK_PROGS(PYTHON,python,no)
   if test "x$PYTHON" = "xno"; then
		AC_MSG_ERROR([Giws needs Python])
   fi

## If the user is providing a path to the option
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
chmod +x $GIWS_BIN
AC_SUBST(GIWS_BIN)

])