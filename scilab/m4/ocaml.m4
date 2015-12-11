dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2006/2008 - Sylvestre Ledru
dnl
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at
dnl http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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