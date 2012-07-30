# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2011 - DIGITEO - Antoine ELIAS
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
includes/Thread_Wrapper.h

include $(top_srcdir)/Makefile.incl.am
