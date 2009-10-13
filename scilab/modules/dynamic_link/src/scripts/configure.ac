##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########
### Configure.ac is used to create the configure file which will be 
### used by the incremental link to detect compilers, flags, linker...
### Should be multiplateform
##########

# Initialise autoconf
AC_INIT([scilab],[0],[http://bugzilla.scilab.org/index.cgi])

# Initialise automake (foreign is low strictness)
AM_INIT_AUTOMAKE([foreign]) 

AM_MAINTAINER_MODE

# Detect C compiler
AC_PROG_CC

# Detect C++ compiler
AC_PROG_CXX

# Detect Fortran Compiler (77 and 90)
AC_PROG_FC

# Initialise libtool
AC_PROG_LIBTOOL

# Specify which Makefile.in should be "adapted"
AC_CONFIG_FILES([Makefile])

# x86_64 needs -fPIC
case "$host" in
  x86_64-*-linux*)
    CFLAGS="$CFLAGS -fPIC"
	CXXFLAGS="$CXXFLAGS -fPIC"
	FFLAGS="$CXXFLAGS -fPIC"
	LDFLAGS="$LDFLAGS -fPIC"
	F77_LDFLAGS="$F77_LDFLAGS -fPIC"
	;;
esac

# Mac OS >= 10.6 need -arch xx
case "$host" in
     i386-apple-darwin*)
	CC=" $CC -arch i386 "
	CXX=" $CXX -arch i386 "
	;;
     x86_64-apple-darwin*)
	CC=" $CC -arch x86_64 "
	CXX=" $CXX -arch x86_64 "
        ;;
esac

# Configure the Makefile
AC_OUTPUT


echo ""
echo "Options used to compile and link:"
echo "  CC               = $CC"
echo "  CFLAGS           = $CFLAGS"
echo "  CPP              = $CPP"
echo "  CPPFLAGS         = $CPPFLAGS"
echo "  PCFLAGS          = $PCFLAGS"
echo "  DEFS             = $DEFS"
echo "  LD               = $LD"
echo "  LDFLAGS          = $LDFLAGS"
echo "  LIBS             = $LIBS"
echo "  CXX              = $CXX"
echo "  CXXFLAGS         = $CXXFLAGS"
echo "  F77              = $F77"
echo "  FFLAGS           = $FFLAGS"
echo "  F77_LDFLAGS      = $F77_LDFLAGS"
echo ""
