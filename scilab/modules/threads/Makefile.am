# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2011 - DIGITEO - Antoine ELIAS
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

THREADS_CPP_SOURCES = \
	src/cpp/Thread_Wrapper.cpp

libscithreads_la_CPPFLAGS= \
	-I$(srcdir)/includes/
if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscithreads.la
else
noinst_LTLIBRARIES = libscithreads.la
endif

libscithreads_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) -lpthread

libscithreads_la_SOURCES = $(THREADS_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(THREADS_CPP_SOURCES)
INCLUDE_FLAGS = $(libscithreads_la_CPPFLAGS)

# Many stuff depends on this one ... commented
# libscithreads_la_LIBADD =  libscithreads.la

#### Target ######
modulename=threads

#### threads : Conf files ####
libscithreads_la_rootdir = $(mydatadir)


#### threads : init scripts ####
# libscithreads_la_etcdir = $(mydatadir)/etc
# libscithreads_la_etc_DATA = etc/threads.quit etc/threads.start

#### threads : gateway declaration ####
# libscithreads_la_sci_gatewaydir = $(mydatadir)/sci_gateway
# libscithreads_la_sci_gateway_DATA = sci_gateway/threads_gateway.xml

#### threads : include files ####
libscithreads_la_includedir=$(pkgincludedir)
libscithreads_la_include_HEADERS = \
includes/Thread_Wrapper.h \
includes/dynlib_threads.h

include $(top_srcdir)/Makefile.incl.am
