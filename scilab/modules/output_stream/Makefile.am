##
##  -*- makefile -*-
##
## Makefile.am
## Made by  Bruno JOFRET <bruno.jofret@inria.fr>
##
## Started on  Fri Oct 26 13:48:04 2007 Bruno
## Last update Fri, 23 Nov 2007 15:00:33 +0100 Sylvestre
##
## Copyright INRIA 2007
##

OUTPUT_STREAM_C_SOURCES = 	src/c/erro.c \
				src/c/cerro.c \
				src/c/Scierror.c \
				src/c/msgstore.c \
				src/c/msgout.c \
				src/c/msgs.c \
				src/c/msgstxt.c \
				src/c/basout.c \
				src/c/sciprint.c \
				src/c/sciprint_nd.c \
				src/c/sciprint_full.c \
				src/c/errmds.c \
				src/c/errmsg.c \
				src/c/error.c \
				src/c/error_internal.c


OUTPUT_STREAM_FORTRAN_SOURCES =

GATEWAY_C_SOURCES =

GATEWAY_FORTRAN_SOURCES =

libscioutput_stream_la_CFLAGS=  -I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/core/includes \
-I$(srcdir)/src/c/ \
-I$(top_srcdir)/modules/string/includes

pkglib_LTLIBRARIES = libscioutput_stream.la

libscioutput_stream_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscioutput_stream_la_SOURCES = $(OUTPUT_STREAM_C_SOURCES) $(OUTPUT_STREAM_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(JNI_SOURCES)

# For the code check (splint)
CHECK_SRC= $(OUTPUT_STREAM_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscioutput_stream_la_CFLAGS)

# Many stuff depends on this one ... commented
#libscioutput_stream_la_LIBADD =  $(top_builddir)/modules/console/libsciconsole.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/shell/libscishell.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la
#### Target ######
modulename=output_stream

#### output_stream : Conf files ####
libscioutput_stream_la_rootdir = $(mydatadir)
libscioutput_stream_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### output_stream : init scripts ####
libscioutput_stream_la_etcdir = $(mydatadir)/etc
libscioutput_stream_la_etc_DATA = etc/output_stream.quit etc/output_stream.start

#### output_stream : gateway declaration ####
libscioutput_stream_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscioutput_stream_la_sci_gateway_DATA =

#### output_stream : include files ####
libscioutput_stream_la_includedir=$(pkgincludedir)/output_stream
libscioutput_stream_la_include_HEADERS = includes/cerro.h \
includes/erro.h \
includes/do_error_number.h \
includes/errmds.h \
includes/sciprint.h \
includes/sciprint_nd.h \
includes/Scierror.h \
includes/basout.h \
includes/msgs.h \
includes/sciprint_full.h

include $(top_srcdir)/Makefile.incl.am

