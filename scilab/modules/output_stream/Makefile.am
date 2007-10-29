##
##  -*- makefile -*-
##
## Makefile.am
## Made by  Bruno JOFRET <bruno.jofret@inria.fr>
##
## Started on  Fri Oct 26 13:48:04 2007 bruno
## Last update Fri Oct 26 17:24:00 2007 bruno
##
## Copyright INRIA 2007
##

OUTPUT_STREAM_C_SOURCES = 	src/c/erro.c \
				src/c/cerro.c \
				src/c/Scierror.c \
				src/c/msgstore.c \
				src/c/msgout.c \
				src/c/basout.c

OUTPUT_STREAM_FORTRAN_SOURCES =

GATEWAY_C_SOURCES =

GATEWAY_FORTRAN_SOURCES =

MODULE_CHECKS =

libscioutput_stream_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/modules/core/include -Isrc/c/

libscioutput_stream_la_LDFLAGS =
pkglib_LTLIBRARIES = libscioutput_stream.la

libscioutput_stream_la_SOURCES = $(OUTPUT_STREAM_C_SOURCES) $(OUTPUT_STREAM_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(JNI_SOURCES)

libscioutput_stream_la_LIBADD =

#### Target ######
modulename=output_stream

#### output_stream : Conf files ####
libscioutput_stream_la_rootdir = $(mydatadir)
libscioutput_stream_la_root_DATA = changelog.txt licence.txt readme.txt version.xml


#### output_stream : init scripts ####
libscioutput_stream_la_etcdir = $(mydatadir)/etc
libscioutput_stream_la_etc_DATA = etc/output_stream.quit etc/output_stream.start

#### output_stream : gateway declaration ####
libscioutput_stream_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscioutput_stream_la_sci_gateway_DATA =

#### output_stream : include files ####
libscioutput_stream_la_includedir=$(pkgincludedir)/output_stream
libscioutput_stream_la_include_HEADERS = includes/cerro.h \
		includes/erro.h \
		includes/Scierror.h



#### output_stream : demos files ####
libscioutput_stream_la_demosintrodir=$(mydatadir)/demos/intro/
libscioutput_stream_la_demosintro_DATA =

include $(top_srcdir)/Makefile.incl.am

