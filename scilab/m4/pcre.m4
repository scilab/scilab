dnl
dnl Sylvestre Ledru <sylvestre.ledru@inria.fr>
dnl INRIA - Scilab 2007
dnl

dnl pcre is now mandatory in Scilab 
dnl When we check :
dnl * if the path is provided or that we have to find it ourself
dnl * if it is available
dnl * what are the compilation flags 
dnl * what are linking flags
AC_DEFUN([AC_PCRE], [

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
saved_cflags=$CFLAGS
		
PCRE_CFLAGS=`$PCRE_CONFIG --cflags`
PCRE_LIBS=`$PCRE_CONFIG --libs-posix`

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

CFLAGS=$saved_cflags
AC_SUBST(PCRE_FLAGS)
AC_SUBST(PCRE_LIBS)

#CFLAGS="$CFLAGS $PCRE_FLAGS"
#AC_CHECK_LIB(xml2,xmlInitParserCtxt,,[AC_MSG_ERROR([pcre : library missing])])


#AC_CHECK_HEADERS([pcre/tree.h],,[AC_MSG_ERROR([pcre : library missing missing])])	

# Gets compilation and library flags
])
