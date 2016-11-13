#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009-2011 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL <pierre.marechal@scilab.org>
# Copyright (C) Scilab Enterprises - 2014 - Clement DAVID <clement.david@scilab-enterprises.com>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

dnl Process this file with autoconf to produce a configure script.


AC_REVISION($Revision$)dnl
AC_INIT([Scilab],[6],[http://bugzilla.scilab.org/])
AC_PREREQ(2.69)
AC_CONFIG_MACRO_DIR([m4])

SCI_BUILDDIR="`pwd`"
SCI_SRCDIR="$srcdir"
SCI_SRCDIR_FULL="`cd $SCI_SRCDIR && pwd`"

if test -d "$SCI_SRCDIR_FULL/usr"; then
    WITH_DEVTOOLS=true
    DEVTOOLS_BINDIR="$SCI_SRCDIR_FULL/usr/bin"
    DEVTOOLS_INCDIR="$SCI_SRCDIR_FULL/usr/include"
    DEVTOOLS_LIBDIR="$SCI_SRCDIR_FULL/usr/lib"
else
    WITH_DEVTOOLS=false
    DEVTOOLS_BINDIR=
    DEVTOOLS_INCDIR=
    DEVTOOLS_LIBDIR=
fi

SCILAB_VERSION_MAJOR=6
SCILAB_VERSION_MINOR=0
SCILAB_VERSION_MAINTENANCE=0
AC_SUBST(SCILAB_VERSION_MAJOR)
AC_SUBST(SCILAB_VERSION_MINOR)
AC_SUBST(SCILAB_VERSION_MAINTENANCE)


SCILAB_LIBRARY_VERSION=$SCILAB_VERSION_MAJOR:$SCILAB_VERSION_MINOR:$SCILAB_VERSION_MAINTENANCE
AC_SUBST(SCILAB_LIBRARY_VERSION)

SCILAB_BINARY_VERSION=$SCILAB_VERSION_MAJOR.$SCILAB_VERSION_MINOR.$SCILAB_VERSION_MAINTENANCE
AC_SUBST(SCILAB_BINARY_VERSION)

#shared library versioning
#GENERIC_LIBRARY_VERSION=1:2:0
#                       | | |
#                +------+ | +---+
#                |        |     |
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed

# Check if we have a space in the path to the source tree
SPACE_IN_PATH=`echo "$SCI_SRCDIR_FULL"|grep " " > /dev/null; echo $?`
if test "$SPACE_IN_PATH" = "0"; then
   AC_MSG_WARN([=====================================])
   AC_MSG_WARN([Configure thinks that there is a space in the path to the source. This may cause problem with libtool and some other things...])
   AC_MSG_WARN([=====================================])
   sleep 180
fi

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([modules/core/includes/machine.h])

AC_PROG_LN_S
AM_PROG_AR

AC_CUSTOM_LARGE_FILE

# In order to be able to change the scilab directory
# See http://wiki.debian.org/RpathIssue
AC_RELOCATABLE
AC_RELOCATABLE_LIBRARY
AC_LIB_RPATH

# If configure detect that timestamp changed,
# it tries to rebuild configure & makefile which can be a painmaker
# if the version is different
AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE([-Wall foreign serial-tests subdir-objects]) # Not using -Werror because we override {C,F}FLAGS in order to disable optimisation

AC_CANONICAL_HOST

#################################
## all the --with-* argument help
#################################


AC_ARG_ENABLE(debug,
   AS_HELP_STRING([--enable-debug],[Do not optimize and print warning messages (C/C++/Fortran/Java code)]))

AC_ARG_ENABLE(debug-C,
   AS_HELP_STRING([--enable-debug-C],[Do not optimize and print warning messages (C code)]))

AC_ARG_ENABLE(debug-CXX,
   AS_HELP_STRING([--enable-debug-CXX],[Do not optimize and print warning messages (C++ code)]))

AC_ARG_ENABLE(debug-java,
   AS_HELP_STRING([--enable-debug-java],[Print warning messages and line numbers (Java code)]))

AC_ARG_ENABLE(debug-fortran,
   AS_HELP_STRING([--enable-debug-fortran],[Do not optimize and print warning messages (Fortran code)]))

AC_ARG_ENABLE(debug-linker,
   AS_HELP_STRING([--enable-debug-linker],[Print warning messages from the linker (ld)]))

AC_ARG_ENABLE(code-coverage,
   AS_HELP_STRING([--enable-code-coverage],[Enable code coverage]))

AC_ARG_ENABLE(stop-on-warning,
   AS_HELP_STRING([--enable-stop-on-warning],[Stop the compilation on the first warning found in the C/C++ code]))

AC_ARG_WITH(gcc,
    AS_HELP_STRING([--with-gcc],[Use gcc C compiler ]))

AC_ARG_WITH(gfortran,
    AS_HELP_STRING([--with-gfortran],[Use gfortran, GNU Fortran 95 compiler]))

AC_ARG_WITH(intelcompilers,
    AS_HELP_STRING([--with-intelcompilers],[Use Intel C (icc) and Fortran (ifort) proprietary compilers (GNU/Linux only) ]))

AC_ARG_WITH(tk,
    AS_HELP_STRING([--without-tk],[Disable the interface to Tcl/Tk ]))

AC_ARG_WITH(javasci,
    AS_HELP_STRING([--without-javasci],[Disable the Java/Scilab interface (javasci)]))

AC_ARG_ENABLE(compilation-tests,
    AS_HELP_STRING([--enable-compilation-tests],[Enable unitary tests and checks at compilation time]))

AC_ARG_WITH(gui,
    AS_HELP_STRING([--without-gui],[Disable the Scilab Graphical User Interface (GUI). Intended for embedded/clustering/grid Scilab ]))

AC_ARG_ENABLE(build-swig,
    AS_HELP_STRING([--enable-build-swig=path],[Regenerate Java => C and Scilab => C wrappers produces by Swig]))


AC_ARG_ENABLE(build-giws,
    AS_HELP_STRING([--enable-build-giws],[Regenerate C/C++ => Java wrappers produces by Giws]))

AC_ARG_ENABLE(build-parser,
    AS_HELP_STRING([--enable-build-parser],[Regenerate code for the parser (Lex & YACC)]))


################################################
########## compilator & misc programs detection
################################################

AC_PROG_CPP
AC_PROG_GREP

######
#### Mac OS X set of fink path with provided
######

AC_ARG_WITH(fink_prefix,
AS_HELP_STRING([--with-fink-prefix],[Provide a fink prefix. Default: /sw/ ]))
# Need MacosX Version to specify some path.
case "$host_os" in
     *Darwin* | *darwin*)
               AC_GET_MACOSX_VERSION
     ;;
esac

case "$host" in
    *darwin*)
        if test -n "$with_fink_prefix"; then
            # If with-fink-prefix is provided, use the provided path to make sure that we will
            # use it to detect dependencies (for example, gfortran is not provided in xcode.
            # Therefor, we use the one in fink)
            FINK_PREFIX="$with_fink_prefix/"

            # Exec the init script of fink to make sure that the env is set
            if test -f $with_fink_prefix/bin/init.sh; then
                echo "source $with_fink_prefix/bin/init.sh executed to set the env"
                source $with_fink_prefix/bin/init.sh
            else
                AC_MSG_WARN([Could not find the fink init.sh script: $with_fink_prefix/bin/init.sh])
            fi
        else
            FINK_PREFIX="/sw/"
        fi

        #Append to the default flags on Apple machines
        CPPFLAGS="$CPPFLAGS -I$FINK_PREFIX/include/"
        LDFLAGS="$LDFLAGS -L$FINK_PREFIX/lib/"
        ;;
esac

AC_ARG_WITH(min_macosx_version,
AS_HELP_STRING([--with-min-macosx-version],[Force compilers to generate binaries compatible with MacOSX minimal version.]))
case "$host" in
    *darwin*)
    if test -n "$with_min_macosx_version"; then
       MIN_MACOSX_VERSION=$with_min_macosx_version
       #Append to the default flags on Apple machines
       ARCH_CFLAGS="-mmacosx-version-min=$MIN_MACOSX_VERSION"
       ARCH_CXXFLAGS="-mmacosx-version-min=$MIN_MACOSX_VERSION"
       ARCH_FFLAGS="-mmacosx-version-min=$MIN_MACOSX_VERSION"
       ARCH_LDFLAGS="-mmacosx-version-min=$MIN_MACOSX_VERSION"
       # We need this to be passed to all linker commands
       LDFLAGS="$LDFLAGS -mmacosx-version-min=$MIN_MACOSX_VERSION"
    fi
;;
esac


#####################################################
## Look for pkg-config
#####################################################
PKG_PROG_PKG_CONFIG

#####################################################
## check if options are correct (or not)
#####################################################

if test "$with_intelcompilers" = yes -a "$with_gcc" = yes; then
    AC_MSG_ERROR([Conflicting options : you specified two compiler series])
fi

######## fortran ########

if test "$with_gfortran" = yes; then
   AC_PROG_F77(gfortran)
   if test -z "$F77"; then
     AC_MSG_ERROR([You asked me to use gfortran but i haven't been able to find it])
    fi
fi

if test "$with_intelcompilers" = yes; then
   AC_PROG_F77(ifc ifort)
   if test -z "$F77"; then
     AC_MSG_ERROR([You asked me to use ifc (intel fortran compiler) but i haven't been able to find it])
    fi
fi

if test -z "$F77"; then
## No Fortran compiler specified... Prefer gfortran and intel compilers
   AC_PROG_F77([gfortran ifc ifort])
   if test -z "$F77"; then
        # Best effort to find a compiler (might be g77)
    AC_PROG_F77
   fi
fi


# case statements were introduced in fortran 90 so we can use that
# as a test to see if our compiler is fortran 90 compatible.
f90compatible=false
if test -z "$F77"; then
  AC_MSG_ERROR([No fortran compiler found. Cannot compile scilab without a fortran compiler])
fi

AC_MSG_CHECKING([if $F77 is a fortran 90 compatible compiler])
 f90compatible=false
AC_LANG_PUSH([Fortran 77])
AC_COMPILE_IFELSE([
      PROGRAM hello
        do 50 i = 1, 5
           select case ( i )
              case (1)
                 print*, "case is 1, i is ", i
              case ( 2 : 3 )
                 print*, "case is 2 to 3, i is ", i
              case default
                 print*, "default case, i is ", i
              end select
 50           continue
      END
    ],
    [AC_MSG_RESULT([yes])
     AC_DEFINE([G95_FORTRAN],[],[uses G95 fortran])
     f90compatible=true
    ],
    [AC_MSG_RESULT([no])]
)
AC_LANG_POP([Fortran 77])

############ C ###############


if test "$with_gcc" = yes; then
   AC_PROG_CC(gcc)
   if test -z "$CC"; then
     AC_MSG_ERROR([You asked me to use gcc but i haven't been able to find it])
    fi
fi

if test "$with_intelcompilers" = yes; then
   AC_PROG_CC(icc ecc)
   if test -z "$CC"; then
     AC_MSG_ERROR([You asked me to use icc (intel C compiler) but I haven't been able to find it])
    fi
fi

if test -z "$CC"; then
# No C compiler specified... We rely on Autoconf to find the best
   AC_PROG_CC
fi

if test -z "$CC"; then
  AC_MSG_ERROR([No C Compiler found. Cannot compile Scilab without a C compiler])
fi

AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long])

### C++ ###
AC_PROG_CXX
# we can't just do something like
# AC_CHECK_PROG(cxx_present, $CXX, "yes", "no")
# because if the user has specified the full path of the desired C++ compiler then AC_CHECK_PROG
# will fail.  If AC_PROG_CXX fails to find a c++ compiler it will set CXX=g++ so just run AC_CHECK_PROG
# in this special case
case $CXX in
    g++)
        AC_CHECK_PROG([cxx_present], [$CXX], [yes], [no])
        if test "x$cxx_present" != "xyes"; then
            AC_MSG_ERROR([No C++ compiler found. Cannot compile scilab without a C++ compiler])
        fi
        ;;
esac

# for "subdir-objects"

AM_PROG_CC_C_O
AC_PROG_F77_C_O


if test "$enable_debug" = yes; then
   enable_debug_fortran=yes
   enable_debug_C=yes
   enable_debug_CXX=yes
   enable_debug_java=yes
else
   enable_debug=no
fi

if test "$enable_debug_fortran" = yes; then
   FFLAGS="`echo "$FFLAGS"| sed -e 's|-O[0-9+]|-O0|'`"
else
   enable_debug_fortran=no
fi

if test "$enable_debug_C" = yes; then
   CFLAGS="`echo "$CFLAGS"| sed -e 's|-O[0-9+]|-O0|'`"
else
   enable_debug_C=no
fi

if test "$enable_debug_CXX" = yes; then
   CXXFLAGS="`echo "$CXXFLAGS"| sed -e 's|-O[0-9+]|-O0|'`"
else
   enable_debug_CXX=no
fi

if test "$enable_debug_java" = yes; then
   LOGGING_LEVEL="INFO"
else
   LOGGING_LEVEL="SEVERE"
fi
AC_SUBST(LOGGING_LEVEL)

if test "x${prefix}" = "xNONE"; then
  prefix="${ac_default_prefix}"
fi

###############################
## get the version
###############################

SCIVERSION=`cat $SCI_SRCDIR/Version.incl | sed -e "s/SCIVERSION=//" `

#############################################
## Compilers and options according to machine
#############################################

######################
######## Set compilation options for intel C/Fortran compilers
######################

if test "$with_intelcompilers" = yes; then
   SCI_INTEL_COMPILER()
fi

########### FORTRAN ######################


######################
######## With gfortran ...
######################
if test -n "$F77"; then
   case "$F77" in
     gfortran*)
        ## With GNU Compiler enable the code coverage
        if test "$enable_code_coverage" = yes; then
            CODECOVERAGE_FFLAGS="-fprofile-arcs -ftest-coverage"
        fi
     ;;
     g77*)
        AC_MSG_ERROR([g77 is no longer supported. Please consider switching to gfortran])
     ;;
   esac

   if test "$enable_debug_fortran" = yes; then
    AC_LANG_PUSH([Fortran 77])
    for flag in -g -Wall -Wsurprising; do
        case " ${FFLAGS} " in
            *\ ${flag}\ *)
                # flag is already present
                ;;

            *)
                AC_MSG_CHECKING([if the Fortran compiler accepts ${flag}])
                ac_save_FFLAGS="$FFLAGS"
                FFLAGS="$FFLAGS ${flag}"
                AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
                    [AC_MSG_RESULT([yes])
                     DEBUG_FFLAGS="${flag}"
                    ],
                    [AC_MSG_RESULT([no])]
                )
                ;;
        esac
    done
    AC_LANG_POP([Fortran 77])
   else
      DEBUG_FFLAGS="-DNDEBUG"
   fi
      case "$host" in
         x86_64-*-linux*)
            ARCH_FFLAGS="-m64 -fPIC"
                    ;;
        # Dec Alpha OSF 4
        alpha*-dec-osf4.*)
          ARCH_FFLAGS="-fpe3"
        ;;
        alpha*-dec-osf*)
            ARCH_FFLAGS="-fpe3 -switch nosqrt_recip"
        ;;
        rs6000-*-*)
            ARCH_FFLAGS="-qcharlen=4096"
            ;;
        mips-*-ultrix*)
            ARCH_FFLAGS="-O0 -fpe1"
        ;;
          *-*-hpux9.*)
            ARCH_FFLAGS="+Obb1200 +E4 -Dhpux"
        ;;
          *-*-hpux10.*)
            if test "$enable_debug_fortran" = yes; then
                ARCH_FFLAGS="+E4 +Z +DAportable -Dhpux"
            else
                ARCH_FFLAGS="+O2 +E4 +Z +DAportable -Dhpux"
            fi
#            ARCH_LDFLAGS="-Wl,+vnocompatwarnings,-E /usr/lib/libdld.sl"

            ;;
          *-*-hpux11.*)
           if test "$enable_debug_fortran" = yes; then
              ARCH_FFLAGS=" +Z +DAportable -Dhpux"
             else
              ARCH_FFLAGS="+O2 +Z +DAportable -Dhpux"
           fi
           ARCH_LDFLAGS="-ldld -lnsl -lU77 -lm"

     ;;
      esac

fi

#########################
# setting parameters according to system types
#########################

case "$host" in
    *-*-hpux9.*|*-*-hpux10.*|*-*-hpux11.*)
        HPUX=1
    ;;
    sparc-*)
        SPARC=1
    ;;
    mips-sgi-irix*)
      MIPS_SGI_IRIX=1
    ;;
     *-*-solaris*)
     SOLARIS=1
     ;;
    *darwin*)
      MACOSX=1
    ;;
esac

AM_CONDITIONAL(IS_MACOSX, test -n "$MACOSX")
AM_CONDITIONAL(IS_HPUX, test -n "$HPUX")
AM_CONDITIONAL(IS_SPARC, test -n "$SPARC")
AM_CONDITIONAL(IS_SOLARIS, test -n "$SOLARIS")
AM_CONDITIONAL(IS_MIPS_SGI_IRIX, test -n "$MIPS_SGI_IRIX")



############
## C++
############


if test -z "$CXX"; then
  AC_MSG_ERROR([No C++ compiler found. Cannot compile scilab without a C++ compiler])
fi

case "$CXX" in
    g++-* | g++ | ccache*g++ | ccache*g++-* )
        ## With GNU C++ Compiler

        # enable the code coverage
        if test "$enable_code_coverage" = yes; then
            CODECOVERAGE_CXXFLAGS="-fprofile-arcs -ftest-coverage"
        fi

        if test "$enable_debug_CXX" = yes; then
            DEBUG_CXXFLAGS="-Wall -Wextra -O0 -g3"
            case "$host" in
                *-linux-gnu )
                    # Only doing that under Linux
                    DEBUG_CXXFLAGS="$DEBUG_CXXFLAGS -fdiagnostics-show-option -Werror=format-security"
                ;;
            esac
        else
            DEBUG_CXXFLAGS="-DNDEBUG"
        fi
        COMPILER_CXXFLAGS="$COMPILER_CXXFLAGS -fno-stack-protector " # bug 3131
    ;;

esac

#### 64 bits detection
IS_64_BITS_CPU=false
case "$host" in
         x86_64-*-linux-gnu | x86_64-linux-gnu | ia64-*-linux-gnu | alpha-*-linux-gnu | alpha-*-netbsd* | x86_64-*-netbsd* | sparc64-*-netbsd*)
                IS_64_BITS_CPU=true
    ;;
esac

##########"

case "$CC" in
     gcc-* | gcc | ccache*gcc | ccache*gcc-* )
   ## With GNU Compiler

   # enable the code coverage
   if test "$enable_code_coverage" = yes; then
    CODECOVERAGE_CFLAGS="-fprofile-arcs -ftest-coverage"
   fi

   if test "$enable_debug_C" = yes; then
      DEBUG_CFLAGS="-Wall -Wextra -O0 -g3"
      # used to be -O0
      case "$host" in
      *-linux-gnu )
      # Only doing that under Linux
        if test "$enable_debug_linker" = yes; then
            LDFLAGS="$LDFLAGS -Wl,--warn-common,-x"
        fi
          DEBUG_CFLAGS="$DEBUG_CFLAGS -fdiagnostics-show-option -Werror=format-security"
          ;;
          esac
   else
      DEBUG_CFLAGS="-DNDEBUG"
   fi

   COMPILER_CFLAGS="$COMPILER_CFLAGS -fno-stack-protector " # bug 3131
   # Explictly disable the as needed. It was disable by default but Ubuntu
   # activated it by default since release 11.04. See bug #8961.
   # Once all cyclic dependencies have been dropped, this line could be removed.
   # Check if linker supports --as-needed and --no-as-needed options
   if $LD --help 2>/dev/null | grep no-as-needed > /dev/null; then
      LDFLAGS="$LDFLAGS -Wl,--no-as-needed"
   fi


   case "$host" in
     x86_64-*-linux-gnu | x86_64-linux-gnu)
        ARCH_CFLAGS="-m64"
    ;;
     alpha-*-linux-gnu)
        ARCH_CFLAGS="-mieee-with-inexact"
        ARCH_LDFLAGS="-mieee-with-inexact"
    ;;
     powerpc-*-linux-gnu)
        ARCH_CFLAGS="-D_GNU_SOURCE"
    ;;
     *-*-solaris*)
        ARCH_CFLAGS="-DSVR4 -DSYSV -Dsolaris"
    ;;
   *-*-freebsd*)
     ARCH_CFLAGS="-Dfreebsd"
     ARCH_LDFLAGS="-lm"
     ;;

  alpha-*-netbsd*)
    ARCH_CFLAGS="-Dnetbsd -mieee"
    ARCH_FFLAGS="-Dnetbsd -mieee"
    ;;

  *-*-netbsd*)
    ARCH_CFLAGS="-Dnetbsd"
    ;;

  *-*-darwin*)
    # Flag no more added since :
    #  - No more supported in recent gcc versions (> 4.6)
    #  - only needed when compiling with flag -mmacosx-min-version=10.5
    #ARCH_CFLAGS="$ARCH_CFLAGS -no_compact_linkedit"
    #ARCH_CXXFLAGS="$ARCH_CXXFLAGS -no_compact_linkedit"
    #ARCH_LDFLAGS="$ARCH_LDFLAGS -no_compact_linkedit"
    # We need this to be passed to all linker commands
    #LDFLAGS="$LDFLAGS -no_compact_linkedit"

    case "$F77" in
         gfortran-*|gfortran)
             # Extract from gfortran -v the version it has been built for
               MAC_DETECTED_ARCH="`$F77 -v 2>&1|grep "Target:"|sed -e "s/Target: \([[a-z0-9A-Z_]]*\).*/\1/g"`"
         ;;
         *)
        AC_MSG_WARN([gfortran not used. Could not detect the architecture. Switch to the default case: x86_64])
        MAC_DETECTED_ARCH="x86_64"
         ;;
    esac
    CC="$CC -arch $MAC_DETECTED_ARCH"
    CXX="$CXX -arch $MAC_DETECTED_ARCH"
    ;;
esac
;; # end of the gcc case on the $CC
   *)

        ## CC compiler (not GCC)
        if test "$enable_debug_C" != yes; then
             ARCH_CFLAGS="-DNDEBUG"
        fi

case "$host" in
# Dec Alpha OSF 4
  alpha*-dec-osf4.*)
      ARCH_CFLAGS="-std -ieee_with_inexact"
      ARCH_LDFLAGS="-ieee_with_inexact"
      ;;
  alpha*-dec-osf*)
      ARCH_CFLAGS="-ieee_with_inexact"
      ARCH_LDFLAGS="-ieee_with_inexact"
      ;;
  rs6000-*-*) # IBM AIX RS 6000 (NO LONGER SUPPORTED)
    ARCH_CFLAGS="-Daix -DSYSV"
  ;;
  mips-sgi-irix*) # SGI
    ARCH_CFLAGS="-DSYSV -DSVR4"
  ;;
  *-*-hpux9.*)
    ARCH_CFLAGS="-DSYSV -Dhpux"
  ;;
  *-*-hpux10.*) # HP 10
    if test "$enable_debug_C" = yes; then
      ARCH_CFLAGS="-DSYSV -Dhpux"
    else
      ARCH_CFLAGS="-DSYSV -Dhpux +Z +DAportable"
    fi
  ;;

  *-*-hpux11.*) # HP 11
    ARCH_CFLAGS="-DSYSV -Dhpux -Dhppa +Z +DAportable"
  ;;
  *-*-solaris*) # SUN SOLARIS
    ARCH_CFLAGS="-DSVR4 -DSYSV -Dsolaris -I/usr/local/include/"
    ARCH_LDFLAGS="-L/usr/local/lib/"
  ;;
esac
;; # end of not a gcc compiler
esac


# enable the code coverage
if test "$enable_code_coverage" = yes; then
   AC_PATH_PROG(LCOV, lcov)
   if test -z "$LCOV" ; then
        AC_MSG_ERROR([Cannot find lcov. Please install it (package lcov under Debian) or remove the option --enable-code-coverage])
   fi
   AC_PATH_PROG(GENHTML, genhtml)
   if test -z "$GENHTML" ; then
        AC_MSG_ERROR([Cannot find genhtml. Please install it (package lcov under Debian) or remove the option --enable-code-coverage])
   fi
   CODECOVERAGE_LDFLAGS="-coverage -lgcov"
fi
AM_CONDITIONAL(CODE_COVERAGE, test "$enable_code_coverage" = yes)
AM_CONDITIONAL(ENABLE_DEBUG, test "$enable_debug" = yes)

# flag for possible compilations in configure
SCILIBS_CFLAGS=''
SCILIBS_CXXFLAGS=''
SCILIBS_FFLAGS=''

if test $IS_64_BITS_CPU = true -o "$MACOSX" = "1"; then
   if test $f90compatible = false; then
      AC_MSG_ERROR([64 bits support needs a fortran 90 compiler (try --with-gfortran).])
   fi
fi

AM_CONDITIONAL(IS_64_BITS_CPU, test $IS_64_BITS_CPU = true)

# check if the compiler supports c++11
saved_cppflags="$CXXFLAGS"
CXXFLAGS=""
AX_CXX_COMPILE_STDCXX_11(noext,mandatory)
STDCXX_11_CXXFLAGS="$CXXFLAGS"
CXXFLAGS="$saved_cppflags"

################
## SSE
## By default, for x86 CPU, enable the SSE2.
## Note that it is already the case with 64 bits CPU
## (these extensions are enabled by default by gcc)
################

case "$host" in
     i*86-linux-gnu|i*86-*-linux-gnu)
        SSE_CFLAGS="-msse2"
        SSE_FFLAGS="-msse2"
        SSE_CXXFLAGS="-msse2"
     ;;
esac

#######################
## test for underscores (name mangling issues between C and fortran)
#######################
AC_CHECK_UNDERSCORE_FORTRAN()

###############################
### Regenerate the parser files
### Flex & Yacc
###############################

if test "$enable_build_parser" = yes; then

   AC_PROG_LEX
   if test -z "$LEX" -o "$LEX" = ":"; then
	  AC_MSG_ERROR([No implementation of Lex (flex, lex) detected. Please install it (package flex)])
   fi

   AC_PROG_YACC
   if test -z "$YACC"; then
	  AC_MSG_ERROR([No implementation of Yacc (bison, yacc) detected. Please install it (package bison)])
  fi
# Actually, AC_PROG_YACC is lying. It sometimes put yacc into $YACC even it
# hasn't been able to find it.
#  AC_CHECK_PROG(YACC_PRESENT, $YACC, AC_MSG_ERROR([No implementation of Yacc (bison, yacc) detected. Please install it (package bison)]) )

   AC_CHECK_PROG(yacc_present, $YACC, "yes","no")
   if test "x$yacc_present" != "xyes"; then
      AC_MSG_ERROR([No implementation of Yacc (bison, yacc) detected. Please install it (package bison)])
   fi

fi
AM_CONDITIONAL(BUILD_PARSER, test "$enable_build_parser" = yes)

#################
## HDF5
#################

AC_HDF5()
# The Java detection is done after in this file.


#################
## scirenderer
#################

AC_ARG_WITH(external-scirenderer,
    AS_HELP_STRING([--with-external-scirenderer],[Disable the usage of the internal copy of scirenderer. Intended for packaging of Scilab]))

AM_CONDITIONAL(EXTERNAL_SCIRENDERER, test "$with_external_scirenderer" = yes)


#################
## XCOS
#################

AC_ARG_WITH(xcos,
    AS_HELP_STRING([--without-xcos], [Disable Xcos]))

AC_ARG_WITH(modelica,
    AS_HELP_STRING([--without-modelica],[Disable the OCaml module (modelica)]))


if test "$with_gui" = no; then
   AC_MSG_WARN([GUI is disabled. Disabling then Xcos GUI])
fi

XCOS_ENABLE=no

if test "$with_xcos" != no; then
   AC_DEFINE([WITH_XCOS], [], [with XCos])

   save_LIBS="$LIBS"

   AC_CHECK_LIB([rt], [clock_gettime],
              [RT_LIB="-lrt";
              AC_DEFINE([HAVE_CLOCK_GETTIME], [1],[Whether clock_gettime is available]) ],
        [AC_MSG_WARN([librt: library missing (Cannot find symbol clock_gettime). Check if librt is installed (it is usually provided by the libc) and if the version is correct])]
   )
   LIBS="$save_LIBS"
   AC_SUBST(RT_LIB)


#################
## ocaml which only called when using Xcos
#################

   if test "$with_modelica" != no; then
      AC_CHECK_PROG_OCAML()
   fi
   XCOS_ENABLE=yes

fi

AC_SUBST(XCOS_ENABLE)

AM_CONDITIONAL(OCAML, test "$with_modelica" != no -a "$OCAMLC" != no -a "$OCAMLOPT" != no)
AM_CONDITIONAL(XCOS, test "$XCOS_ENABLE" != no)

###########################
## test for JAVA compiler
###########################
if test "$with_javasci" != no -o "$with_gui" != no -o "$enable_build_help" != no; then

     # See if --with-jdk command line argument is given
     # Try to detect the installed JVM, this could be controlled
     # by the above --with options
     AC_JAVA_WITH_JDK
     if test "$with_jdk" != no; then

     AC_JAVA_DETECT_JVM
     case "$ac_java_jvm_version" in
        1.8)
        ;;
        *)
            AC_MSG_ERROR([Wrong version of Java. Expected at least 1.8. Found $ac_java_jvm_version])
        ;;
     esac

   if test "$ac_java_jvm_name" = "jdk"; then
        JAVA_HOME=$ac_java_jvm_dir
        JAVA_VERSION=$ac_java_jvm_version
#     AC_JAVA_TOOLS

        AC_JAVA_JNI_INCLUDE
        JAVA_JNI_INCLUDE=$ac_java_jvm_jni_include_flags

        case "$host_os" in
             *Darwin* | *darwin*)
               # Mac OS X does not link against the lib but uses -framework
               JAVA_JNI_LIBS="-framework JavaVM"
             ;;
             *)
               AC_JAVA_JNI_LIBS
               JAVA_JNI_LIBS=$ac_java_jvm_jni_lib_flags
               JAVA_JNI_LIBS_PRELOAD=$ac_java_jvm_ld_preload
             ;;
        esac

        AC_JAVA_CLASSPATH
        JAVA_CLASSPATH=$ac_java_classpath
        AC_JAVA_TOOLS
        AC_JAVA_ANT

        case "$host_os" in
             *Darwin* | *darwin*)
                  JAVA_HOME=""
             ;;
        esac


        if test "$with_gui" != no; then

                if test "$with_xcos" != no; then
                   # jgraphx
                   AC_JAVA_CHECK_JAR([jgraphx],[com.mxgraph.view.mxGraph],[Diagram design],[],[VERSION],[2.0.0.1])
                   JGRAPHX=$PACKAGE_JAR_FILE
                   AC_SUBST(JGRAPHX)
                fi

                if test "$with_external_scirenderer" = yes; then
                   # scirenderer
                   AC_JAVA_CHECK_JAR([scirenderer],[org.scilab.forge.scirenderer.PackageInfo],[Scilab Renderer],[],[VERSION],[1.1.0])
                   SCIRENDERER=$PACKAGE_JAR_FILE
                   SCIRENDERER_CP=$PACKAGE_JAR_FILE
                 else
                   echo "Use external version of scirenderer"
                   # Use the scirenderer in Scilab sources
                   SCIRENDERER="\${modules.dir}/scirenderer/\${build.jar.dir}/scirenderer.jar"
                   SCIRENDERER_CP="\$SCILAB/modules/scirenderer/jar/scirenderer.jar"
                fi
                AC_SUBST(SCIRENDERER)
                AC_SUBST(SCIRENDERER_CP)

               # Docking system
                AC_JAVA_CHECK_JAR([flexdock],[org.flexdock.util.Utilities],[Scilab Gui],[],[VERSION],[1.2.4])
                FLEXDOCK=$PACKAGE_JAR_FILE
                AC_SUBST(FLEXDOCK)

                # Swing look&feel implementations
                AC_JAVA_CHECK_JAR([looks],[com.jgoodies.looks.common.MenuItemRenderer],[Scilab Gui - Look and feel],"yes")
                LOOKS=$PACKAGE_JAR_FILE

                # Named differently under ArchLinux or Fedora
                if test -z "$LOOKS"; then
                    AC_JAVA_CHECK_JAR([jgoodies-looks],[com.jgoodies.looks.common.MenuItemRenderer],[Scilab Gui - Look and feel])
                    LOOKS=$PACKAGE_JAR_FILE
                fi
                AC_SUBST(LOOKS)

                #  Skin Look and Feel
                AC_JAVA_CHECK_JAR([skinlf],[com.l2fprod.util.AccessUtils],[Scilab Gui - Skin Look and Feel])
                SKINLF=$PACKAGE_JAR_FILE
                AC_SUBST(SKINLF)

                # JOGL 2
                AC_JAVA_CHECK_JAR([jogl2],[javax.media.opengl.glu.GLUnurbs],[Scilab 3D rendering - Version 2.0])
                JOGL2=$PACKAGE_JAR_FILE
                AC_SUBST(JOGL2)

                AC_JAVA_CHECK_VERSION_MANIFEST([jogl2],$JOGL2,[2.2],[Specification-Version])

                if test "$MACOSX" = 1; then
                   echo "Check of the presence of libjogl.jnilib and libjogl_awt.jnilib disabled under Mac OS X"
                else
                   LDFLAGS_save=$LDFLAGS
                   # Provide known paths where distribs/OS can store JNI libs
                   LDFLAGS="$LDFLAGS -L/usr/lib/jni -L/usr/lib64/jni"                   # Debian
                   LDFLAGS="$LDFLAGS -L/usr/lib/java -L/usr/lib64/java"                 # jpackage.org
                   LDFLAGS="$LDFLAGS -L/usr/lib/jogl2 -L/usr/lib64/jogl2"               # RedHat
                   LDFLAGS="$LDFLAGS -L$SCI_SRCDIR/thirdparty -L$SCI_SRCDIR/lib/thirdparty -L$SCI_SRCDIR/bin"        # Scilab thirdparties
                   AC_CHECK_LIB([jogl_desktop], [glTexParameterf], [JOGL2_LIBS="-ljogl_desktop"],[AC_MSG_WARN([Could not link against -ljogl_desktop. Will try against -ljogl_desktop -lGL])])
                   if test -z "$JOGL2_LIBS"; then # The previous test failed add more options to the LDFLAGS
                       # the space after "jogl" in the following line is on
                       # purpose to disable the cache
                          AC_CHECK_LIB([jogl_desktop ], [glTexParameterf],
                       [JOGL2_LIBS="-ljogl_desktop -lGL"],
                       [AC_MSG_ERROR(["libjogl: Library
missing (Cannot find symbol glTexParameterf). Check if libjogl - C/Java (JNI)
interface for JOGL2 - or libGL (OpenGL library) are installed and if the version is correct. Note that you might have to update etc/librarypath.xml to provide the actual path to the JNI libraries."])],
                       [-lGL])
                   fi

                   LDFLAGS=$LDFLAGS_save
                fi

                # JoGL Native <=> Java connector
                AC_JAVA_CHECK_JAR([gluegen2-rt],[jogamp.common.os.MachineDescriptionRuntime],[Scilab 3D rendering])
                GLUEGEN2_RT=$PACKAGE_JAR_FILE
                AC_SUBST(GLUEGEN2_RT)

                if test "$MACOSX" = 1; then
                   echo "Check of the presence of libgluegen-rt.jnilib disabled under Mac OS X"
                else
                   LDFLAGS_save=$LDFLAGS
                   # Provide known paths where distribs/OS can store JNI libs
                   LDFLAGS="$LDFLAGS -L/usr/lib/jni -L/usr/lib64/jni"                   # Debian
                   LDFLAGS="$LDFLAGS -L/usr/lib/java -L/usr/lib64/java"                 # jpackage.org
                   LDFLAGS="$LDFLAGS -L/usr/lib/gluegen2 -L/usr/lib64/gluegen2"         # RedHat
                   LDFLAGS="$LDFLAGS -L$SCI_SRCDIR/thirdparty -L$SCI_SRCDIR/lib/thirdparty -L$SCI_SRCDIR/bin"        # Scilab thirdparties
                   symbol="Java_jogamp_common_jvm_JVMUtil_initialize"
                   AC_CHECK_LIB([gluegen2-rt], [$symbol], [GLUEGEN2_RT_LIBS="-lgluegen2-rt"],
                   [AC_MSG_ERROR([libgluegen2-rt: Library missing (Cannot find symbol $symbol). Check if libgluegen-rt - C/Java (JNI) interface for GLUEGEN2 - is installed and if the version is correct. Note that you might have to update etc/librarypath.xml to provide the actual path to the JNI libraries.])],
                   [-ldl])
                   LDFLAGS=$LDFLAGS_save
                fi

                # Jhall
                AC_JAVA_CHECK_JAR([jhall],[javax.help.JHelp],[Scilab Help Browser],"yes")
                # Named differently under Mandriva or Fedora
                if test -z "$PACKAGE_JAR_FILE"; then
                    AC_JAVA_CHECK_JAR([javahelp2],[javax.help.JHelp],[Scilab Help Browser])
                fi
                JHALL=$PACKAGE_JAR_FILE
                AC_SUBST(JHALL)

                # Console API
                AC_JAVA_CHECK_JAR([jrosetta-API],[com.artenum.rosetta.interfaces.core.ConsoleConfiguration],[JRosetta : Console API Artenum / Scilab],"yes")
                if test -z "$PACKAGE_JAR_FILE"; then
                    AC_JAVA_CHECK_JAR([jrosetta-api],[com.artenum.rosetta.interfaces.core.ConsoleConfiguration],[JRosetta : Console API Artenum / Scilab])
                fi
                JROSETTA_API=$PACKAGE_JAR_FILE
                AC_SUBST(JROSETTA_API)

                # Console Core
                AC_JAVA_CHECK_JAR([jrosetta-engine],[com.artenum.rosetta.util.ConfigurationBuilder],[JRosetta : Console Core Artenum / Scilab],[],[getVersion],[1.0.4])
                JROSETTA_ENGINE=$PACKAGE_JAR_FILE
                AC_SUBST(JROSETTA_ENGINE)


                # MathML rendering solution
                # Used in both graphic & help
                AC_JAVA_CHECK_JAR([jeuclid-core],[net.sourceforge.jeuclid.LayoutContext],[MathML rendering solution (at least version 3.1.X)])
                JEUCLID_CORE=$PACKAGE_JAR_FILE
                AC_SUBST(JEUCLID_CORE)


                ################ Mandatory for graphic_export features #####
                # XML to PDF/other Translator
                AC_JAVA_CHECK_JAR([fop],[org.apache.fop.pdf.PDFInfo],[XML to PDF Translator (fop)])
                FOP=$PACKAGE_JAR_FILE
                AC_SUBST(FOP)

                AC_ARG_WITH(emf, [AS_HELP_STRING([--without-emf],
                    [disable support for EMF (Windows Metafile) export])])
                if test "x$with_emf" != xno; then
                    # Freehep Graphics2D
                    AC_JAVA_CHECK_JAR([freehep-graphics2d],[org.freehep.graphics2d.VectorGraphics],[Freehep Graphics2D])
                    FREEHEP_GRAPHICS2D=$PACKAGE_JAR_FILE
                    AC_SUBST(FREEHEP_GRAPHICS2D)

                    # Freehep GraphicsIO
                    AC_JAVA_CHECK_JAR([freehep-graphicsio],[org.freehep.graphicsio.VectorGraphicsIO],[Freehep GraphicsIO])
                    FREEHEP_GRAPHICSIO=$PACKAGE_JAR_FILE
                    AC_SUBST(FREEHEP_GRAPHICSIO)

                    # Freehep GraphicsIO EMF
                    AC_JAVA_CHECK_JAR([freehep-graphicsio-emf],[org.freehep.graphicsio.emf.EMFGraphics2D],[Freehep GraphicsIO EMF])
                    FREEHEP_GRAPHICSIO_EMF=$PACKAGE_JAR_FILE
                    AC_SUBST(FREEHEP_GRAPHICSIO_EMF)

                    # Freehep IO
                    AC_JAVA_CHECK_JAR([freehep-io],[org.freehep.util.io.XMLSequence],[Freehep IO])
                    FREEHEP_IO=$PACKAGE_JAR_FILE
                    AC_SUBST(FREEHEP_IO)

                    # Freehep Util
                    AC_JAVA_CHECK_JAR([freehep-util],[org.freehep.util.StringUtilities],[Freehep Util])
                    FREEHEP_UTIL=$PACKAGE_JAR_FILE
                    AC_SUBST(FREEHEP_UTIL)
                fi

                # xml.apache.org SVG Library (under mandriva for example)
                AC_JAVA_CHECK_JAR([batik-all],[org.apache.batik.Version],
                                  [Apache SVG Library],"yes",[getVersion],[1.8])
                BATIK=$PACKAGE_JAR_FILE

                if test -z "$BATIK"; then
                   # Other other distribs
                   AC_JAVA_CHECK_JAR([batik],[org.apache.batik.Version],
                                     [Apache SVG Library],[],[getVersion],[1.8])
                   BATIK=$PACKAGE_JAR_FILE
                fi
                AC_SUBST(BATIK)

                # Commons I/O library
                AC_JAVA_CHECK_JAR([commons-io],[org.apache.commons.io.output.CountingOutputStream],[Commons I/O library])
                COMMONS_IO=$PACKAGE_JAR_FILE
                AC_SUBST(COMMONS_IO)

                # XML graphics common
                AC_JAVA_CHECK_JAR([xmlgraphics-commons],[org.apache.xmlgraphics.util.Service],[Commons graphics library])
                XMLGRAPHICS_COMMONS=$PACKAGE_JAR_FILE
                AC_SUBST(XMLGRAPHICS_COMMONS)

                # Avalon Framework (PDF)
                AC_JAVA_CHECK_JAR([avalon-framework],[org.apache.avalon.framework.configuration.ConfigurationException],[Common framework for Java server application])
                AVALON_FRAMEWORK=$PACKAGE_JAR_FILE
                AC_SUBST(AVALON_FRAMEWORK)

                # XML API EXT (conversion of a SVG => PNG)
                AC_JAVA_CHECK_JAR([xml-apis-ext],[org.w3c.dom.svg.SVGDocument],[XML Commons external code],"yes")
                XML_APIS_EXT=$PACKAGE_JAR_FILE

                if test -z "$XML_APIS_EXT"; then
                    # Other other distribs (Ex: Fedora/Redhat)
                    AC_JAVA_CHECK_JAR([xml-commons-apis-ext],[org.w3c.dom.svg.SVGDocument],[XML Commons external code])
                    XML_APIS_EXT=$PACKAGE_JAR_FILE
                fi
                AC_SUBST(XML_APIS_EXT)

                ################ END Mandatory for graphic_export features #####


                # Logging (flexdock dep)
                AC_JAVA_CHECK_JAR([commons-logging],[org.apache.commons.logging.LogFactory],[Apache logging])
                COMMONS_LOGGING=$PACKAGE_JAR_FILE
                AC_SUBST(COMMONS_LOGGING)

                # JLaTeXMath
                AC_JAVA_CHECK_JAR([jlatexmath],[org.scilab.forge.jlatexmath.TeXFormula],[LaTex Rendering],[],[VERSION],[1.0.3])
                JLATEXMATH=$PACKAGE_JAR_FILE
                AC_SUBST(JLATEXMATH)

                # JLaTeXMath FOP
                AC_JAVA_CHECK_JAR([jlatexmath-fop],[org.scilab.forge.jlatexmath.fop.JLaTeXMathObj],[LaTex Rendering - FOP plugin])
                JLATEXMATH_FOP=$PACKAGE_JAR_FILE
                AC_SUBST(JLATEXMATH_FOP)

                AC_DEFINE([WITH_GUI],[],[With the JAVA stuff (GUI, Console, JOGL...)])

        fi

        # Checkstyle (code checking)
        AC_JAVA_CHECK_JAR([checkstyle],[com.puppycrawl.tools.checkstyle.CheckStyleTask],[Checkstyle - code checking],"yes")
        CHECKSTYLE=$PACKAGE_JAR_FILE
        AC_SUBST(CHECKSTYLE)

        # Commons beanutils (dependency of checkstyle)
        AC_JAVA_CHECK_JAR([commons-beanutils],[org.apache.commons.beanutils.Converter],[Bean utility],"yes")
        COMMONS_BEANUTILS=$PACKAGE_JAR_FILE
        AC_SUBST(COMMONS_BEANUTILS)

        # antlr (dependency of checkstyle)
        AC_JAVA_CHECK_JAR([antlr],[antlr.TokenStreamException],[language tool for constructing recognizers],"yes")
        ANTLR=$PACKAGE_JAR_FILE
        AC_SUBST(ANTLR)

        # Junit 4 (java unitary test)
        AC_JAVA_CHECK_JAR([junit4],[org.junit.Assert],[Junit4 - Unit tests],"yes")
        if test -z "$PACKAGE_JAR_FILE"; then
                AC_JAVA_CHECK_JAR([junit],[org.junit.Assert],[Junit4 -
Unit tests],"yes")
        fi
        JUNIT4=$PACKAGE_JAR_FILE
        AC_SUBST(JUNIT4)
        
        # hamcrest (junit4 dependency)
        AC_JAVA_CHECK_JAR([hamcrest-all],[org.hamcrest.core.IsNull],[Hamcrest- library matchers],"yes")
        if test -z "$PACKAGE_JAR_FILE"; then
            AC_JAVA_CHECK_JAR([hamcrest/all],[org.hamcrest.core.IsNull],[Hamcrest- library matchers],"yes")
        fi
        HAMCREST=$PACKAGE_JAR_FILE
        AC_SUBST(HAMCREST)

        # Cobertura (java code coverage)
        AC_JAVA_CHECK_JAR([cobertura],[net.sourceforge.cobertura.merge.Main],[cobertura - Java code coverage],"yes")
        COBERTURA=$PACKAGE_JAR_FILE
        AC_SUBST(COBERTURA)

        # ASM (a dependency of Cobertura)
        AC_JAVA_CHECK_JAR([asm3],[org.objectweb.asm.Type],[Java bytecode manipulation (dep of cobertura)],"yes")
        if test -z "$ASM3"; then
                AC_JAVA_CHECK_JAR([asm],[org.objectweb.asm.Type],[Java bytecode manipulation (dep of cobertura)],"yes")
        fi
        ASM3=$PACKAGE_JAR_FILE
        AC_SUBST(ASM3)

        AC_JAVA_CHECK_JAR([ecj],[org.eclipse.jdt.core.compiler.batch.BatchCompiler],[Eclipse Java compiler],"yes")
        ECJ=$PACKAGE_JAR_FILE
        if test -z "$ECJ"; then
           ECJ='$SCILAB/thirdparty/ecj.jar'
        fi
        AC_SUBST(ECJ)

    else
        AC_MSG_WARN([Sun javac not found: I will not build the java interface])
        if test "$ac_java_jvm_name" != ""; then
            AC_MSG_WARN([We do not support $ac_java_jvm_name yet])
        fi
    fi
    AC_SUBST(JAVA_JNI_INCLUDE)
    AC_SUBST(JAVA_JNI_LIBS)
    AC_SUBST(JAVA_HOME)


    if test "$enable_debug_java" = yes; then
        JAVAC_DEBUG="on"
    else
        JAVAC_DEBUG="off"
    fi
    AC_SUBST(JAVAC_DEBUG)

    if test "$enable_build_swig" != no -a "$enable_build_swig" != ""; then
        SWIG_PROG()
        SWIG_ENABLE_JAVA()
        SWIG_ENABLE_SCILAB()
        AC_SUBST(SWIG_BIN)
        AC_SUBST(SWIG_JAVA)
        AC_SUBST(SWIG_SCILAB)

    fi

    # Giws is the equivalent of Swig developed by the Scilab team
    # in order to provide a wrapper to Java from C/C++
    if test "$enable_build_giws" != no -a "$enable_build_giws" != ""; then
        AC_GIWS([1.3.0])
    fi

    fi # "$with_jdk" != no;
fi

JAVA_ENABLE=yes
if test -z "$JAVAC"; then
    JAVA_ENABLE=no
fi

AC_SUBST(JAVA_ENABLE)

# Xcos is not checked here because gui=no disables it
AM_CONDITIONAL(NEED_JAVA, [test "$with_jdk" != no -a \( "$with_javasci" != no -o "$with_gui" != no -o "$enable_build_help" != no \)])
AM_CONDITIONAL(GUI, [test "$with_jdk" != no -a "$with_gui" != no])
AM_CONDITIONAL(JAVASCI, [test "$with_jdk" != no -a "$with_javasci" != no])
AM_CONDITIONAL(SWIG, [test "$with_jdk" != no -a "$enable_build_swig" != no -a "$enable_build_swig" != ""])
AM_CONDITIONAL(GIWS, [test "$with_jdk" != no -a "$enable_build_giws" != no -a "$enable_build_giws" != ""])

##############################################################
## GUI module
##############################################################

GUI_ENABLE=no

if test "$JAVA_ENABLE" != no; then
   GUI_ENABLE=yes
fi

if test "$with_gui" != no; then
   GUI_ENABLE=yes
fi

AC_SUBST(GUI_ENABLE)

##############################################################
## test for functions in standard C library and C math library
##############################################################

# Provided by unistd.h
AC_CHECK_FUNCS([sleep] [usleep] [dup2] [getcwd] [getpagesize] [getpass])
AC_CHECK_FUNCS([rmdir])

AC_CHECK_FUNC([getwd],AC_DEFINE([HAVE_GETWD],[1],[Define to 1 if you have the `getwd' function.]),[AC_DEFINE([getwd(x)],[getcwd(x,1024)],[Don't use getwd but getcwd])])

# Provided by <regex.h>
AC_CHECK_FUNCS([regcomp])

# Provided by stdlib.h
AC_CHECK_FUNCS([atexit] [putenv] [setenv])

# Provided by String.h
AC_CHECK_FUNCS([bzero] [memmove] [memset] [strcasecmp] [strerror] [strchr] [strdup] [strpbrk] [strrchr] [strstr] [strtol])

# Provided by select.h
AC_CHECK_FUNCS([select])

# Provided by pwd.h
AC_CHECK_FUNCS([endpwent])

# Provided by netdb.h
AC_CHECK_FUNCS([gethostbyaddr] [gethostbyname] [gethostname])

# Provided by time.h
AC_CHECK_FUNCS([gettimeofday])

# Provided by ctype.h
AC_CHECK_FUNCS([isascii])

# Provided by wctype.h
AC_CHECK_FUNCS([iswprint])

# Provided by types.h
AC_CHECK_FUNCS([mkdir])

# Provided by mman.h
AC_CHECK_FUNCS([munmap])

# Provided by signal.h
AC_CHECK_FUNCS([strsignal])


# Check of the libm (lib math). Macro provided by libtool.
save_LDFLAGS="$LDFLAGS"
LT_LIB_M()

# Provided by math.h

LDFLAGS="$LDFLAGS $LIBM"
AC_CHECK_FUNCS([pow] [sqrt] [finite] [floor] [exp10] [erf] [erfc] [isnan])

### If isinf exists or not (used to not be the case under Solaris)
### See bug #4164
AC_DEFINE([HAVE_ISINF],[1],[Have isinf function or macro equivalent])
AC_CHECK_FUNC([isinf],,[
        AC_DEFINE([isinf(x)],[(!finite(x) && x==x)],[Provide a macro to do isinf])
])
LIBS="$LIBS $LIBM"
LDFLAGS="$save_LDFLAGS"

# Provided by regex.h
AC_CHECK_FUNCS([re_comp])

# Provided by socket.h
AC_CHECK_FUNCS([socket])

# Provided by utsname.h
AC_CHECK_FUNCS([uname])

# Provided by wtloop.c
AC_CHECK_FUNCS([setlocale])

# Function memcmp used in modules/fileio/src/c/xls.c
AC_FUNC_MEMCMP

# function stat used in modules/core/src/c/link_std.c
AC_FUNC_STAT

# function strtod used in modules/core/src/c/getval.c
AC_FUNC_STRTOD

########################
## test for header files
########################

AC_CHECK_HEADERS([limits.h values.h])

AC_CHECK_HEADERS([fcntl.h float.h libintl.h locale.h malloc.h netdb.h netinet/in.h nlist.h sgtty.h stddef.h sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/timeb.h sys/utsname.h syslog.h term.h termcap.h termio.h termios.h wchar.h wctype.h time.h])

# check header dirent
AC_HEADER_DIRENT

# static struct timeval defined or not | used in modules/core/src/c/timer.c
AC_HEADER_TIME

# check if the specific header is available or not | used in modules/core/src/c/link_SYSV.c
AC_HEADER_SYS_WAIT


#######################
## Test for structures ##
#######################
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_CHECK_MEMBERS([struct stat.st_rdev])


#######################
## MISC Test
#######################

# gettext.  See http://www.gnu.org/software/hello/manual/gettext/AM_005fGNU_005fGETTEXT.html
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.16])

# function closedir used in modules/metanet/src/c/files.c
AC_FUNC_CLOSEDIR_VOID

# Signals used in modules/core/src/c/realmain.c
AC_TYPE_SIGNAL

# struct tm used in modules/core/src/c/history.c
AC_STRUCT_TM

# st_blocks in the struct in modules/io/sci_gateway/c/intfilestat.c
AC_STRUCT_ST_BLOCKS


##################
## termcap library
##################

# some systems may have a system curses implementation as well as ncurses
# installed.  We need to be consistent in making sure we get the correct
# library to go with the correct header and also provide a way for the user
# to have some control over which is picked when both are available.
#
# For now, just check for -lcurses and then -lncurses.  The user control
# may need to be revisited

TERMCAP_LIB=no

# Various observations:
#
#  ncurses
#    - installs ncurses.h and possibly curses.h as a link to ncurses.h
#    - installs -lncurses.
#    - need to include term.h for tgetent() but tgetent() is in -lncurses.a
#
# curses as found in NetBSD-4 and NetBSD-5
#    - installs curses.h
#    - need to include termcap.h and link with -ltermcap for tgetent()
#

AC_CHECK_LIB([curses],[main])
if test $ac_cv_lib_curses_main = no ; then
    AC_CHECK_LIB([ncurses],[main])
fi

# make sure we have what we need for tgetent
AC_SEARCH_LIBS([tgetent],[termcap])

AC_CHECK_HEADERS([ncurses.h curses.h])

if test "x$ac_cv_lib_curses_main" = "xyes" -o "x$ac_cv_lib_ncurses_main" = "xyes" ; then
  AC_DEFINE([HAVE_TERMCAP],[],[Have Term Cap])
else
 AC_MSG_ERROR([No termcap library detected. Please install ncurses dev library (or termcap library)])
fi

##################
## other libraries
##################

AC_CHECK_LIB(dl, dlopen)

AC_SEARCH_LIBS([pthread_join],[pthread])

#################
## FFTW
#################

AC_ARG_WITH(fftw,
    AS_HELP_STRING([--without-fftw],[Disable the interface to the FFTW 3 library]))

FFTW_ENABLE=no
if test "$with_fftw" != no; then
   AC_FFTW()
   FFTW_ENABLE=yes
fi

AC_SUBST(FFTW_ENABLE)
AM_CONDITIONAL(FFTW, test "$with_fftw" != no)


#################
## EIGEN
#################

AC_EIGEN()


#################
## MPI
#################

# Disable by default the build of MPI:
# * It is hard to package
# * People are administrating cluster know about rebuilding packages
# * They use their own MPI library
AC_ARG_WITH(mpi,
    AS_HELP_STRING([--with-mpi],[compile with MPI library]))

MPI_ENABLE=no
if test "$with_mpi" == yes; then
   AC_OPENMPI()
   # We will have to detect other implementation of OpenMPI
   MPI_ENABLE=yes
fi
AC_SUBST(MPI_ENABLE)

AM_CONDITIONAL(MPI, test "$with_mpi" == yes)

#################
## OpenMP
#################

AC_ARG_WITH(openmp,
    AS_HELP_STRING([--without-openmp],[Disable the usage of OpenMP (parallelization of some algoritms)]))

OPENMP_ENABLE=no
if test "$with_openmp" != no; then
#   AC_OPENMP
    OPENMP_CFLAGS="-fopenmp"
    OPENMP_CXXFLAGS="-fopenmp"
    OPENMP_LIBS="-lgomp" 

    AC_CHECK_HEADERS([omp.h], [],
     [AC_MSG_ERROR([Could not find omp.h])])

    OPENMP_ENABLE=yes
fi
AC_SUBST(OPENMP_ENABLE)
AC_SUBST(OPENMP_CFLAGS)
AC_SUBST(OPENMP_CXXFLAGS)
AC_SUBST(OPENMP_LIBS)

AM_CONDITIONAL(OPENMP, test "$with_openmp" != no)


#######################
## Test for libxml
#######################

AC_LIBXML2()

#######################
## Test for gettext
#######################

ALL_LINGUAS="en_US fr_FR zh_CN zh_TW ru_RU ca_ES de_DE es_ES pt_BR ja_JP it_IT uk_UA pl_PL cs_CZ"
ALL_LINGUAS_DOC="en_US fr_FR pt_BR ja_JP ru_RU"

AC_ARG_ENABLE(build-localization,
        AS_HELP_STRING([--disable-build-localization],[Disable the localization build])
        )
BUILD_LOCALIZATION_ENABLE=no

if test "$enable_build_localization" != no; then

   AC_SUBST(ALL_LINGUAS)
   AC_SUBST(ALL_LINGUAS_DOC)
   AC_CHECK_FUNCS([bind_textdomain_codeset])

   AC_PATH_PROG(MSGCAT, msgcat, no)
#   AC_PATH_PROG(MSGFMT, msgfmt, no)
#   AC_PATH_PROG(XGETTEXT, xgettext, no)

   if test x$MSGFMT = xno; then
      AC_MSG_ERROR([The msgfmt command is required to build Scilab.  If it is installed on your system, ensure that it is in your path.  If it is not, install GNU gettext to continue or use the option --disable-build-localization ])
      fi
    if test x$MSGCAT = xno; then
      AC_MSG_ERROR([The msgcat command is required to build Scilab.  If it is installed on your system, ensure that it is in your path.  If it is not, install GNU gettext to continue or use the option --disable-build-localization ])
    fi
    BUILD_LOCALIZATION_ENABLE=yes
fi
AM_CONDITIONAL(GENERATE_LOCALIZATION_FILES, test "$BUILD_LOCALIZATION_ENABLE" = yes)

#######################
## Test for blas/Atlas and lapack
#######################
AC_MSG_CHECKING([if BLAS, ATLAS or MKL is available])
echo ""
ACX_BLAS(
        [AC_MSG_RESULT([$BLAS_TYPE found])]
        ,
        AC_MSG_ERROR([Impossible to find a BLAS compatible library (see BLAS or ATLAS).])
        )

AC_MSG_CHECKING([if LAPACK is available])
echo ""
ACX_LAPACK(
        [AC_MSG_RESULT([$LAPACK_TYPE found])],
        AC_MSG_ERROR([Impossible to find the LAPACK library.])
        )


AC_ARG_WITH(arpack-ng,
    AS_HELP_STRING([--without-arpack-ng],[Disable the interface to ARPACK-NG]))

ARPACK_NG=no
if test "$with_arpack_ng" != no; then
   ARPACK_NG=yes
   AC_MSG_CHECKING([if ARPACK-NG is available])
echo ""
ACX_ARPACK(
        [AC_MSG_RESULT([ARPACK-NG library found])],
        AC_MSG_ERROR([Impossible to find the ARPACK library. Please note that arpack was bundled with version prior to 5.4.0 and Scilab requires arpack-ng ( http://forge.scilab.org/index.php/p/arpack-ng/ ).])
        )

CHECK_ARPACK_OK(
[AC_MSG_RESULT([Working ARPACK-NG library found (probably ARPACK-NG or a patched version of ARPACK)])],
[AC_MSG_ERROR([ARPACK library found, but seems not to work properly. Please make sure you are using arpack-ng])
])
else
   AC_MSG_CHECKING([Skip ARPACK-NG detection])
fi

AM_CONDITIONAL(ARPACK_NG, test "$ARPACK_NG" != "no")


#################
## UMFPACK
#################

AC_ARG_WITH(umfpack,
    AS_HELP_STRING([--without-umfpack],[Disable the interface to the UMFPACK library]))

UMFPACK_ENABLE=no
if test "$with_umfpack" != no; then
   AC_UMFPACK([$BLAS_LIBS])
   UMFPACK_ENABLE=yes
fi

AC_SUBST(UMFPACK_ENABLE)
AM_CONDITIONAL(UMFPACK, test "$with_umfpack" != no)

#######################
## Test for PCRE
#######################

AC_PCRE()

#######################
## Test for CURL
#######################

AC_CURL()

#################
## Tcl/Tk library
#################

WITH_TKSCI=no
if test "$with_tk" != no; then
   if test "$MACOSX" = "1"; then
      AC_MSG_ERROR([Due to technical constraints, Tcl/Tk must be disabled under Mac OS X (--without-tk)])
   fi

    # check user arguments
    USER_TCL_LIB_PATH=""
    USER_TCL_INC_PATH=""
    AC_ARG_WITH(tcl-library,
        AS_HELP_STRING([--with-tcl-library=DIR],[Set the path to the TCL library]),
        [  USER_TCL_LIB_PATH=$withval
    ])
    AC_ARG_WITH(tcl-include,
        AS_HELP_STRING([--with-tcl-include=DIR],[Set the path to the TCL headers]),
        [  USER_TCL_INC_PATH=$withval
    ])

    USER_TK_LIB_PATH=$USER_TCL_LIB_PATH
    USER_TK_INC_PATH=$USER_TCL_INC_PATH

    AC_ARG_WITH(tk-library,
        AS_HELP_STRING([--with-tk-library=DIR],[Set the path to the TK library]),
        [  USER_TK_LIB_PATH=$withval
    ])

    AC_ARG_WITH(tk-include,
        AS_HELP_STRING([--with-tk-include=DIR],[Set the path to the TK headers]),
        [  USER_TK_INC_PATH=$withval
    ])

###########################
########## X11 checks
###########################
        ## This check is mandatory since tk needs Xlib headers and libs
        AC_PATH_XTRA
        ##

    saved_cflags="$CFLAGS"
    saved_ldflags="$LDFLAGS"
    saved_cppflags="$CXXFLAGS"

    AC_CHECK_LIB([dl], [main], [TCLTK_LIBS=" -ldl"])
    AC_CHECK_TCLTK

    # set variables
    if test "$WITH_TKSCI" = yes; then
        AC_DEFINE([WITH_TK], [], [With TK])
    else
        AC_MSG_ERROR([TCL/TK not found. Use --without-tk or specify the librairies and include paths manually])
    fi

    AC_SUBST(TCLTK_LIBS)
    AC_SUBST(TCL_INC_PATH)
    AC_SUBST(TK_INC_PATH)

fi
AC_SUBST(WITH_TKSCI)
AM_CONDITIONAL(TCLTK, test "$WITH_TKSCI" = yes)

#################
## MATIO LIBRARY (MAT File I/O Library)
#################


AC_ARG_WITH(matio,
    AS_HELP_STRING([--without-matio],[Disable the interface to Matio (MAT File I/O library)]))

AC_ARG_WITH(matio_include,
        AS_HELP_STRING([--with-matio-include=DIR],[Set the path to the MATIO headers]),
        [with_matio_include="-I$withval"], [])

AC_ARG_WITH(matio_library,
        AS_HELP_STRING([--with-matio-library=DIR],[Set the path to the MATIO libraries]),
        [with_matio_library="-L$withval"], [])

MATIO_ENABLE=no

if test "$with_matio" != no; then
   if test -n "$with_matio_include" -o -n "$with_matio_library" ; then
      MATIO_CFLAGS="$with_matio_include"
      MATIO_LIBS="$with_matio_library -lm -lz -lmatio -lhdf5"
   else
      if $WITH_DEVTOOLS; then # Scilab thirdparties
         MATIO_CFLAGS="-I$DEVTOOLS_INCDIR"
         MATIO_LIBS="-L$DEVTOOLS_LIBDIR -lm -lz -lmatio -lhdf5"
      else
         PKG_CHECK_MODULES(MATIO, [matio >= 1.5.0])
      fi
   fi

   save_CFLAGS="$CFLAGS"
   save_LIBS="$LIBS"

   CFLAGS="$CFLAGS $MATIO_CFLAGS"
   LIBS="$LIBS $MATIO_LIBS"

   AC_CHECK_HEADERS([matio.h], [],
     [AC_MSG_ERROR([Invalid MATIO_CFLAGS returned by pkg-config. Try to define MATIO_CFLAGS.])])
   AC_CHECK_LIB([matio], [Mat_Open], [],
     [AC_MSG_ERROR([Invalid MATIO_LIBS returned by pkg-config. Try to define MATIO_LIBS.])])

   LIBS="$save_LIBS"
   CFLAGS="$save_CFLAGS"

   AC_DEFINE([WITH_MATIO], [], [With the MATIO library])
   MATIO_ENABLE=yes

   AC_SUBST(MATIO_LIBS)
   AC_SUBST(MATIO_CFLAGS)
fi


AC_SUBST(MATIO_ENABLE)
AM_CONDITIONAL(MATIO, test "$with_matio" != no)

#############################
## Documentation management #
#############################

HELP_ENABLE=yes

AC_ARG_ENABLE(build-help,
        AS_HELP_STRING([--disable-build-help],[Disable the help build])
        )

if test "$enable_build_help" != no; then
    AC_DOCBOOK()
else
    HELP_ENABLE=no
fi

if test "$JAVA_ENABLE" = no; then
    HELP_ENABLE=no
fi

AC_SUBST(HELP_ENABLE)

AM_CONDITIONAL(BUILD_HELP, test $HELP_ENABLE = yes)

## Install XML help files ###

AC_ARG_WITH(install-help-xml,
        AS_HELP_STRING([--with-install-help-xml],[make install will install XML files])
        )
HELP_INSTALL_ENABLE=no
if test "$with_install_help_xml" != no -a "$with_install_help_xml" != ""; then
   HELP_INSTALL_ENABLE=yes
fi
AM_CONDITIONAL(INSTALL_HELP_XML, test "$HELP_INSTALL_ENABLE" != "")

## Doxygen help generation
AC_ARG_ENABLE(build-doxygen,
    AS_HELP_STRING([--enable-build-doxygen],[Generate doxygen C/C++ documentation]))

DOXYGEN_ENABLE=yes
if test "$enable_build_doxygen" != no -a "$enable_build_doxygen" != ""; then
    AC_DOXYGEN()
else
    DOXYGEN_ENABLE=no
fi
AM_CONDITIONAL(DOXYGEN, test $DOXYGEN_ENABLE = yes)

## Valgrind specific code
PKG_CHECK_EXISTS([valgrind], [PKG_CHECK_MODULES([VALGRIND], [valgrind], [AC_DEFINE([VALGRIND_ENABLE], [], [Enable valgrind specific tweaks])])], [])

##############################################################
## Javasci module
##############################################################

JAVASCI_ENABLE=yes

if test "$JAVA_ENABLE" = no -o "$JAVASCI" = no; then
   JAVASCI_ENABLE=no
fi

AC_SUBST(JAVASCI_ENABLE)


##############################################################
## Enable test at compilation time
##############################################################

COMPILATION_TESTS=no
if test "$enable_compilation_tests" != no -a "$enable_compilation_tests" != ""; then
   COMPILATION_TESTS=yes
fi

if test COMPILATION_TESTS=yes -a "$JUNIT4" == ""; then
   AC_MSG_WARN([--enable-compilation-tests deactivated: Could not find Junit4"])
   COMPILATION_TESTS=no
fi

AM_CONDITIONAL(COMPILATION_TESTS, test "$COMPILATION_TESTS" != "no")

##############################################################
## Enable the global force link
##############################################################

AC_ARG_ENABLE(force-full-link,
    AS_HELP_STRING([--enable-force-full-link],[Forces the explicit link between libscilab and some "on-the-fly" loaded libraries. NOT USE IN PRODUCTION.]))

FORCE_FULL_LINK="no"
if test "$enable_force_full_link" == "yes"; then
    FORCE_FULL_LINK="yes"
fi

AM_CONDITIONAL(FORCE_LINK, test "$FORCE_FULL_LINK" == "yes")

##############################################################
## demo_tools module
##############################################################

DEMOTOOLS_ENABLE=yes

if test "$GUI_ENABLE" = no; then
   DEMOTOOLS_ENABLE=no
fi

AC_SUBST(DEMOTOOLS_ENABLE)

##############################################################
## graphics/renderer/graphic_export module
##############################################################

GRAPHICS_ENABLE=yes

if test "$GUI_ENABLE" = no; then
   GRAPHICS_ENABLE=no
fi

AC_SUBST(GRAPHICS_ENABLE)

#########################
## libtool
#########################

LT_PREREQ([2.2.7])
LT_INIT([shared dlopen disable-static])

# Avoid to link all the dep from others libraries (*.la included by LIBADD)
link_all_deplibs=no

# Check to see if building shared libraries
libtool_build_shared_libs=no
if test "$enable_shared" = "yes"; then
  libtool_build_shared_libs=yes
fi

# Check to see if building static libraries
libtool_build_static_libs=no
if test "$enable_static" = "yes"; then
  libtool_build_static_libs=yes
fi

# AM_CONDITIONAL(ENABLE_STATIC, test "$libtool_build_static_libs" = yes)
# Fake to disable the static build
AM_CONDITIONAL(ENABLE_STATIC, test "$libtool_build_static_libs" = xxxx)

AC_C_CONST()
AC_C_INLINE()

AC_BACKTRACE()

##########
##### Code quality
##########
AC_PATH_PROG(SPLINT, splint, no)

##########
##### Detect ccache and use it by default if available
##########

AC_ARG_ENABLE(ccache,
        AS_HELP_STRING([--disable-ccache],[Disable the use of ccache])
        )
AC_PATH_PROG(CCACHE, ccache)
if test x"$CCACHE" != x -a "$enable_ccache" != no; then
  CC="$CCACHE $CC"
  CXX="$CCACHE $CXX"
fi

#######################
###### Creation of the header file (machine.h)
#######################

AC_DEFINE_UNQUOTED([PATH_SEPARATOR], ["$PATH_SEPARATOR"],
[The default path separator character.])

AH_TOP([#ifndef MACHINE_H
#define MACHINE_H
/* This file defines global element configuration of the build host */
])

AH_BOTTOM([

#ifdef DIR_SEPARATOR
       #undef DIR_SEPARATOR
#endif
#define DIR_SEPARATOR "/"

#ifdef DIR_SEPARATORW
	   #undef DIR_SEPARATORW
#endif
#define DIR_SEPARATORW L"/"

#define bsiz 4096

#endif /* MACHINE_H  */
])

# Define the standard extension of a dynamic library
AC_DEFINE_UNQUOTED([SHARED_LIB_EXT],["$shrext_cmds"],[Extension of a shared library])
AC_DEFINE_UNQUOTED([SHARED_LIB_EXTW],[L"$shrext_cmds"],[Extension of a shared library ( wide string )])



#################
## Update the years in the various part of the code.
#################

if test ! -f modules/core/src/cpp/banner.cpp; then
   AC_MSG_ERROR([Could not find the Scilab banner file.])
fi

CURRENT_YEAR=`date "+%Y"`
DETECTED_YEAR=`grep "Copyright (c) 2011-" modules/core/src/cpp/banner.cpp|sed -e "s/.*Copyright (c) 2011-\([[0-9]]*\).*/\1/g"`

if test "$CURRENT_YEAR" != "$DETECTED_YEAR"; then
   sed -i -e "s| 2011-$DETECTED_YEAR | 2011-$CURRENT_YEAR |g" modules/core/src/cpp/banner.cpp modules/windows_tools/src/c/scilab_windows/console.c
   sed -i -e "s| 2011-$DETECTED_YEAR Scilab Enterprises| 2011-$CURRENT_YEAR Scilab Enterprises|g" etc/Info.plist.in
   sed -i -e "s|VALUE \"LegalCopyright\", \"Copyright (C) $DETECTED_YEAR|VALUE \"LegalCopyright\", \"Copyright (C) $CURRENT_YEAR|" $(find . -iname '*.rc')
   AC_MSG_WARN([New year ($CURRENT_YEAR) detected. Copyright files updated. Please commit them ASAP.])
fi


#########
## Make sure that the libstdc++ libgcc and libgfortran can be compiled as 
## static if requested
#########
AC_CHECK_STDCPP_STATIC()

#################
## stop on warning
#################

# Stop to compile scilab when a warning is found ...
# This stuff is at the end of the configure.ac because it causes some
# problem with AC_COMPILE (the -Werror is added to the test)
#
if test "$enable_stop_on_warning" = yes; then
    WARNING_CFLAGS="-Werror=implicit-function-declaration -Werror=declaration-after-statement "
    WARNING_CXXFLAGS="-Werror=implicit-function-declaration "
fi

# SCI_*FLAGS contains all defaults values detected on configure
SCI_CFLAGS=$(echo                                             \
             $LARGEFILE_CFLAGS                                \
             $CODECOVERAGE_CFLAGS                             \
             $DEBUG_CFLAGS                                    \
             $ARCH_CFLAGS                                     \
             $COMPILER_CFLAGS                                 \
             $SCILIBS_CFLAGS                                  \
             $SSE_CFLAGS                                      \
             $BACKTRACE_CFLAGS                                \
             $WARNING_CFLAGS                                  )
SCI_CXXFLAGS=$(echo                                           \
               $STDCXX_11_CXXFLAGS                            \
               $LARGEFILE_CXXFLAGS                            \
               $CODECOVERAGE_CXXFLAGS                         \
               $DEBUG_CXXFLAGS                                \
               $ARCH_CXXFLAGS                                 \
               $COMPILER_CXXFLAGS                             \
               $SCILIBS_CXXFLAGS                              \
               $SSE_CXXFLAGS                                  \
               $BACKTRACE_CXXFLAGS                            \
               $WARNING_CXXFLAGS                              )
SCI_FFLAGS=$(echo                                             \
             $LARGEFILE_FFLAGS                                \
             $CODECOVERAGE_FFLAGS                             \
             $DEBUG_FFLAGS                                    \
             $ARCH_FFLAGS                                     \
             $COMPILER_FFLAGS                                 \
             $SCILIBS_FFLAGS                                  \
             $SSE_FFLAGS                                      \
             $BACKTRACE_FFLAGS                                \
             $WARNING_FFLAGS                                  )
SCI_LDFLAGS=$(echo                                            \
              $LARGEFILE_LDFLAGS                              \
              $CODECOVERAGE_LDFLAGS                           \
              $DEBUG_LDFLAGS                                  \
              $ARCH_LDFLAGS                                   \
              $COMPILER_LDFLAGS                               \
              $SCILIBS_LDFLAGS                                \
              $SSE_LDFLAGS                                    \
              $BACKTRACE_LDFLAGS                              \
              $WARNING_LDFLAGS                                )
AC_SUBST(SCI_CFLAGS)
AC_SUBST(SCI_CXXFLAGS)
AC_SUBST(SCI_FFLAGS)
AC_SUBST(SCI_LDFLAGS)

AC_CONFIG_FILES([
contrib/Makefile
desktop/images/icons/Makefile
desktop/Makefile
modules/threads/Makefile
modules/ast/Makefile
modules/functions_manager/Makefile
modules/helptools/Makefile
modules/data_structures/Makefile
modules/differential_equations/Makefile
modules/optimization/Makefile
modules/elementary_functions/Makefile
modules/special_functions/Makefile
modules/io/Makefile
modules/completion/Makefile
modules/history_manager/Makefile
modules/jvm/Makefile
modules/commons/Makefile
modules/sound/Makefile
modules/statistics/Makefile
modules/mexlib/Makefile
modules/sparse/Makefile
modules/linear_algebra/Makefile
modules/polynomials/Makefile
modules/signal_processing/Makefile
modules/arnoldi/Makefile
modules/interpolation/Makefile
modules/localization/Makefile
modules/cacsd/Makefile
modules/boolean/Makefile
modules/integer/Makefile
modules/fileio/Makefile
modules/spreadsheet/Makefile
modules/string/Makefile
modules/time/Makefile
modules/graphics/Makefile
modules/graphic_export/Makefile
modules/graphic_objects/Makefile
modules/renderer/Makefile
modules/action_binding/Makefile
modules/gui/Makefile
modules/mpi/Makefile
modules/randlib/Makefile
modules/tclsci/Makefile
modules/windows_tools/Makefile
modules/core/Makefile
modules/prebuildjava/Makefile
modules/api_scilab/Makefile
modules/call_scilab/Makefile
modules/types/Makefile
modules/hdf5/Makefile
modules/fftw/Makefile
modules/umfpack/Makefile
modules/scicos/Makefile
modules/scicos_blocks/Makefile
modules/functions/Makefile
modules/dynamic_link/Makefile
modules/overloading/Makefile
modules/javasci/Makefile
modules/m2sci/Makefile
modules/compatibility_functions/Makefile
modules/development_tools/Makefile
modules/output_stream/Makefile
modules/console/Makefile
modules/demo_tools/Makefile
modules/genetic_algorithms/Makefile
modules/simulated_annealing/Makefile
modules/parameters/Makefile
modules/matio/Makefile
modules/atoms/Makefile
modules/xcos/Makefile
modules/scinotes/Makefile
modules/ui_data/Makefile
modules/graph/Makefile
modules/parallel/Makefile
modules/modules_manager/Makefile
modules/history_browser/Makefile
modules/preferences/Makefile
modules/xml/Makefile
modules/external_objects/Makefile
modules/external_objects_java/Makefile
modules/slint/Makefile
modules/coverage/Makefile
modules/Makefile
Makefile
scilab.pc
etc/modules.xml
etc/classpath.xml
etc/Info.plist
etc/logging.properties
scilab.properties
scilab-lib.properties
scilab-lib-doc.properties
modules/helptools/etc/SciDocConf.xml
modules/core/includes/version.h
modules/atoms/etc/repositories
modules/atoms/tests/unit_tests/repositories.orig
])

# Detection of the module for the future version of Scilab 6
# ie we detect module which ends with _yasp

if test "$enable_yasp" = yes; then
   AC_CONFIG_FILES([
   modules/development_tools/src/fake/Makefile
  ])
fi

# This script is used by Xcos in order to regenerate the function/block list
AC_CONFIG_COMMANDS_POST([chmod +x $SCI_SRCDIR_FULL/modules/scicos_blocks/src/scripts/GenBlocks.sh $SCI_SRCDIR_FULL/modules/dynamic_link/src/scripts/scicompile.sh $SCI_SRCDIR_FULL/modules/dynamic_link/src/scripts/compilerDetection.sh $SCI_SRCDIR_FULL/modules/dynamic_link/src/scripts/configure])

AC_OUTPUT

# To distribution packager, you can uncomment this stuff is you want to
# disable the rpath issue
# However, you will have to set all the LD_LIBRARY_PATH to .libs/ directory
# since scilab is compiling macros and help into the source tree (ie before
# the "make install")
# You should consider using chrpath:
# http://directory.fsf.org/project/chrpath/
# to remove it before the make install

#case ${host} in
#  *-pc-linux-gnu)
#    AC_MSG_RESULT([Fixing libtool for -rpath problems.])
#    sed < libtool > libtool-2 \
#    's/^hardcode_libdir_flag_spec.*$/hardcode_libdir_flag_spec=" -D__LIBTOOL_IS_A_FOOL__ "/'
#    mv libtool-2 libtool
#    chmod 755 libtool
#  ;;
#esac
#

echo ""

echo "Scilab is configured as follows. Please verify that this configuration"
echo "matches your expectations."
echo ""
echo "Host system type : $host"
echo ""
echo "                  Option                        Value"
echo "-------------------------------------------------------------------------"
echo "Shared libraries.......      --enable-shared=$libtool_build_shared_libs"
echo "Static libraries.......      --enable-static=$libtool_build_static_libs"
echo "GNU ld.................      --with-gnu-ld=$with_gnu_ld"
echo "Enable debug ..........      --enable-debug=$enable_debug"
echo "Enable debug C.........      --enable-debug-C=$enable_debug_C"
echo "Enable debug C++.......      --enable-debug-CXX=$enable_debug_CXX"
echo "Enable debug Java......      --enable-debug-java=$enable_debug_java"
echo "Enable debug Fortran...      --enable-debug-fortran=$enable_debug_fortran"
echo "Enable stop on warning.      --enable-stop-on-warning=$enable_stop_on_warning"

echo ""
echo "Compiler Configuration:"
echo "  Intel (--with-intelcompilers) .... =        $with_intelcompilers"
echo "  GNU gcc (--with-gcc) ............. =        $with_gcc"
echo "  GNU Fortran 95 (--with-gfortran) . =        $with_gfortran"
echo ""
echo "Options:"
echo "  Do not use TCL/TK (--without-tk) ................. = $with_tk"
echo "  TCL include (--with-tcl-include) ................. = $USER_TCL_INC_PATH"
echo "  TCL library (--with-tcl-library) ................. = $USER_TCL_LIB_PATH"
echo "  TK include (--with-tk-include) ................... = $USER_TK_INC_PATH"
echo "  TK library (--with-tk-library) ................... = $USER_TK_LIB_PATH"
echo "  Install XML Help (--with-install-help-xml) ....... = $with_install_help_xml"
echo "  Compilation tests (--enable-compilation-tests) ... = $COMPILATION_TESTS"
echo "  Make the package relocatable (--enable-relocatable)= $RELOCATABLE"
echo "  Use FFTW (--without-fftw) ........................ = $with_fftw"
echo "  Use MATIO (--without-matio) ...................... = $with_matio"
echo ""
echo "  Compile with Scilab thirdparties ................. = $WITH_DEVTOOLS"

echo ""
if test "$with_gui" = no; then
   echo "Not using Xcos because of the option --without-gui"
else
if test $XCOS_ENABLE = yes; then
   echo "Xcos enable"
   echo "Build modelica compiler (--without-modelica) ....... = $with_modelica"
   echo ""
   if test "$with_modelica" != no -a "$OCAMLC" != no -a "$OCAMLOPT" != no; then
     echo "Ocaml Configuration (for Modelica compiler):"
     echo "  OCAMLC ............. = $OCAMLC"
     echo "  OCAMLOPT ........... = $OCAMLOPT"
     echo "  OCAMLDEP ........... = $OCAMLDEP"
   else
     echo "Will not build Modelica compiler"
   fi
else
   echo "Not using Xcos"
fi
fi

echo ""
if test "$enable_code_coverage" = yes;  then
   echo "Code coverage configuration:"
   echo "  LCOV .............. = $LCOV"
   echo "  GENHTML ........... = $GENHTML"
else
   echo "Not using code coverage"
fi

echo ""
if test $OPENMP_ENABLE = yes; then
   echo "OpenMP Configuration:"
   echo "OpenMP CFLAGS ...... = $OPENMP_CFLAGS"
   echo "OpenMP CXXFLAGS .... = $OPENMP_CXXFLAGS"
   echo "OpenMP LIBS ........ = $OPENMP_LIBS"
   echo "OpenMP LDFLAGS ..... = $OPENMP_LDFLAGS"
else
   echo "Not using OpenMP"
fi

echo ""
if test $FFTW_ENABLE = yes;  then
   echo "FFTW Configuration:"
   echo "  FFTW LIBS .......... = $FFTW3_LIB"
   echo "  FFTW CFLAGS ........ = $FFTW3_CFLAGS"
else
   echo "Not using FFTW"
fi

echo ""
echo "Eigen3 Configuration:"
echo "  EIGEN CPPFLAGS ...... = $EIGEN_CPPFLAGS"

echo ""
if test $MATIO_ENABLE = yes;  then
   echo "MATIO Configuration:"
   echo "  MATIO LIBS .......... = $MATIO_LIBS"
   echo "  MATIO CFLAGS ........ = $MATIO_CFLAGS"
else
   echo "Not using MATIO"
fi

echo ""
if test $UMFPACK_ENABLE = yes;  then
   echo "UMFPACK Configuration:"
   echo "  UMFPACK LIBS ....... = $UMFPACK_LIB"
   echo "  UMFPACK CFLAGS ..... = $UMFPACK_CFLAGS"
   if test $SUITESPARSE = yes; then
      echo "  UMFPACK SUITESPARSE  = Yes"
   else
      echo "  UMFPACK SUITESPARSE  = No"
   fi
else
   echo "Not using UMFPACK"
fi


echo ""
echo "BLAS/LAPACK/ATLAS Configuration:"
echo "  BLAS LIBS ............. = $BLAS_LIBS"
echo "  BLAS TYPE ............. = $BLAS_TYPE"
echo "  LAPACK LIBS ........... = $LAPACK_LIBS"
echo "  LAPACK TYPE ........... = $LAPACK_TYPE"
echo "  ARPACK LIBS ........... = $ARPACK_LIBS"
echo ""


if test "$with_mpi" == yes; then
   echo "OpenMPI Configuration:"
   echo "OpenMPI LIBS ........... = $OPENMPI_LIBS"
   echo "OpenMPI C Compiler ..... = $OPENMPI_CC"
   echo "OpenMPI C++ Compiler ... = $OPENMPI_CXX"
   echo "OpenMPI F77 Compiler ... = $MPIF77"
else
   echo "Not using MPI"
fi

echo ""
if test $BUILD_LOCALIZATION_ENABLE != no; then
   echo "Gettext/localization configuration:"
   echo "  xgettext ............... = $XGETTEXT"
   echo "  msgfmt  ................ = $MSGFMT"
   echo "  msgfmt_opts ............ = $MSGFMT_OPTS"
   echo "  msgcat  ................ = $MSGCAT"
else
   echo "Won't generate localization files"
fi

echo ""
if test $HELP_ENABLE = yes; then
   echo "Documentation building configuration:"
   echo "  Docbook XSL path ....... = $DOCBOOK_ROOT"
   echo "  Saxon XSLT ............. = $SAXON"
   echo "  XML commons external ... = $XML_APIS_EXT"
else
   echo "No documentation generated"
fi

echo ""
echo "Java Configuration:"
if test ! -z "$JAVAC"; then
echo "  JAVA_HOME ........... = $JAVA_HOME"
echo "  JAVAC ............... = $JAVAC"
echo "  JAVA_CLASSPATH ...... = $JAVA_CLASSPATH"
echo "  JAVA_VERSION ........ = $JAVA_VERSION"
echo "  JAVAC_FLAGS ......... = $JAVAC_FLAGS"
echo "  JAVA_JNI_INCLUDE .... = $JAVA_JNI_INCLUDE"
echo "  JAVA_JNI_LIBS ....... = $JAVA_JNI_LIBS"
echo "  JAVA_JNI_LIBS_PRELOAD = $JAVA_JNI_LIBS_PRELOAD"
echo "  JAVA ................ = $JAVA"
echo "  JAVADOC ............. = $JAVADOC"
echo "  JAR ................. = $JAR"
echo "  ANT ................. = $ANT"
else
echo "  JAVA disabled"
fi


echo ""
echo "Java dependencies:"
if test ! -z "$JAVAC"; then
echo "  Flexdock ............ = $FLEXDOCK"
echo "  JOGL 2............... = $JOGL2"
echo "  JOGL 2 LIBS (JNI) ... = $JOGL2_LIBS"
echo "  Gluegen 2 ........... = $GLUEGEN2_RT"
echo "  Gluegen 2 LIBS (JNI)  = $GLUEGEN2_RT_LIBS"
echo "  Jeuclid (MathML) .... = $JEUCLID_CORE"
echo "  Jhall  .............. = $JHALL"
echo "  Jrosetta (API) ...... = $JROSETTA_API"
echo "  Jrosetta (Engine) ... = $JROSETTA_ENGINE"
echo "  Commons Logging ..... = $COMMONS_LOGGING"
echo "  JGraph X ............ = $JGRAPHX"
echo "  SciRenderer ......... = $SCIRENDERER"
echo "  JLaTeXMath .......... = $JLATEXMATH"
echo "  ECJ ................. = $ECJ"
fi

echo ""
if test ! -z "$JAVAC"; then
echo "Documentation, graphic export:"
echo "  FOP (XML => PDF) .... = $FOP"
echo "  JLaTeXMath Fop ...... = $JLATEXMATH_FOP"
echo "  Batik (SVG) ......... = $BATIK"
echo "  Avalon Framework .... = $AVALON_FRAMEWORK"
echo "  Commons I/O ......... = $COMMONS_IO"
echo "  XML graphics commons  = $XMLGRAPHICS_COMMONS"
fi


echo ""
echo "Code quality (optional):"
echo "  Checkstyle .......... = $CHECKSTYLE"
echo "  Commons-beanutils ... = $COMMONS_BEANUTILS"
echo "  Antlr ............... = $ANTLR"
echo "  Junit4 .............. = $JUNIT4"
echo "  Hamcrest ............ = $HAMCREST"
echo "  Cobertura ........... = $COBERTURA"
echo "  splint .............. = $SPLINT"

echo ""
echo "TCL/TK configuration:"
echo "  TK_INC_PATH ........ = $TK_INC_PATH"
echo "  TCL_INC_PATH ....... = $TCL_INC_PATH"
echo "  TCLTK_LIBS ......... = $TCLTK_LIBS"
echo "  TCL_SERIAL_VERSION . = $TCL_SERIAL_VERSION"
echo "  TK_SERIAL_VERSION .. = $TK_SERIAL_VERSION"

echo ""
echo "XML configuration:"
echo "  XML_FLAGS .......... = $XML_FLAGS"
echo "  XML_LIBS ........... = $XML_LIBS"
echo "  XML_VERSION ........ = $XML_VERSION"

echo ""
echo "HDF5 configuration:"
echo "  HDF5 CFLAGS ......... = $HDF5_CFLAGS"
echo "  HDF5 LIBS ........... = $HDF5_LIBS"

echo ""
echo "PCRE configuration:"
echo "  PCRE_CFLAGS ........ = $PCRE_CFLAGS"
echo "  PCRE_LIBS .......... = $PCRE_LIBS"
echo "  PCRE_VERSION ....... = $PCRE_VERSION"

echo ""
echo "Regenerate Scilab's parser"
if test "$enable_build_parser" != no; then
echo "  YACC ............... = $YACC"
echo "  LEX ................ = $LEX"
echo "  YFLAGS ............. = $YFLAGS"
else
echo "  Regenerate parser disabled (will use embedded sources)"
fi

echo ""
echo "CURL configuration:"
echo "  CURL_CFLAGS ........ = $CURL_CFLAGS"
echo "  CURL_LIBS .......... = $CURL_LIBS"
echo "  CURL_VERSION ....... = $CURL_VERSION"

echo ""
echo "SWIG Configuration:"
if test ! -z "$SWIG_BIN"; then
echo "  SWIG_BIN ........... = $SWIG_BIN"
echo "  SWIG_JAVA .......... = $SWIG_JAVA"
else
echo "  SWIG generation disabled"
fi

echo ""
echo "GIWS Configuration:"
if test ! -z "$GIWS_BIN"; then
echo "  GIWS_BIN ........... = $GIWS_BIN"
else
echo "  GIWS generation disabled"
fi

echo ""
echo "Valgrind configuration:"
echo "  VALGRIND_CFLAGS .... = $VALGRIND_CFLAGS"
echo "  VALGRIND_LIBS ...... = $VALGRIND_LIBS"
echo ""

echo ""
echo "Libtool config:"
echo "  objext .............. = $objext"
echo "  libext (static) ..... = $libext"
echo "  shrext_cmds ......... = $shrext_cmds"
echo "  exeext .............. = $exeext"

echo ""

echo "Compilation paths:"
echo "  srcdir .............. = $SCI_SRCDIR"
echo "  srcdir_full ......... = $SCI_SRCDIR_FULL"
echo "  builddir ............ = $SCI_BUILDDIR"
if test "$SCI_SRCDIR_FULL" != "$SCI_BUILDDIR"; then
echo "  VPATH build ......... = Activated"
fi

echo ""
echo "Platform information:"
echo "  host ........... = $host"
echo "  host_cpu ....... = $host_cpu"
echo "  host_vendor .... = $host_vendor"
echo "  host_os ... .... = $host_os"
echo "  hostname ....... = $ac_hostname"
echo "  CPU 64 bits .... = $IS_64_BITS_CPU"
if test -n "$MAC_DETECTED_ARCH"; then
echo "  Mac OS X arch .. = $MAC_DETECTED_ARCH"
echo "  Mac OS X version = $macosx_version"
fi

echo ""
echo "Options used to compile and link:"
echo "  prefix ......... = $prefix"
echo "  localedir ...... = $localedir"
echo "  VERSION ........ = $PACKAGE_VERSION"
echo "  CC ............. = $CC"
echo "  CFLAGS ......... = $CFLAGS"
echo "  SCI_CFLAGS ..... = $SCI_CFLAGS"
echo "  DEFS ........... = $DEFS"
echo "  LD ............. = $LD"
echo "  LDFLAGS ........ = $LDFLAGS"
echo "  SCI_LDFLAGS .... = $SCI_LDFLAGS"
echo "  LIBS ........... = $LIBS"
echo "  CXX ............ = $CXX"
echo "  CXXFLAGS ....... = $CXXFLAGS"
echo "  SCI_CXXFLAGS ... = $SCI_CXXFLAGS"
echo "  F77 ............ = $F77"
echo "  FFLAGS ......... = $FFLAGS"
echo "  SCI_FFLAGS ..... = $SCI_FFLAGS"
echo "  F77_LDFLAGS .... = $F77_LDFLAGS"
echo "  FLIBS...... .... = $FLIBS"
echo "  TERMCAP_LIB .... = $TERMCAP_LIB"
echo ""
