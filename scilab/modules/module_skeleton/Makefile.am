##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### TEMPLATE MAKEFILE 
### INRIA - Scilab 2007
##########

# List of the c files
SKELETON_C_SOURCES = src/c/skeleton_c.c

# List of the fortran files
SKELETON_FORTRAN_SOURCES = src/fortran/skeleton_f.f

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/sci_skeleton_c.c \
sci_gateway/c/gw_skeleton.c

# List of the gateway fortran files
GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_skeleton_f.f

# Includes need for the compilation
libsciskeleton_la_CFLAGS=  -Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libsciskeleton.la

# All the sources needed by libsciskeleton.la
libsciskeleton_la_SOURCES = $(SKELETON_C_SOURCES) $(SKELETON_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

## Libraries necessary
libsciskeleton_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Target ######
target=$(datadir)/skeleton

#### skeleton : Conf files ####
libsciskeleton_la_rootdir = $(target)
libsciskeleton_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### skeleton : init scripts #### 
libsciskeleton_la_etcdir = $(target)/etc
libsciskeleton_la_etc_DATA = etc/skeleton.quit etc/skeleton.start

#### skeleton : gateway declaration #### 
libsciskeleton_la_sci_gatewaydir = $(target)/sci_gateway
libsciskeleton_la_sci_gateway_DATA = sci_gateway/skeleton

#### skeleton : include files ####
libsciskeleton_la_includedir=$(sciincludedir)/skeleton
libsciskeleton_la_include_HEADERS = includes/gw_skeleton.h

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
