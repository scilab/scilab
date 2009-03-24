##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

DOUBLE_C_SOURCES = 	src/c/matmult.c \
			src/c/matldiv.c \
			src/c/matrdiv.c 

GATEWAY_C_SOURCES = sci_gateway/c/gw_double.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/vecldiv.f \
sci_gateway/fortran/vecmul.f \
sci_gateway/fortran/matxpow.f \
sci_gateway/fortran/vecimpl.f \
sci_gateway/fortran/matldiv.f \
sci_gateway/fortran/matsubst.f \
sci_gateway/fortran/vecrdiv.f \
sci_gateway/fortran/matchsgn.f \
sci_gateway/fortran/matrdiv.f \
sci_gateway/fortran/matrc.f \
sci_gateway/fortran/matpow.f \
sci_gateway/fortran/matins1.f \
sci_gateway/fortran/matins2.f \
sci_gateway/fortran/matcmp.f \
sci_gateway/fortran/matext1.f \
sci_gateway/fortran/matext2.f \
sci_gateway/fortran/mattr.f \
sci_gateway/fortran/matcc.f \
sci_gateway/fortran/mattrc.f \
sci_gateway/fortran/matadd.f

pkglib_LTLIBRARIES = libscidouble.la

libscidouble_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscidouble_la_CFLAGS = 	-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/elementary_functions/includes \
				-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/double/includes

libscidouble_la_SOURCES = $(DOUBLE_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscidouble_la_CFLAGS)

libscidouble_la_LIBADD =  $(top_builddir)/modules/boolean/libsciboolean.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/polynomials/libscipolynomials.la $(top_builddir)/modules/linear_algebra/libscilinear_algebra.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/modules/operations/libscioperations.la

#### Target ######
modulename=double


#### double : Conf files ####
libscidouble_la_rootdir = $(mydatadir)
libscidouble_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### double : init scripts ####
libscidouble_la_etcdir = $(mydatadir)/etc
libscidouble_la_etc_DATA = etc/double.quit etc/double.start

include $(top_srcdir)/Makefile.incl.am
