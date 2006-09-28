##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########
dnl Process this file with autoconf to produce a configure script.

## Copyright INRIA/Enpc
## TODO :
## Installer tous les fichiers dans les bons paths
## Faire en sorte qu'il copie bien les libs au bon endroit
## * verifier les problemes de xlibs sur d'autres plateformes et voir si c'est pas liés à xorg
## * dans la compilation finale, il faut rajouter le flag -xsci, -L./libs/ pour la lib graphique ainsi que  -L/usr/X11R6/lib/ -lXaw
## 
## * enlever toutes les references a SONAME qui ne sert a rien grace a libtool
## * ne pas oublier le systeme de DESTDIR 
## * voir si on gere les options conflictuelles (with-gnu et with-intelcompilers)
## dans routines/control/Makefile.in on doit compiler certains fichiers sans les optimisations !!! 
## idem pour : routines/interf/intmgetl.f
## idem pour : routines/calelm/icopy.f et unsfdcopy.f
## Rajouter le fortran dans le texte final
## Comportement par defaut quand il n'y a pas de parametre au ./configure ???
## Fortran F77 est tres largement factorisable grace aux macros M4 qui existent
## Regarder ou est utilisé FLIBS dans les autres fichiers de conf
## Regarder si LD=ld est utile (sans doute pas vu qu'il devient caduc par l'utilisation de libtool)
## Penser a rajouter les flags $with_prof et  $with_gprof (cf imagemagick l2600 environ)
## Verifier le probleme quand on compile avec le compiler intel fortran (merde au niveau du linkage)
## Rajouter une verification que pvm3.h existe ou pas avant la compilation (pvm_grp.c(20): catastrophic error: could not open source file "pvm3.h")
## Ne pas oublier de regarder du cote de AC_LIBTOOL_DLOPEN
## Voir ce que fait helpbrower et voir comment je dois le remettre en place dans libtool
## Nettoyer (enlever a priori) les fichiers ?getarg-hpux.{f,c} (ok pour le .f a priori mais verifier pour le .c vu qu'il n'est different quand dans la derniere version de hpux)
## Probleme de compilation dans poly (duplicate machin)
## libtool sur pvm 
## que faire pour scicos ??
## Regarder pourquoi system & system2 merdent 
## Faire X11/Xmu et Xaw3D (ou voir pour les delete)
## voir si c'est possible de faire xsci/Makefile.am avec les bons repertoires pour X11 
## probleme de compilation sur xdr
## dlopen voir /home/sylvestre/dev/misc/app/compiz/src/plugin.c
## a t on besoin de xxx_la_DEPENDENCIES dans libtoo
## idem pour xxx_HEADERS
## Dans le repertoire routines/xxxx/ voir pourquoi ca merde quand on fait "make check"
## Enlever fichier config/configuration
## Ne pas oublier de preciser quelque par que j'ai recupere la version CVS du fichier tcl.m4
## Des infos ont sauté du fichier machine.h
## voir si ca serait utile d'avoir le mmx (cf configure.in de gimp ligne 565)

## xerbla existe dans lapack et arpack. Ce que l'on veut est que l'on puisse dans scilab définir notre propre xerbla.o qui existe (pour l'instant il est modifié dans la lib itself) il est dispo dans system
## Regarder si on peut factoriser les makefile pour en avoir des plus simples pour les developpeurs
## Dans system, j'ai enleve dans le makefile.am des fichiers qui ne servaient qu'a loader les autres. Cependant pas sur que ca marche ainsi !
## regarder encore de quoi depend interf
## Passer le contenu de default en lib et juste garder le main


## déplacement du fichier caleml/dcabs1.f vers blas/dcabs1.f idem pour lsame.f 	xerbla.f 

## Enlever Makefile.OBJ & Makefile.incl

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


## TOUT IMPORTER DANS LE SVN

AC_REVISION($Revision: 1.132.2.6 $)dnl
AC_INIT([scilab],[0],[http://scilabsoft.inria.fr/cgi-bin/bugzilla_bug_II/index.cgi])
AC_PREREQ(2.60)


#echo "CFLAGS $CFLAGS"
SCIDIR=$srcdir
SCIDIRFULL=`cd $SCIDIR && pwd`
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([modules/core/includes/machine.h])

# strip executable
AM_PROG_INSTALL_STRIP
#AC_PACKAGE_VERSION([4.0])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_FC
AC_PROG_CC

dnl Get system type
dnl use "config.guess" and "config.sub" from directory "config"
AC_CANONICAL_HOST

#
# Evaluate shell variable equivalents to Makefile directory variables
#
#if test "x$prefix" = xNONE
#then
#  prefix=$ac_default_prefix
#fi
# Let make expand exec_prefix.
#if test "x$exec_prefix" = xNONE
#then
#  exec_prefix='${prefix}'
#fi

#
eval "eval PREFIX_DIR=${prefix}"
AC_SUBST(PREFIX_DIR)
eval "eval EXEC_PREFIX_DIR=${exec_prefix}"
AC_SUBST(EXEC_PREFIX_DIR)

# default prefix
ac_default_prefix=/usr


#################################
## all the --with-* argument help
#################################

AC_ARG_ENABLE(debug,
   [  --enable-debug          compile with "-g" option and do not optimize ])

AC_ARG_WITH(gcc,
    [  --with-gcc              use gcc C compiler ])
    
AC_ARG_WITH(g77,
    [  --with-g77              use g77, GNU Fortran 77 compiler ])

AC_ARG_WITH(gfortran,
    [  --with-gfortran         use gfortran, GNU Fortran 95 compiler ])

AC_ARG_WITH(gnu,
    [  --with-gnu              use GNU C (gcc) compiler and GNU Fortran compilers (g77 or gfortran) ])

AC_ARG_WITH(intelcompilers,
    [  --with-intelcompilers   use Intel C (icc) and Fortran (ifort) compilers (GNU/Linux only) ])

AC_ARG_WITH(local-xaw,
    [  --with-local-xaw        use Xaw3d widgets given with Scilab ])

AC_ARG_WITH(tk,
    [  --without-tk            do not compile with TCL/TK ])

AC_ARG_WITH(ocam,
    [  --without-ocaml         do not compile with ocaml ])

#################################
## check this is a source version
#################################

if test -f .binary; then
  echo "Humm... this is a binary version";
  echo "  you'd better read the README file first."
  exit
fi

###############################
## creating Path.incl if needed
###############################
# 
# change scidir for mingw 
#   *-*-cygwin* |   *-*-mingw32*)

case "$host" in
   *-*-mingw32*)
    SCIDIR=`echo $SCIDIR | sed -e 's+/cygdrive/\([[a-zA-Z]]*\)/+\1:/+g' `
    ;;
esac

###############################
## get the version 
###############################

SCIVERSION=`cat $SCIDIR/Version.incl | sed -e "s/SCIVERSION=//" `

########################
## removing and cleaning
########################

RM='rm -f'
FILES_TO_CLEAN='*.CKP *.ln *.BAK *.bak core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut .*~ *.trace so_locations *.orig *.cos *.obj' 




#####################################################
## Check if options are correct (or not)
#####################################################

if test "$with_g77" = yes -a "$with_gfortran" = yes; then
	AC_MSG_ERROR([Conflicting options : you specified two fortran compiler])
fi

if test "$with_intelcompilers" = yes -a "$with_gnu" = yes; then
	AC_MSG_ERROR([Conflicting options : you specified two compiler series])
fi


#####################################################
## if specific compilers are required, check if they exist
#####################################################

if test "$with_gnu" = yes; then
	# GNU C and Fortran compilers required
   AC_CHECK_PROG_GNU()
fi
if test "$with_intelcompilers" = yes; then
	# intel C and Fortran compilers required
	AC_CHECK_PROG_INTEL()
fi
if test "$with_gcc" = yes; then
	# GNU gcc required
	AC_CHECK_PROG_GCC()
fi
if test "$with_g77" = yes; then
	# GNU g77 required
	AC_CHECK_PROG_G77()
fi
if test "$with_gfortran" = yes; then
	# GNU g95 (gfortran) required
	AC_CHECK_PROG_GFORTRAN()
fi

#############################################
## Find the X11 include and library directories.
#############################################


AC_PATH_XTRA

if test "$no_x" != 'yes'
then
#  LDFLAGS="$LDFLAGS $X_LIBS"
  X11_LIBS="$X_PRE_LIBS $X_EXTRA_LIBS"
#  CFLAGS="$CFLAGS $X_CFLAGS"
else
  AC_MSG_ERROR([Scilab requires the X Window System header files and libraries !])
  exit 1
fi
#AC_SUBST(X11_LIBS)


#############################################
## Compilers and options according to machine
#############################################


# use for making scripts in script directory
#   different for MIPS Ultrix
SCILAB_GS=sh
# scilab script file in script directory: default is "scilab.g" for UNIX
#   for gcwin32 use "scilab-gcwin32.g"
SCILABSCRIPT=scilab.g

# For old Hp computer (9.x), used in "routines/os_specific/Makefile.in"
#   for choosing the good file between link_std.c and link_SYSV.c
HPPA_OLD=

GUI='X11'

# specific scilab dirs for graphics: default is "xsci" for X11
#   different for Win32
GUILIB=xsci 

# specific scilab dirs for xdr:  default is void for X11
#   xdr for Win32
XDRLIB=
XDRLIBNAME=
# objects files in routines/menusX: default is XOBJS for UNIX
#   for Win32 use WOBJS
MENUSOBJS=XOBJS
# use other malloc (in "routines/os_specific" directory) for some 64 bits machines
MALLOC=


# on windows target generating scripts for 
# printing is useless 
BLPR_SCRIPTS='$(BPROGRAMS)'
# on windows target when generating script 
# cygdrive/xxx must be replaced by xxx:/ 
# for mingw32 
SED_CYGDRIVE=
# subset of examples which are supposed to work 
EXAMPLES_TESTS='$(SUBDIRS1) $(SUBDIRS2)'
# special stuff for cygwin in impdir 
IMPPREF=S
IMPENV= 
# periX11.c must be compiled without optimization on Darwin
PERIX11NOPT=periX11darwin
NMOPT=

AC_CHECK_LIB(ieee,main,[LIEEELIB=-lieee])
######################
######## Set compilation options for intel C/Fortran compilers
######################

if test "$with_intelcompilers" = yes; then
		
		CFLAGS=' -Dlinux -DNARROWPROTO -mp'
		# -cm remove comments displays, -w90  -w95 remove warnings abort f90 f95 non standard

		FFLAGS=' -cm  -w90 -w95 -mp'


		case "$host" in
		  i*86-*-linux-gnu | *-pc-linux-gnu )
			# -pc64 : double precision 
			EXTRA_OPTIONS=" -pc64 -ffnalias -falias"
			;;
		esac

		if test "$enable_debug" = yes; then
			CFLAGS="-g $CFLAGS $EXTRA_OPTIONS"
			FFLAGS="-g $FFLAGS $EXTRA_OPTIONS"
		else
			CFLAGS="-O -DNDEBUG $CFLAGS "
			FFLAGS="-O $FFLAGS "
			FFLAGS_O0="$FFLAGS "
		fi
		# TODO : voir si obligatoire
fi

########### FORTRAN ######################


######################
######## With F77 / G77 / GFortran ...
######################

if test -n "$FC"; then
# TODOO : il faut peut etre verifier que c'est bien gfortran
   if test "$enable_debug" != yes; then 
	  FFLAGS="$FFLAGS -DNDEBUG -D_FORTIFY_SOURCE=2"
   else
	  FFLAGS="$FFLAGS -g -Wall "
   fi
	  case "$host" in
		 x86_64-*-linux-gnu)
			FFLAGS='$FFLAGS -march=athlon64 -mfpmath=sse -msse2 -m3dnow -m64 -fPIC'
					;;
		# Dec Alpha OSF 4
		alpha*-dec-osf4.*)
	      FFLAGS='$FFLAGS -fpe3'
		;;
		alpha*-dec-osf*)
		    FFLAGS='$FFLAGS -fpe3 -switch nosqrt_recip'
		;;
		rs6000-*-*)
		    FFLAGS='$FFLAGS -qcharlen=4096'
			;;
		mips-*-ultrix*)
		    FFLAGS='-O0 -fpe1'
		;;
		  *-*-hpux9.*)
			FFLAGS='$FFLAGS +Obb1200 +E4 -Dhpux'
		;;
		  *-*-hpux10.*)
		    if test "$enable_debug" != yes; then 
	   		    FFLAGS='+O2 +E4 +Z +DAportable -Dhpux'
			else
			    FFLAGS='+E4 +Z +DAportable -Dhpux'
			fi
#		    FC_LDFLAGS='-Wl,+vnocompatwarnings,-E /usr/lib/libdld.sl'

			;;
		  *-*-hpux11.*)
		   if test "$enable_debug" != yes; then 
		      FFLAGS='+O2 +Z +DAportable -Dhpux'
			 else
			  FFLAGS=' +Z +DAportable -Dhpux'
		  fi
			  FLIBS='-ldld -lnsl -lU77 -lm'
    #do not call AC_F77_LIBRARY_LDFLAGS because if assumes PA2.0 while Scilab is built with 
    #DAportable=PA1.1
    #AC_F77_LIBRARY_LDFLAGS
	# TODO : check why and if it is still the case

#    FC_LDFLAGS='-Wl,+vnocompatwarnings,-E /usr/lib/libdld.sl'
	 ;;
	  esac
#	   FFLAGS_O0=`echo $FFLAGS | sed 's/-O[0-9]*//g'`
else
	AC_MSG_ERROR([No fortran compiler found])
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


##########" 

GCCFIX=

if test "$CC" = "gcc"; then
   ## With GNU Compiler
   if test "$enable_debug" != yes; then 
	  CFLAGS="$CFLAGS -DNDEBUG -D_FORTIFY_SOURCE=2" # used to be -O
   else
	  CFLAGS="$CFLAGS -g -Wall -ANSI "
   fi
   echo $CFLAGS
   case "$host" in
   # Linux sparc | Linux, old binary format a.out (NO LONGER SUPPORTED)
	 sparc-*-linux-gnu|*-pc-linux-gnuaout)
	    CFLAGS='$CFLAGS -Dlinux -fwritable-strings'
		LDFLAGS="$LDFLAGS $LIEEELIB"
	;;
	 x86_64-*-linux-gnu)
		CFLAGS="$CFLAGS -Dlinux -DNARROWPROTO -march=athlon64 -mfpmath=sse -msse2 -m3dnow -m64"
    ;;
	 i*86-*-linux-gnu | *-pc-linux-gnu | ia64-*-linux-gnu )
		CFLAGS="$CFLAGS -Dlinux -DNARROWPROTO"
    ;;
	 alpha-*-linux-gnu)
		CFLAGS="$CFLAGS  -mieee-with-inexact -Dlinux -fwritable-strings"
		LDFLAGS="$LDFLAGS -mieee-with-inexact"
	;;
	 powerpc-*-linux-gnu)
		CFLAGS="$CFLAGS -D_GNU_SOURCE -Dlinux -fwritable-strings"
		LDFLAGS="$LDFLAGS $LIEEELIB"
	;;
	 *-*-solaris*)
	    if test "$enable_debug" != yes; then 
			 CFLAGS='-O2 -DNDEBUG -DSVR4 -DSYSV -Dsolaris'
		else
			 CFLAGS='$CFLAGS -DSVR4 -DSYSV -Dsolaris'
		fi
	;;
   *-*-freebsd*)
     CFLAGS="$CFLAGS -Dfreebsd"
	 LDFLAGS="$LDFLAGS -m"
	 ;;
  *-*-netbsd*)
    CFLAGS="$CFLAGS -Dnetbsd"

	;;
  *-*-cygwin* )
     if test "$enable_debug" != yes; then 
	 	 CFLAGS='-O2 -DNDEBUG -mieee-fp '
	 else
	 	 CFLAGS='$CFLAGS -mieee-fp '
	 fi
	;;
   *-*-mingw32*)
    CFLAGS='$CFLAGS -mno-cygwin -mieee-fp -fwritable-strings '
	;;

  *-*-darwin*)
     if test "$enable_debug" != yes; then 
	      CFLAGS=' -O2 -DNDEBUG -no-cpp-precomp -I/sw/include -L/sw/lib'
	 else
	      CFLAGS=' $CFLAGS -I/sw/include -L/sw/lib'
	 fi
     LDFLAGS='$LDFLAGS -L/sw/lib'
	;;
esac

else

		## CC compiler (not GCC)
		if test "$enable_debug" != yes; then 
		     CFLAGS="$CFLAGS -DNDEBUG " # used to be -O
		else
		     CFLAGS="$CFLAGS -g"
		fi

case "$host" in
# Dec Alpha OSF 4
  alpha*-dec-osf4.*)
      CFLAGS='$CFLAGS -std -ieee_with_inexact' 
      LD_LDFLAGS='-ieee_with_inexact'
	  ;;
  alpha*-dec-osf*)
      CFLAGS='$CFLAGS -ieee_with_inexact' 
      LD_LDFLAGS='-ieee_with_inexact'
      ;;
  rs6000-*-*) # IBM AIX RS 6000 (NO LONGER SUPPORTED)
    CFLAGS='-Daix -DSYSV'
  ;;
  mips-sgi-irix*) # SGI
	CFLAGS="$CFLAGS -DSYSV -DSVR4"
  ;;
  *-*-hpux9.*)
	CFLAGS="$CFLAGS -DSYSV -Dhpux"
  ;;
  *-*-hpux10.*) # HP 10
	if test "$enable_debug" = yes; then 
	  CFLAGS="$CFLAGS -DSYSV -Dhpux"
    else
	  CFLAGS="$CFLAGS -DSYSV -Dhpux +Z +DAportable"
    fi
  ;;

  *-*-hpux11.*) # HP 11
    CFLAGS="$CFLAGS -DSYSV -Dhpux -Dhppa +Z +DAportable"
  ;;
  *-*-solaris*) # SUN SOLARIS
    CFLAGS='$CFLAGS -DSVR4 -DSYSV -Dsolaris'
  ;;
esac
fi


### TODO : a confirmer que c'st ok
## On passe le flag directement pour ces fichiers
#FFLAGS1=$FFLAGS
#if test "$WITH_G77" = yes; then
#  if test "$enable_debug" = yes; then   
#    FFLAGS1=$FFLAGS
#  else
#    FFLAGS1=$FFLAGS_O0
#  fi
#fi

### 
#if test "$WITH_GFORTRAN" = yes; then
#  if test "$enable_debug" = yes; then   
#    FFLAGS1=$FFLAGS
#  else
#    FFLAGS1=$FFLAGS_O0
#  fi
#fi




# flag for possible compilations in configure
CFLAGS="$CFLAGS -I${SCIDIRFULL}/libs/MALLOC/includes/"
FFLAGS="$FFLAGS -I${SCIDIRFULL}/modules/core/includes"

###################
## find X11 release
###################

if test "x$GUI" = "xX11"; then 
   AC_CACHE_CHECK([for X11 release],ac_cv_X11_release,
   [
   cat << EOF > testX11.c
#include <X11/Xlib.h>
main()
{
#ifdef XlibSpecificationRelease
if (XlibSpecificationRelease == 5)
printf("R5\n");
else if (XlibSpecificationRelease == 6)
printf("R6\n");
else
printf("R?\n");
#else
printf("R4\n");
#endif
}
EOF
	eval "$CC $X_CFLAGS -o testX11 testX11.c > /dev/null 2>&1"
	ac_cv_X11_release=`./testX11`
	$RM testX11.c testX11
	])
	if test "$ac_cv_X11_release" = R4; then
	   AC_MSG_ERROR([Scilab cannot be installed with X11R4.])
	fi
fi

##########################################
## test for X11 and Xaw3d  Athena widgets
##########################################

if test "x$GUI" = "xX11"; then 
  ac_save_LDFLAGS="$LDFLAGS"
  if test -n "$x_libraries"; then
    LDFLAGS="$LDFLAGS -L$x_libraries"
  fi

##test Xaw3d (can be build locally)
  if test "$with_local_xaw" = yes; then
    XAWLIB='$(SCIDIR)/libs/libXaw3d.a'
    XMULIB='$(SCIDIR)/libs/libXmu.a'
    XAW_LOCAL_LIB='$(SCIDIR)/libs/libXaw3d.a $(SCIDIR)/libs/libXmu.a'
    XAW_LOCAL_INCLUDES='-I$(SCIDIR)/routines'
    XAW_LOCAL_SUBDIR=X11
    XAW=Xaw3d
  else
##test Xmu (Required)
    AC_CHECK_LIB(Xmu,main,[XMu=yes],[XMu=no],-lXt -lXext $X_PRE_LIBS -lX11 $X_EXTRA_LIBS)
    if test "$XMu" = no; then
		AC_MSG_ERROR([Cannot find the library Xmu])	
    else
      XMULIB=-lXmu
      AC_CHECK_LIB(Xaw3d,main,[XAthena3d=yes],[XAthena3d=no],-lXmu -lXt -lXext $X_PRE_LIBS -lX11 $X_EXTRA_LIBS)
      if test "$XAthena3d" = no; then
		AC_MSG_ERROR([Cannot find the library Xaw3d (Xathena3d)])	
      else
		X_LIBS="$X_LIBS -lXaw3d -lXmu"
        XAW=Xaw3d
		X_LIB_PATH=/usr/X11R6/lib/
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <X11/IntrinsicP.h> #include <X11/Xaw3d/ViewportP.h>]], [[]])],[],[AC_MSG_ERROR(Include file "ViewportP.h" for Xaw3d not found. 
			       Get and put it in <X11/Xaw3d/> .)])
	CFLAGS=$save_cflags

      fi
    fi
  fi 
fi

#######################
## Xt 
#######################

# Pb of automatic insertion of -lXt -lXext -lX11 in Makefile.incl.in
# which does not work with mingwin. Thus I add this two vars to 
# configure.in 

#XXTXEXT="-lXt -lXext"
#XX11="-lX11"

#  case "$host" in
#      *-*-mingw32*)
#      XXTXEXT=
#      XX11=
#    ;;
#  esac
  
###########################
## test for JAVA compiler
###########################

if test "$with_java" != no; then
   # See if --with-jdk command line argument is given
     # Try to detect the installed JVM, this could be controlled
	 # by the above --with options
	 AC_JAVA_WITH_JDK
	 AC_JAVA_DETECT_JVM
   if test "$ac_java_jvm_name" = "jdk"; then
	    JAVA_VERSION=$ac_java_jvm_version
#     AC_JAVA_TOOLS
		AC_JAVA_JNI_INCLUDE
		JAVA_JNI_INCLUDE=$ac_java_jvm_jni_include_flags

		AC_JAVA_JNI_LIBS		
		JAVA_JNI_LIBS=$ac_java_jvm_jni_lib_flags
		JAVA_JNI_LIBS_PRELOAD=$ac_java_jvm_ld_preload
		JAVA_OK=1
	else
		AC_MSG_WARN([Sun javac not found: I will not build the java interface])
		if test "$ac_java_jvm_name" != ""; then
			AC_MSG_WARN([We do not support $ac_java_jvm_name yet])
		fi
	fi
fi
AM_CONDITIONAL(JAVA, test "$JAVA_OK" = "1")

##############################################################
## test for functions in standard C library and C math library
##############################################################

# Provided by unistd.h
AC_CHECK_FUNCS([sleep] [usleep] [dup2] [getcwd] [getpagesize] [getpass])

AC_CHECK_FUNC([getwd],AC_DEFINE([HAVE_GETWD],[1],[Define to 1 if you have the `getwd' function. ]),[AC_DEFINE([getwd(x)],[getcwd(x,1024)],[Don't use getwd but getcwd])])

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

########################
## test for header files
########################

AC_CHECK_HEADERS([limits.h values.h]) 

# Added by Sylvestre (16 june 2006)

AC_CHECK_HEADERS([fcntl.h float.h locale.h malloc.h netdb.h netinet/in.h nlist.h sgtty.h stddef.h sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/timeb.h syslog.h termio.h termios.h wchar.h wctype.h libintl.h])


#######################
## Test for structures ##
#######################
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_CHECK_MEMBERS([struct stat.st_rdev])

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
 AC_MSG_WARN([No termcap library detected])
 TERMCAPLIB=
fi

#################
## PVM
#################

AC_ARG_WITH(pvm,
    [  --with-pvm	           compile with PVM library ])

if test "$with_pvm" = yes; then
   AC_PVM()
fi
AM_CONDITIONAL(PVM, test "$PVM_OK" = "1")

#################
## Tcl/Tk library
#################

TK_INC_PATH=
TK_LIB_PATH=
TCLTK_LIBS=
TCLSCILIB=
TCLSCIDIR=
TCLTKHELP=

if test "$with_tk" != no; then

	TCLTKHELP=tksci

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
	
	saved_cflags="$CFLAGS"
	saved_ldflags="$LDFLAGS"
	saved_cppflags="$CPPFLAGS"
	
	AC_CHECK_LIB(dl, main,TCLTK_LIBS=" -ldl")
	AC_CHECK_TCLTK()
	
	# set variables
	if test $WITH_TKSCI = 1; then
		AC_DEFINE([WITH_TK], [], [With TK])
		TCLSCILIB="\$(SCIDIR)/libs/tclsci.a"
		TCLSCIDIR=tclsci
		tcldir="$PREFIX_DIR/tcl/"
	else
		AC_MSG_ERROR([TCL/TK not found. Use --without-tk or specify the librairies and include paths manually])
	fi
	
	TKBASE=
	case "$host" in
	*-*-mingw32*)
		TKBASE=$SCIDIR
		TK_INC_PATH=`echo $TK_INC_PATH | sed -e "s+$SCIDIR+\\$(TKBASE)+g" `
		TCL_INC_PATH=`echo $TCL_INC_PATH | sed -e "s+$SCIDIR+\\$(TKBASE)+g" `
		TCLTK_LIBS=`echo $TCLTK_LIBS | sed -e "s+$SCIDIR+\\$(TKBASE)+g" `
	;;
	esac
	## voir quoi faire maintenant avec le SONAME qui se barre
	GETLIBTK=libtk$TK_MAJOR_VERSION.$TK_MINOR_VERSION.so
	
	case "$host" in
		*-*-darwin*)
		GETLIBTK=/sw/lib/libtk$TK_MAJOR_VERSION.$TK_MINOR_VERSION.dylib
		;;
		*-*-cygwin* |   *-*-mingw32*)
		GETLIBTK=tk$TK_MAJOR_VERSION$TK_MINOR_VERSION.dll
	esac 

fi


#################
## atlas 
#################

# check user arguments
USER_ATLAS_LIB_PATH=""

AC_ARG_WITH(atlas-library,
    [  --with-atlas-library=DIR  Atlas library files are in DIR and we use Atlas ],
    [  USER_ATLAS_LIB_PATH=$withval
    CC_OPTIONS="$CC_OPTIONS -DWITH_ATLAS"],
    )

if test "$USER_ATLAS_LIB_PATH" = ""; then
	WHICH_BLAS="STD_BLAS"
	IN_ATLAS=LOCAL
else 
	echo "Using Atlas library"
	WHICH_BLAS="ATLAS_LAPACKBLAS"
	ATLAS_DIR=$USER_ATLAS_LIB_PATH
	IN_ATLAS=ATLAS
	AC_DEFINE([WITH_ATLAS],[],[With the Atlas Lib])
fi


#################
## ocaml 
#################

if test "$with_ocaml" != no; then
   AC_CHECK_PROG_OCAML()
fi


#########################
## libtool 
#########################



if test -n $FC; then 
   # LIBTOOL uses F77 as var for the fortran compiler
   F77=$FC
   FFLAGS="$FFLAGS $FCFLAGS"
fi

echo "CC 4 : $CC"

AM_PROG_LIBTOOL
LT_INIT


		
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
if test "x$MANGENERATOR" == 'x' -o "x$with_xsltproc" != 'x'; then
   AC_CHECK_MAN_GENERATOR([$with_xsltproc],[xsltproc],[xsltproc])
fi

#######################
## test for underscores
#######################
AC_CHECK_UNDERSCORE_FORTRAN()

#######################
## Test for libxml
#######################

AC_ARG_WITH(libxml2,
		[  --with-libxml2=PREFIX    set the path to your libxml2 installation],
		[with_libxml2=$withval], 
		[with_libxml2='yes']
		)

if test "$with_libxml2" != 'yes' -a "$with_libxml2" != 'no'; then
   AC_MSG_CHECKING([libxml2, for xml-config])
   XML2_CONFIG="$with_libxml2/bin/xml2-config"
        if test -x "$XML_CONFIG"; then
                AC_MSG_RESULT([$XML_CONFIG])
        else
                AC_MSG_ERROR([Unable to find $XML_CONFIG. Please check the path you provided])
		fi
else
		AC_CHECK_PROGS(XML_CONFIG,xml2-config,no)
		if test "x$XML_CONFIG" = "xno"; then
				AC_MSG_ERROR([Unable to find xml2-config in the path. Please check  your installation of libxml2])
		fi
fi
XML_FLAGS=`$XML_CONFIG --cflags`
XML_LIBS=`$XML_CONFIG --libs`
AC_C_CONST()
AC_C_INLINE()

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

typedef int integer;

/* define boolean type */
#ifdef BOOL
	#undef BOOL
#endif

typedef int BOOL ;

#ifdef TRUE
	#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
	#undef FALSE
#endif
#define FALSE 0

/*  params.h */
#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		paramlist
#endif
#else
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		()
#endif
#endif



#endif /* MACHINE_H  */
])


AM_CONDITIONAL(GUI_X11, test "$GUI" = "X11")
AM_CONDITIONAL(GUI_WIN32, test "$GUI" = "WIN32")
AM_CONDITIONAL(TERM_ZTERM, test "$TERM_NAME" = "zterm")
AM_CONDITIONAL(TERM_VTE, test "$GUI" = "vte")



AC_CONFIG_COMMANDS_POST([chmod +x $SCIDIR/modules/gui/src/c/xsci/ad2c.sh])
AC_CONFIG_COMMANDS_POST([chmod +x $SCIDIR/modules/scicos/src/c/GenBlocks.sh])
dnl #########################

dnl ## substituting variables
dnl #########################


AC_SUBST(SCIDIR)
AC_SUBST(tcldir)

AC_SUBST(RM)
AC_SUBST(AR)
AC_SUBST(AR_FLAGS)
AC_SUBST(FILES_TO_CLEAN)
AC_SUBST(CC)
AC_SUBST(CC_OPTIONS)
AC_SUBST(CC_LDFLAGS)
AC_SUBST(CC_PICFLAGS)
AC_SUBST(CXX)
AC_SUBST(CXXFLAGS)
AC_SUBST(CXXCPP)
AC_SUBST(FC)


AC_SUBST(FFLAGS)
AC_SUBST(FFLAGS1)
AC_SUBST(FC_LDFLAGS)
AC_SUBST(FC_PICFLAGS)
AC_SUBST(LD)
AC_SUBST(LD_LDFLAGS)
AC_SUBST(SCILAB_GS)
AC_SUBST(SCILABSCRIPT)
AC_SUBST(HPPA_OLD)
AC_SUBST(DLDLIB)
AC_SUBST(DLD_SUBDIR)
AC_SUBST(XAWLIB)
AC_SUBST(X_LIB_PATH)
AC_SUBST(XAW)
AC_SUBST(XMULIB)
AC_SUBST(XAW_LOCAL_SUBDIR)
AC_SUBST(XAW_LOCAL_INCLUDES)
AC_SUBST(XAW_LOCAL_LIB)
AC_SUBST(TERMCAP_LIB)
AC_SUBST(GUI)
AC_SUBST(GUILIB)
AC_SUBST(GZVT)
AC_SUBST(XDRLIB)
AC_SUBST(XDRLIBNAME)
AC_SUBST(WINXFLAGS)
AC_SUBST(MENUSOBJS)
AC_SUBST(MALLOC)

AC_SUBST(TKBASE)
AC_SUBST(TK_INC_PATH)
AC_SUBST(TCL_INC_PATH)
AC_SUBST(TCLTK_LIBS)
AC_SUBST(TCLSCILIB)
AC_SUBST(TCLSCIDIR)
AC_SUBST(TCLTKHELP)
AC_SUBST(TK_MINOR_VERSION)
AC_SUBST(TK_MAJOR_VERSION)

AC_SUBST(NOINLINE)
AC_SUBST(WHICH_BLAS)
AC_SUBST(ATLAS_DIR)
AC_SUBST(IN_ATLAS)

AC_SUBST(XML_FLAGS)
AC_SUBST(XML_LIBS)

AC_SUBST(MANTARGET)
# for Makemex

AC_SUBST(SHELL)
AC_SUBST(DLLTOOL)
AC_SUBST(LD)
AC_SUBST(LIBM)
#AC_SUBST(LIBTOOL)
AC_SUBST(LIBTOOL_DEPS)
AC_SUBST(LN_S)
AC_SUBST(NM)
AC_SUBST(OBJDUMP)
AC_SUBST(PACKAGE)
AC_SUBST(RANLIB)
AC_SUBST(STATIC)
AC_SUBST(SCIVERSION)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

AC_SUBST(X_LIBS)
AC_SUBST(X_CFLAGS)


AC_SUBST(LIBS)
AC_SUBST(FLIBS)
#AC_SUBST(CC)
AC_SUBST(CFLAGS)
AC_SUBST(FC_SUBDIR)
AC_SUBST(BLPR_SCRIPTS)
AC_SUBST(SED_CYGDRIVE)
AC_SUBST(EXAMPLES_TESTS)
AC_SUBST(IMPPREF)
AC_SUBST(IMPENV)
AC_SUBST(ZZLEDT)
#AC_SUBST(STRIP)
AC_SUBST(HTMLFLAGS)
AC_SUBST(XXTXEXT)
AC_SUBST(PERIX11NOPT)
AC_SUBST(GETLIBTK)
AC_SUBST(MANGENERATOR)

AC_SUBST(JAVA_JNI_INCLUDE)
AC_SUBST(JAVA_JNI_LIBS)
AC_SUBST(GCCFIX)

## Faudriat mettre -I/home/sylvestre/dev/scilab5/libs/MALLOC/includes/ juste dans les modules ou c'est necessaire

AC_CONFIG_FILES([ 
libs/lapack/Makefile
libs/blas/Makefile
libs/MALLOC/Makefile
libs/hashtable/Makefile
libs/f2c/Makefile
modules/data_structures/Makefile
modules/differential_equations/Makefile
modules/optimization/Makefile
modules/elementaries_functions/Makefile
modules/special_functions/Makefile
modules/io/Makefile
modules/jvm/Makefile
modules/sound/Makefile
modules/statistics/Makefile
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
modules/string/Makefile
modules/time/Makefile
modules/graphics/Makefile
modules/gui/Makefile
modules/pvm/Makefile
modules/randlib/Makefile
modules/tclsci/Makefile
modules/wintools/Makefile
modules/core/Makefile
modules/scicos/Makefile
modules/Makefile
libs/Makefile 
Makefile
])#MAN_FR MAN_ENG


#AC_CONFIG_FILES([ 
#modules/Makefile 
#Makefile
#modules/os_specific/os_specific.pc modules/fileio/fileio.pc  modules/default/Makefile modules/os_specific/Makefile modules/graphics/Makefile  modules/console/Makefile  modules/calelm/Makefile modules/menusX/Makefile modules/metanet/Makefile modules/xsci/Makefile modules/scicos/Makefile modules/slicot/Makefile modules/blas/Makefile modules/tclsci/Makefile modules/randlib/Makefile modules/xdr/Makefile modules/wintools/Makefile modules/system/Makefile modules/system2/Makefile modules/signal/Makefile modules/arpack/Makefile modules/sparse/Makefile modules/interf/Makefile modules/int/Makefile modules/integ/Makefile modules/control/Makefile modules/dcd/Makefile modules/dld/Makefile   modules/gd/Makefile modules/fileio/Makefile   modules/lapack/Makefile modules/optim/Makefile modules/version.h  modules/intersci/Makefile modules/gtksci/Makefile  modules/gzvt2/Makefile modules/poly/Makefile modules/Javasci/Makefile modules/pvm/Makefile 
#scripts/Makefile scripts/scilab.g scripts/scilab-gtk.g
#macros/util/gettklib.sci macros/util/xmlfiletohtml.sci
#examples/Makefile
#imp/Makefile
#Makemex
#])#MAN_FR MAN_ENG
#
# config/Makeso.incl config/Makefile.aix config/Makefile.ultrix no longer used 

#for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
#  test -f $top_builddir/configure && break
#done

AC_OUTPUT

echo ""

echo "Scilab is configured as follows. Please verify that this configuration"
echo "matches your expectations."
echo ""
echo "Host system type : $host"
echo ""
echo "                  Option                        Value"
echo "-------------------------------------------------------------------------"
echo "Shared libraries..		--enable-shared=$enable_shared	$libtool_build_shared_libs"
echo "Static libraries..		--enable-static=$enable_static	$libtool_build_static_libs"
echo "Module support....		--with-modules=$with_modules	$with_modules"
echo "GNU ld............		--with-gnu-ld=$with_gnu_ld		$lt_cv_prog_gnu_ld"
echo "Enable debug......		--enable-debug=$enable_debug	$enable_debug"
echo ""
echo "Compiler Configuration:"
echo "GNU compilers (--with-gnu) ....... :		$with_gnu"
echo "Intel (--with-intelcompilers) .... :		$with_intelcompilers"
echo "GNU gcc (--with-gcc) ............. :		$with_gcc"
echo "GNU Fortran 77 (--with-g77) ...... :		$with_g77"
echo "GNU Fortran 95 (--with-gfortran) . :		$with_gfortran"
echo ""
echo "Options:"
echo "Uses local xaw (--with-local-xaw) ............... : $with_local_xaw"
echo "Use PVM (--with-pvm) ............................ : $with_pvm"
echo "PVM include (--with-pvm-include) ................ : $PVMINCLUDE"
echo "PVM library (--with-pvm-library) ................ : $PVMLIBDIR"
echo "Don't use TCL/TK (--without-tk) ................. : $with_tk"
echo "Don't compile with Ocaml (--without-ocaml) ...... : $with_ocaml"
echo "TCL include (--with-tcl-include) ................ : $USER_TCL_INC_PATH"
echo "TCL library (--with-tcl-library) ................ : $USER_TCL_LIB_PATH"
echo "TK include (--with-tk-include) .................. : $USER_TK_INC_PATH"
echo "TK library (--with-tk-library) .................. : $USER_TK_LIB_PATH"
echo "Atlas library (--with-atlas-library) ............ : $USER_ATLAS_LIB_PATH"
echo "Sablotron processor (--with-sablotron) .......... : $with_sablotron"
echo "Xsltproc processor (--with-xsltproc) ............ : $with_xsltproc"
echo ""
echo "GUI : $GUI"
echo ""
echo "X11 Configuration:"
if test "$have_x" != 'no'
then
  echo "X_CFLAGS .... : $X_CFLAGS"
  echo "X_PRE_LIBS .. : $X_PRE_LIBS"
  echo "X_LIBS ...... : $X_LIBS"
  echo "X_EXTRA_LIBS  : $X_EXTRA_LIBS"
else
  echo ""
  echo "Not using X11."
fi

echo ""
if test ! -z "$PVM_INCLUDE"; then
   echo "PVM Configuration:"
   echo "PVM Architecture : $PVMARCH"
   echo "PVM INCLUDE .... : $PVM_INCLUDE"
   echo "PVM LIBS ....... : $PVM_LIB"
else
   echo "Not using PVM"
fi

echo ""
if test ! -z "$MANGENERATOR"; then
   echo "XSL Processor Configuration:"
   echo "XSL Processor .. : $MANGENERATOR"
else
   echo "No documentation generated"
fi

echo ""
echo "Options used to compile and link:"
echo "  PREFIX           = $PREFIX_DIR"
echo "  EXEC-PREFIX      = $EXEC_PREFIX_DIR"
echo "  VERSION          = $PACKAGE_VERSION"
echo "  CC               = $CC"
echo "  CFLAGS           = $CFLAGS"
#echo "  CPPFLAGS         = $CPPFLAGS"
echo "  PCFLAGS          = $PCFLAGS"
echo "  DEFS             = $DEFS"
echo "  LD               = $LD"
echo "  LDFLAGS          = $LDFLAGS"
echo "  LIBS             = $LIBS"
#echo "  CXX              = $CXX"
#echo "  CXXFLAGS         = $CXXFLAGS"
echo "  FC               = $FC"
echo "  FFLAGS           = $FFLAGS"
echo "  FC_LDFLAGS       = $FC_LDFLAGS"
echo "  XML_FLAGS        = $XML_FLAGS"
echo "  XML_LIBS         = $XML_LIBS"
echo "  TERMCAP_LIB      = $TERMCAP_LIB"
echo "  TK_INC_PATH      = $TK_INC_PATH"
echo "  TCL_INC_PATH     = $TCL_INC_PATH"
echo "  TCLTK_LIBS       = $TCLTK_LIBS"
if test ! -z "$JAVAC"; then
echo "  JAVAC            = $JAVAC"
echo "  JAVA_VERSION     = $JAVA_VERSION"
echo "  JAVAC_FLAGS      = $JAVAC_FLAGS"
echo "  JAVA_JNI_INCLUDE = $JAVA_JNI_INCLUDE"
echo "  JAVA_JNI_LIBS    = $JAVA_JNI_LIBS"
echo "  JAVA_JNI_LIBS_PRELOAD = $JAVA_JNI_LIBS_PRELOAD"
fi
echo ""
