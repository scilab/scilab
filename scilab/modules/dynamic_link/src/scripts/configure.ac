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

# Detect C compiler
AC_PROG_CC

# Detect C++ compiler
AC_PROG_CXX

# Detect Fortran Compiler (77 and 90)
AC_PROG_F77

# Initialise libtool
AC_PROG_LIBTOOL

# Specify which Makefile.in should be "adapted"
AC_CONFIG_FILES([Makefile])

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
