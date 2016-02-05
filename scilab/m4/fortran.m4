dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2006 - Sylvestre Ledru
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

dnl Fortran Macros

dnl ------------------------------------------------------
dnl AC_CHECK_UNDERSCORE_FORTRAN
dnl Look for trailing or leading underscores
dnl 
AC_DEFUN([AC_CHECK_UNDERSCORE_FORTRAN],[

AC_CHECK_PROGS(NM,nm,no)
if test "x$NM" = "xno"; then
	AC_MSG_ERROR([Unable to find nm in the path. nm is used to list all the symbol from a lib])
fi
AC_MSG_CHECKING([for leading underscores with Fortran (name-mangling scheme)])

cat << EOF > pipof.f
       subroutine pipof
       end
EOF

dnl expand possible $SCIDIR in $FC (wizard command...)
eval "$F77 -c pipof.f > /dev/null 2>&1"

FC_LEADING_UNDERSCORE=no
FC_TRAILING_UNDERSCORE=no

output=`$NM $NMOPT pipof.o|grep _pipof 2>&1`
if test ! -z "$output"; then
  FC_LEADING_UNDERSCORE=yes
  FC_TRAILING_UNDERSCORE=no
fi

output=`$NM $NMOPT pipof.o|grep pipof_ 2>&1`
if test ! -z "$output"; then
  FC_LEADING_UNDERSCORE=no
  FC_TRAILING_UNDERSCORE=yes
fi

output=`$NM $NMOPT pipof.o|grep _pipof_ 2>&1`
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

rm -f pipof.f pipof.o

AC_MSG_RESULT([$FC_LEADING_UNDERSCORE])
AC_MSG_CHECKING([for trailing underscores with Fortran (name-mangling scheme)])
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
	AC_MSG_RESULT([Define C2F with Trailing Underscore and Sharp Sign])
		AC_DEFINE([C2F(name)], [name##_],[Define C2F with Trailing Underscore and Sharp Sign])
		AC_DEFINE([F2C(name)], [name##_],[Define F2C with Trailing Underscore and Sharp Sign])
	else
	AC_MSG_RESULT([Define C2F with Trailing Underscore and without Sharp Sign])
		AC_DEFINE([C2F(name)], [name/**/_],[Define C2F with Trailing Underscore and without Sharp Sign])
		AC_DEFINE([F2C(name)], [name/**/_],[Define F2C with Trailing Underscore and without Sharp Sign])
	fi
else
		AC_MSG_RESULT([Define C2F without Trailing Underscore])
	AC_DEFINE([C2F(name)], [name],[Define C2F without Trailing Underscore])
	AC_DEFINE([F2C(name)], [name],[Define C2F without Trailing Underscore])
fi

])dnl AC_CHECK_UNDERSCORE_FORTRAN

