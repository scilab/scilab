# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007-2008 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
# Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

if GUI

USEANT=1

# List of the c files
ACTION_BINDING_C_SOURCES = src/c/InterpreterManagement.c

# List of the c++ file
ACTION_BINDING_CPP_SOURCES = \
    src/jni/Signal.cpp \
    src/jni/Signal.hxx \
    src/cpp/action_binding_GiwsExports.cpp

# List of the gateway c files
CONSOLE_C_JNI_SOURCES = src/jni/InterpreterManagement_wrap.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/action_binding_gw.cpp \
    sci_gateway/cpp/sci_notify.cpp

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

libsciaction_binding_disable_la_SOURCES = src/noaction_binding/noaction_binding.cpp

libsciaction_binding_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/localization/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/libs/dynamiclibrary/includes \
	$(AM_CPPFLAGS)

# Includes need for the compilation
libsciaction_binding_la_CPPFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/functions_manager/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/string/includes \
    $(AM_CPPFLAGS)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libsciaction_binding-disable.la

if GUI
pkglib_LTLIBRARIES += libsciaction_binding.la
noinst_LTLIBRARIES = libsciaction_binding-algo.la
endif


# All the sources needed by libsciaction_binding.la
libsciaction_binding_algo_la_SOURCES = $(ACTION_BINDING_C_SOURCES) $(ACTION_BINDING_CPP_SOURCES) $(CONSOLE_C_JNI_SOURCES)
libsciaction_binding_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libsciaction_binding_algo_la_CPPFLAGS = $(libsciaction_binding_la_CPPFLAGS)


# For the code check (splint)
CHECK_SRC= $(ACTION_BINDING_CPP_SOURCES) $(GATEWAY_CPP_SOURCES)
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
libsciaction_binding_la_etc_DATA = \
    etc/action_binding.quit \
    etc/action_binding.start

####�action_binding : gateway declaration ####
libsciaction_binding_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciaction_binding_la_sci_gateway_DATA = sci_gateway/action_binding_gateway.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1
include $(top_srcdir)/Makefile.incl.am
