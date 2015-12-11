dnl Because this macro is used by AC_PROG_GCC_TRADITIONAL, which must
dnl come early, it is not included in AC_BEFORE checks.
dnl AC_GREP_CPP(PATTERN, PROGRAM, [ACTION-IF-FOUND [,
dnl              ACTION-IF-NOT-FOUND]])
AC_DEFUN([AC_GREP_CPP],
[AC_REQUIRE_CPP()dnl
cat > conftest.$ac_ext <<EOF
[#]line __oline__ "configure"
#include "confdefs.h"
[$2]
EOF
dnl eval is necessary to expand ac_cpp.
dnl Ultrix and Pyramid sh refuse to redirect output of eval, so use subshell.
if (eval "$ac_cpp conftest.$ac_ext") 2>&AC_FD_CC |
dnl Prevent m4 from eating character classes:
changequote(, )dnl
  grep "$1" >/dev/null 2>&1; then
changequote([, ])dnl
  ifelse([$3], , :, [rm -rf conftest*
  $3])
ifelse([$4], , , [else
  rm -rf conftest*
  $4
])dnl
fi
rm -f conftest*
])


AC_DEFUN([AC_CHECK_TCL_VERSION], [
dnl INPUTS :
dnl  $1 : Path where to find the include file (/include f. ex.)
dnl  $2 : Major version number ( 8 f. ex)
dnl  $3 : Minor version number (0 f. ex.)
dnl  $4 : include file name (tcl.h f. ex.)
dnl
dnl OUTPUTS
dnl  TCL_VERSION_OK : 1 if OK, 0 otherwise
dnl  TCL_INC_PATH : include path flag for tcl.h (-I/usr/include f. ex.)
dnl  TCL_LIB : tcl lib name ( tcl8.1 f. ex.)
dnl  TCL_VERSION : ( 8.1 f. ex.)
dnl  TCL_MAJOR_VERSION: 
dnl  TCL_MINOR_VERSION: 
dnl  Check the version of tcl associated to header file tcl.h 

CHK_TCL_INCLUDE_PATH=$1
CHK_TCL_MAJOR=$2
CHK_TCL_MINOR=$3
CHK_TCL_INC_NAME=$4
saved_cflags="$CFLAGS"
saved_cppflags="$CPPFLAGS"
CFLAGS="$CFLAGS -I$CHK_TCL_INCLUDE_PATH"
CPPFLAGS="$CPPFLAGS -I$CHK_TCL_INCLUDE_PATH"

AC_MSG_CHECKING([if tcl is version $CHK_TCL_MAJOR.$CHK_TCL_MINOR or later])
AC_GREP_CPP(TCL_VERSION_OK,
[
#include "$CHK_TCL_INCLUDE_PATH/$CHK_TCL_INC_NAME"
#if (TCL_MAJOR_VERSION > $CHK_TCL_MAJOR)
TCL_VERSION_OK
#else
#if ((TCL_MAJOR_VERSION == $CHK_TCL_MAJOR) && (TCL_MINOR_VERSION >= $CHK_TCL_MINOR))
TCL_VERSION_OK
#endif
#endif
],\
TCL_VERSION_OK=1,\
TCL_VERSION_OK=0 )

AC_RUN_IFELSE(
    [AC_LANG_SOURCE([dnl
#include "confdefs.h"
#include <stdio.h>
#include <$CHK_TCL_INC_NAME>
int main(void) {
        FILE *maj = fopen("tclmajor","w");
        FILE *min = fopen("tclminor","w");
        FILE *serial = fopen("tclserial","w");
        fprintf(maj,"%d",TCL_MAJOR_VERSION);
        fprintf(min,"%d",TCL_MINOR_VERSION);
        fprintf(serial,"%s",TCL_PATCH_LEVEL);
        fclose(maj);
        fclose(min);
        fclose(serial);
        return 0;
}
])],
       [
  TCL_MAJOR_VERSION=`cat tclmajor`
  TCL_MINOR_VERSION=`cat tclminor`
  TCL_SERIAL_VERSION=`cat tclserial`
  TCL_VERSION=$TCL_MAJOR_VERSION.$TCL_MINOR_VERSION
  rm -f tclmajor tclminor tclserial
],
[AC_MSG_FAILURE([The TCL detection of the version failed. 
If you are using Intel Compiler, check if Intel Library (ex : libimf.so) is available (in the LD_LIBRARY_PATH for example)])])

case $host_os in
  darwin* | rhapsody*) 
# already added to CFLAGS 
  ;;
  *)
 TCL_INC_PATH=-I$i 
  ;;
esac

TCL_LIB=tcl$TCL_VERSION
if test $TCL_VERSION_OK = 1; then 
	AC_MSG_RESULT([($TCL_VERSION) yes])
else 
	AC_MSG_RESULT([($TCL_VERSION) no])
	if test "$TCL_VERSION" == "can't happen"; then
		AC_MSG_ERROR([can't happen])
        else 
	   AC_MSG_ERROR([You need at least version $CHK_TCL_MAJOR.$CHK_TCL_MINOR of tcl])
	fi
fi
CFLAGS=$saved_cflags
CPPFLAGS=$saved_cppflags
]) dnl End of AC_CHECK_TCL_VERSION


AC_DEFUN([AC_CHECK_TCL_LIB], [
dnl INPUTS :
dnl  $1 : major tcl version number
dnl  $2 : minor tcl version number
dnl OUPUTS :
dnl  TCL_LIB_OK : 1 if link is OK; 0 otherwise
dnl  TCL_LIB : flag to link against tcl lib

CHK_TCL_MAJ=$1
CHK_TCL_MIN=$2
TCL_LIB_OK=0
  AC_MSG_CHECKING([for tcl library tcl$1.$2])
MULTIARCH_HOST=`gcc -v 2>&1|grep Target|cut -d" " -f2`
dirs="$USER_TCL_LIB_PATH $DEVTOOLS_LIBDIR /lib64 /usr/lib64 /lib /usr/lib /usr/lib/tcl /usr/lib/tcl8.* /shlib /shlib/tcl /shlib/tcl8.* /usr/shlib /shlib/tcl /usr//shlib/tcl8.* /usr/local/lib /usr/local/lib/tcl /usr/local/lib/tcl8.* /usr/local/shlib /usr/X11/lib/tcl /usr/X11/lib/tcl8.* /usr/lib/X11 /usr/lib/X11/tcl /usr/lib/X11/tcl8.* ../lib ../../lib  /usr/local/tcl /usr/tcl /usr/tcl/lib /usr/local/tcl/lib /sw/lib/ /usr/lib/$MULTIARCH_HOST ."
libexts="so so.1.0 sl dylib a"
libnames="tcl$CHK_TCL_MAJ.$CHK_TCL_MIN tcl.$CHK_TCL_MAJ.$CHK_TCL_MIN tcl$CHK_TCL_MAJ$CHK_TCL_MIN tcl.$CHK_TCL_MAJ$CHK_TCL_MIN"
for e in $libexts; do
	for j in $dirs; do
		for n in $libnames; do
			m="$j/lib$n.$e"
			if test -r $m; then
				PATH_LIB_TCL=$j
				EXT_LIB_TCL=$e
				NAME_LIB_TCL=$n
				AC_MSG_RESULT([found $m using -L$PATH_LIB_TCL -l$NAME_LIB_TCL])
				saved_cflags="$CFLAGS"
				saved_ldflags="$LDFLAGS"
				saved_cppflags="$CPPFLAGS"
				CFLAGS="$CFLAGS $TCL_INC_PATH"
				CPPFLAGS="$CPPFLAGS $TCL_INC_PATH"
				LDFLAGS=$LDFLAGS
				TCL_LDFLAGS="$X_LIBS $X_EXTRA_LIBS $TCLTK_LIBS"
				TCL_LIB_OK=1
				# Check for Tcl lib
				if test "$USER_TCL_LIB_PATH" = ""
				then 
					LDFLAGS="$LDFLAGS -L$PATH_LIB_TCL"
					AC_CHECK_LIB([$NAME_LIB_TCL], Tcl_DoOneEvent, TCL_LIB_OK=1,TCL_LIB_OK=0,
						  [-l$NAME_LIB_TCL $TCL_LDFLAGS])
				else 
					LDFLAGS="$LDFLAGS -L$USER_TCL_LIB_PATH"
					AC_CHECK_LIB([$NAME_LIB_TCL], Tcl_DoOneEvent, TCL_LIB_OK=1,TCL_LIB_OK=0,
						  [-l$NAME_LIB_TCL $TCL_LDFLAGS])
				fi
				CFLAGS="$saved_cflags"
				CPPFLAGS="$saved_cppflags"
				LDFLAGS="$saved_ldflags"
				if test $TCL_LIB_OK = 1; then 
					if test "$USER_TCL_LIB_PATH" = ""
					then TCL_LIB=" -L$PATH_LIB_TCL -l$NAME_LIB_TCL"
					else TCL_LIB=" -L$USER_TCL_LIB_PATH  -l$NAME_LIB_TCL"
					fi
					break 3;
				fi
			fi
		dnl end of name finding loop
		done
	dnl end of dir finding loop
	done 
dnl end of extension finding loop
done
])



AC_DEFUN([AC_CHECK_TK_VERSION], [
dnl INPUTS :
dnl  $1 : Path where to find the include file (/include f. ex.)
dnl  $2 : Major version number ( 8 f. ex)
dnl  $3 : Minor version number (0 f. ex.)
dnl  $4 : include file name (tk.h f. ex.)
dnl  ** WARNING : uses TCL_INC_PATH. it must be set correctly **
dnl
dnl OUTPUTS
dnl  TK_VERSION_OK : 1 if OK, 0 otherwise
dnl  TK_INC_PATH : include path flag for tcl.h (-I/usr/include f. ex.)
dnl  TK_LIB : tcl lib name ( tk8.1 f. ex.)
dnl  TK_VERSION : ( 8.1 f. ex.)
dnl  TK_MAJOR_VERSION: 
dnl  TK_MINOR_VERSION: 

CHK_TK_INCLUDE_PATH=$1
CHK_TK_MAJOR=$2
CHK_TK_MINOR=$3
CHK_TK_INC_NAME=$4
saved_cflags="$CFLAGS"
saved_cppflags="$CPPFLAGS"
CFLAGS="$CFLAGS $TCL_INC_PATH -I$CHK_TK_INCLUDE_PATH $X_CFLAGS"
CPPFLAGS="$CPPFLAGS $TCL_INC_PATH -I$CHK_TK_INCLUDE_PATH $X_CFLAGS"
AC_MSG_CHECKING([if tk is version $CHK_TK_MAJOR.$CHK_TK_MINOR or later])

AC_GREP_CPP(TK_VERSION_OK,
[
#include "$CHK_TK_INCLUDE_PATH/$CHK_TK_INC_NAME"
#if (TK_MAJOR_VERSION > $CHK_TK_MAJOR)
TK_VERSION_OK
#else
#if ((TK_MAJOR_VERSION == $CHK_TK_MAJOR) && (TK_MINOR_VERSION >= $CHK_TK_MINOR))
TK_VERSION_OK
#endif
#endif
],\
TK_VERSION_OK=1,\
TK_VERSION_OK=0 )


cat > conftest.$ac_ext <<EOF
#include "confdefs.h"
#include <stdio.h>
#include <$CHK_TK_INC_NAME>
int main(void) {
        FILE *maj = fopen("tkmajor","w");
        FILE *min = fopen("tkminor","w");
        FILE *serial = fopen("tkserial","w");
        fprintf(maj,"%d",TK_MAJOR_VERSION);
        fprintf(min,"%d",TK_MINOR_VERSION);
        fprintf(serial,"%s",TK_PATCH_LEVEL);
        fclose(maj);
        fclose(min);
        fclose(serial);
        return 0;
}
EOF

eval $ac_link
if test -s conftest && (./conftest; exit) 2>/dev/null; then
  TK_MAJOR_VERSION=`cat tkmajor`
  TK_MINOR_VERSION=`cat tkminor`
  TK_SERIAL_VERSION=`cat tkserial`
  TK_VERSION=$TK_MAJOR_VERSION.$TK_MINOR_VERSION
  rm -f tkmajor tkminor tkserial
else
  TK_VERSION="can't happen"
fi

case $host_os in
  darwin* | rhapsody*) 
# already added to CFLAGS 
  ;;
  *)
 TK_INC_PATH=-I$i 
  ;;
esac

TK_LIB=tk$TK_VERSION
if test $TK_VERSION_OK = 1; then 
	AC_MSG_RESULT([($TK_VERSION) yes])
else 
	AC_MSG_RESULT([($TK_VERSION) no])
	if $TK_VERSION = "can't happen"; then
		AC_MSG_ERROR([can(t happen])
        else 
	   AC_MSG_ERROR([you need at least version 8.4 of tk])
	fi
fi
CFLAGS=$saved_cflags
CPPFLAGS=$saved_cppflags
]) dnl End of AC_CHECK_TK_VERSION



AC_DEFUN([AC_CHECK_TK_LIB], [
dnl INPUTS :
dnl  $1 : major tcl version number
dnl  $2 : minor tcl version number
dnl OUPUTS :
dnl  TK_LIB_OK : 1 if link is OK; 0 otherwise
dnl  TK_LIB : flag to link against tcl lib

CHK_TK_MAJ=$1
CHK_TK_MIN=$2
TK_LIB_OK=0

AC_MSG_CHECKING([for tk library tk$1.$2])
MULTIARCH_HOST=`gcc -v 2>&1|grep Target|cut -d" " -f2`

dirs="$USER_TK_LIB_PATH $DEVTOOLS_LIBDIR /lib /usr/lib /usr/lib64 /usr/lib/tk /usr/lib/tk8.* /shlib /shlib/tk /shlib/tk8.* /usr/shlib /shlib/tk /usr/shlib/tk8.* /usr/local/lib /usr/local/lib/tk /usr/local/lib/tk8.* /usr/local/shlib /usr/X11/lib/tk /usr/X11/lib/tk8.*  /usr/lib/X11 /usr/lib/X11/tk /usr/lib/X11/tk8.* ../lib ../../lib /usr/tk /usr/local/tk /usr/local/tk/lib /usr/tk/lib /usr/local/tcl /usr/tcl /usr/local/tcl/lib /usr/tcl/lib /sw/lib /usr/lib/$MULTIARCH_HOST ."
libexts="so so.1.0 sl dylib a"
libnames="tk$CHK_TK_MAJ.$CHK_TK_MIN tk.$CHK_TK_MAJ.$CHK_TK_MIN tk$CHK_TK_MAJ$CHK_TK_MIN tk.$CHK_TK_MAJ$CHK_TK_MIN"

TK_PLUS="$TCL_LIB -lX11"

case $host_os in
  darwin* | rhapsody*) 
	TK_PLUS="-L${X11BASE}/lib -lX11"
  ;;
esac

# needed for cygwin with X11 
case "$host" in
  *-*-cygwin* ) 
	TK_PLUS=" "
    ;;
esac


for e in $libexts; do
	for j in $dirs; do
		for n in $libnames; do
			m="$j/lib$n.$e"
			if test -r $m; then
				PATH_LIB_TK=$j
				EXT_LIB_TK=$e
				NAME_LIB_TK=$n
				AC_MSG_RESULT([found $m using -L$PATH_LIB_TK -l$NAME_LIB_TK])
				saved_cflags="$CFLAGS"
				saved_ldflags="$LDFLAGS"
				saved_cppflags="$CPPFLAGS"
				CFLAGS="$CFLAGS $TK_INC_PATH"
				CPPFLAGS="$CPPFLAGS $TK_INC_PATH"
				LDFLAGS=$LDFLAGS
				TK_LDFLAGS="$X_LIBS $X_EXTRA_LIBS $TCLTK_LIBS"
				# Check for Tk lib
				if test "$USER_TK_LIB_PATH" = ""
				then
					LDFLAGS="$LDFLAGS $TK_LDFLAGS -L$PATH_LIB_TK"
					AC_CHECK_LIB([$NAME_LIB_TK], Tk_BindEvent, TK_LIB_OK=1,TK_LIB_OK=0,
						  [-l$NAME_LIB_TK $TK_PLUS])
				else
					LDFLAGS="$LDFLAGS $TK_LDFLAGS -L$USER_TK_LIB_PATH" 

					AC_CHECK_LIB([$NAME_LIB_TK], Tk_BindEvent, TK_LIB_OK=1,TK_LIB_OK=0,
						  [-l$NAME_LIB_TK $TK_PLUS])
				fi
				CFLAGS="$saved_cflags"
				CPPFLAGS="$saved_cppflags"
				LDFLAGS="$saved_ldflags"
				if test $TK_LIB_OK = 1; then 
					if test "$USER_TK_LIB_PATH" = ""
					then TK_LIB=" -L$PATH_LIB_TK -l$NAME_LIB_TK"
					else TK_LIB=" -L$USER_TK_LIB_PATH  -l$NAME_LIB_TK"
					fi
					break 3;
				fi
			fi
		dnl end of name finding loop
		done
	dnl end of dir finding loop
	done 
dnl end of extension finding loop
done
])





AC_DEFUN([AC_CHECK_TCLTK], [
dnl this macro finds and test the tcl/tk files
dnl **** it uses $X_LDFLAGS $X_LIBS  to find X libs  ****
dnl
dnl INPUT : it has no input but uses the following variables
dnl  $X_LDFLAGS $X_LIBS : for X libs
dnl  $TCLTK_LIBS : must contain the (sometimes) optional -ldl link flags an paths
dnl  $USER_TCL_INC_PATH : user specified path where to look for tcl.h
dnl  $USER_TK_INC_PATH : user specified path where to look for tk.h
dnl  $USER_TCL_LIB_PATH : user specified path where to look for libtcl
dnl  $USER_TK_LIB_PATH : user specified path where to look for libtk
dnl
dnl OUPUTS :  the following variables are set
dnl  WITH_TKSCI : =yes if all was OK, no otherwise
dnl  TCL_INC_PATH : flag to give to cpp if one wants to include tcl.h
dnl  TK_INC_PATH : flag to give to cpp if one wants to include tk.h
dnl  TCLTK_LIBS : complete line to link tcl/tk with scilab
dnl    it might be something like "/usr/lib/tk8.x.so /usr/lib/ltcl8.x.so -ldl"
dnl  TCL_VERSION : version of the found tcl includes and libs
dnl  TK_VERSION : version of the found tk includes and libs
dnl In addition, if the test was OK, the WITH_TK cpp symbol is defined

  WITH_TKSCI=no
  TCL_LIB_OK=0
  TCL_INCLUDE_OK=0
  # Check for tcl header file
  AC_MSG_CHECKING([for header file tcl.h])
  dirs="$USER_TCL_INC_PATH $DEVTOOLS_INCDIR /include /usr/include /usr/include/tcl /usr/include/tcl8.5 /usr/include/tcl8.* /usr/local/include /usr/local/include/tcl /usr/local/include/tcl8.*  /usr/X11/include/tcl /usr/X11/include/tcl8.* /usr/include/X11 /usr/include/X11/tcl /usr/include/X11/tcl8.* ../include ../../include /usr/tcl /usr/local/tcl /usr/local/tcl/include /usr/tcl/include /sw/include"
  for i in $dirs ; do
	if test -r $i/tcl.h; then 
		AC_MSG_RESULT([found in $i ])
		AC_CHECK_TCL_VERSION($i,8,4,tcl.h)
		if test $TCL_VERSION_OK = 1; then
			TCL_INCLUDE_OK=1	
			AC_CHECK_TCL_LIB($TCL_MAJOR_VERSION,$TCL_MINOR_VERSION)
			if test $TCL_LIB_OK = 1; then 
				TCLTK_LIBS=" $TCL_LIB $TCLTK_LIBS"
				break;
			fi
		fi
	fi
  done

  #perform tk tests if tcl test passed
  if test $TCL_LIB_OK = 0; then 
	if test $TCL_INCLUDE_OK = 0; then 
        	AC_MSG_ERROR([no header file tcl.h found for 8.4* or 8.5*. Please install the dev library of tcl 8.4 or 8.5])
        else
			AC_MSG_ERROR([header file tcl.h has been found for 8.4* or 8.5* but no corresponding tcl library (ie libtcl8.4.so or libtcl8.5.so)])
        fi	
  fi
# Check if X11/Xlib.h is available or not (tk.h needs it ...)
   AC_CHECK_HEADERS([X11/Xlib.h], [],
     [AC_MSG_ERROR([Could not find X11/Xlib.h ... This dependency is necessary because of tk.h... Provided by package libx11-dev under Debian/Ubuntu for example.])])

  # Check for tk header file
  AC_MSG_CHECKING([for header file tk.h])
  TK_LIB_OK=0
  TK_INCLUDE_OK=0
  dirs="$USER_TK_INC_PATH $DEVTOOLS_INCDIR /include /usr/include /usr/include/tk /usr/include/tk8.5 /usr/include/tk8.* /usr/local/include /usr/local/include/tk /usr/local/include/tk8.* /usr/X11/include/tk /usr/X11/include/tk8.* /usr/include/X11 /usr/include/X11/tk /usr/include/X11/tk8.* ../include ../../include /usr/tk /usr/local/tk /usr/local/tk/include /usr/tk/include  /usr/local/tcl /usr/tcl /usr/tcl/include /usr/local/tcl/include /sw/include /usr/include/tcl8.5 /usr/include/tcl8.*"
# /usr/include/tcl8.5 is checked because some distribs store tk headers
# in /usr/include/tcl8.*
  for i in $dirs ; do
	if test -r $i/tk.h; then 
		AC_MSG_RESULT([found in $i ])
		AC_CHECK_TK_VERSION($i,8,4,tk.h)
		if test $TK_VERSION_OK = 1; then 
			TK_INCLUDE_OK=1
			AC_CHECK_TK_LIB($TK_MAJOR_VERSION,$TK_MINOR_VERSION)
			if test $TK_LIB_OK = 1; then 
				TKTK_LIBS=" $TK_LIB $TKTK_LIBS"
				break;
			fi
		fi
	fi
  done

  if test $TK_LIB_OK = 0; then 
	if test $TK_INCLUDE_OK = 0; then 
        	AC_MSG_ERROR([no header file tk.h found for 8.4* or 8.5*. Please install the dev library of tk 8.4 or 8.5])	
        else
		AC_MSG_ERROR([header file tk.h has been found for 8.4* or 8.5* but no corresponding tk library (libtk8.4.so or libtk8.5.so)])
        fi

  else 
        TCLTK_LIBS=$TK_LIB" $TCLTK_LIBS"
        WITH_TKSCI=yes
  fi
])
