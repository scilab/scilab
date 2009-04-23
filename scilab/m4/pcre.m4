dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) INRIA - 2008 - Sylvestre Ledru
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dnl
dnl PCRE detection
dnl

dnl PCRE is mandatory in Scilab 
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_PCRE], [


AC_ARG_WITH(pcre,
		AC_HELP_STRING([--with-pcre=DIR],[Set the path to your PCRE (Perl 5 Compatible Regular Expression Library) installation]),
		[with_pcre=$withval],
		[with_pcre='yes']
		)

if test "$with_pcre" != 'yes' -a "$with_pcre" != 'no'; then
   # Look if pcre-config (which provides cflags and ldflags) is available
   AC_MSG_CHECKING([pcre, for pcre-config])
   PCRE_FOUND=0
   PCRE_CONFIG="$with_pcre/bin/pcre-config" 
        if test -x "$PCRE_CONFIG"; then
                AC_MSG_RESULT([$PCRE_CONFIG])
				PCRE_FOUND=1
		fi
   if test $PCRE_FOUND -eq 0; then
   PCRE_CONFIG="$with_pcre/pcre-config" 
        if test -x "$PCRE_CONFIG"; then
				PCRE_FOUND=1
                AC_MSG_RESULT([$PCRE_CONFIG])
        fi
   fi
		if test $PCRE_FOUND -eq 0; then
                AC_MSG_ERROR([Unable to find $PCRE_CONFIG. Please check the path you provided])
		else
			unset PCRE_FOUND
		fi
else
		AC_CHECK_PROGS(PCRE_CONFIG,pcre-config,no)
		if test "x$PCRE_CONFIG" = "xno"; then
				AC_MSG_ERROR([Unable to find pcre-config in the path. Please check your installation of pcre (example : libpcre3 & libpcre3-dev with Debian)])
		fi
fi
saved_cflags="$CFLAGS"
saved_LIBS="$LIBS"
		
PCRE_CFLAGS=`$PCRE_CONFIG --cflags`
PCRE_LIBS=`$PCRE_CONFIG --libs-posix`
PCRE_VERSION=`$PCRE_CONFIG --version`

LIBS="$PCRE_LIBS $LIBS"
CFLAGS="$PCRE_CFLAGS $CFLAGS"

AC_CHECK_LIB([pcre], [pcre_compile],
               [],
               [AC_MSG_ERROR([pcre : library missing. (Cannot find symbol pcre_compile). Check if pcre is installed and if the version is correct])]
               )

AC_CHECK_LIB([pcreposix], [regexec],
               [],
               [AC_MSG_ERROR([pcreposix : library missing. (Cannot find symbol regexec). Check if pcre is installed and if the version is correct])]
               )
AC_CHECK_HEADERS([pcre.h])

LIBS="$saved_LIBS"
CFLAGS="$saved_cflags"

AC_SUBST(PCRE_CFLAGS)
AC_SUBST(PCRE_LIBS)
AC_SUBST(PCRE_VERSION)

AC_DEFINE_UNQUOTED([PCRE_CFLAGS],["$PCRE_CFLAGS"],[PCRE flags])
AC_DEFINE_UNQUOTED([PCRE_LIBS],["$PCRE_LIBS"],[PCRE library])
AC_DEFINE_UNQUOTED([PCRE_VERSION],["$PCRE_VERSION"],[PCRE version])

# Gets compilation and library flags
])
