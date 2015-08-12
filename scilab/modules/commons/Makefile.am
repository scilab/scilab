#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
#

#### Target ######
modulename=commons


#### commons : Conf files ####
libscicommons_la_rootdir = $(mydatadir)
libscicommons_la_root_DATA =  license.txt 


#### commons : init scripts ####
libscicommons_la_etcdir = $(mydatadir)/etc
libscicommons_la_etc_DATA = \
	etc/commons.quit \
	etc/commons.start

COMMONS_JNI_SOURCES = src/jni/FindIconHelper.cpp

COMMONS_C_SOURCES =  src/jni/ScilabCommons_wrap.c \
		     src/jni/CommonFileUtils_wrap.c \
		     src/c/fileutils.c

COMMONS_CXX_SOURCES = 
if NEED_JAVA
COMMONS_CXX_SOURCES +=  src/jni/GiwsException.cpp
endif

COMMONS_FORTRAN_SOURCES = src/fortran/empty.f

libscicommons_la_CPPFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/jni/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			$(AM_CPPFLAGS)


libscicommons_disable_la_SOURCES = $(COMMONS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscicommons-disable.la

if GUI
pkglib_LTLIBRARIES += libscicommons.la 
endif


libscicommons_la_SOURCES = $(COMMONS_C_SOURCES) $(COMMONS_FORTRAN_SOURCES) $(COMMONS_JNI_SOURCES) $(COMMONS_CXX_SOURCES)

# For the code check (splint)
CHECK_SRC= $(COMMONS_C_SOURCES)
INCLUDE_FLAGS = $(libscicommons_la_CPPFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/ScilabCommons.i \
                src/jni/CommonFileUtils.i

GIWS_WRAPPERS = src/jni/FindIconHelper.giws.xml

BUILT_SOURCES=
if SWIG
BUILT_SOURCES+=swig
endif
if GIWS
BUILT_SOURCES+=giws-exception
endif

#### commons : gateway declaration ####
libscicommons_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicommons_la_sci_gateway_DATA = 

if GUI
USEANT=1
endif

include $(top_srcdir)/Makefile.incl.am
