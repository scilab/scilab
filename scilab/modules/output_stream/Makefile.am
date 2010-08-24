# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Bruno JOFRET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

OUTPUT_STREAM_CPP_SOURCES = src/cpp/Diary.cpp \
	src/cpp/DiaryList.cpp \
	src/cpp/diary_manager.cpp \
	src/cpp/getDiaryDate.cpp \
	src/cpp/getFullFilename.cpp

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_diary.cpp

OUTPUT_STREAM_C_SOURCES = 	src/c/erro.c \
				src/c/cerro.c \
				src/c/Scierror.c \
				src/c/msgstore.c \
				src/c/msgout.c \
				src/c/msgs.c \
				src/c/msgstxt.c \
				src/c/basout.c \
				src/c/sciprint.c \
				src/c/sciprint_full.c \
				src/c/errmds.c \
				src/c/errmsg.c \
				src/c/error.c \
				src/c/error_internal.c \
				src/c/do_xxprintf.c \
				src/c/set_xxprintf.c

OUTPUT_STREAM_FORTRAN_SOURCES = src/fortran/wspdsp.f \
		src/fortran/wmdsp.f \
		src/fortran/strdsp.f \
		src/fortran/prntid.f \
		src/fortran/print.f \
		src/fortran/lspdsp.f \
		src/fortran/fmttyp.f \
		src/fortran/fmt.f \
		src/fortran/dspdsp.f \
		src/fortran/dmrdsp.f \
		src/fortran/dmpdsp.f \
		src/fortran/dmdspf.f \
		src/fortran/dmdsp.f \
		src/fortran/dldsp.f \
		src/fortran/basouttofile.f \
		src/fortran/formatnumber.f


GATEWAY_C_SOURCES = sci_gateway/c/gw_output_stream.c \
		sci_gateway/c/sci_print.c \
		sci_gateway/c/sci_mprintf.c \
		sci_gateway/c/sci_msprintf.c \
		sci_gateway/c/sci_disp.c
		

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/intprint.f \
			sci_gateway/fortran/intdisp.f

libscioutput_stream_la_CFLAGS=  -I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/core/includes \
-I$(srcdir)/src/c/ \
-I$(top_srcdir)/modules/shell/includes \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/string/includes

libscioutput_stream_la_CPPFLAGS=  -I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/core/includes \
-I$(srcdir)/src/cpp/ \
-I$(srcdir)/src/c/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/string/includes


pkglib_LTLIBRARIES = libscioutput_stream.la

if GUI
USEANT=1
endif

libscioutput_stream_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscioutput_stream_la_SOURCES = $(OUTPUT_STREAM_C_SOURCES) $(OUTPUT_STREAM_CPP_SOURCES) $(OUTPUT_STREAM_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(JNI_SOURCES)

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
libscioutput_stream_la_sci_gateway_DATA = sci_gateway/output_stream_gateway.xml

#### output_stream : include files ####
libscioutput_stream_la_includedir=$(pkgincludedir)
libscioutput_stream_la_include_HEADERS = includes/do_error_number.h \
includes/sciprint.h \
includes/cerro.h \
includes/Scierror.h

include $(top_srcdir)/Makefile.incl.am

