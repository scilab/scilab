##########
### Allan CORNET
### DIGITEO - Scilab 2009
##########

FUNCTIONS__C_SOURCES = 

FUNCTIONS__FORTRAN_SOURCES = 

GATEWAY_C_SOURCES = sci_gateway/c/gw_functions.c \
sci_gateway/c/sci_lib.c \
sci_gateway/c/sci_deff.c \
sci_gateway/c/sci_getf.c \
sci_gateway/c/sci_execstr.c \
sci_gateway/c/sci_exec.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/intlib.f \
sci_gateway/fortran/intdeff.f \
sci_gateway/fortran/intgetf.f \
sci_gateway/fortran/intexecstr.f \
sci_gateway/fortran/intexec.f

libscifunctions_la_CFLAGS=		-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/io/includes \
				-I$(top_srcdir)/modules/string/includes \
				-I$(top_srcdir)/modules/localization/includes \
				-I$(top_srcdir)/modules/operations/includes

pkglib_LTLIBRARIES = libscifunctions.la

libscifunctions_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscifunctions_la_SOURCES = $(FUNCTIONS__C_SOURCES) $(FUNCTIONS__FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(FUNCTIONS_SOURCES) $(GATEWAY_SOURCES)
INCLUDE_FLAGS = $(libscifunctions_la_CFLAGS)

libscifunctions_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/modules/string/libscistring.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la \
$(top_builddir)/modules/api_scilab/libsciapi_scilab.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Target ######
modulename=functions


#### functions : Conf files ####
libscifunctions_la_rootdir = $(mydatadir)
libscifunctions_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### functions : init scripts ####
libscifunctions_la_etcdir = $(mydatadir)/etc
libscifunctions_la_etc_DATA = etc/functions.quit etc/functions.start

#### functions : gateway declaration ####
libscifunctions_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifunctions_la_sci_gateway_DATA = sci_gateway/functions_gateway.xml

#### functions : include files ####
libscifunctions_la_includedir=$(pkgincludedir)/functions
libscifunctions_la_include_HEADERS = includes/gw_functions.h 

include $(top_srcdir)/Makefile.incl.am
