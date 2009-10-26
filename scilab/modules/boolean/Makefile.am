##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

BOOLEAN_C_SOURCES = src/c/matboolean.c \
src/c/vect_or.c \
src/c/vect_and.c

BOOLEAN_FORTRAN_SOURCES = src/fortran/logic.f \
src/fortran/find.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_and.c \
sci_gateway/c/sci_or.c \
sci_gateway/c/sci_find.c \
sci_gateway/c/sci_bool2s.c \
sci_gateway/c/gw_boolean.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_bool2s.f \
sci_gateway/fortran/sci_find.f

libsciboolean_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libsciboolean.la

libsciboolean_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libsciboolean_la_SOURCES = $(BOOLEAN_C_SOURCES) $(BOOLEAN_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(BOOLEAN_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciboolean_la_CFLAGS)

libsciboolean_la_LIBADD = \
	$(top_builddir)/modules/arnoldi/libsciarnoldi.la \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la

modulename=boolean

#### boolean : Conf files ####
libsciboolean_la_rootdir = $(mydatadir)
libsciboolean_la_root_DATA = changelog.txt license.txt readme.txt version.xml


####�boolean : init scripts ####
libsciboolean_la_etcdir = $(mydatadir)/etc
libsciboolean_la_etc_DATA = etc/boolean.quit etc/boolean.start

####�boolean : gateway declaration ####
libsciboolean_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciboolean_la_sci_gateway_DATA = sci_gateway/boolean_gateway.xml

include $(top_srcdir)/Makefile.incl.am
