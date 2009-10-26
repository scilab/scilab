##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

RANDLIB_C_SOURCES = src/c/fsultra.c \
src/c/mt.c \
src/c/igngeom.c \
src/c/kiss.c \
src/c/urand.c \
src/c/clcg2.c \
src/c/clcg4.c \
src/c/sexpo.c \
src/c/ignbin.c \
src/c/ignpoi.c \
src/c/sgamma.c \
src/c/genbet.c 

RANDLIB_FORTRAN_SOURCES = \
src/fortran/gennf.f \
src/fortran/genchi.f \
src/fortran/setgmn.f \
src/fortran/gengam.f \
src/fortran/snorm.f \
src/fortran/gennch.f \
src/fortran/genmn.f \
src/fortran/phrtsd.f \
src/fortran/ignnbn.f \
src/fortran/genf.f \
src/fortran/genunf.f \
src/fortran/genexp.f \
src/fortran/genmul.f \
src/fortran/genprm.f \
src/fortran/gennor.f
#src/fortran/genbet.f
#src/fortran/sexpo.f 
#src/fortran/ignbin.f
#src/fortran/ignpoi.f
#src/fortran/sgamma.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_randlib.c \
sci_gateway/c/sci_grand.c

libscirandlib_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libscirandlib.la

libscirandlib_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscirandlib_la_SOURCES = $(RANDLIB_C_SOURCES) $(GATEWAY_C_SOURCES) $(RANDLIB_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(RANDLIB_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscirandlib_la_CFLAGS)

libscirandlib_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la




#### Target ######
modulename=randlib


#### randlib : Conf files ####
libscirandlib_la_rootdir = $(mydatadir)
libscirandlib_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### randlib : init scripts ####
libscirandlib_la_etcdir = $(mydatadir)/etc
libscirandlib_la_etc_DATA = etc/randlib.quit etc/randlib.start

#### randlib : gateway declaration ####
libscirandlib_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscirandlib_la_sci_gateway_DATA = sci_gateway/randlib_gateway.xml

include $(top_srcdir)/Makefile.incl.am
