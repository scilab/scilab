##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2008
##########

# Sablotron or xsltproc
# ===================

#AC_CHECK_MAN_GENERATOR([$with_sablotron],[Sablotron],[sabcmd])
#AC_CHECK_MAN_GENERATOR([$with_xsltproc],[xsltproc],[xsltproc])


AC_DEFUN([AC_CHECK_MAN_GENERATOR],[
	with=$1
	progname=$2
	prog=$3

	dnl check for sabcmd
if test "x$with" != 'x' -a "x$with" != "xyes"; then
   dnl path provided in the command line
   AC_MSG_CHECKING([$progname])
   MANBIN="$with/bin/$prog"
        if test -x "$MANBIN"; then
                AC_MSG_RESULT([$MANBIN])
        else
			   dnl Wrong path provided ! 
                AC_MSG_ERROR([Unable to find $PROGNAME. Please check the path you provided])
		fi
else
dnl look for the program in the path
    AC_MSG_CHECKING([$progname])
	AC_CHECK_PROG(WITH,$prog,yes,no)
	if test "x$WITH" = "xyes"; then
		MANGENERATOR=$prog
	else
	if test "x$with" != "x"; then
		dnl The user specified that he wants to use \$prog but we haven't been able to find it
		AC_MSG_ERROR([Unable to find $progname in the path. Please check your installation of $progname or try to use the other XSL processor if available])		   
	fi
fi

fi
unset ac_cv_prog_WITH # Void the caching of WITH
unset WITH

])


