##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

DYNAMIC_LINK_C_SOURCES = src/c/addinter.c \
src/c/dynamic_link.c \
src/c/dl_genErrorMessage.c \
src/c/AddFunctionInTable.c \
src/c/ilib_verbose.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_dynamic_link.c \
sci_gateway/c/sci_getdynlibext.c \
sci_gateway/c/sci_addinter.c \
sci_gateway/c/sci_call.c \
sci_gateway/c/sci_link.c \
sci_gateway/c/sci_ulink.c \
sci_gateway/c/sci_c_link.c \
sci_gateway/c/sci_ilib_verbose.c

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/sci_fort.f 

libscidynamic_link_la_CFLAGS = \
-I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/modules/fileio/includes \
-I$(top_srcdir)/modules/io/includes \
-I$(top_srcdir)/libs/dynamiclibrary/includes \
-I$(srcdir)/src/c/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(srcdir)/includes/

pkglib_LTLIBRARIES = libscidynamic_link.la

libscidynamic_link_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)
libscidynamic_link_la_SOURCES = $(DYNAMIC_LINK_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC=  $(DYNAMIC_LINK_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscidynamic_link_la_CFLAGS)

# cyclic $(top_builddir)/modules/core/libscicore.la 
libscidynamic_link_la_LIBADD =  $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/libs/dynamiclibrary/libscidynamiclibrary.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Target ######
modulename=dynamic_link


#### dynamic_link : Conf files ####
libscidynamic_link_la_rootdir = $(mydatadir)
libscidynamic_link_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### dynamic_link : init scripts #### 
libscidynamic_link_la_etcdir = $(mydatadir)/etc
libscidynamic_link_la_etc_DATA = etc/dynamic_link.quit etc/dynamic_link.start

#### dynamic_link : gateway declaration ####
libscidynamic_link_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscidynamic_link_la_sci_gateway_DATA = sci_gateway/dynamic_link_gateway.xml

#### dynamic_link : compilation scripts #### 
libscidynamic_link_la_scriptdir = $(mydatadir)/src/scripts/
libscidynamic_link_la_script_DATA = src/scripts/Makefile.in \
src/scripts/configure.ac \
src/scripts/ltmain.sh \
src/scripts/Makefile.am \
src/scripts/aclocal.m4

libscidynamic_link_la_script_SCRIPTS = src/scripts/compilerDetection.sh \
src/scripts/config.guess \
src/scripts/config.sub \
src/scripts/configure \
src/scripts/depcomp \
src/scripts/install-sh \
src/scripts/missing \
src/scripts/scicompile.sh

include $(top_srcdir)/Makefile.incl.am
