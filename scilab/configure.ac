##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007/2008
##########
dnl Process this file with autoconf to produce a configure script.


AC_REVISION($Revision$)dnl
AC_INIT([Scilab],[5],[http://bugzilla.scilab.org/])
AC_PREREQ(2.60)

SCI_BUILDDIR="`pwd`"
SCI_SRCDIR="$srcdir"
SCI_SRCDIR_FULL="`cd $SCI_SRCDIR && pwd`"

SCILAB_LIBRARY_VERSION=5:1:0
AC_SUBST(SCILAB_LIBRARY_VERSION)

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

# strip executable
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S

AC_SYS_LARGEFILE

# In order to be able to change the scilab directory
# See http://wiki.debian.org/RpathIssue
AC_RELOCATABLE
AC_RELOCATABLE_LIBRARY


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
   AC_HELP_STRING([--enable-debug],[Do not optimize and print warning messages (C/C++/Fortran code)]))

AC_ARG_ENABLE(debugC,
   AC_HELP_STRING([--enable-debug-C],[Do not optimize and print warning messages (C code)]))

AC_ARG_ENABLE(debugCXX,
   AC_HELP_STRING([--enable-debug-CXX],[Do not optimize and print warning messages (C++ code)]))

AC_ARG_ENABLE(debugFortran,
   AC_HELP_STRING([--enable-debug-fortran],[Do not optimize and print warning messages (Fortran code)]))

AC_ARG_ENABLE(debugLinker,
   AC_HELP_STRING([--enable-debug-linker],[Print warning messages from the linker (ld)]))

AC_ARG_ENABLE(stopOnWarning,
   AC_HELP_STRING([--enable-stop-on-warning],[Stop the compilation on the first warning found in the C/C++ code]))

AC_ARG_WITH(gcc,
    AC_HELP_STRING([--with-gcc],[Use gcc C compiler ]))

AC_ARG_WITH(g77,
    AC_HELP_STRING([--with-g77],[Use g77, GNU Fortran 77 compiler ]))

AC_ARG_WITH(gfortran,
    AC_HELP_STRING([--with-gfortran],[Use gfortran, GNU Fortran 95 compiler ]))

AC_ARG_WITH(intelcompilers,
    AC_HELP_STRING([--with-intelcompilers],[Use Intel C (icc) and Fortran (ifort) proprietary compilers (GNU/Linux only) ]))

AC_ARG_WITH(tk,
    AC_HELP_STRING([--without-tk],[Do not compile with TCL/TK ]))

AC_ARG_WITH(javasci,
    AC_HELP_STRING([--without-javasci],[Do not compile the Java/Scilab module ]))

AC_ARG_WITH(gui,
    AC_HELP_STRING([--without-gui],[Do not compile the Graphical User Interface (GUI). Intended for embedded/clustering/grid Scilab ]))

AC_ARG_ENABLE(build-swig,
    AC_HELP_STRING([--enable-build-swig],[Regenerate Java => C wrappers produces by Swig (A Java Development Kit (JDK) is mandatory for this option)]))

AC_ARG_ENABLE(build-giws,
    AC_HELP_STRING([--enable-build-giws],[Regenerate C/C++ => Java wrappers produces by Giws (A Java Development Kit (JDK) is mandatory for this option)]))


################################################
########## compilator & misc programs detection
################################################

AC_PROG_CPP
AC_PROG_GREP
PKG_PROG_PKG_CONFIG

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

f90compatible=false
if test -z "$F77"; then
  AC_MSG_ERROR([No fortran compiler found. Cannot compile scilab without a fortran compiler])
else
	case "$F77" in
	 *90*|*95*|gfortran|fort|ifort|ifc|efc|ftn|pghpf)
	 AC_DEFINE([G95_FORTRAN],[],[uses G95 fortran])
	 f90compatible=true
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

AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long])

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
	  FFLAGS="$FFLAGS -g -Wall -Wimplicit -Wsurprising"
	  # -D_FORTIFY_SOURCE=2
   else
	  FFLAGS="$FFLAGS -DNDEBUG"
   fi
	  case "$host" in
		 x86_64-*-linux*)
			FFLAGS="$FFLAGS -m64 -fPIC"
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
   if test "$enable_debug_CXX" = yes; then
	  CXXFLAGS="$CXXFLAGS -pipe -Wshadow -Wpointer-arith -Wcast-align -Wreturn-type -Wswitch -Wtrigraphs -Wcomment -W -Wchar-subscripts -Wformat -Wparentheses -Wsign-compare -Wwrite-strings -Wunused -Wno-strict-aliasing -Wextra -Wall -Wold-style-cast -g3 -Weffc++"
          case "$host" in
          *-linux-gnu )
          # Only doing that under Linux
                  CXXFLAGS="$CXXFLAGS -fdiagnostics-show-option -Werror=format-security"
          ;;
	  esac
   else
	  CXXFLAGS="$CXXFLAGS -DNDEBUG"
   fi
   CXXFLAGS="$CXXFLAGS -fno-stack-protector" # bug 3131
   ;;
esac

##########"
# not useful under Linux (and was only defined here with scilab 4)
#AC_CHECK_LIB(ieee,main,[LIEEE="-lieee"])

#LDFLAGS="$LIEEE $LDFLAGS"
#F77_LDFLAGS="$F77_LDFLAGS $LIEEE"
IS_64_BITS_CPU=false
case "$CC" in
	 gcc-* | gcc | ccache*gcc | ccache*gcc-* )
   ## With GNU Compiler
   if test "$enable_debug_C" = yes; then
	  CFLAGS="$CFLAGS -pipe -Wformat -Wshadow -Wfloat-equal -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations  -Wstrict-prototypes  -Wmissing-noreturn -Wendif-labels -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wwrite-strings -Winline -Wredundant-decls -Wall -Wchar-subscripts -Wextra  -Wuninitialized -Wno-format-y2k -Wmissing-format-attribute -Wno-missing-field-initializers -Wno-strict-aliasing  -Wold-style-definition -g3"
	  # used to be -O
	  # -D_FORTIFY_SOURCE=2
	  case "$host" in
	  *-linux-gnu )
	  # Only doing that under Linux
		if test "$enable_debugLinker" = yes; then
	  	  LDFLAGS="$LDFLAGS -Wl,--warn-common,-x"
		fi
		  CFLAGS="$CFLAGS -fdiagnostics-show-option -Werror=format-security"
		  ;;
		  esac
   else
	  CFLAGS="$CFLAGS  -DNDEBUG"
   fi
   CFLAGS="$CFLAGS -fno-stack-protector" # bug 3131

   case "$host" in
   # Linux sparc | Linux, old binary format a.out (NO LONGER SUPPORTED)
	 sparc-*-linux-gnu|*-pc-linux-gnuaout)
	    CFLAGS="$CFLAGS -fwritable-strings"
	;;
	 x86_64-*-linux-gnu | x86_64-linux-gnu)
		CFLAGS="$CFLAGS -DNARROWPROTO -m64"
		IS_64_BITS_CPU=true
    ;;
	 i*86-*-linux-gnu | *-pc-linux-gnu | i*86-*-linux )
		CFLAGS="$CFLAGS  -DNARROWPROTO"
	;;		
	 ia64-*-linux-gnu )
		CFLAGS="$CFLAGS -DNARROWPROTO"
		IS_64_BITS_CPU=true
    ;;
	 alpha-*-linux-gnu)
		CFLAGS="$CFLAGS  -mieee-with-inexact -fwritable-strings"
		LDFLAGS="$LDFLAGS -mieee-with-inexact"
		IS_64_BITS_CPU=true
	;;
	 powerpc-*-linux-gnu)
		CFLAGS="$CFLAGS -D_GNU_SOURCE -fwritable-strings"
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
	 CFLAGS=" $CFLAGS -I/sw/include "
	 CPPFLAGS=" $CPPFLAGS -I/sw/include "
	 LDFLAGS=" $LDFLAGS -L/sw/lib/ "
	;;
esac
;; # end of the gcc case on the $CC
   *)

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
;; # end of not a gcc compiler
esac


# flag for possible compilations in configure
CFLAGS="$CFLAGS "'-I$(top_srcdir)/modules/core/includes/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/localization/includes/'
CXXFLAGS="$CXXFLAGS "'-I$(top_srcdir)/modules/core/includes/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/localization/includes/'
FFLAGS="$FFLAGS "'-I$(top_srcdir)/modules/core/includes/'

if test $IS_64_BITS_CPU = true -o "$MACOSX" = "1"; then
   if test $f90compatible = false; then
      AC_MSG_ERROR([64 bits support needs a fortran 90 compiler (try --with-gfortran).])
   fi
   AC_DEFINE([USE_DYNAMIC_STACK], [], [If we are building a 64 bits version])
fi

AM_CONDITIONAL(IS_64_BITS_CPU, test $IS_64_BITS_CPU = true)
AM_CONDITIONAL(USE_DYNAMIC_STACK, test $IS_64_BITS_CPU = true -o "$MACOSX" = "1")

#######################
## test for underscores (name mangling issues between C and fortran)
#######################
AC_CHECK_UNDERSCORE_FORTRAN()

###########################
## test for JAVA compiler
###########################

if test "$with_javasci" != no -o "$with_gui" != no -o "$enable_build_help" != no; then
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

				# Docking system
				AC_JAVA_CHECK_PACKAGE([flexdock],[org.flexdock.docking.DockingManager],[Scilab Gui])
				FLEXDOCK=$PACKAGE_JAR_FILE
				AC_SUBST(FLEXDOCK)

				# Swing look&feel implementations
				AC_JAVA_CHECK_PACKAGE([looks],[com.jgoodies.looks.common.RenderingUtils],[Scilab Gui - Look and feel],"yes")
				LOOKS=$PACKAGE_JAR_FILE

				# Named differently under Opensuse
				if test -z "$LOOKS"; then
				    AC_JAVA_CHECK_PACKAGE([looks2],[com.jgoodies.looks.common.RenderingUtils],[Scilab Gui - Look and feel])
					LOOKS=$PACKAGE_JAR_FILE
				fi
				AC_SUBST(LOOKS)

				AC_SUBST(LOOKS)

				#  Skin Look and Feel 
				AC_JAVA_CHECK_PACKAGE([skinlf],[com.l2fprod.util.AccessUtils],[Scilab Gui - Skin Look and Feel])
				SKINLF=$PACKAGE_JAR_FILE
				AC_SUBST(SKINLF)

				# JOGL
				AC_JAVA_CHECK_PACKAGE([jogl],[javax.media.opengl.glu.GLUnurbs],[Scilab 3D rendering - Version 1.1.1])
				JOGL=$PACKAGE_JAR_FILE
				AC_SUBST(JOGL)

				# Jhall
				AC_JAVA_CHECK_PACKAGE([jhall],[javax.help.JHelp],[Scilab Help Browser],"yes")
				JHALL=$PACKAGE_JAR_FILE

				# Named differently under Mandriva
				if test -z "$JHALL"; then
					AC_JAVA_CHECK_PACKAGE([javahelp2],[javax.help.JHelp],[Scilab Help Browser])
					JHALL=$PACKAGE_JAR_FILE
				fi
				AC_SUBST(JHALL)

				# JoGL Native <=> Java connector
				AC_JAVA_CHECK_PACKAGE([gluegen-rt],[com.sun.gluegen.runtime.CPU],[Scilab 3D rendering])
				GLUEGEN_RT=$PACKAGE_JAR_FILE
				AC_SUBST(GLUEGEN_RT)

				# Console API
				AC_JAVA_CHECK_PACKAGE([jrosetta-API],[com.artenum.rosetta.interfaces.core.ConsoleConfiguration],[JRosetta : Console API Artenum / Scilab])
				JROSETTA_API=$PACKAGE_JAR_FILE
				AC_SUBST(JROSETTA_API)

				# Console Core
				AC_JAVA_CHECK_PACKAGE([jrosetta-engine],[com.artenum.rosetta.core.action.AbstractConsoleAction],[JRosetta : Console Core Artenum / Scilab])
				JROSETTA_ENGINE=$PACKAGE_JAR_FILE
				AC_SUBST(JROSETTA_ENGINE)

				# Logging (flexdock dep)
				AC_JAVA_CHECK_PACKAGE([commons-logging],[org.apache.commons.logging.LogFactory],[Apache logging])
				COMMONS_LOGGING=$PACKAGE_JAR_FILE
				AC_SUBST(COMMONS_LOGGING)

				AC_DEFINE([WITH_GUI],[],[With the JAVA stuff (GUI, Console, JOGL...)])
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



	if test "$enable_build_swig" != no -a "$enable_build_swig" != ""; then
		SWIG_PROG()
		SWIG_ENABLE_JAVA()
		AC_SUBST(SWIG_BIN)
        AC_SUBST(SWIG_JAVA)

	fi

	# Giws is the equivalent of Swig developped by the Scilab team
	# in order to provide a wrapper to Java from C/C++
	if test "$enable_build_giws" != no -a "$enable_build_giws" != ""; then
		AC_GIWS()
	fi
fi

AM_CONDITIONAL(NEED_JAVA, test "$with_javasci" != no -o "$with_gui" != no -o "$enable_build_help" != no)
AM_CONDITIONAL(GUI, test "$with_gui" != no)
AM_CONDITIONAL(JAVASCI, test "$with_javasci" != no)
AM_CONDITIONAL(SWIG, test "$enable_build_swig" != no -a "$enable_build_swig" != "")
AM_CONDITIONAL(GIWS, test "$enable_build_giws" != no  -a "$enable_build_giws" != "")

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

# Provided by math.h
save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -lm"

AC_CHECK_FUNCS([pow] [sqrt] [finite] [floor])

AC_CHECK_FUNC([exp10],,[
				AC_DEFINE([log_10_],[2.3025850929940456840179914546844],[Provide a macro to do exp10])
				AC_DEFINE([exp10(x)],[exp( (log_10_) * (x) )],[Provide a macro to do exp10])
				])

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

AC_CHECK_HEADERS([fcntl.h float.h libintl.h locale.h malloc.h netdb.h netinet/in.h nlist.h sgtty.h stddef.h sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/timeb.h sys/utsname.h syslog.h termio.h termios.h wchar.h wctype.h time.h])

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
if test "$TERMCAP_LIB" = no; then
  AC_CHECK_LIB(curses,main,[TERMCAP_LIB=-lcurses])
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

AC_ARG_WITH(scicos,
    AC_HELP_STRING([--without-scicos],[Compile without Scicos]))

AC_ARG_WITH(ocaml,
    AC_HELP_STRING([--without-ocaml],[Do not compile ocaml modules (modelica)]))


SCICOS_ENABLE=no
if test "$with_scicos" != no -a "$with_gui" != no; then
   AC_DEFINE([WITH_SCICOS], [], [With SCICOS])

   save_LIBS="$LIBS"

   AC_CHECK_LIB([rt], [clock_gettime],
		      [RT_LIB="-lrt";
		      AC_DEFINE([HAVE_CLOCK_GETTIME], [1],[Whether clock_gettime is available]) ],
        [AC_MSG_WARN([librt: library missing (Cannot find symbol clock_gettime). Check if librt is installed (it is usually provided by the libc) and if the version is correct])]
   )
   LIBS="$save_LIBS"
   AC_SUBST(RT_LIB)

#################
## ocaml which only called with using Scicos
#################

		if test "$with_ocaml" != no; then
		   AC_CHECK_PROG_OCAML()
		fi
		SCICOS_ENABLE=yes
fi
AC_SUBST(SCICOS_ENABLE)

AM_CONDITIONAL(OCAML, test "$with_ocaml" != no)		
AM_CONDITIONAL(SCICOS, test "$with_scicos" != no -a "$with_gui" != no)


#################
## FFTW
#################

AC_ARG_WITH(fftw,
    AC_HELP_STRING([--without-fftw],[Disable the interface to the FFTW 3 library]))

FFTW_ENABLE=no
if test "$with_fftw" != no; then
   AC_FFTW()
   FFTW_ENABLE=yes
fi

AC_SUBST(FFTW_ENABLE)
AM_CONDITIONAL(FFTW, test "$with_fftw" != no)

#################
## MPI
#################

# Not in Scilab 5.0
#AC_ARG_WITH(mpi,
#    [  --with-mpi	           compile with MPI library])
#
#if test "$with_mpi" = yes; then
#   AC_OPENMPI()
#   # We will have to detect other implementation of OpenMPI
#fi
#AM_CONDITIONAL(MPI, test "$with_mpi" = yes)

#################
## PVM
#################

AC_ARG_WITH(pvm,
    AC_HELP_STRING([--without-pvm],[Disable the interface to the PVM library]))

PVM_ENABLE=no
if test "$with_pvm" != no; then
   AC_PVM()	
   PVM_ENABLE=yes
fi
AC_SUBST(PVM_ENABLE)

AM_CONDITIONAL(PVM, test "$with_pvm" != no)

#######################
## Test for libxml
#######################

AC_LIBXML2()

#######################
## Test for gettext
#######################

ALL_LINGUAS="en_US fr_FR zh_CN zh_TW ru_RU"

AC_ARG_ENABLE(build-localization,
		AC_HELP_STRING([--disable-build-localization],[Disable the localization build])
		)
BUILD_LOCALIZATION_ENABLE=no

if test "$enable_build_localization" != no; then

   AC_SUBST(ALL_LINGUAS)
   AC_CHECK_FUNCS([bind_textdomain_codeset])

   AC_PATH_PROG(MSGCAT, msgcat, no)
   AC_PATH_PROG(MSGFMT, msgfmt, no)
   AC_PATH_PROG(XGETTEXT, xgettext, no)

   if test x$MSGFMT = xno; then
	  AC_ERROR([As you used to the option --with-build-localization, the msgfmt command is required to build Scilab.  If it is installed on your system, ensure that it is in your path.  If it is not, install GNU gettext to continue.])
	  fi
	if test x$MSGCAT = xno; then
	  AC_ERROR([As you used to the option --with-localization-generation, the msgcat command is required to build Scilab. If it is installed on your system, ensure that it is in your path.  If it is not, install GNU gettext to continue.])
	fi
	BUILD_LOCALIZATION_ENABLE=yes
fi
AM_CONDITIONAL(GENERATE_LOCALIZATION_FILES, test "$BUILD_LOCALIZATION_ENABLE" = yes)

#######################
## Test for blas/Atlas and lapack
#######################
AC_MSG_CHECKING([if BLAS, ATLAS or MKL is available])
echo ""
EMBEDDED_BLAS=0
ACX_BLAS(
		[AC_MSG_RESULT([$BLAS_TYPE found])]
		,
		AC_MSG_WARN([Impossible to find a blas compatible library (see BLAS or ATLAS). Use the version of generic blas embedded])
		EMBEDDED_BLAS=1
		)
AM_CONDITIONAL(USE_EMBEDDED_BLAS, test $EMBEDDED_BLAS = 1)


AC_MSG_CHECKING([if LAPACK is available])
echo ""
EMBEDDED_LAPACK=0
ACX_LAPACK(
		[AC_MSG_RESULT([$LAPACK_TYPE found])],
		AC_MSG_WARN([Impossible to find the LAPACK library. Use the version of lapack embedded])
		EMBEDDED_LAPACK=1
		)
AM_CONDITIONAL(USE_EMBEDDED_LAPACK, test $EMBEDDED_LAPACK = 1)



#################
## UMFPACK
#################

AC_ARG_WITH(umfpack,
    AC_HELP_STRING([--without-umfpack],[Disable the interface to the UMFPACK library]))

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
		AC_HELP_STRING([--with-tcl-library=DIR],[Set the path to the TCL library]),
		[  USER_TCL_LIB_PATH=$withval
	])
	AC_ARG_WITH(tcl-include,
		AC_HELP_STRING([--with-tcl-include=DIR],[Set the path to the TCL headers]),
		[  USER_TCL_INC_PATH=$withval
	])

	USER_TK_LIB_PATH=$USER_TCL_LIB_PATH
	USER_TK_INC_PATH=$USER_TCL_INC_PATH

	AC_ARG_WITH(tk-library,
		AC_HELP_STRING([--with-tk-library=DIR],[Set the path to the TK library]),
		[  USER_TK_LIB_PATH=$withval
	])

	AC_ARG_WITH(tk-include,
		AC_HELP_STRING([--with-tk-include=DIR],[Set the path to the TK headers]),
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
## MATIO LIBRARY (MAT File I/O Library)
#################


AC_ARG_WITH(matio,
    AC_HELP_STRING([--without-matio],[Compile with the MAT File I/O library]))

AC_ARG_WITH(matio_include,
		AC_HELP_STRING([--with-matio-include=DIR],[Set the path to the MATIO headers]),
		[with_matio_include="-I$withval"], [])

AC_ARG_WITH(matio_library,
		AC_HELP_STRING([--with-matio-library=DIR],[Set the path to the MATIO libraries]),
		[with_matio_library="-L$withval"], [])

MATIO_ENABLE=no

if test "$with_matio" != no; then
   if test -n "$with_matio_include" -o -n "$with_matio_library" ; then
      MATIO_CFLAGS="$with_matio_include"
      MATIO_LIBS="$with_matio_library -lm -lz -lmatio"
   else
      PKG_CHECK_MODULES(MATIO, [matio >= 1.3.3])
   fi
   
   save_CFLAGS="$CFLAGS"
   save_LIBS="$LIBS"

   CFLAGS="$CFLAGS $MATIO_CFLAGS"
   LIBS="$LIBS $MATIO_LIBS"
   
   AC_CHECK_HEADERS([matio.h matioConfig.h], [],
     [AC_MSG_ERROR([Invalid MATIO_CFLAGS returned by pkg-config. Try to define MATIO_CFLAGS.])])
   AC_CHECK_LIB([matio], [Mat_Open], [],
     [AC_MSG_ERROR([Invalid MATIO_LIBS returned by pkg-config. Try to define MATIO_LIBS.])])
   
   LIBS="$save_LIBS"
   CFLAGS="$save_CFLAGS"
   
   AC_DEFINE([WITH_MATIO], [], [With the MATIO library])
   MATIO_ENABLE=yes
fi


AC_SUBST(MATIO_ENABLE)
AM_CONDITIONAL(MATIO, test "$with_matio" != no)

#############################
## Documentation management #
#############################

AC_ARG_ENABLE(build-help,
		AC_HELP_STRING([--disable-build-help],[Disable the help build])
		)
HELP_ENABLE=no

if test "$enable_build_help" != no; then
    AC_DOCBOOK()
	HELP_ENABLE=yes

fi 
AM_CONDITIONAL(BUILD_HELP, test $HELP_ENABLE = yes)

## Install XML help files ###
AC_ARG_WITH(install-help-xml,
		AC_HELP_STRING([--with-install-help-xml],[make install will install XML files])
		)
HELP_INSTALL_ENABLE=no
if test "$with_install_help_xml" != no -a "$with_install_help_xml" != ""; then
   HELP_INSTALL_ENABLE=yes
fi 
AM_CONDITIONAL(INSTALL_HELP_XML, test "$HELP_INSTALL_ENABLE" != "")


#########################
## libtool
#########################

AM_DISABLE_STATIC
AC_PROG_LIBTOOL([shared dlopen])
AM_PROG_LIBTOOL


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

#endif /* MACHINE_H  */
])

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
contrib/Makefile
modules/helptools/Makefile
modules/helptools/macros/xmlfiletohtml.sci
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
modules/graphic_export/Makefile
modules/renderer/Makefile
modules/action_binding/Makefile
modules/gui/Makefile
modules/pvm/Makefile
modules/randlib/Makefile
modules/tclsci/Makefile
modules/windows_tools/Makefile
modules/core/Makefile
modules/fftw/Makefile
modules/umfpack/Makefile
modules/scicos/Makefile
modules/scicos_blocks/Makefile
modules/functions/Makefile
modules/dynamic_link/Makefile
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
modules/demo_tools/Makefile
modules/genetic_algorithms/Makefile
modules/simulated_annealing/Makefile
modules/parameters/Makefile
modules/matio/Makefile
modules/Makefile
libs/Makefile
Makefile
scilab.pc
etc/modules.xml
etc/classpath.xml
modules/jvm/etc/jvm_options.xml
scilab-lib.properties
scilab-lib-doc.properties
])

# Detection of the module for the future version of Scilab 6
# ie we detect module which ends with _yasp

if test "$enable_yasp" = yes; then
   AC_CONFIG_FILES([
   modules/development_tools/src/fake/Makefile
  ])
fi

# This script is used by Scicos in order to regenerate the function/block list
AC_CONFIG_COMMANDS_POST([chmod +x $SCI_SRCDIR_FULL/modules/scicos_blocks/src/scripts/GenBlocks.sh $SCI_SRCDIR_FULL/modules/dynamic_link/src/scripts/scicompile.sh $SCI_SRCDIR_FULL/modules/dynamic_link/src/scripts/compilerDetection.sh $SCI_SRCDIR_FULL/modules/dynamic_link/src/scripts/configure])


#MAN_FR MAN_ENG
#modules/mpi/Makefile
#for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
#  test -f $top_builddir/configure && break
#done


#if test "$with_mpi" = yes; then
#AC_CONFIG_FILES([modules/mpi/Makefile])
#fi

# Generate stack.h
if test "$IS_64_BITS_CPU" = true -o "$MACOSX" = "1"; then
   stack_h_cpp_flags=-DUSE_DYNAMIC_STACK
else
   stack_h_cpp_flags=
fi

AC_CONFIG_COMMANDS([modules/core/includes/stack.h],
  [if ! $CPP $stack_h_cpp_flags - < $srcdir/modules/core/includes/stack.h.in |\
              $GREP -v '^#' > $srcdir/modules/core/includes/stack.h ; then
     AC_MSG_ERROR([stack.h generation failed])
   fi],
  [CPP="$CPP"
   GREP="$GREP"
   stack_h_cpp_flags="$stack_h_cpp_flags"
  ])

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
echo "Intel (--with-intelcompilers) .... =		$with_intelcompilers"
echo "GNU gcc (--with-gcc) ............. =		$with_gcc"
echo "GNU Fortran 77 (--with-g77) ...... =		$with_g77"
echo "GNU Fortran 95 (--with-gfortran) . =		$with_gfortran"
echo ""
echo "Options:"
echo "Use PVM (--with-pvm) ............................. = $with_pvm"
echo "PVM include (--with-pvm-include) ................. = $PVMINCLUDE"
echo "PVM library (--with-pvm-library) ................. = $PVMLIBDIR"
echo "Do not use TCL/TK (--without-tk) ................. = $with_tk"
echo "Do not compile with Ocaml (--without-ocaml) ...... = $with_ocaml"
echo "TCL include (--with-tcl-include) ................. = $USER_TCL_INC_PATH"
echo "TCL library (--with-tcl-library) ................. = $USER_TCL_LIB_PATH"
echo "TK include (--with-tk-include) ................... = $USER_TK_INC_PATH"
echo "TK library (--with-tk-library) ................... = $USER_TK_LIB_PATH"
echo "Install XML Help (--with-install-help-xml) ....... = $with_install_help_xml"
echo "Make the package relocatable (--enable-relocatable)= $RELOCATABLE"
echo "Use FFTW (--with-fftw) ........................... = $with_fftw"
echo "Don't use MATIO (--without-matio) ................ = $without_matio"

echo ""
if test "$with_gui" = no; then
   echo "Not using Scicos because of the option --without-gui"
else
if test $SCICOS_ENABLE = yes; then
   echo "Scicos enable"
   echo ""
   echo "Ocaml Configuration (for Modelica compiler):"
   echo "OCAMLC ............. = $OCAMLC"
   echo "OCAMLOPT ........... = $OCAMLOPT"
   echo "OCAMLDEP ........... = $OCAMLDEP"
else 
   echo "Not using Scicos"
fi
fi

echo ""
if test $PVM_ENABLE = yes; then
   echo "PVM Configuration:"
   echo "PVM Architecture ... = $PVMARCH"
   echo "PVM INCLUDE ........ = $PVM_INCLUDE"
   echo "PVM LIBS ........... = $PVM_LIB"
else
   echo "Not using PVM"
fi

echo ""
if test $FFTW_ENABLE = yes;  then
   echo "FFTW Configuration:"
   echo "FFTW LIBS .......... = $FFTW3_LIB"
   echo "FFTW CFLAGS ........ = $FFTW3_CFLAGS"
else
   echo "Not using FFTW"
fi

echo ""
if test $MATIO_ENABLE = yes;  then
   echo "MATIO Configuration:"
   echo "MATIO LIBS .......... = $MATIO_LIBS"
   echo "MATIO CFLAGS ........ = $MATIO_CFLAGS"
else
   echo "Not using MATIO"
fi

echo ""
if test $UMFPACK_ENABLE = yes;  then
   echo "UMFPACK Configuration:"
   echo "UMFPACK LIBS ....... = $UMFPACK_LIB"
   echo "UMFPACK CFLAGS ..... = $UMFPACK_CFLAGS"
   if test $SUITESPARSE = yes; then
      echo "UMFPACK SUITESPARSE  = Yes"
   else
      echo "UMFPACK SUITESPARSE  = No"
   fi
else
   echo "Not using UMFPACK"
fi


echo ""
echo "BLAS/LAPACK/ATLAS Configuration:"
if test $EMBEDDED_BLAS = 0; then
   echo "BLAS LIBS ............. = $BLAS_LIBS"
   echo "BLAS TYPE ............. = $BLAS_TYPE"
else
   echo "BLAS LIBS ............. = Use the generic version of BLAS embedded"
fi

if test $EMBEDDED_LAPACK = 0; then
   echo "LAPACK LIBS ........... = $LAPACK_LIBS"
   echo "LAPACK TYPE ........... = $LAPACK_TYPE"
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

#if test "$with_openmpi" != "no"; then
#   echo "OpenMPI Configuration:"
#   echo "OpenMPI LIBS ........... = $OPENMPI_LIBS"
#   echo "OpenMPI C Compiler ..... = $OPENMPI_CC"
#   echo "OpenMPI F77 Compiler ... = $MPIF77"
#else
#   echo "Not using MPI"
#fi

echo ""
if test $BUILD_LOCALIZATION_ENABLE != no; then
   echo "Gettext/localization configuration:"
   echo "xgettext ............... = $XGETTEXT"
   echo "msgfmt  ................ = $MSGFMT"
   echo "msgfmt_opts ............ = $MSGFMT_OPTS"
   echo "msgcat  ................ = $MSGCAT"
else
   echo "Won't generate localization files"
fi

echo ""
if test $HELP_ENABLE = yes; then
   echo "Docbook XSL path ....... = $DOCBOOK_ROOT"
   echo "Batik (SVG) ............ = $BATIK"
   echo "Saxon XSLT ............. = $SAXON"
   echo "FOP (XML => PDF) ....... = $FOP"
   echo "Jeuclid (MathML) ....... = $JEUCLID_CORE"
   echo "Commons I/O ............ = $COMMONS_IO"
   echo "XML graphics commons ... = $XMLGRAPHICS_COMMONS"
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
echo "PCRE configuration:"
echo "  PCRE_CFLAGS ........ = $PCRE_CFLAGS"
echo "  PCRE_LIBS .......... = $PCRE_LIBS"
echo "  PCRE_VERSION ....... = $PCRE_VERSION"


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
echo "  CPU 64 bits .... = $IS_64_BITS_CPU"

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
echo "  TERMCAP_LIB .... = $TERMCAP_LIB"
echo "
"
