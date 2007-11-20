##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########
dnl Process this file with autoconf to produce a configure script.

## Copyright INRIA

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

AC_REVISION($Revision$)dnl
AC_INIT([Scilab],[0],[http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi])
AC_PREREQ(2.60)

SCI_BUILDDIR="`pwd`"
SCI_SRCDIR="$srcdir"
SCI_SRCDIR_FULL="`cd $SCI_SRCDIR && pwd`"

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

# strip executable
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S
AC_SYS_LARGEFILE
#AC_PACKAGE_VERSION([4.0])

# If configure detect that timestamp changed,
# it tries to rebuild configure & makefile which can be a painmaker
# if the version is different
AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE([-Wall foreign]) # Not using -Werror because we override {C,F}FLAGS in order to disable optimisation

AC_CANONICAL_HOST

#################################
## all the --with-* argument help
#################################


AC_ARG_ENABLE(debug,
   [  --enable-debug           do not optimize and print warning messages (C/C++/Fortran code)])

AC_ARG_ENABLE(debugC,
   [  --enable-debug-C         do not optimize and print warning messages (C code)])

AC_ARG_ENABLE(debugCXX,
   [  --enable-debug-CXX       do not optimize and print warning messages (C++ code)])

AC_ARG_ENABLE(debugFortran,
   [  --enable-debug-fortran   do not optimize and print warning messages (Fortran code)])

AC_ARG_ENABLE(stopOnWarning,
   [  --enable-stop-on-warning Stop the compilation on the first warning found in the C/C++ code])

AC_ARG_WITH(gcc,
    [  --with-gcc              use gcc C compiler ])

AC_ARG_WITH(g77,
    [  --with-g77              use g77, GNU Fortran 77 compiler ])

AC_ARG_WITH(gfortran,
    [  --with-gfortran         use gfortran, GNU Fortran 95 compiler ])

AC_ARG_WITH(intelcompilers,
    [  --with-intelcompilers   use Intel C (icc) and Fortran (ifort) proprietary compilers (GNU/Linux only) ])

AC_ARG_WITH(tk,
    [  --without-tk            do not compile with TCL/TK ])

AC_ARG_WITH(javasci,
    [  --without-javasci       do not compile the Java/Scilab module ])

AC_ARG_WITH(gui,
    [  --without-gui           do not compile the Graphical User Interface (GUI). Intended for embedded/clustering/grid Scilab ])

AC_ARG_WITH(swig-generation,
    [  --with-swig-generation  Regenerate Java => C wrappers produces by Swig (A Java Development Kit (JDK) is mandatory for this option)])

AC_ARG_WITH(giws-generation,
    [  --with-giws-generation  Regenerate C/C++ => Java wrappers produces by Giws (A Java Development Kit (JDK) is mandatory for this option)])


##############################
########## compilator detection
##############################

#####################################################
## check if options are correct (or not)
#####################################################

if test "$with_g77" = yes -a "$with_gfortran" = yes; then
	AC_MSG_ERROR([Conflicting options : you specified two fortran compiler])
fi

if test "$with_intelcompilers" = yes -a "$with_gcc" = yes; then
	AC_MSG_ERROR([Conflicting options : you specified two compiler series])
fi

######## fortran ########
if test "$with_g77" = yes; then
   AC_PROG_F77(g77)
   if test -z "$F77"; then
     AC_MSG_ERROR([You asked me to use g77 but i haven't been able to find it])
	fi
fi

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
## No Frotran compiler specified... We rely on Autoconf to find the best
   AC_PROG_F77
fi

if test -z "$F77"; then
  AC_MSG_ERROR([No fortran compiler found. Cannot compile scilab without a fortran compiler])
else
	case "$F77" in
	 *90*|*95*|gfortran|fort|ifort|ifc|efc|ftn|pghpf)
	 AC_DEFINE([G95_FORTRAN],[],[uses G95 fortran])
	 ;;
	esac
fi

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


### C++ ###
AC_PROG_CXX
AC_CHECK_PROG(cxx_present, $CXX, "yes", "no")
if test "x$cxx_present" != "xyes"; then
    AC_MSG_ERROR([No C++ compiler found. Cannot compile scilab without a C++ compiler])
fi

# for "subdir-objects"

AM_PROG_CC_C_O
AC_PROG_F77_C_O

# If this option is provided, enable the debug on C & C++
if test "$enable_stop_on_warning" = yes; then
   enable_debug_C=yes
   enable_debug_CXX=yes
fi

if test "$enable_debug" = yes; then
   enable_debug_fortran=yes
   enable_debug_C=yes
   enable_debug_CXX=yes
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



if test "x${prefix}" = "xNONE"; then
  prefix="${ac_default_prefix}"
fi

###############################
## get the version
###############################

SCIVERSION=`cat $SCI_SRCDIR/Version.incl | sed -e "s/SCIVERSION=//" `

########################
## removing and cleaning
########################

RM='rm -f'
FILES_TO_CLEAN='*.CKP *.ln *.BAK *.bak core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut .*~ *.trace so_locations *.orig *.cos *.obj'


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
######## With F77 / G77 / GFortran ...
######################

if test -n "$F77"; then
dnl @TODO : il faut peut etre verifier que c'est bien gfortran
   if test "$enable_debug_fortran" = yes; then
	  FFLAGS="$FFLAGS -g -Wall "
	  # -D_FORTIFY_SOURCE=2
   else
	  FFLAGS="$FFLAGS -DNDEBUG"
   fi
	  case "$host" in
		 x86_64-*-linux-gnu)
			FFLAGS="$FFLAGS -march=athlon64 -mfpmath=sse -msse2 -m3dnow -m64 -fPIC"
					;;
		# Dec Alpha OSF 4
		alpha*-dec-osf4.*)
	      FFLAGS="$FFLAGS -fpe3"
		;;
		alpha*-dec-osf*)
		    FFLAGS="$FFLAGS -fpe3 -switch nosqrt_recip"
		;;
		rs6000-*-*)
		    FFLAGS="$FFLAGS -qcharlen=4096"
			;;
		mips-*-ultrix*)
		    FFLAGS="-O0 -fpe1"
		;;
		  *-*-hpux9.*)
			FFLAGS="$FFLAGS +Obb1200 +E4 -Dhpux"
		;;
		  *-*-hpux10.*)
			if test "$enable_debug_fortran" = yes; then
			    FFLAGS="+E4 +Z +DAportable -Dhpux"
			else
	   		    FFLAGS="+O2 +E4 +Z +DAportable -Dhpux"
			fi
#		    F77_LDFLAGS="-Wl,+vnocompatwarnings,-E /usr/lib/libdld.sl"

			;;
		  *-*-hpux11.*)
		   if test "$enable_debug_fortran" = yes; then
			  FFLAGS=" +Z +DAportable -Dhpux"
			 else
		      FFLAGS="+O2 +Z +DAportable -Dhpux"
		   fi
			  FLIBS="-ldld -lnsl -lU77 -lm"
    #do not call AC_F77_LIBRARY_LDFLAGS because if assumes PA2.0 while Scilab is built with
    #DAportable=PA1.1
    #AC_F77_LIBRARY_LDFLAGS
	# @TODO : check why and if it is still the case

#    F77_LDFLAGS="-Wl,+vnocompatwarnings,-E /usr/lib/libdld.sl"
	 ;;
	  esac
#	   FFLAGS_O0=`echo $FFLAGS | sed 's/-O[0-9]*//g'`
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
esac

AM_CONDITIONAL(IS_HPUX, test -n "$HPUX")
AM_CONDITIONAL(IS_SPARC, test -n "$SPARC")
AM_CONDITIONAL(IS_MIPS_SGI_IRIX, test -n "$MIPS_SGI_IRIX")



############
## C++
############


if test -z "$CXX"; then
  AC_MSG_ERROR([No C++ compiler found. Cannot compile scilab without a C++ compiler])
fi

if test "$CXX" = "g++"; then
   ## With GNU C++ Compiler
   if test "$enable_debug_CXX" = yes; then
	  CXXFLAGS="$CXXFLAGS -Wshadow -Wpointer-arith -Wcast-align -Wreturn-type -Wswitch -Wtrigraphs -Wcomment -W -Wchar-subscripts -Wformat -Wparentheses -Wsign-compare -Wwrite-strings -Wunused -Wstrict-aliasing -Wextra -Wall "
   else
	  CXXFLAGS="$CXXFLAGS -DNDEBUG"
   fi
fi

##########"
# not useful under Linux (and was only defined here with scilab 4)
#AC_CHECK_LIB(ieee,main,[LIEEE="-lieee"])

#LDFLAGS="$LIEEE $LDFLAGS"
#F77_LDFLAGS="$F77_LDFLAGS $LIEEE"
CPU_64_BITS=false
if test "$CC" = "gcc"; then
   ## With GNU Compiler
   if test "$enable_debug_C" = yes; then
	  CFLAGS="$CFLAGS -Wformat -Wshadow -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations -Wall "
	  # used to be -O
	  # -D_FORTIFY_SOURCE=2
   else
	  CFLAGS="$CFLAGS  -DNDEBUG"
   fi

   case "$host" in
   # Linux sparc | Linux, old binary format a.out (NO LONGER SUPPORTED)
	 sparc-*-linux-gnu|*-pc-linux-gnuaout)
	    CFLAGS="$CFLAGS -Dlinux -fwritable-strings"
	;;
	 x86_64-*-linux-gnu)
		CFLAGS="$CFLAGS -Dlinux -DNARROWPROTO -march=athlon64 -mfpmath=sse -msse2 -m3dnow -m64"
		CPU_64_BITS=enabled
    ;;
	 i*86-*-linux-gnu | *-pc-linux-gnu )
		CFLAGS="$CFLAGS -Dlinux -DNARROWPROTO"
	;;		
	 ia64-*-linux-gnu )
		CFLAGS="$CFLAGS -Dlinux -DNARROWPROTO"
		CPU_64_BITS=enabled
    ;;
	 alpha-*-linux-gnu)
		CFLAGS="$CFLAGS  -mieee-with-inexact -Dlinux -fwritable-strings"
		LDFLAGS="$LDFLAGS -mieee-with-inexact"
		CPU_64_BITS=enabled
	;;
	 powerpc-*-linux-gnu)
		CFLAGS="$CFLAGS -D_GNU_SOURCE -Dlinux -fwritable-strings"
	;;
	 *-*-solaris*)
        if test "$enable_debug_C" = yes; then
			 CFLAGS="$CFLAGS -DSVR4 -DSYSV -Dsolaris"
		else
			 CFLAGS="-O2 -DNDEBUG -DSVR4 -DSYSV -Dsolaris"
		fi
	;;
   *-*-freebsd*)
     CFLAGS="$CFLAGS -Dfreebsd"
	 LDFLAGS="$LDFLAGS -m"
	 ;;
  *-*-netbsd*)
    CFLAGS="$CFLAGS -Dnetbsd"

	;;
  *-*-darwin*)
     if test "$enable_debug_C" = yes; then
	      CFLAGS=" $CFLAGS -I/sw/include -L/sw/lib"
	 else
	      CFLAGS=" -O2 -DNDEBUG -no-cpp-precomp -I/sw/include -L/sw/lib"
	 fi
     LDFLAGS="$LDFLAGS -L/sw/lib"
	;;
esac

else

		## CC compiler (not GCC)
        if test "$enable_debug_C" = yes; then
		     CFLAGS="$CFLAGS -g"
		else
		     CFLAGS="$CFLAGS -DNDEBUG"
		fi

case "$host" in
# Dec Alpha OSF 4
  alpha*-dec-osf4.*)
      CFLAGS="$CFLAGS -std -ieee_with_inexact"
      LDFLAGS="$LDFLAGS -ieee_with_inexact"
	  ;;
  alpha*-dec-osf*)
      CFLAGS="$CFLAGS -ieee_with_inexact"
      LDFLAGS="$LDFLAGS -ieee_with_inexact"
      ;;
  rs6000-*-*) # IBM AIX RS 6000 (NO LONGER SUPPORTED)
    CFLAGS="-Daix -DSYSV"
  ;;
  mips-sgi-irix*) # SGI
	CFLAGS="$CFLAGS -DSYSV -DSVR4"
  ;;
  *-*-hpux9.*)
	CFLAGS="$CFLAGS -DSYSV -Dhpux"
  ;;
  *-*-hpux10.*) # HP 10
    if test "$enable_debug_C" = yes; then
	  CFLAGS="$CFLAGS -DSYSV -Dhpux"
    else
	  CFLAGS="$CFLAGS -DSYSV -Dhpux +Z +DAportable"
    fi
  ;;

  *-*-hpux11.*) # HP 11
    CFLAGS="$CFLAGS -DSYSV -Dhpux -Dhppa +Z +DAportable"
  ;;
  *-*-solaris*) # SUN SOLARIS
    CFLAGS="$CFLAGS -DSVR4 -DSYSV -Dsolaris -I/usr/local/include/"
	LDFLAGS="$LDFLAGS  -L/usr/local/lib/"
  ;;
esac
fi


# flag for possible compilations in configure
CFLAGS="$CFLAGS "'-I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/localization/includes/'
CXXFLAGS="$CXXFLAGS "'-I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/localization/includes/'
FFLAGS="$FFLAGS "'-I$(top_srcdir)/modules/core/includes/'

AM_CONDITIONAL(CPU_64_BITS, test "$CPU_64_BITS" = "enabled")

#######################
## test for underscores (name mangling issues between C and fortran)
#######################
AC_CHECK_UNDERSCORE_FORTRAN()

###########################
## test for JAVA compiler
###########################

if test "$with_javasci" != no -o "$with_gui" != no; then
   # See if --with-jdk command line argument is given
     # Try to detect the installed JVM, this could be controlled
	 # by the above --with options
	 AC_JAVA_WITH_JDK
	 AC_JAVA_DETECT_JVM
   if test "$ac_java_jvm_name" = "jdk"; then
		JAVA_HOME=$ac_java_jvm_dir
	    JAVA_VERSION=$ac_java_jvm_version
#     AC_JAVA_TOOLS
		AC_JAVA_JNI_INCLUDE
		JAVA_JNI_INCLUDE=$ac_java_jvm_jni_include_flags

		AC_JAVA_JNI_LIBS
		JAVA_JNI_LIBS=$ac_java_jvm_jni_lib_flags
		JAVA_JNI_LIBS_PRELOAD=$ac_java_jvm_ld_preload
		AC_JAVA_CLASSPATH
		JAVA_CLASSPATH=$ac_java_classpath
		AC_JAVA_TOOLS
		AC_JAVA_ANT
		if test "$with_gui" != no; then
				AC_JAVA_CHECK_PACKAGE([flexdock],[org.flexdock.docking.DockingManager],[Scilab Gui])
				AC_JAVA_CHECK_PACKAGE([jogl],[javax.media.opengl.glu.GLUnurbs],[Scilab 3D rendering - Version 1.1.1-pre 20071112])
				AC_JAVA_CHECK_PACKAGE([gluegen-rt],[com.sun.gluegen.runtime.CPU],[Scilab 3D rendering])
				AC_JAVA_CHECK_PACKAGE([JRosetta-API-1.0],[com.artenum.rosetta.interfaces.core.ConsoleConfiguration],[JRosetta : Console API Artenum / Scilab])
				AC_JAVA_CHECK_PACKAGE([JRosetta-1.0],[com.artenum.rosetta.core.action.AbstractConsoleAction],[JRosetta : Console Core Artenum / Scilab])
				AC_JAVA_CHECK_PACKAGE([commons-logging],[org.apache.commons.logging.LogFactory],[Apache logging])
				AC_DEFINE([WITH_GUI],[],[With the JAVA stuff (GUI, Console, JOGL...])
		fi
	else
		AC_MSG_WARN([Sun javac not found: I will not build the java interface])
		if test "$ac_java_jvm_name" != ""; then
			AC_MSG_WARN([We do not support $ac_java_jvm_name yet])
		fi
	fi
	AC_SUBST(JAVA_JNI_INCLUDE)
    AC_SUBST(JAVA_JNI_LIBS)
    AC_SUBST(JAVA_HOME)

	if test "$with_swig_generation" != no -a "$with_swig_generation" != ""; then
		SWIG_PROG()
		SWIG_ENABLE_JAVA()
		AC_SUBST(SWIG_BIN)
        AC_SUBST(SWIG_JAVA)

	fi

	# Giws is the equivalent of Swig developped by the Scilab team
	# in order to provide a wrapper to Java from C/C++
	if test "$with_giws_generation" != no -a "$with_giws_generation" != ""; then
		AC_GIWS()
	fi

fi

AM_CONDITIONAL(GUI, test "$with_gui" != no)
AM_CONDITIONAL(JAVASCI, test "$with_javasci" != no)
AM_CONDITIONAL(SWIG, test "$with_swig_generation" != no -a "$with_swig_generation" != "")
AM_CONDITIONAL(GIWS, test "$with_giws_generation" != no  -a "$with_giws_generation" != "")

##############################################################
## test for functions in standard C library and C math library
##############################################################

# Provided by unistd.h
AC_CHECK_FUNCS([sleep] [usleep] [dup2] [getcwd] [getpagesize] [getpass])
AC_CHECK_FUNCS([rmdir])

AC_CHECK_FUNC([getwd],AC_DEFINE([HAVE_GETWD],[1],[Define to 1 if you have the `getwd' function. ]),[AC_DEFINE([getwd(x)],[getcwd(x,1024)],[Don't use getwd but getcwd])])

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

# Provided by math.h
AC_CHECK_FUNCS([pow] [sqrt] [finite] [floor])

AC_CHECK_FUNC([exp10],,[
				AC_DEFINE([log_10_],[2.3025850929940456840179914546844],[Provide a macro to do exp10])
				AC_DEFINE([exp10(x)],[exp( (log_10_) * (x) )],[Provide a macro to do exp10])
				])

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

AC_CHECK_HEADERS([fcntl.h float.h locale.h malloc.h netdb.h netinet/in.h nlist.h sgtty.h stddef.h sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/timeb.h syslog.h termio.h termios.h wchar.h wctype.h libintl.h])

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

TERMCAP_LIB=no
AC_CHECK_LIB(ncurses,main,[TERMCAP_LIB=-lncurses])
if test "$TERMCAPLIB" = no; then
  AC_CHECK_LIB(curses,main,[TERMCAP_LIB=-lcurses])
fi
if test "$TERMCAPLIB" = no; then
  AC_CHECK_LIB(termcap,main,[TERMCAP_LIB=-ltermcap])
fi
if test "$TERMCAPLIB" = no; then
  AC_CHECK_LIB(termlib,main,[TERMCAP_LIB=-ltermlib])
fi
if test "$TERMCAP_LIB" != no; then
  AC_DEFINE([HAVE_TERMCAP],[],[Have Term Cap])
else
 AC_MSG_ERROR([No termcap library detected. Please install ncurses dev library (or termcap library)])
fi
AC_SUBST(TERMCAP_LIB)



#################
## SCICOS
#################

AC_ARG_WITH(ocaml,
    [  --without-ocaml         do not compile ocaml modules ])

AC_ARG_WITH(scicos,
    [  --without-scicos        do not compile scicos ])

if test "$with_scicos" = yes; then
   AC_DEFINE([WITHOUT_SCICOS], [], [Without SCICOS])
fi
AM_CONDITIONAL(SCICOS, test "$with_scicos" = yes)


#################
## FFTW
#################

AC_ARG_WITH(fftw,
    [  --with-fftw	           compile with the FFTW 3 library ])
# @TODO change that to add the possibility to specify a directory
if test "$with_fftw" = yes; then
   AC_FFTW()
fi
AM_CONDITIONAL(FFTW, test "$with_fftw" = yes)

#################
## MPI
#################

AC_ARG_WITH(mpi,
    [  --with-mpi	           compile with MPI library ])

if test "$with_mpi" = yes; then
   AC_OPENMPI()
   # We will have to detect other implementation of OpenMPI
fi
AM_CONDITIONAL(MPI, test "$with_mpi" = yes)

#################
## PVM
#################

AC_ARG_WITH(pvm,
    [  --with-pvm	           compile with PVM library ])

if test "$with_pvm" = yes; then
   AC_PVM()
fi
AM_CONDITIONAL(PVM, test "$PVM_OK" = "1")

#######################
## Test for libxml
#######################

AC_LIBXML2()

#######################
## Test for gettext
#######################

AC_ARG_WITH(localization-generation,
		[  --with-localization-generation    enable the localization generation],
		[with_localization=yes],
		[with_localization='no']
		)

if test $with_localization != no; then

   ALL_LINGUAS="en_US fr_FR"
   AC_SUBST(ALL_LINGUAS)

   AC_PATH_PROG(MSGCAT, msgcat, no)
   AC_PATH_PROG(MSGFMT, msgfmt, no)
   AC_PATH_PROG(XGETTEXT, xgettext, no)

   if test x$MSGFMT = xno; then
	  AC_ERROR([As you used to the option --with-localization-generation, the msgfmt command is required to build Scilab.  If it is installed on your system, ensure that it is in your path.  If it is not, install GNU gettext to continue.])
	  fi
	if test x$MSGCAT = xno; then
	  AC_ERROR([As you used to the option --with-localization-generation, the msgcat command is required to build Scilab. If it is installed on your system, ensure that it is in your path.  If it is not, install GNU gettext to continue.])
	fi

fi
AM_CONDITIONAL(GENERATE_LOCALIZATION_FILES, test "$with_localization" != no)

#######################
## Test for blas/Atlas and lapack
#######################
AC_MSG_CHECKING([if BLAS, ATLAS or MKL is available])
echo ""
EMBEDDED_BLAS=0
ACX_BLAS(
		[]
		,
		AC_MSG_WARN([Impossible to find a blas compatible library (see BLAS or ATLAS). Use the version of generic blas embedded])
		EMBEDDED_BLAS=1
		)
AM_CONDITIONAL(USE_EMBEDDED_BLAS, test $EMBEDDED_BLAS = 1)


AC_MSG_CHECKING([if LAPACK is available])
echo ""
EMBEDDED_LAPACK=0
ACX_LAPACK(
		[],
		AC_MSG_WARN([Impossible to find the LAPACK library. Use the version of lapack embedded])
		EMBEDDED_LAPACK=1
		)
AM_CONDITIONAL(USE_EMBEDDED_LAPACK, test $EMBEDDED_LAPACK = 1)

#######################
## Test for PCRE
#######################

AC_PCRE()
	  	 
 #############################################
#################
## Tcl/Tk library
#################

WITH_TKSCI=0
if test "$with_tk" != no; then

	# check user arguments
	USER_TCL_LIB_PATH=""
	USER_TCL_INC_PATH=""
	AC_ARG_WITH(tcl-library,
		[  --with-tcl-library=DIR  Tcl library files are in DIR ],
		[  USER_TCL_LIB_PATH=$withval
	])
	AC_ARG_WITH(tcl-include,
		[  --with-tcl-include=DIR  Tcl include files are in DIR ],
		[  USER_TCL_INC_PATH=$withval
	])

	USER_TK_LIB_PATH=$USER_TCL_LIB_PATH
	USER_TK_INC_PATH=$USER_TCL_INC_PATH

	AC_ARG_WITH(tk-library,
		[  --with-tk-library=DIR   Tk library files are in DIR ],
		[  USER_TK_LIB_PATH=$withval
	])

	AC_ARG_WITH(tk-include,
		[  --with-tk-include=DIR   Tk include files are in DIR ],
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
	saved_cppflags="$CPPFLAGS"

	AC_CHECK_LIB([dl], [main], [TCLTK_LIBS=" -ldl"])
	AC_CHECK_TCLTK

	# set variables
	if test $WITH_TKSCI = 1; then
		AC_DEFINE([WITH_TK], [], [With TK])
	else
		AC_MSG_ERROR([TCL/TK not found. Use --without-tk or specify the librairies and include paths manually])
	fi

	AC_SUBST(TCLTK_LIBS)
	AC_SUBST(TCL_INC_PATH)
	AC_SUBST(TK_INC_PATH)

fi
AM_CONDITIONAL(TCLTK, test "$WITH_TKSCI" = 1)

#################
## ocaml
#################

if test "$with_ocaml" != no; then
   AC_CHECK_PROG_OCAML()
fi


#################
## Man management
#################

dnl check for xsltproc
AC_ARG_WITH(sablotron,
		[  --with-sablotron compile man pages with sablotron],
		[with_sablotron=$withval],
		[with_sablotron='']
	)

AC_ARG_WITH(xsltproc,
		[  --with-xsltproc  compile man pages with xsltproc],
		[with_xsltproc=$withval],
		[with_xsltproc='']
		)

if test "x$with_xsltproc" != 'x' -a "x$with_sablotron" != 'x'; then
	AC_MSG_ERROR([Conflicting options : you specified two xsl compilers (sablotron & sabcmd)])
fi

AC_CHECK_MAN_GENERATOR([$with_sablotron],[Sablotron],[sabcmd])
if test "x$MANGENERATOR" = 'x' -o "x$with_xsltproc" != 'x'; then
   AC_CHECK_MAN_GENERATOR([$with_xsltproc],[xsltproc],[xsltproc])
fi
AC_SUBST(MANGENERATOR)

AC_ARG_WITH(install-help-xml,
		[  --with-install-help-xml  make install will install XML files ],
		[with_install_help_xml=$withval],
		[with_install_help_xml='']
		)

AM_CONDITIONAL(INSTALL_HELP_XML, test "$with_install_help_xml" = "yes")


#########################
## libtool
#########################

AC_PROG_LIBTOOL
LT_INIT([shared static dlopen])


# Check to see if building shared libraries
libtool_build_shared_libs='no'
if test "$enable_shared" = 'yes'; then
  libtool_build_shared_libs='yes'
fi

# Check to see if building static libraries
libtool_build_static_libs='no'
if test "$enable_static" = 'yes'; then
  libtool_build_static_libs='yes'
fi

# Mandatory for Debian (and more generaly for Linux)
# See http://wiki.debian.org/RpathIssue
# by Marcelo Magallon <mmagallo@efis.ucr.ac.cr>
# Turn around -rpath problem with libtool
# This define should be improbable enough to not conflict with anything
 case ${host} in
   *-pc-linux-gnu)
     AC_MSG_RESULT([Fixing libtool for -rpath problems.])
     sed < $SCI_SRCDIR/libtool > $SCI_SRCDIR/libtool-2 \
    's/^hardcode_libdir_flag_spec.*$/hardcode_libdir_flag_spec=" -D__LIBTOOL_IS_A_FOOL__ "/'
     mv $SCI_SRCDIR/libtool-2 $SCI_SRCDIR/libtool
     chmod 755 $SCI_SRCDIR/libtool
   ;;
 esac
####### End of the workaround


AC_C_CONST()
AC_C_INLINE()


#######################
###### Creation of the header file (machine.h)
#######################

AC_DEFINE_UNQUOTED([PATH_SEPARATOR], "$PATH_SEPARATOR",
[The default path separator character.])

AH_TOP([#ifndef MACHINE_H
#define MACHINE_H

])

AH_BOTTOM([
/*
   Define integer C type which must fit Fortran integer
   For Scilab to work, the rule is:
          size of Fortran double precision = 2 * size of Fortran integer

   At the present time, we suppose:
       size of Fortran integer = 4 bytes
       size of Fortran double precision = 8 bytes
       size of C int = 4 bytes
*/

#ifdef DIR_SEPARATOR
	   #undef DIR_SEPARATOR
#endif
#define DIR_SEPARATOR "/"

/* Defines PATH_MAX */
#include "PATH_MAX.h"

/* Defines BOOL type */
#include "BOOL.h"

/* Defines PARAMS type */
#include "PARAMS.h"

#endif /* MACHINE_H  */
])

AM_CONDITIONAL(TERM_ZTERM, test "$TERM_NAME" = "zterm")
AM_CONDITIONAL(TERM_VTE, test "$GUI" = "vte")

AC_CONFIG_COMMANDS_POST([chmod +x $SCI_SRCDIR_FULL/modules/scicos/src/c/GenBlocks.sh])

dnl #########################
dnl ## substituting variables
dnl #########################
AC_SUBST(tcldir)

# Define the standard extension of a dynamic library
AC_DEFINE_UNQUOTED([SHARED_LIB_EXT],["$shrext_cmds"],[Extension of a shared library])


#################
## stop on warning
#################

# Stop to compile scilab when a warning is found ...
# This stuff is at the end of the configure.ac because it causes some
# problem with AC_COMPILE (the -Werror is added to the test)
#
dnl @TODO : -O0 is only to bypass Stack shit... Must be removed.
if test "$enable_stop_on_warning" = yes; then
   CFLAGS="$CFLAGS -Werror -O0"
   CXXFLAGS="$CXXFLAGS -Werror -O0"
fi

AC_CONFIG_FILES([
libs/lapack/Makefile
libs/blas/Makefile
libs/MALLOC/Makefile
libs/dynamiclibrary/Makefile
libs/doublylinkedlist/Makefile
libs/hashtable/Makefile
libs/libst/Makefile
modules/helptools/Makefile
modules/data_structures/Makefile
modules/differential_equations/Makefile
modules/optimization/Makefile
modules/elementary_functions/Makefile
modules/special_functions/Makefile
modules/io/Makefile
modules/shell/Makefile
modules/completion/Makefile
modules/history_manager/Makefile
modules/jvm/Makefile
modules/sound/Makefile
modules/statistics/Makefile
modules/mexlib/Makefile
modules/sparse/Makefile
modules/linear_algebra/Makefile
modules/polynomials/Makefile
modules/symbolic/Makefile
modules/signal_processing/Makefile
modules/arnoldi/Makefile
modules/interpolation/Makefile
modules/intersci/Makefile
modules/localization/Makefile
modules/metanet/Makefile
modules/cacsd/Makefile
modules/boolean/Makefile
modules/integer/Makefile
modules/double/Makefile
modules/fileio/Makefile
modules/spreadsheet/Makefile
modules/string/Makefile
modules/time/Makefile
modules/graphics/Makefile
modules/renderer/Makefile
modules/action_binding/Makefile
modules/gui/Makefile
modules/pvm/Makefile
modules/randlib/Makefile
modules/tclsci/Makefile
modules/windows_tools/Makefile
modules/core/Makefile
modules/fftw/Makefile
modules/scicos/Makefile
modules/scicos/src/ocaml/Makefile
modules/functions/Makefile
modules/incremental_link/Makefile
modules/others/Makefile
modules/overloading/Makefile
modules/javasci/Makefile
modules/maple2scilab/Makefile
modules/m2sci/Makefile
modules/compatibility_functions/Makefile
modules/texmacs/Makefile
modules/scipad/Makefile
modules/scilab2fortran/Makefile
modules/development_tools/Makefile
modules/output_stream/Makefile
modules/console/Makefile
modules/Makefile
libs/Makefile
Makefile
etc/modules.xml
modules/helptools/macros/xmlfiletohtml.sci
])#MAN_FR MAN_ENG
#modules/mpi/Makefile
#for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
#  test -f $top_builddir/configure && break
#done


#if test "$with_mpi" = yes; then
#AC_CONFIG_FILES([modules/mpi/Makefile])
#fi

AC_OUTPUT

echo ""

echo "Scilab is configured as follows. Please verify that this configuration"
echo "matches your expectations."
echo ""
echo "Host system type : $host"
echo ""
echo "                  Option                        Value"
echo "-------------------------------------------------------------------------"
echo "Shared libraries.......      --enable-shared=$libtool_build_shared_libs	$libtool_build_shared_libs"
echo "Static libraries.......      --enable-static=$libtool_build_static_libs	$libtool_build_static_libs"
echo "GNU ld.................      --with-gnu-ld=$with_gnu_ld		$lt_cv_prog_gnu_ld"
echo "Enable debug ..........      --enable-debug=$enable_debug	$enable_debug"
echo "Enable debug C.........      --enable-debug-C=$enable_debug_C	$enable_debug_C"
echo "Enable debug C++.......      --enable-debug-CXX=$enable_debug_CXX	$enable_debug_CXX"
echo "Enable debug Fortran...      --enable-debug-fortran=$enable_debug_fortran	$enable_debug_fortran"
echo "Enable stop on warning.      --enable-stop-on-warning=$enable_stop_on_warning	$enable_stop_on_warning"

echo ""
echo "Compiler Configuration:"
echo "Intel (--with-intelcompilers) .... :		$with_intelcompilers"
echo "GNU gcc (--with-gcc) ............. :		$with_gcc"
echo "GNU Fortran 77 (--with-g77) ...... :		$with_g77"
echo "GNU Fortran 95 (--with-gfortran) . :		$with_gfortran"
echo ""
echo "Options:"
echo "Use PVM (--with-pvm) ............................ : $with_pvm"
echo "PVM include (--with-pvm-include) ................ : $PVMINCLUDE"
echo "PVM library (--with-pvm-library) ................ : $PVMLIBDIR"
echo "Do not use TCL/TK (--without-tk) ................ : $with_tk"
echo "Do not compile with Ocaml (--without-ocaml) ..... : $with_ocaml"
echo "TCL include (--with-tcl-include) ................ : $USER_TCL_INC_PATH"
echo "TCL library (--with-tcl-library) ................ : $USER_TCL_LIB_PATH"
echo "TK include (--with-tk-include) .................. : $USER_TK_INC_PATH"
echo "TK library (--with-tk-library) .................. : $USER_TK_LIB_PATH"
echo "Atlas library (--with-atlas-library) ............ : $USER_ATLAS_LIB_PATH"
echo "Sablotron processor (--with-sablotron) .......... : $with_sablotron"
echo "Xsltproc processor (--with-xsltproc) ............ : $with_xsltproc"
echo "Install XML Help (--with-install-help-xml) ...... : $with_install_help_xml"
echo "Do not compile Scicos (--without-scicos) ........ : $without_scicos"
echo "Use FFTW (--with-fftw) .......................... : $with_fftw"

echo ""
if test ! -z "$PVM_INCLUDE"; then
   echo "PVM Configuration:"
   echo "PVM Architecture ... = $PVMARCH"
   echo "PVM INCLUDE ........ = $PVM_INCLUDE"
   echo "PVM LIBS ........... = $PVM_LIB"
else
   echo "Not using PVM"
fi

echo ""
if test "$with_fftw" != no;  then
   echo "FFTW Configuration:"
   echo "FFTW LIBS .......... = $FFTW3_LIB"
else
   echo "Not using FFTW"
fi


echo ""
echo "BLAS/LAPACK/ATLAS Configuration:"
if test $EMBEDDED_BLAS = 0; then
   echo "BLAS LIBS ............. = $BLAS_LIBS"
else
   echo "BLAS LIBS ............. = Use the generic version of BLAS embedded"
fi

if test $EMBEDDED_LAPACK = 0; then
   echo "LAPACK LIBS ........... = $LAPACK_LIBS"
else
   echo "LAPACK LIBS ........... = Use the LAPACK embedded"
fi

echo ""

#if test "$with_mpi" = yes; then
#   echo "MPI Configuration:"
#   echo "MPI LIBS ........... = $MPILIBS"
#   echo "MPI C Compiler ..... = $MPICC"
#   echo "MPI C++ Compiler ... = $MPICXX"
#   echo "MPI F77 Compiler ... = $MPIF77"
#else
#   echo "Not using MPI"
#fi

if test "$with_openmpi" != "no"; then
   echo "OpenMPI Configuration:"
   echo "OpenMPI LIBS ........... = $OPENMPI_LIBS"
   echo "OpenMPI C Compiler ..... = $OPENMPI_CC"
   echo "OpenMPI F77 Compiler ... = $MPIF77"
else
   echo "Not using MPI"
fi

echo ""
if test $with_localization != no; then
   echo "Gettext/localization configuration:"
   echo "xgettext ............... = $XGETTEXT"
   echo "msgfmt  ................ = $MSGFMT"
   echo "msgfmt_opts ............ = $MSGFMT_OPTS"
   echo "msgcat  ................ = $MSGCAT"
else
   echo "Won't generate localization files"
fi

echo ""
if test ! -z "$MANGENERATOR"; then
   echo "XSL Processor Configuration:"
   echo "XSL Processor ...... = $MANGENERATOR"
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
echo "  JAR ................. = $JAR"
echo "  ANT ................. = $ANT"
else
echo "  JAVA disabled"
fi


echo ""
echo "SWIG Configuration:"
if test ! -z "$SWIG_BIN"; then
echo "  SWIG_BIN ............ = $SWIG_BIN"
echo "  SWIG_JAVA ........... = $SWIG_JAVA"
else
echo "  SWIG generation disabled"
fi

echo ""
echo "GIWS Configuration:"
if test ! -z "$GIWS_BIN"; then
echo "  GIWS_BIN ............ = $GIWS_BIN"
else
echo "  GIWS generation disabled"
fi

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
echo "Platform informations:"
echo "  host ........... = $host"
echo "  host_cpu ....... = $host_cpu"
echo "  host_vendor .... = $host_vendor"
echo "  host_os ... .... = $host_os"
echo "  hostname ....... = $ac_hostname"
echo "  64 bits CPU .... = $CPU_64_BITS"

echo ""
echo "Options used to compile and link:"
echo "  prefix ......... = $prefix"
echo "  localedir ...... = $localedir"
echo "  VERSION ........ = $PACKAGE_VERSION"
echo "  CC ............. = $CC"
echo "  CFLAGS ......... = $CFLAGS"
echo "  PCFLAGS ........ = $PCFLAGS"
echo "  DEFS ........... = $DEFS"
echo "  LD ............. = $LD"
echo "  LDFLAGS ........ = $LDFLAGS"
echo "  LIBS ........... = $LIBS"
echo "  CXX ............ = $CXX"
echo "  CXXFLAGS ....... = $CXXFLAGS"
echo "  F77 ............ = $F77"
echo "  FFLAGS ......... = $FFLAGS"
echo "  F77_LDFLAGS .... = $F77_LDFLAGS"
echo "  XML_FLAGS ...... = $XML_FLAGS"
echo "  XML_LIBS ....... = $XML_LIBS"
echo "  PCRE_CFLAGS .... = $PCRE_CFLAGS"
echo "  PCRE_LIBS ...... = $PCRE_LIBS"
echo "  TERMCAP_LIB .... = $TERMCAP_LIB"
echo "  TK_INC_PATH .... = $TK_INC_PATH"
echo "  TCL_INC_PATH ... = $TCL_INC_PATH"
echo "  TCLTK_LIBS ..... = $TCLTK_LIBS"
echo ""
