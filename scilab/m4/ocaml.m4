dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2006/2008 - Sylvestre Ledru
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
dnl Macros which process ./configure arguments
dnl Ocaml Macros

dnl Check if Ocaml is available on the system
dnl @TODO add the possibility to specific a path
AC_DEFUN([AC_CHECK_PROG_OCAML],[
# checking for ocamlc
	
	AC_CHECK_PROG(OCAMLC,ocamlc,ocamlc,no)
	if test "$OCAMLC" = no; then
		AC_MSG_ERROR([ocamlc not found. Mandatory to build the Scicos modelica compiler (Use --without-modelica to disable the Modelica compiler).])
	fi
	# since ocaml 4.06, unsafe string is no more the default parameter ; enforce it
        "$OCAMLC" -unsafe-string
	if test "$?" -ne "0"; then
		AC_MSG_WARN([ocamlc does not support "-unsafe-string". Needed to build the Scicos modelica compiler (Use --without-modelica to disable the Modelica compiler).])
	fi
        OCAMLCFLAGS="$OCAMLCFLAGS -unsafe-string"
	AC_CHECK_PROG(OCAMLOPT,ocamlopt,ocamlopt,no)
	if test "$OCAMLOPT" = no; then
		AC_MSG_ERROR([ocamlopt not found. Mandatory to build the Scicos modelica compiler.])
	fi
	# since ocaml 4.06, unsafe string is no more the default parameter ; enforce it
        "$OCAMLOPT" -unsafe-string
	if test "$?" -ne "0"; then
		AC_MSG_WARN([ocamlc does not support "-unsafe-string". Needed to build the Scicos modelica compiler (Use --without-modelica to disable the Modelica compiler).])
	fi
        OCAMLOPTFLAGS="$OCAMLOPTFLAGS -unsafe-string"
	AC_CHECK_PROG(OCAMLDEP,ocamldep,ocamldep,no)
	if test "$OCAMLDEP" = no; then
		AC_MSG_ERROR([ocamldep not found. Mandatory to build the Scicos modelica compiler.])
	fi
	AC_CHECK_PROG(OCAMLYACC,ocamlyacc,ocamlyacc,no)
	if test "$OCAMLYACC" = no; then
		AC_MSG_ERROR([ocamlyacc not found. Mandatory to build the Scicos modelica compiler.])
	fi
	AC_CHECK_PROG(OCAMLLEX,ocamllex,ocamllex,no)
	if test "$OCAMLLEX" = no; then
		AC_MSG_ERROR([ocamllex not found. Mandatory to build the Scicos modelica compiler.])
	fi
	AC_DEFINE([WITH_OCAML],[],[With OCAML])

	AC_SUBST(OCAMLC)
	AC_SUBST(OCAMLCFLAGS)
	AC_SUBST(OCAMLOPT)
	AC_SUBST(OCAMLOPTFLAGS)
	AC_SUBST(OCAMLDEP)
	AC_SUBST(OCAMLYACC)
	AC_SUBST(OCAMLLEX)
	AC_SUBST(WITH_OCAML)
]
)
