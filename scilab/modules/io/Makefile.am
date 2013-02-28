# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


IO_C_SOURCES = src/c/getpidc.c \
src/c/setenvc.c \
src/c/systemc.c \
src/c/getenvc.c \
src/c/GetenvB.c \
src/c/v2cuniterror.c

IO_FORTRAN_SOURCES = src/fortran/v2unit.f \
src/fortran/newsave.f \
src/fortran/v2cunit.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_read4b.c \
sci_gateway/c/sci_getio.c \
sci_gateway/c/sci_readb.c \
sci_gateway/c/sci_host.c \
sci_gateway/c/sci_file.c \
sci_gateway/c/sci_getenv.c \
sci_gateway/c/sci_write.c \
sci_gateway/c/sci_writeb.c \
sci_gateway/c/sci_setenv.c \
sci_gateway/c/gw_io.c \
sci_gateway/c/sci_read.c \
sci_gateway/c/sci_getpid.c \
sci_gateway/c/sci_write4b.c \
sci_gateway/c/sci_load.c \
sci_gateway/c/sci_unix.c \
sci_gateway/c/sci_save.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/intwrite4b.f \
sci_gateway/fortran/intwritb.f \
sci_gateway/fortran/intwrite.f \
sci_gateway/fortran/intread4b.f \
sci_gateway/fortran/intread.f \
sci_gateway/fortran/intreadb.f \
sci_gateway/fortran/intfile.f

libsciio_la_CPPFLAGS = -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(top_srcdir)/modules/core/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciio.la
noinst_LTLIBRARIES = libsciio-algo.la
else
noinst_LTLIBRARIES = libsciio-algo.la libsciio.la
endif



libsciio_algo_la_SOURCES = $(IO_C_SOURCES) $(IO_FORTRAN_SOURCES)
libsciio_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libsciio_algo_la_CPPFLAGS = $(libsciio_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(IO_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciio_la_CPPFLAGS)

libsciio_la_LIBADD = libsciio-algo.la


#### Target ######
modulename=io

#### io : Conf files ####
libsciio_la_rootdir = $(mydatadir)
libsciio_la_root_DATA =  license.txt 


####io : init scripts ####
libsciio_la_etcdir = $(mydatadir)/etc
libsciio_la_etc_DATA = etc/io.quit etc/io.start

####io : gateway declaration ####
libsciio_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciio_la_sci_gateway_DATA = sci_gateway/io_gateway.xml

include $(top_srcdir)/Makefile.incl.am

