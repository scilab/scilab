# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
#List of the c files
COMPLETION_JNI_SOURCES = \
    src/jni/Completion_wrap.c
USEANT=1

endif

# List of the c files
COMPLETION_C_SOURCES = \
    src/c/completion.c \
    src/c/completion_generic.c \
    src/c/getfilesdictionary.c \
    src/c/toolsdictionary.c \
    src/c/completion_wrap_java.c \
    src/c/getPartLine.c \
    src/c/getCommonPart.c \
    src/c/completeLine.c \
    src/c/completeLine_wrap_java.c

# List of the c++ file
COMPLETION_CPP_SOURCES = \
    src/cpp/FieldsManager.cpp \
    src/cpp/XMLFieldsGetter.cpp \
    src/cpp/getfields.cpp \
    src/cpp/getfieldsdictionary.cpp \
    src/cpp/XMLDocFieldsGetter.cpp \
    src/cpp/XMLElemFieldsGetter.cpp \
    src/cpp/XMLAttrFieldsGetter.cpp \
    src/cpp/XMLNsFieldsGetter.cpp \
    src/cpp/XMLListFieldsGetter.cpp \
    src/cpp/XMLSetFieldsGetter.cpp \
    src/cpp/EOFieldsGetter.cpp \
    src/cpp/UnknownMlistFieldsGetter.cpp \
    src/cpp/StructFieldsGetter.cpp \
    src/cpp/HandleFieldsGetter.cpp \
    src/cpp/DllmainCompletion.cpp

# List of the gateway c files
GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_completion.cpp

# Includes need for the compilation
libscicompletion_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
	-I$(top_srcdir)/modules/localization/includes/ \
	-I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/xml/includes/ \
    -I$(top_srcdir)/modules/xml/src/cpp/ \
	-I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/external_objects/includes \
    $(JAVA_JNI_INCLUDE) \
    $(XML_FLAGS) \
    $(AM_CPPFLAGS)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscicompletion.la
noinst_LTLIBRARIES = libscicompletion-algo.la


# All the sources needed by libscicompletion.la
libscicompletion_algo_la_SOURCES = $(COMPLETION_JNI_SOURCES) $(COMPLETION_C_SOURCES) $(COMPLETION_CPP_SOURCES)
libscicompletion_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libscicompletion_algo_la_CPPFLAGS = $(libscicompletion_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(COMPLETION_C_SOURCES) $(GATEWAY_CPP_SOURCES)
INCLUDE_FLAGS = $(libscicompletion_la_CPPFLAGS)

## Libraries necessary
libscicompletion_la_LIBADD = libscicompletion-algo.la

#### Name of the module and the path ######
modulename=completion

#### completion : Conf files ####
libscicompletion_la_rootdir = $(mydatadir)
libscicompletion_la_root_DATA =  license.txt

#### completion : init scripts ####
libscicompletion_la_etcdir = $(mydatadir)/etc
libscicompletion_la_etc_DATA = \
    etc/completion.quit \
    etc/completion.start

#### completion : gateway declaration ####
libscicompletion_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicompletion_la_sci_gateway_DATA = sci_gateway/completion_gateway.xml

#### completion : include files ####
libscicompletion_la_includedir=$(pkgincludedir)
libscicompletion_la_include_HEADERS = \
    includes/completion.h \
    includes/getCommonPart.h \
    includes/getPartLine.h \
    includes/dynlib_completion.h \
    includes/completeLine.h

#### completion : test file ####
libscicompletion_la_testutilitiesdir = $(mydatadir)/tests/utilities
libscicompletion_la_testutilities_DATA = \
    tests/utilities/build_primitives.sce \
    tests/utilities/sci_completeline.c \
    tests/utilities/sci_getcommonpart.c \
    tests/utilities/sci_getfields.c \
    tests/utilities/sci_getfilepartlevel.c \
    tests/utilities/sci_getpartlevel.c

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/Completion.i

if SWIG
BUILT_SOURCES=swig
endif


# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1


include $(top_srcdir)/Makefile.incl.am
