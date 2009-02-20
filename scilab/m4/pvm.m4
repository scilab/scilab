dnl AC_PVM
dnl ------------------------------------------------------
dnl  Check if PVM is usable
dnl INPUTS :
dnl   Nothing
dnl 
dnl OUTPUTS
dnl   PVM_OK : 1 if it is possible to use it
dnl  
AC_DEFUN([AC_PVM], [

   AC_MSG_CHECKING([for PVM])
    if test "x$PVM_ROOT" == "x"; then
		echo "PVM_ROOT not set"
	fi
	
	# check for a PVM provided by the distrubtion in the PATH
	AC_CHECK_PROG(PVMGETARCH,pvmgetarch,pvmgetarch,no)
    if test "$PVMGETARCH" = no; then
	   if test -x $PVM_ROOT/lib/pvmgetarch; then  
	   	# check for a PVM provided by the distrubtion in the PATH
		  PVMGETARCH=$PVM_ROOT/lib/pvmgetarch
	   else
	      AC_MSG_ERROR([Unable to find pvmgetarch in PATH or PVM_ROOT. (Scilab no longer provides an embedded PVM)])
	   fi
    fi

	AC_CACHE_CHECK([for PVM architecture],PVMARCH, [
	PVMARCH=`$PVMGETARCH`
	])
	if test "$PVMARCH" = UNKNOWN; then
		AC_MSG_ERROR([PVM is unable to recognise the architecture])
	fi

	PVMROOT="$SCIDIR/pvm3"
	PVMROOTR='$SCI/pvm3'
	
	AC_ARG_WITH(pvm-include,
		AC_HELP_STRING([--with-pvm-include=DIR],[Set the path to the PVM headers]),
		[  USER_PVMINCLUDE=$withval
	])

	AC_PVM_INCLUDE($USER_PVMINCLUDE)

	AC_DEFINE([WITH_PVM],[],[With PVM])
		
	PVMLIBDIR=$PVMROOT/lib/$PVMARCH
	AC_ARG_WITH(pvm-library,
		AC_HELP_STRING([--with-pvm-library=DIR],[Set the path to the PVM library]),
		[  USER_PVMLIBDIR=$withval
	])
	AC_PVM_LIBRARY($USER_PVMLIBDIR)


if test ! -z "$PVM_INCLUDE" -a ! -z "$PVM_LIB"; then
	PVM_OK=1
else
    AC_MSG_ERROR([Cannot find headers (pvm3.h) or PVM library. Please install the dev package.])
fi 
AC_SUBST(PVMARCH)
AC_SUBST(PVM_LIB)
AC_SUBST(PVM_INCLUDE)
])



# INPUTS :
#  $1 : Path of PVM provided by the user (/usr/include/pvm)
#
# OUTPUTS
#  PVM_INCLUDE : Flag to include files
#  Check the version of tcl associated to header file tcl.h 
AC_DEFUN([AC_PVM_INCLUDE], [
	AC_MSG_CHECKING([PVM header files])
	USER_PVMINCLUDE=$1
	F=$USER_PVMINCLUDE/pvm3.h
    if test -f "$F" ; then ## A voir ce que l'on fait quand le param USER_PVMINCLUDE=$1 est fournit mais n'existe pas ...
         PVM_INCLUDE="-I`dirname $F`"
	else
		if test ! -z "$USER_PVMINCLUDE"; then ## 
			AC_MSG_WARN([Cannot find PVM sources in the specified directory ($F). Looking for PVM sources elsewhere.])
		fi 
		incpath="$PVMROOT/include $PVM_ROOT/include /usr/include /usr/share /usr/share/include /usr/array/PVM /usr/local /usr/local/include /usr/local/src"
		for INC_PATH in $incpath; do
		    F="$INC_PATH/pvm3.h"
			if test -f "$F"; then
		         PVM_INCLUDE="-I`dirname $F`"
				 break 1;
			fi
		done
		if test -z $PVM_INCLUDE; then
	   	     AC_MSG_ERROR([Could not locate PVM include file])
		fi
	fi
	echo "PVM sources found in $PVM_INCLUDE"

    ac_save_CPPFLAGS=$CPPFLAGS
    ac_save_CFLAGS=$CFLAGS

    CPPFLAGS="$PVM_INCLUDE $CPPFLAGS"
    CFLAGS="$PVM_INCLUDE $CFLAGS"

    AC_CHECK_HEADER([pvm3.h],,
                    [
                    if test "$pvm" = yes; then
                      AC_MSG_ERROR([check for PVM library failed])
                    else
                      pvm=no
                    fi
                    ])

    CPPFLAGS=$ac_save_CPPFLAGS
    CFLAGS=$ac_save_CFLAGS
	
])


# INPUTS :
#  $1 : Path of PVM lib provided by the user (/usr/lib/pvm)
# OUPUTS :
#  PVM_LIB_OK : 1 if link is OK; 0 otherwise
#  PVM_LIB : flag to link against pvm lib

AC_DEFUN([AC_PVM_LIBRARY], [
USER_PVMLIBDIR=$1
  AC_MSG_CHECKING([for PVM3 library])
dirs="$USER_PVMLIBDIR /lib /usr/lib /usr/lib/pvm3 /shlib /shlib/pvm3 /usr/shlib /usr/shlib/pvm3 /usr/local/lib /usr/local/lib/pvm3 /usr/local/shlib /usr/local/pvm3 /usr/pvm3 /usr/local/pvm3/lib /sw/lib/ /usr/share/pvm3 ."
libexts="so so.1.0 sl dylib a"
libnames="pvm pvm3"

for EXT_LIB_PVM in $libexts; do
	for PATH_LIB_PVM in $dirs; do
		for NAME_LIB_PVM in $libnames; do
			m="$PATH_LIB_PVM/lib$NAME_LIB_PVM.$EXT_LIB_PVM"
			if test -r $m; then
				AC_MSG_RESULT([found $m using -L$PATH_LIB_PVM -l$NAME_LIB_PVM])

				saved_cflags="$CFLAGS"
				saved_ldflags="$LDFLAGS"
				saved_cppflags="$CPPFLAGS"
				CFLAGS="$CFLAGS $PVM_INC_PATH"
				CPPFLAGS="$CPPFLAGS $PVM_INC_PATH"
				LDFLAGS="$LDFLAGS -L$PATH_LIB_PVM"


		        AC_CHECK_LIB([$NAME_LIB_PVM], pvm_spawn,
		        [
	    		    LDFLAGS="-lpvm3 $LDFLAGS"
			        AC_CHECK_LIB(pvm3, pvm_barrier,
    		    		[PVMLIBS="-lpvm3"; PVM_LIB_OK=1],
		        		[
        				AC_CHECK_LIB(gpvm3, pvm_barrier,
			        		[PVMLIBS="-lgpvm3 -lpvm3"; 
							PVM_LIB_OK=1])
		        		])
        		])



				AC_CHECK_LIB([$NAME_LIB_PVM], pvm_spawn, PVM_LIB_OK=1,PVM_LIB_OK=0)

				CFLAGS="$saved_cflags"
				CPPFLAGS="$saved_cppflags"
				LDFLAGS="$saved_ldflags"

				if test $PVM_LIB_OK = 1; then 
					PVM_LIB=" -L$PATH_LIB_PVM $PVMLIBS"
					break 3;
				fi
			fi
		done
	done
done
if test -z "$PVM_LIB"; then
	AC_MSG_ERROR([check for PVM library failed])
fi
])
