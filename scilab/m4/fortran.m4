dnl Macros which process ./configure arguments


dnl Fortran Macros
dnl ------------------------------------------------------
dnl Copyright INRIA
dnl Sylvestre Ledru - June 2006
dnl 
dnl ------------------------------------------------------
dnl AC_CHECK_UNDERSCORE_FORTRAN
dnl Look for trailing or leading underscores
dnl 
AC_DEFUN([AC_CHECK_UNDERSCORE_FORTRAN],[
AC_MSG_CHECKING([for leading underscores with Fortran])
cat << EOF > pipof.f
       subroutine pipof
       end
EOF

dnl expand possible $SCIDIR in $FC (wizard command...)
eval "$F77 -c pipof.f > /dev/null 2>&1"

FC_LEADING_UNDERSCORE=no
FC_TRAILING_UNDERSCORE=no

output=`nm $NMOPT pipof.o|grep _pipof 2>&1`
if test ! -z "$output"; then
  FC_LEADING_UNDERSCORE=yes
  FC_TRAILING_UNDERSCORE=no
fi

output=`nm $NMOPT pipof.o|grep pipof_ 2>&1`
if test ! -z "$output"; then
  FC_LEADING_UNDERSCORE=no
  FC_TRAILING_UNDERSCORE=yes
fi

output=`nm $NMOPT pipof.o|grep _pipof_ 2>&1`
if test ! -z "$output"; then
  FC_LEADING_UNDERSCORE=yes
  FC_TRAILING_UNDERSCORE=yes
fi

if test "$FC_LEADING_UNDERSCORE" = yes; then
  AC_DEFINE([WLU],,[If leading underscores])
fi
if test "$FC_TRAILING_UNDERSCORE" = yes; then
  AC_DEFINE([WTU],,[If trailing underscores])
fi

$RM pipof.f pipof.o

AC_MSG_RESULT([$FC_LEADING_UNDERSCORE])
AC_MSG_CHECKING([for trailing underscores with Fortran])
AC_MSG_RESULT([$FC_TRAILING_UNDERSCORE])

#####################
## test for sharpsign
#####################

AC_MSG_CHECKING([use of the sharpsign in CPP])

AC_COMPILE_IFELSE(
	[ 
		AC_LANG_PROGRAM(
			[[#define C2F(name) name##_]], 
			[[C2F(toto)()]]
		)
	],
	[AC_MSG_RESULT(yes)
	AC_DEFINE([CNAME(name1,name2)], [name1##name2],[Cname])
	USE_SHARP_SIGN=yes]
	,
	[AC_MSG_RESULT(no)
	AC_DEFINE([CNAME(name1,name2)], [name1/**/name2],[Cname])
	USE_SHARP_SIGN=no]
)

## Define  C2F and F2C entry point conversion ##
if test "$FC_TRAILING_UNDERSCORE" = yes; then
	if test "$USE_SHARP_SIGN" = yes; then
	echo "Define C2F with Trailing Underscore and Sharp Sign"
		AC_DEFINE([C2F(name)], [name##_],[Define C2F with Trailing Underscore and Sharp Sign])
		AC_DEFINE([F2C(name)], [name##_],[Define F2C with Trailing Underscore and Sharp Sign])
	else
	echo "Define C2F with Trailing Underscore and without Sharp Sign"
		AC_DEFINE([C2F(name)], [name/**/_],[Define C2F with Trailing Underscore and without Sharp Sign])
		AC_DEFINE([F2C(name)], [name/**/_],[Define F2C with Trailing Underscore and without Sharp Sign])
	fi
else
		echo "Define C2F without Trailing Underscore"
	AC_DEFINE([C2F(name)], [name],[Define C2F without Trailing Underscore])
	AC_DEFINE([F2C(name)], [name],[Define C2F without Trailing Underscore])
fi

])dnl AC_CHECK_UNDERSCORE_FORTRAN

