##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########


GATEWAY_C_SOURCES = sci_gateway/c/sci_dlgamma.c \
sci_gateway/c/sci_oldbessely.c \
sci_gateway/c/sci_bessely.c \
sci_gateway/c/sci_beta.c \
sci_gateway/c/sci_gamma.c \
sci_gateway/c/sci_oldbesseli.c \
sci_gateway/c/sci_legendre.c \
sci_gateway/c/sci_besselh.c \
sci_gateway/c/sci_oldbesselj.c \
sci_gateway/c/sci_besseli.c \
sci_gateway/c/sci_besselj.c \
sci_gateway/c/sci_oldbesselk.c \
sci_gateway/c/sci_besselk.c \
sci_gateway/c/sci_calerf.c \
sci_gateway/c/gw_special_functions1.c \
sci_gateway/c/gw_special_functions2.c \
sci_gateway/c/sci_lgamma.c

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/sci_f_dlgamma.f \
sci_gateway/fortran/sci_f_lgamma.f \
sci_gateway/fortran/sci_f_oldbesseli.f \
sci_gateway/fortran/sci_f_gamma.f \
sci_gateway/fortran/sci_f_oldbessely.f \
sci_gateway/fortran/sci_f_oldbesselj.f \
sci_gateway/fortran/sci_f_oldbesselk.f \
sci_gateway/fortran/sci_f_calerf.f

libscispecial_functions_la_includedir=$(pkgincludedir)/special_functions

libscispecial_functions_la_include_HEADERS = includes/gw_special_functions1.h \
includes/gw_special_functions2.h

libscispecial_functions_la_CFLAGS=  	-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/elementary_functions/includes/ \
-I$(top_srcdir)/modules/output_stream/includes/ \
-I$(top_srcdir)/modules/operations/includes/

pkglib_LTLIBRARIES = libscispecial_functions.la

libscispecial_functions_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscispecial_functions_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC = $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscispecial_functions_la_CFLAGS)

libscispecial_functions_la_LIBADD =   $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/statistics/libscistatistics.la


#### Target ######
modulename=special_functions


#### special_functions : Conf files ####
libscispecial_functions_la_rootdir = $(mydatadir)
libscispecial_functions_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### special_functions : init scripts ####
libscispecial_functions_la_etcdir = $(mydatadir)/etc
libscispecial_functions_la_etc_DATA = etc/special_functions.quit etc/special_functions.start

#### special_functions : gateway declaration ####
libscispecial_functions_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscispecial_functions_la_sci_gateway_DATA = sci_gateway/special_functions_gateway.xml

include $(top_srcdir)/Makefile.incl.am
