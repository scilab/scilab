dnl AC_PVM
dnl ------------------------------------------------------
dnl  Check if PVM is usable
dnl INPUTS :
dnl   Nothing
dnl 
dnl OUTPUTS
dnl   PVM_OK : 1 if it is possible to use it
dnl  
dnl
dnl --------------------------------------------------------
dnl
dnl General comments about PVM detection
dnl
dnl A typical installation has a hierarchy under PVM_ROOT that includes
dnl PVM_ROOT/include/pvm3.h
dnl PVM_ROOT/lib/pvmgetarch
dnl PVM_ROOT/lib/<pvmarch>/libfpvm3.{a,so}
dnl PVM_ROOT/lib/<pvmarch>/libgpvm3.{a,so}
dnl PVM_ROOT/lib/<pvmarch>/libpvm3.{a,so}
dnl
dnl The setting for <pvmarch> is given by the output of the
dnl PVM_ROOT/lib/pvmgetarch program.
dnl
dnl This typical installation has been seen on several different
dnl operating systems and several different packaging systems.  It can
dnl be considered the "normal" case.  In addition, the documentation
dnl that comes with pvm3 says that the variable 
dnl PVM_ROOT should be set in the users environment when using
dnl pvm3 or compiling programs that use pvm3.
dnl
dnl --------------------------------------------------------
dnl Variations that have been encountered "in the field" are:
dnl
dnl --------------------------------------------------------
dnl header installed to /usr/include/pvm3.h but the rest of the install
dnl to PVM_ROOT (where PVM_ROOT=/usr/share/pvm3 in one particular instance).
dnl
dnl This case can be dealt with by using the --with-pvm-includes option
dnl --------------------------------------------------------
dnl No shared libraries installed.  Only static libs.  This may cause
dnl issues with linking on some systems.  This may have to wait for a real
dnl world trial to see how it pans out.
dnl --------------------------------------------------------
dnl 
dnl
dnl The libraries installed by pvm3 are:
dnl $PVM_ROOT/lib/$PVM_ARCH/libpvm3.a   -  C (base) library
dnl $PVM_ROOT/lib/$PVM_ARCH/libfpvm3.a  -  Fortran wrapper library
dnl $PVM_ROOT/lib/$PVM_ARCH/libgpvm3.a  -  Group function library

AC_DEFUN([AC_PVM], [

    AC_MSG_CHECKING([if PVM_ROOT has been set])
    if test "x$PVM_ROOT" = "x"; then
        AC_MSG_RESULT([no.  I will try to guess a value for it])
        _pvm_path="${PATH}"
        # this variable will be used later to see if the user specified
        # PVM_ROOT or if we guessed at it based on the location of the
        # header
        _pvm_root_specified=no
    else
        AC_MSG_RESULT([yes ($PVM_ROOT)])

        _pvm_root_specified=yes
        # PVM_ROOT is set, make sure the directory exists
        AC_MSG_CHECKING([if the specified PVM_ROOT directory exists])
        if test -d "$PVM_ROOT" ; then
            AC_MSG_RESULT([yes])
        else
            AC_MSG_RESULT([no])
            AC_MSG_ERROR([The directory specified by PVM_ROOT does not exist.
The value of PVM_ROOT must either be left unspecified (empty or not defined)
or set to a valid directory that is the root of a pvm3 installation.
])
        fi
        
        # PVM_ROOT is set and the directory exists.  Now make sure it looks
        # like a pvm installation!
        AC_MSG_CHECKING([if the specified PVM_ROOT contains a pvm installation])
        if test -r "${PVM_ROOT}/include/pvm3.h" ; then
            AC_MSG_RESULT([yes])
        else
            AC_MSG_RESULT([no])
            AC_MSG_ERROR([The directory specified by PVM_ROOT does not appear
to be a valid pvm installation directory.  This is based on PVM_ROOT/include/pvm3.h
not existing.  The value of PVM_ROOT must either be left unspecified (empty or
not defined) or set to a valid directory that is the root of a pvm3 installation.])
        fi

	# If we made it this far then it means PVM_ROOT is set and appears to
	# be valid.
        # we'll use this path for looking for pvmgetarch.
        _pvm_path="${PVM_ROOT}/lib:${PATH}"
    fi

    # check for pvmgetarch.  If specified then PVM_ROOT/lib is placed at the
    # beginning of the search path.  The user can force this by 
    # simply setting PVMGETARCH on the configure command line like
    # PVMGETARCH=/path/to/pvmgetarch if they need to force a particular value.
    AC_PATH_PROG([PVMGETARCH],[pvmgetarch],[no], [$_pvm_path])

    if test "$PVMGETARCH" = "no" ; then
        AC_MSG_ERROR([Unable to find pvmgetarch in PVM_ROOT/lib or PATH])
     fi


    # FIXME -- if we ever wish to cross compile scilab with pvm support in place
    # then the user will need o specify PVM_ARCH correctly.  See pvm_intro(1PVM) for
    # more details on the supported environment variables.
	AC_CACHE_CHECK([for PVM architecture],[PVM_ARCH], [PVM_ARCH=`$PVMGETARCH`])
	if test "x$PVM_ARCH" = "xUNKNOWN"; then
		AC_MSG_ERROR([PVM is unable to recognise the architecture.  This means that
pvmgetarch ($PVMGETARCH) is broken on your system.
])
	fi


    # now we start looking for the pvm3.h header.
    USER_PVMINCLUDE=""	
	AC_ARG_WITH(pvm-include,
		AC_HELP_STRING([--with-pvm-include=DIR],[Set the path to the PVM headers]),
		[  USER_PVMINCLUDE=$withval
	])

	AC_PVM_INCLUDE($USER_PVMINCLUDE)

	AC_DEFINE([WITH_PVM],[],[With PVM])

    if test "x$PVM_ROOT" = "x" ; then
        # PVM_ROOT was not set so try to guess it from the header location.
        # Since if we made it this far, we must have found the pvm3.h header
        # somewhere and that somewhere is in $PVM_INCLUDE_DIR.
        AC_MSG_CHECKING([for a reasonable guess at PVM_ROOT since PVM_ROOT was not set])
        PVM_ROOT="`dirname $PVM_INCLUDE_DIR`"
        AC_MSG_RESULT([$PVM_ROOT])
    fi

    PVMLIBDIR=$PVM_ROOT/lib/$PVM_ARCH
    USER_PVMLIBDIR=""	
    AC_ARG_WITH(pvm-library,
        AC_HELP_STRING([--with-pvm-library=DIR],[Set the path to the PVM library directory]),
            [  USER_PVMLIBDIR=$withval
	])
    AC_PVM_LIBRARY($USER_PVMLIBDIR)


    if test ! -z "$PVM_INCLUDE" -a ! -z "$PVM_LIB"; then
	    PVM_OK=1
    else
        AC_MSG_ERROR([Cannot find headers (pvm3.h) or PVM library. Please install the dev package for pvm3.])
    fi 
    AC_SUBST(PVM_ARCH)
    AC_SUBST(PVM_LIB)
    AC_SUBST(PVM_INCLUDE)
])



# INPUTS :
#  $1 : Path of PVM provided by the user (/usr/include/pvm)
#
# OUTPUTS
#  PVM_INCLUDE : Flag to include files
AC_DEFUN([AC_PVM_INCLUDE], [
	AC_MSG_CHECKING([PVM header files])
	USER_PVMINCLUDE="$1"

    _pvm_header=pvm3.h
    # if the user has given a --with-pvm-include configure option but the specified
    # directory does not exist, then error out.  We don't keep searching in this case
    # because the user has explicitly told us where to look and that location didn't
    # exist.
    if test "x$USER_PVMINCLUDE" != "x" -a ! -d "$USER_PVMINCLUDE" ; then
        AC_MSG_ERROR([The specified PVM include directory, $USER_PVMINCLUDE, does not exist.])
    fi

    if test "x$USER_PVMINCLUDE" != "x" ; then
        # if the user has given a --with-pvm-include configure option but there is no pvm3.h
        # there then error out.  We don't keep searching in this case because the user has 
        # explicitly told us where to look but we didn't find what we needed there.
        if test -f "$USER_PVMINCLUDE/${_pvm_header}" ; then 
            PVM_INCLUDE_DIR="$USER_PVMINCLUDE"
	    else
            AC_MSG_ERROR([Cannot find PVM header (${_pvm_header})  in the specified directory ($USER_PVMINCLUDE).])
		fi 
    else
        # we have not been given a --with-pvm-include option, so we have to go looking
        # for the pvm header
        incpath=""

        # if PVM_ROOT is set, then look there first
        if test "x$PVM_ROOT" != "x" ; then
            incpath="$PVM_ROOT/include $incpath"
        else
            # PVM_ROOT is not set so lets take a guess at what it may be.
            # pvmgetarch is supposed to be installed as PVM_ROOT/lib/pvmgetarch
            # so dirname $PVMGETARCH should give PVM_ROOT/lib
            # and dirname again should give a guess at PVM_ROOT
            tmp=`dirname $PVMGETARCH`
            tmp=`dirname $tmp`
            incpath="${tmp}/include $incpath"
        fi

        # add a bunch of other paths
        incpath="$incpath /usr/include /usr/share /usr/share/include /usr/array/PVM /usr/local /usr/local/include /usr/local/src"
        PVM_INCLUDE=""
		for INC_PATH in $incpath; do
		    F="$INC_PATH/${_pvm_header}"
			if test -f "$F"; then
		         PVM_INCLUDE_DIR="${INC_PATH}"
				 break 1;
			fi
		done
		if test -z "$PVM_INCLUDE_DIR"; then
	   	     AC_MSG_ERROR([Could not locate PVM include file (${_pvm_header})])
		fi
	fi
    AC_MSG_RESULT([PVM header found in $PVM_INCLUDE_DIR])

    ac_save_CPPFLAGS="$CPPFLAGS"
    ac_save_CFLAGS="$CFLAGS"

    CPPFLAGS="-I$PVM_INCLUDE_DIR $CPPFLAGS"
    CFLAGS="-I$PVM_INCLUDE_DIR $CFLAGS"

    AC_CHECK_HEADER([pvm3.h],,[AC_MSG_ERROR([check for PVM headers failed])])

    CPPFLAGS=$ac_save_CPPFLAGS
    CFLAGS=$ac_save_CFLAGS
    PVM_INCLUDE="-I${PVM_INCLUDE_DIR}"	
])


# INPUTS :
#  $1 : Path of PVM lib provided by the user (/usr/lib/pvm)
# OUPUTS :
#  PVM_LIB_OK : 1 if link is OK; 0 otherwise
#  PVM_LIB : flag to link against pvm lib

AC_DEFUN([AC_PVM_LIBRARY], [
    USER_PVMLIBDIR="$1"

    # if the user has given a --with-pvm-library configure option but the specified
    # directory does not exist, then error out.  We don't keep searching in this case
    # because the user has explicitly told us where to look and that location did not
    # exist.
    if test "x$USER_PVMLIBDIR" != "x" ; then
        AC_MSG_CHECKING([if the specified PVM library directory ($USER_PVMLIBDIR) exists])
        if test -d "$USER_PVMLIBDIR" ; then
            AC_MSG_RESULT([yes])
        else
            AC_MSG_ERROR([The specified PVM library directory ($USER_PVMLIBDIR) does not exist.
When specifying a PVM library directory with --with-pvm-libraries, the directory must
exist and contain PVM libraries])
        fi
    fi


    dirs="$USER_PVMLIBDIR $PVMLIBDIR /lib /usr/lib /usr/lib/pvm3 /shlib /shlib/pvm3 /usr/shlib /usr/shlib/pvm3 /usr/local/lib /usr/local/lib/pvm3 /usr/local/shlib /usr/local/pvm3 /usr/pvm3 /usr/local/pvm3/lib /sw/lib/ /usr/share/pvm3 ."
    libnames="pvm3 pvm"


    # if the user has given a --with-pvm-library configure option, only look there.
    if test "x$USER_PVMLIBDIR" != "x" ; then
        dirs="$USER_PVMLIBDIR"
        AC_MSG_CHECKING([for PVM libraries in $USER_PVMLIBDIR])
    elif test "$_pvm_root_specified" = "yes" ; then
        # otherwise, if the user has specified PVM_ROOT (as opposed to configure
        # having guessed it), then only look in PVMLIBDIR (derived from PVM_ROOT
        # and PVMGETARCH output
        AC_MSG_CHECKING([for PVM libraries in $PVMLIBDIR])
        dirs="$PVMLIBDIR"
    else
        AC_MSG_CHECKING([for PVM libraries])
    fi

    # put the path loop on the outside because we always want to look in 
    # the user specified directory and the PVM_ROOT directory first for all possible
    # names instead of accidentally picking up something from a different directory
    for PATH_LIB_PVM in $dirs; do
        for NAME_LIB_PVM in $libnames; do
            m=lib${NAME_LIB_PVM}
            # note that the * is outside the quotes to allow the shell to expand it.
            # The rest is quoted in case we have spaces or anything weird in the path.
            ls "${PATH_LIB_PVM}/${m}."* >/dev/null 2>/dev/null
            rc=$?
            if test $rc -eq 0 ; then
                # we found a candidate directory
                # so check it out and see if it works
                AC_MSG_RESULT([found $m in $PATH_LIB_PVM])
                saved_cflags="$CFLAGS"
                saved_ldflags="$LDFLAGS"
                saved_cppflags="$CPPFLAGS"
                saved_libs="$LIBS"
                CFLAGS="$PVM_INCLUDE $CFLAGS"
                CPPFLAGS="$PVM_INCLUDE $CPPFLAGS"
                LDFLAGS="$LDFLAGS -L$PATH_LIB_PVM"

                # pvm_spawn() is a basic pvm3 function and should be in libpvm3
                AC_SEARCH_LIBS([pvm_spawn],[pvm3 pvm],[PVM_SPAWN=$ac_res], 
                    [AC_MSG_ERROR([Could not find the library with pvm_spawn().  Check that your PVM installation is not broken])]
                )

                if test "$PVM_SPAWN" = "none required" ; then 
		    PVM_SPAWN=""
		fi

                # pvm_barrier() is one of the group functions and should be in libgpvm3
                AC_SEARCH_LIBS([pvm_barrier],[pvm3 gpvm3],[PVM_BARRIER=$ac_res], 
                    [AC_MSG_ERROR([Could not find the library with pvm_barrier().  Check that your PVM installation is not broken])]
                )

                if test "$PVM_BARRIER" = "none required" ; then 
		    PVM_BARRIER=""
		fi

                PVMLIBS="$PVM_SPAWN $PVM_BARRIER"
		PVM_LIB_OK=1

                CFLAGS="$saved_cflags"
                CPPFLAGS="$saved_cppflags"
                LDFLAGS="$saved_ldflags"
                LIBS="$saved_libs"

                if test $PVM_LIB_OK = 1; then 
                    PVM_LIB="-L$PATH_LIB_PVM $PVMLIBS"
                    break 2;
                fi
            fi
        done # end of libname loop
    done # end of directory loop


# if we didn't find the PVM library, then spit out an error message
# about it.  The error message is customized based on if the user
# has specified PVM_ROOT or --with-pvm-libraries
if test -z "$PVM_LIB"; then

    if test "x$USER_PVMLIBDIR" != "x" ; then

        AC_MSG_ERROR([could not locate PVM libraries in the directory
specified by --with-pvm-libraries ($USER_PVMLIBDIR)])

    elif test "$_pvm_root_specified" = "yes" ; then

        AC_MSG_ERROR([could not locate PVM libraries by way of 
PVM_ROOT/lib/<PVM_ARCH> ($PVMLIBDIR).  Either correct your PVM
installation, PVM_ROOT, or use the --with-pvm-libraries configure
option if your pvm libraries are installed in a non-standard
location.])

    else

    	AC_MSG_ERROR([check for PVM library failed.  You may wish
to try setting PVM_ROOT to the root of your PVM installation or use
the --with-pvm-libraries option to specify the directory containing
the PVM library.])
    fi
fi

])
