# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007-2008 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
# Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

if GUI

USEANT=1

# List of the c files
ACTION_BINDING_C_SOURCES = src/c/InterpreterManagement.c

# List of the c++ file
ACTION_BINDING_CXX_SOURCES = src/jni/Signal.cpp

# List of the gateway c files
CONSOLE_C_JNI_SOURCES = src/jni/InterpreterManagement_wrap.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_action_binding.c

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_notify.cpp

endif

SWIG_WRAPPERS = src/jni/InterpreterManagement.i
GIWS_WRAPPERS = src/jni/Signal.giws.xml

BUILT_SOURCES=
if SWIG
BUILT_SOURCES+=swig
endif
if GIWS
BUILT_SOURCES+=giws
endif

libsciaction_binding_disable_la_SOURCES = src/noaction_binding/noaction_binding.c
libsciaction_binding_disable_la_CPPFLAGS = 	-I$(srcdir)/includes/ \
											-I$(top_srcdir)/modules/localization/includes/ \
											-I$(top_srcdir)/modules/output_stream/includes \
											$(AM_CPPFLAGS)

# Includes need for the compilation
libsciaction_binding_la_CPPFLAGS= 	$(JAVA_JNI_INCLUDE) \
-I$(srcdir)/includes/ \
-I$(srcdir)/src/c/ \
-I$(srcdir)/src/jni/ \
-I$(top_srcdir)/modules/jvm/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes/ \
-I$(top_srcdir)/modules/commons/src/jni/ \
-I$(top_srcdir)/modules/localization/includes/ \
-I$(top_srcdir)/modules/output_stream/includes \
$(AM_CPPFLAGS)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libsciaction_binding-disable.la

if GUI
pkglib_LTLIBRARIES += libsciaction_binding.la
noinst_LTLIBRARIES = libsciaction_binding-algo.la
endif


# All the sources needed by libsciaction_binding.la
libsciaction_binding_algo_la_SOURCES = $(ACTION_BINDING_C_SOURCES) $(ACTION_BINDING_CXX_SOURCES) $(CONSOLE_C_JNI_SOURCES)
libsciaction_binding_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libsciaction_binding_algo_la_CPPFLAGS = $(libsciaction_binding_la_CPPFLAGS)


# For the code check (splint)
CHECK_SRC= $(ACTION_BINDING_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciaction_binding_la_CPPFLAGS)

## Libraries necessary
libsciaction_binding_la_LIBADD = libsciaction_binding-algo.la

#### Name of the module and the path ######
modulename=action_binding

#### action_binding : Conf files ####
libsciaction_binding_la_rootdir = $(mydatadir)
libsciaction_binding_la_root_DATA =  license.txt 

####�action_binding : init scripts ####
libsciaction_binding_la_etcdir = $(mydatadir)/etc
libsciaction_binding_la_etc_DATA = etc/action_binding.quit etc/action_binding.start


####�action_binding : gateway declaration ####
libsciaction_binding_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciaction_binding_la_sci_gateway_DATA = sci_gateway/action_binding_gateway.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1
include $(top_srcdir)/Makefile.incl.am
