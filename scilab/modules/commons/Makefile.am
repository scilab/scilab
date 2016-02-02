#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# 
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
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

COMMONS_C_SOURCES =  \
    src/jni/ScilabCommons_wrap.c \
    src/jni/CommonFileUtils_wrap.c \
    src/c/fileutils.c

COMMONS_CPP_SOURCES = 
if NEED_JAVA
COMMONS_CPP_SOURCES +=  src/jni/GiwsException.cpp
endif

libscicommons_la_CPPFLAGS= \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    $(AM_CPPFLAGS)


libscicommons_disable_la_SOURCES = $(COMMONS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscicommons-disable.la

if GUI
pkglib_LTLIBRARIES += libscicommons.la 
endif


libscicommons_la_SOURCES = $(COMMONS_C_SOURCES) $(COMMONS_JNI_SOURCES) $(COMMONS_CPP_SOURCES)

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
