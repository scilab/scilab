dnl Macros which process ./configure arguments
dnl Ocaml Macros
dnl Copyright INRIA
dnl Sylvestre Ledru - 2006/2008

dnl Check if Ocaml is available on the system
dnl @TODO add the possibility to specific a path
AC_DEFUN([AC_CHECK_PROG_OCAML],[
# checking for ocamlc
	OCAMLC=
	OCAMLOPT=
	OCAMLDEP=
	AC_CHECK_PROG(OCAMLC,ocamlc,ocamlc,no)
	if test "$OCAMLC" = no; then
		AC_MSG_ERROR([ocamlc not found])
	else

		AC_CHECK_PROG(OCAMLOPT,ocamlopt,ocamlopt,no)
		if test "$OCAMLOPT" = no; then
			AC_MSG_ERROR([ocamlopt not found])
		else
			AC_CHECK_PROG(OCAMLDEP,ocamldep,ocamldep,no)
			if test "$OCAMLDEP" = no; then
				AC_MSG_ERROR([ocamldep not found])
			else
				AC_CHECK_PROG(OCAMLYACC,ocamlyacc,ocamlyacc,no)
				if test "$OCAMLYACC" = no; then
					AC_MSG_ERROR([ocamlyacc not found])
				else
					AC_DEFINE([WITH_OCAML],[],[With OCAML])
					OCAMLC=ocamlc
					OCAMLOPT=ocamlopt
					OCAMLDEP=ocamldep
					OCAMLDEP=ocamlyacc
				fi
			fi
		fi
	fi
	AC_SUBST(OCAMLC)
	AC_SUBST(OCAMLOPT)
	AC_SUBST(OCAMLDEP)
	AC_SUBST(OCAMLYACC)
	AC_SUBST(WITH_OCAML)
]
)