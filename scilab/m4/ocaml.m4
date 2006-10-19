dnl Macros which process ./configure arguments
dnl Ocam Macros
dnl Copyright INRIA
dnl Sylvestre Ledru - June 2006

dnl Check if Ocaml is available on the system
AC_DEFUN([AC_CHECK_PROG_OCAML],[
# checking for ocamlc
	OCAMLC=
	OCAMLOPT=
	OCAMLDEP=
	AC_CHECK_PROG(OCAMLC,ocamlc,ocamlc,no)
	if test "$OCAMLC" = no; then
		AC_MSG_WARN([ocamlc not found: I will not build Modelica compiler])
	else

		AC_CHECK_PROG(OCAMLOPT,ocamlopt,ocamlopt,no)
		if test "$OCAMLOPT" = no; then
			AC_MSG_WARN([ocamlopt not found: I will not build Modelica compiler])
		else
			AC_CHECK_PROG(OCAMLDEP,ocamldep,ocamldep,no)
			if test "$OCAMLDEP" = no; then
				AC_MSG_WARN([ocamldep not found: I will not build Modelica compiler])
			else
				AC_DEFINE([WITH_OCAML],[],[With OCAML])
				OCAMLC=ocamlc
				OCAMLOPT=ocamlopt
				OCAMLDEP=ocamldep
			fi
		fi
	fi
	AC_SUBST(OCAMLC)
	AC_SUBST(OCAMLOPT)
	AC_SUBST(OCAMLDEP)
	AC_SUBST(WITH_OCAML)
]
)