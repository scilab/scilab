dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) Scilab Enterprises - 2014 - Calixte DENIZET
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dnl
dnl AC_LLVM_CONFIG
dnl ------------------------------------------------------
dnl Check if llvm-config is usable and working
dnl
AC_DEFUN([AC_LLVM_CONFIG], [

AC_ARG_WITH([llvm-config],
		AC_HELP_STRING([--with-llvm-config=DIR],[Set the path to llvm-config (or yes to autodetect)]),
		[with_llvm_config=$withval],
		[with_llvm_config="yes"]
		)

if test "${with_llvm_config}" != "yes" -a "${with_llvm_config}" != "no"; then
   	AC_MSG_CHECKING([LLVM, for llvm-config])
   	LLVM_CONFIG_BIN="${with_llvm_config}/llvm-config"
	if test -x "${LLVM_CONFIG_BIN}"; then
	   	AC_MSG_RESULT([$LLVM_CONFIG_BIN])
	else
		AC_MSG_ERROR([Cannot find llvm-config. Please check the path you provided])
	fi
else
	AC_PATH_PROG(LLVM_CONFIG_BIN, llvm-config, [no])
	if test "x${LLVM_CONFIG_BIN}" = "xno"; then
	   	AC_MSG_ERROR([Cannot find llvm-config. Please check your installation of LLVM])
	fi
fi

LLVM_VERSION="`${LLVM_CONFIG_BIN} --version`"
LLVM_MAJOR="`echo  ${LLVM_VERSION} | sed 's/\([[0-9]]*\).*/\1/'`"
LLVM_MINOR="`echo  ${LLVM_VERSION} | sed 's/\([[0-9]]*\).\([[0-9]]*\).*/\2/'`"
LLVM_SUBMINOR="`echo  ${LLVM_VERSION} | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\).*/\3/'`"

CHK_LLVM_MAJOR=3
CHK_LLVM_MINOR=7

AC_MSG_CHECKING([if LLVM is version $CHK_LLVM_MAJOR.$CHK_LLVM_MINOR or later])

if test "${LLVM_MAJOR}" -le "${CHK_LLVM_MAJOR}" -a "${LLVM_MINOR}" -lt "${CHK_LLVM_MINOR}" -o "${LLVM_MAJOR}" != "${CHK_LLVM_MAJOR}"; then
   	AC_MSG_ERROR([Version $CHK_LLVM_MAJOR.$CHK_LLVM_MINOR of LLVM expected (at least)])
else
	AC_MSG_RESULT([yes])
fi

LLVM_INCLUDE="-I`${LLVM_CONFIG_BIN} --includedir`"
LLVM_LDFLAGS="-L`${LLVM_CONFIG_BIN} --libdir`"
LLVM_LIBS="-lLLVM-${LLVM_MAJOR}.${LLVM_MINOR}"
LLVM_LIBS_STATIC="`${LLVM_CONFIG_BIN} --libfiles core mcjit native`"
LLVM_CPPFLAGS="`${LLVM_CONFIG_BIN} --cppflags | sed 's/-I[[^ ]]*[[ ]]*//g' | sed 's/-DNDEBUG//g'`"



AC_SUBST(LLVM_VERSION)
AC_SUBST(LLVM_CONFIG_BIN)
AC_SUBST(LLVM_INCLUDE)
AC_SUBST(LLVM_LDFLAGS)
AC_SUBST(LLVM_LIBS)
AC_SUBST(LLVM_LIBS_STATIC)
AC_SUBST(LLVM_CPPFLAGS)

AC_DEFINE([WITH_LLVM_CONFIG], [], [With llvm-config])
])
