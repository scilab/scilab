dnl Because this macro is used by AC_PROG_GCC_TRADITIONAL, which must
dnl come early, it is not included in AC_BEFORE checks.
dnl AC_GREP_CPP(PATTERN, PROGRAM, [ACTION-IF-FOUND [,
dnl              ACTION-IF-NOT-FOUND]])
AC_DEFUN(AC_GREP_CPP,
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





AC_DEFUN( AC_CHECK_TCL_VERSION, [
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

CHK_TCL_INCLUDE_PATH=$1
CHK_TCL_MAJOR=$2
CHK_TCL_MINOR=$3
CHK_TCL_INC_NAME=$4

echo $ac_n "  testing version (need $CHK_TCL_MAJOR.$CHK_TCL_MINOR or later)... " $ac_c
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

cat > conftest.$ac_ext <<EOF
#include "confdefs.h"
#include <stdio.h>
#include "$CHK_TCL_INCLUDE_PATH/$CHK_TCL_INC_NAME"
main() {
        FILE *maj = fopen("tclmajor","w");
        FILE *min = fopen("tclminor","w");
        fprintf(maj,"%d",TCL_MAJOR_VERSION);
        fprintf(min,"%d",TCL_MINOR_VERSION);
        fclose(maj);
        fclose(min);
        return 0;
}
EOF
eval $ac_link
if test -s conftest && (./conftest; exit) 2>/dev/null; then
  tclmajor=`cat tclmajor`
  tclminor=`cat tclminor`
  TCL_VERSION=$tclmajor.$tclminor
  echo $ac_n "it's $TCL_VERSION :""$ac_c " 1>&6
  rm -f tclmajor tclminor
else
  echo "$ac_t""can't happen" 1>&6
fi

TCL_INC_PATH=-I$i
TCL_LIB=tcl$TCL_VERSION
if test $TCL_VERSION_OK = 1; then echo "ok"; else echo "oops"; fi

]) dnl End of AC_CHECK_TCL_VERSION



AC_DEFUN( AC_CHECK_TCL_LIB, [
dnl INPUTS :
dnl  $1 : major tcl version number
dnl  $2 : minor tcl version number
dnl OUPUTS :
dnl  TCL_LIB_OK : 1 if link is OK; 0 otherwise
dnl  TCL_LIB : flag to link against tcl lib

CHK_TCL_MAJ=$1
CHK_TCL_MIN=$2
TCL_LIB_OK=0

dirs="$USER_TCL_LIB_PATH /lib /usr/lib /usr/lib/tcl /usr/lib/tcl8.* /shlib /shlib/tcl /shlib/tcl8.* /usr/shlib /shlib/tcl /usr//shlib/tcl8.* /usr/local/lib /usr/local/lib/tcl /usr/local/lib/tcl8.* /usr/local/shlib /usr/X11/lib/tcl /usr/X11/lib/tcl8.* /usr/lib/X11 /usr/lib/X11/tcl /usr/lib/X11/tcl8.* ../lib ../../lib  /usr/local/tcl /usr/tcl /usr/tcl/lib /usr/local/tcl/lib ."
libexts="so so.1.0 sl a"
libnames="tcl$CHK_TCL_MAJ.$CHK_TCL_MIN tcl.$CHK_TCL_MAJ.$CHK_TCL_MIN tcl$CHK_TCL_MAJ$CHK_TCL_MIN tcl.$CHK_TCL_MAJ$CHK_TCL_MIN"
for e in $libexts; do
for j in $dirs; do
for n in $libnames
do
m="$j/lib$n.$e"
if test -r $m; then 
echo "  found $m"
PATH_LIB_TCL=$j
EXT_LIB_TCL=$e
NAME_LIB_TCL=$n
echo "  link with -l$NAME_LIB_TCL -L$PATH_LIB_TCL"

saved_cflags="$CFLAGS"
saved_ldflags="$LDFLAGS"
saved_cppflags="$CPPFLAGS"
CFLAGS=$CFLAGS" $TCL_INC_PATH"
CPPFLAGS=$CPPFLAGS" $TCL_INC_PATH"
LDFLAGS=$LDFLAGS

# Check for Tcl lib
echo $ac_n "  ""$ac_c"
if test "$USER_TCL_LIB_PATH" = ""
then AC_CHECK_LIB(m, Tcl_DoOneEvent, TCL_LIB_OK=1,TCL_LIB_OK=0,[ -l$NAME_LIB_TCL $X_LIBS $X_EXTRA_LIBS $TCLTK_LIBS])
else AC_CHECK_LIB(m, Tcl_DoOneEvent, TCL_LIB_OK=1,TCL_LIB_OK=0,[ -L$USER_TCL_LIB_PATH -l$NAME_LIB_TCL $X_LIBS $X_EXTRA_LIBS $TCLTK_LIBS])
fi
CFLAGS="$saved_cflags"
CPPFLAGS="$saved_ldflags"
LDFLAGS="$saved_cppflags"

fi
if test $TCL_LIB_OK = 1; then 
#TCL_LIB=$m
if test "$USER_TCL_LIB_PATH" = ""
then TCL_LIB=" -l$NAME_LIB_TCL"
else TCL_LIB=" -L$USER_TCL_LIB_PATH  -l$NAME_LIB_TCL"
fi
break 3;
fi
dnl end of name finding loop
done
dnl end of dir finding loop
done 
dnl end of extension finding loop
done
])






AC_DEFUN( AC_CHECK_TK_VERSION, [
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

CHK_TK_INCLUDE_PATH=$1
CHK_TK_MAJOR=$2
CHK_TK_MINOR=$3
CHK_TK_INC_NAME=$4
saved_cppflags="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $TCL_INC_PATH $X_CFLAGS"
echo $ac_n "  testing version (need $CHK_TK_MAJOR.$CHK_TK_MINOR or later)... " $ac_c
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
#include "$CHK_TK_INCLUDE_PATH/$CHK_TK_INC_NAME"
main() {
        FILE *maj = fopen("tkmajor","w");
        FILE *min = fopen("tkminor","w");
        fprintf(maj,"%d",TK_MAJOR_VERSION);
        fprintf(min,"%d",TK_MINOR_VERSION);
        fclose(maj);
        fclose(min);
        return 0;
}
EOF
eval $ac_link
if test -s conftest && (./conftest; exit) 2>/dev/null; then
  tkmajor=`cat tkmajor`
  tkminor=`cat tkminor`
  TK_VERSION=$tkmajor.$tkminor
  echo $ac_n "it's $TK_VERSION :""$ac_c " 1>&6
  rm -f tkmajor tkminor
else
  echo "$ac_t""can't happen" 1>&6
fi

TK_INC_PATH=-I$i
TK_LIB=tk$TK_VERSION
if test $TK_VERSION_OK = 1; then echo "ok"; else echo "oops"; fi
CPPFLAGS=$saved_cppflags
]) dnl End of AC_CHECK_TK_VERSION















AC_DEFUN( AC_CHECK_TCLTK, [
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
dnl  WITH_TKSCI : =1 if all was OK, 0 otherwise
dnl  TCL_INC_PATH : flag to give to cpp if one wants to include tcl.h
dnl  TK_INC_PATH : flag to give to cpp if one wants to include tk.h
dnl  TCLTK_LIBS : complete line to link tcl/tk with scilab
dnl    it might be something like "/usr/lib/tk8.0.so /usr/lib/ltcl8.0.so -ldl"
dnl  TCL_VERSION : version of the found tcl includes and libs
dnl  TK_VERSION : version of the found tk includes and libs
dnl In addition, if the test was OK, the WITH_TK cpp symbol is defined


WITH_TKSCI=0
TCL_LIB_OK=0
# Check for tcl header file
echo "checking for tcl.h"
dirs="$USER_TCL_INC_PATH /include /usr/include /usr/include/tcl /usr/include/tcl8.* /usr/local/include /usr/local/include/tcl /usr/local/include/tcl8.*  /usr/X11/include/tcl /usr/X11/include/tcl8.* /usr/include/X11 /usr/include/X11/tcl /usr/include/X11/tcl8.* ../include ../../include /usr/tcl /usr/local/tcl /usr/local/tcl/include /usr/tcl/include"
for i in $dirs ; do
if test -r $i/tcl.h; then 

echo "  found tcl.h in $i"
AC_CHECK_TCL_VERSION($i,8,0,tcl.h)

TCL_LIB_OK=0;
if test $TCL_VERSION_OK = 1; then
AC_CHECK_TCL_LIB($tclmajor, $tclminor)
fi

if test $TCL_LIB_OK = 1; then 
  TCLTK_LIBS=" $TCL_LIB $TCLTK_LIBS"
   break;
fi
fi
done

#perform tk tests if tcl test passed
if test $TCL_LIB_OK = 1; then 

# Check for tk header file
echo "checking for tk.h"
dirs="$USER_TK_INC_PATH  /include /usr/include /usr/include/tk /usr/include/tk8.* /usr/local/include /usr/local/include/tk /usr/local/include/tk8.* /usr/X11/include/tk /usr/X11/include/tk8.* /usr/include/X11 /usr/include/X11/tk /usr/include/X11/tk8.* ../include ../../include /usr/tk /usr/local/tk /usr/local/tk/include /usr/tk/include  /usr/local/tcl /usr/tcl /usr/tcl/include /usr/local/tcl/include"
for i in $dirs ; do
if test -r $i/tk.h; then 

echo "  found tk.h in $i"
AC_CHECK_TK_VERSION($i,8,0,tk.h)

TK_LIB_OK=0;
if test $TK_VERSION_OK = 1; then
dnl Check for tk library file
CHK_TK_MAJ=$tkmajor
CHK_TK_MIN=$tkminor
dirs="$USER_TK_LIB_PATH /lib /usr/lib /usr/lib/tk /usr/lib/tk8.* /shlib /shlib/tk /shlib/tk8.* /usr/shlib /shlib/tk /usr/shlib/tk8.* /usr/local/lib /usr/local/lib/tk /usr/local/lib/tk8.* /usr/local/shlib /usr/X11/lib/tk /usr/X11/lib/tk8.*  /usr/lib/X11 /usr/lib/X11/tk /usr/lib/X11/tk8.* ../lib ../../lib /usr/tk /usr/local/tk /usr/local/tk/lib /usr/tk/lib /usr/local/tcl /usr/tcl /usr/local/tcl/lib /usr/tcl/lib"
libexts="so so.1.0 sl a"
libnames="tk$CHK_TK_MAJ.$CHK_TK_MIN tk.$CHK_TK_MAJ.$CHK_TK_MIN tk$CHK_TK_MAJ$CHK_TK_MIN tk.$CHK_TK_MAJ$CHK_TK_MIN"
for e in $libexts; do
for j in $dirs ; do
for n in $libnames; do
m="$j/lib$n.$e"
if test -r $m; then 
echo "  found $m"
PATH_LIB_TK=$j
EXT_LIB_TK=$e
NAME_LIB_TK=$n
echo "  link with -l$NAME_LIB_TK -L$PATH_LIB_TK"

saved_cflags="$CFLAGS"
saved_ldflags="$LDFLAGS"
saved_cppflags="$CPPFLAGS"
CFLAGS=$CFLAGS"  $X_CFLAGS $TK_INC_PATH" 
CPPFLAGS=$CPPFLAGS" $TK_INC_PATH" 
LDFLAGS=$LDFLAGS" $X_LDFLAGS $X_LIBS" 

# Check for Tk lib
echo $ac_n "  ""$ac_c"
if test "$USER_TK_LIB_PATH" = ""
then AC_CHECK_LIB(m, Tk_BindEvent, TK_LIB_OK=1, TK_LIB_OK=0,[ -l$NAME_LIB_TK  -lX11 $X_LIBS $X_EXTRA_LIBS $TCLTK_LIBS])
else AC_CHECK_LIB(m, Tk_BindEvent, TK_LIB_OK=1, TK_LIB_OK=0,[ -L$USER_TK_LIB_PATH -l$NAME_LIB_TK  -lX11 $X_LIBS $X_EXTRA_LIBS $TCLTK_LIBS])
fi
CFLAGS="$saved_cflags"
CPPFLAGS="$saved_ldflags"
LDFLAGS="$saved_cppflags"

fi
if test $TK_LIB_OK = 1; then 
#TK_LIB=$m
if test "$USER_TK_LIB_PATH" = ""
then TK_LIB=" -l$NAME_LIB_TK "
else TK_LIB=" -L$USER_TK_LIB_PATH -l$NAME_LIB_TK "
fi
break 3; 
fi
done
done
done
fi

if test $TK_LIB_OK = 1; then 
  TCLTK_LIBS=" $TK_LIB $TCLTK_LIBS"
  WITH_TKSCI=1
  break;
fi
fi
done


# end of tk test
fi

])
## libtool.m4 - Configure libtool for the target system. -*-Shell-script-*-
## Copyright (C) 1996-1999 Free Software Foundation, Inc.
## Originally by Gordon Matzigkeit <gord@gnu.ai.mit.edu>, 1996
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
##
## As a special exception to the GNU General Public License, if you
## distribute this file as part of a program that contains a
## configuration script generated by Autoconf, you may include it under
## the same distribution terms that you use for the rest of that program.

# serial 39 AC_PROG_LIBTOOL
AC_DEFUN(AC_PROG_LIBTOOL,
[AC_REQUIRE([AC_LIBTOOL_SETUP])dnl

# Save cache, so that ltconfig can load it
AC_CACHE_SAVE

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
LD="$LD" LDFLAGS="$LDFLAGS" LIBS="$LIBS" \
LN_S="$LN_S" NM="$NM" RANLIB="$RANLIB" \
DLLTOOL="$DLLTOOL" AS="$AS" OBJDUMP="$OBJDUMP" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig --no-reexec \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $host \
|| AC_MSG_ERROR([libtool configure failed])

# Reload cache, that may have been modified by ltconfig
AC_CACHE_LOAD

# This can be used to rebuild libtool when needed
LIBTOOL_DEPS="$ac_aux_dir/ltconfig $ac_aux_dir/ltmain.sh"

# Always use our own libtool.
LIBTOOL='$(SHELL) $(top_builddir)/libtool'
AC_SUBST(LIBTOOL)dnl

# Redirect the config.log output again, so that the ltconfig log is not
# clobbered by the next message.
exec 5>>./config.log
])

AC_DEFUN(AC_LIBTOOL_SETUP,
[AC_PREREQ(2.13)dnl
AC_REQUIRE([AC_ENABLE_SHARED])dnl
AC_REQUIRE([AC_ENABLE_STATIC])dnl
AC_REQUIRE([AC_ENABLE_FAST_INSTALL])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_LD])dnl
AC_REQUIRE([AC_PROG_NM])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
dnl

# Check for any special flags to pass to ltconfig.
libtool_flags="--cache-file=$cache_file"
test "$enable_shared" = no && libtool_flags="$libtool_flags --disable-shared"
test "$enable_static" = no && libtool_flags="$libtool_flags --disable-static"
test "$enable_fast_install" = no && libtool_flags="$libtool_flags --disable-fast-install"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"
ifdef([AC_PROVIDE_AC_LIBTOOL_DLOPEN],
[libtool_flags="$libtool_flags --enable-dlopen"])
ifdef([AC_PROVIDE_AC_LIBTOOL_WIN32_DLL],
[libtool_flags="$libtool_flags --enable-win32-dll"])
AC_ARG_ENABLE(libtool-lock,
  [  --disable-libtool-lock  avoid locking (might break parallel builds)])
test "x$enable_libtool_lock" = xno && libtool_flags="$libtool_flags --disable-lock"
test x"$silent" = xyes && libtool_flags="$libtool_flags --silent"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case "$host" in
*-*-irix6*)
  # Find out which ABI we are using.
  echo '[#]line __oline__ "configure"' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case "`/usr/bin/file conftest.o`" in
    *32-bit*)
      LD="${LD-ld} -32"
      ;;
    *N32*)
      LD="${LD-ld} -n32"
      ;;
    *64-bit*)
      LD="${LD-ld} -64"
      ;;
    esac
  fi
  rm -rf conftest*
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -belf"
  AC_CACHE_CHECK([whether the C compiler needs -belf], lt_cv_cc_needs_belf,
    [AC_TRY_LINK([],[],[lt_cv_cc_needs_belf=yes],[lt_cv_cc_needs_belf=no])])
  if test x"$lt_cv_cc_needs_belf" != x"yes"; then
    # this is probably gcc 2.8.0, egcs 1.0 or newer; no need for -belf
    CFLAGS="$SAVE_CFLAGS"
  fi
  ;;

ifdef([AC_PROVIDE_AC_LIBTOOL_WIN32_DLL],
[*-*-cygwin* | *-*-mingw*)
  AC_CHECK_TOOL(DLLTOOL, dlltool, false)
  AC_CHECK_TOOL(AS, as, false)
  AC_CHECK_TOOL(OBJDUMP, objdump, false)
  ;;
])
esac
])

# AC_LIBTOOL_DLOPEN - enable checks for dlopen support
AC_DEFUN(AC_LIBTOOL_DLOPEN, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])])

# AC_LIBTOOL_WIN32_DLL - declare package support for building win32 dll's
AC_DEFUN(AC_LIBTOOL_WIN32_DLL, [AC_BEFORE([$0], [AC_LIBTOOL_SETUP])])

# AC_ENABLE_SHARED - implement the --enable-shared flag
# Usage: AC_ENABLE_SHARED[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_SHARED, [dnl
define([AC_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(shared,
changequote(<<, >>)dnl
<<  --enable-shared[=PKGS]  build shared libraries [default=>>AC_ENABLE_SHARED_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_shared=yes ;;
no) enable_shared=no ;;
*)
  enable_shared=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_shared=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_shared=AC_ENABLE_SHARED_DEFAULT)dnl
])

# AC_DISABLE_SHARED - set the default shared flag to --disable-shared
AC_DEFUN(AC_DISABLE_SHARED, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_SHARED(no)])

# AC_ENABLE_STATIC - implement the --enable-static flag
# Usage: AC_ENABLE_STATIC[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_STATIC, [dnl
define([AC_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(static,
changequote(<<, >>)dnl
<<  --enable-static[=PKGS]  build static libraries [default=>>AC_ENABLE_STATIC_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_static=yes ;;
no) enable_static=no ;;
*)
  enable_static=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_static=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_static=AC_ENABLE_STATIC_DEFAULT)dnl
])

# AC_DISABLE_STATIC - set the default static flag to --disable-static
AC_DEFUN(AC_DISABLE_STATIC, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_STATIC(no)])


# AC_ENABLE_FAST_INSTALL - implement the --enable-fast-install flag
# Usage: AC_ENABLE_FAST_INSTALL[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_FAST_INSTALL, [dnl
define([AC_ENABLE_FAST_INSTALL_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(fast-install,
changequote(<<, >>)dnl
<<  --enable-fast-install[=PKGS]  optimize for fast installation [default=>>AC_ENABLE_FAST_INSTALL_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_fast_install=yes ;;
no) enable_fast_install=no ;;
*)
  enable_fast_install=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_fast_install=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_fast_install=AC_ENABLE_FAST_INSTALL_DEFAULT)dnl
])

# AC_ENABLE_FAST_INSTALL - set the default to --disable-fast-install
#AC_DEFUN(AC_DISABLE_FAST_INSTALL, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
# AC_ENABLE_FAST_INSTALL(no)])

# AC_PROG_LD - find the path to the GNU or non-GNU linker
AC_DEFUN(AC_PROG_LD,
[AC_ARG_WITH(gnu-ld,
[  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]],
test "$withval" = no || with_gnu_ld=yes, with_gnu_ld=no)
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
ac_prog=ld
if test "$ac_cv_prog_gcc" = yes; then
  # Check if gcc -print-prog-name=ld gives a path.
  AC_MSG_CHECKING([for ld used by GCC])
  ac_prog=`($CC -print-prog-name=ld) 2>&5`
  case "$ac_prog" in
    # Accept absolute paths.
changequote(,)dnl
    [\\/]* | [A-Za-z]:[\\/]*)
      re_direlt='/[^/][^/]*/\.\./'
changequote([,])dnl
      # Canonicalize the path of ld
      ac_prog=`echo $ac_prog| sed 's%\\\\%/%g'`
      while echo $ac_prog | grep "$re_direlt" > /dev/null 2>&1; do
	ac_prog=`echo $ac_prog| sed "s%$re_direlt%/%"`
      done
      test -z "$LD" && LD="$ac_prog"
      ;;
  "")
    # If it fails, then pretend we aren't using GCC.
    ac_prog=ld
    ;;
  *)
    # If it is relative, then search for the first ld in PATH.
    with_gnu_ld=unknown
    ;;
  esac
elif test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(ac_cv_path_LD,
[if test -z "$LD"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR-:}"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog" || test -f "$ac_dir/$ac_prog$ac_exeext"; then
      ac_cv_path_LD="$ac_dir/$ac_prog"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      if "$ac_cv_path_LD" -v 2>&1 < /dev/null | egrep '(GNU|with BFD)' > /dev/null; then
	test "$with_gnu_ld" != no && break
      else
	test "$with_gnu_ld" != yes && break
      fi
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
fi])
LD="$ac_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_SUBST(LD)
AC_PROG_LD_GNU
])

AC_DEFUN(AC_PROG_LD_GNU,
[AC_CACHE_CHECK([if the linker ($LD) is GNU ld], ac_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
  ac_cv_prog_gnu_ld=yes
else
  ac_cv_prog_gnu_ld=no
fi])
])

# AC_PROG_NM - find the path to a BSD-compatible name lister
AC_DEFUN(AC_PROG_NM,
[AC_MSG_CHECKING([for BSD-compatible nm])
AC_CACHE_VAL(ac_cv_path_NM,
[if test -n "$NM"; then
  # Let the user override the test.
  ac_cv_path_NM="$NM"
else
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR-:}"
  for ac_dir in $PATH /usr/ccs/bin /usr/ucb /bin; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/nm || test -f $ac_dir/nm$ac_exeext ; then
      # Check to see if the nm accepts a BSD-compat flag.
      # Adding the `sed 1q' prevents false positives on HP-UX, which says:
      #   nm: unknown option "B" ignored
      if ($ac_dir/nm -B /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
	ac_cv_path_NM="$ac_dir/nm -B"
	break
      elif ($ac_dir/nm -p /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
	ac_cv_path_NM="$ac_dir/nm -p"
	break
      else
	ac_cv_path_NM=${ac_cv_path_NM="$ac_dir/nm"} # keep the first match, but
	continue # so that we can try to find one that supports BSD flags
      fi
    fi
  done
  IFS="$ac_save_ifs"
  test -z "$ac_cv_path_NM" && ac_cv_path_NM=nm
fi])
NM="$ac_cv_path_NM"
AC_MSG_RESULT([$NM])
AC_SUBST(NM)
])

# AC_CHECK_LIBM - check for math library
AC_DEFUN(AC_CHECK_LIBM,
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
LIBM=
case "$host" in
*-*-beos* | *-*-cygwin*)
  # These system don't have libm
  ;;
*-ncr-sysv4.3*)
  AC_CHECK_LIB(mw, _mwvalidcheckl, LIBM="-lmw")
  AC_CHECK_LIB(m, main, LIBM="$LIBM -lm")
  ;;
*)
  AC_CHECK_LIB(m, main, LIBM="-lm")
  ;;
esac
])

# AC_LIBLTDL_CONVENIENCE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl convenience library, adds --enable-ltdl-convenience to
# the configure arguments.  Note that LIBLTDL is not AC_SUBSTed, nor
# is AC_CONFIG_SUBDIRS called.  If DIR is not provided, it is assumed
# to be `${top_builddir}/libltdl'.  Make sure you start DIR with
# '${top_builddir}/' (note the single quotes!) if your package is not
# flat, and, if you're not using automake, define top_builddir as
# appropriate in the Makefiles.
AC_DEFUN(AC_LIBLTDL_CONVENIENCE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  case "$enable_ltdl_convenience" in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
  LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdlc.la
])

# AC_LIBLTDL_INSTALLABLE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl installable library, and adds --enable-ltdl-install to
# the configure arguments.  Note that LIBLTDL is not AC_SUBSTed, nor
# is AC_CONFIG_SUBDIRS called.  If DIR is not provided, it is assumed
# to be `${top_builddir}/libltdl'.  Make sure you start DIR with
# '${top_builddir}/' (note the single quotes!) if your package is not
# flat, and, if you're not using automake, define top_builddir as
# appropriate in the Makefiles.
# In the future, this macro may have to be called after AC_PROG_LIBTOOL.
AC_DEFUN(AC_LIBLTDL_INSTALLABLE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  AC_CHECK_LIB(ltdl, main,
  [test x"$enable_ltdl_install" != xyes && enable_ltdl_install=no],
  [if test x"$enable_ltdl_install" = xno; then
     AC_MSG_WARN([libltdl not installed, but installation disabled])
   else
     enable_ltdl_install=yes
   fi
  ])
  if test x"$enable_ltdl_install" = x"yes"; then
    ac_configure_args="$ac_configure_args --enable-ltdl-install"
    LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdl.la
  else
    ac_configure_args="$ac_configure_args --enable-ltdl-install=no"
    LIBLTDL="-lltdl"
  fi
])

dnl old names
AC_DEFUN(AM_PROG_LIBTOOL, [indir([AC_PROG_LIBTOOL])])dnl
AC_DEFUN(AM_ENABLE_SHARED, [indir([AC_ENABLE_SHARED], $@)])dnl
AC_DEFUN(AM_ENABLE_STATIC, [indir([AC_ENABLE_STATIC], $@)])dnl
AC_DEFUN(AM_DISABLE_SHARED, [indir([AC_DISABLE_SHARED], $@)])dnl
AC_DEFUN(AM_DISABLE_STATIC, [indir([AC_DISABLE_STATIC], $@)])dnl
AC_DEFUN(AM_PROG_LD, [indir([AC_PROG_LD])])dnl
AC_DEFUN(AM_PROG_NM, [indir([AC_PROG_NM])])dnl

#dnl This is just to quiet aclocal about the macro not being used
#if(a,b,[AC_DISABLE_FAST_INSTALL])dnl
