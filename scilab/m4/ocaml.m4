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
	OCAMLC=
	OCAMLOPT=
	OCAMLDEP=
	AC_CHECK_PROG(OCAMLC,ocamlc,ocamlc,no)
	if test "$OCAMLC" = no; then
		AC_MSG_ERROR([ocamlc not found. Mandatory to build the Scicos modelica compiler (Use --without-modelica to disable the Modelica compiler).])
	fi
	AC_CHECK_PROG(OCAMLOPT,ocamlopt,ocamlopt,no)
	if test "$OCAMLOPT" = no; then
		AC_MSG_ERROR([ocamlopt not found. Mandatory to build the Scicos modelica compiler.])
	fi
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
	AC_SUBST(OCAMLOPT)
	AC_SUBST(OCAMLDEP)
	AC_SUBST(OCAMLYACC)
	AC_SUBST(OCAMLLEX)
	AC_SUBST(WITH_OCAML)
]
)