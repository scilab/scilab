##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

INCREMENTAL_LINK_C_SOURCES = src/c/addinter.c \
src/c/link.c \
src/c/AddFunctionInTable.c

#src/c/link_SYSV.c 
#src/c/link_std.c 
#src/c/link_W95.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_incremental_link.c \
sci_gateway/c/sci_getdynlibext.c \
sci_gateway/c/sci_addinter.c \
sci_gateway/c/sci_call.c \
sci_gateway/c/sci_link.c \
sci_gateway/c/sci_ulink.c \
sci_gateway/c/sci_c_link.c

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/sci_addinter.f \
sci_gateway/fortran/sci_fort.f \
sci_gateway/fortran/sci_link.f \
sci_gateway/fortran/sci_c_link.f

libsciincremental_link_la_CFLAGS = \
-I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/libs/doublylinkedlist/includes \
-Isrc/c/ \
-Iincludes/

pkglib_LTLIBRARIES = libsciincremental_link.la
libsciincremental_link_la_SOURCES = $(INCREMENTAL_LINK_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)


#### Target ######
modulename=incremental_link


#### incremental_link : Conf files ####
libsciincremental_link_la_rootdir = $(mydatadir)
libsciincremental_link_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### incremental_link : init scripts #### 
libsciincremental_link_la_etcdir = $(mydatadir)/etc
libsciincremental_link_la_etc_DATA = etc/incremental_link.quit etc/incremental_link.start

#### incremental_link : gateway declaration ####
libsciincremental_link_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciincremental_link_la_sci_gateway_DATA = sci_gateway/incremental_link_gateway.xml


#### incremental_link : compilation scripts #### 
libsciincremental_link_la_scriptdir = $(mydatadir)/src/scripts/
libsciincremental_link_la_script_DATA = src/scripts/Makefile.in \
src/scripts/configure.ac \
src/scripts/ltmain.sh \
src/scripts/Makefile.am \
src/scripts/aclocal.m4

libsciincremental_link_la_script_SCRIPTS = src/scripts/compilerDetection.sh \
src/scripts/config.guess \
src/scripts/config.sub \
src/scripts/configure \
src/scripts/depcomp \
src/scripts/install-sh \
src/scripts/missing \
src/scripts/scicompile.sh

include $(top_srcdir)/Makefile.incl.am
