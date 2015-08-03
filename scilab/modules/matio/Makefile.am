# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
# Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
#
# This file is distributed under the same license as the Scilab package.

include $(top_srcdir)/Makefile.incl.am

if MATIO

#### List of the gateway files ####
GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_matfile_open.c \
    sci_gateway/c/sci_matfile_close.c \
    sci_gateway/c/sci_matfile_listvar.c \
    sci_gateway/c/sci_matfile_varreadnext.c \
    sci_gateway/c/sci_matfile_varwrite.c

#### List of the C files ####
MATIO_C_SOURCES = \
    src/c/matfile_manager.c \
    src/c/CreateMatlabVariable.c \
    src/c/CreateBooleanVariable.c \
    src/c/CreateCharVariable.c \
    src/c/CreateDoubleVariable.c \
    src/c/CreateIntegerVariable.c \
    src/c/CreateSparseVariable.c \
    src/c/GetMatlabVariable.c

#### List of the C++ files ####
MATIO_CPP_SOURCES = \
    src/cpp/CreateCellVariable.cpp \
    src/cpp/CreateHyperMatrixVariable.cpp \
    src/cpp/CreateMatlabTreeVariable.cpp \
    src/cpp/GetCellVariable.cpp \
    src/cpp/GetStructVariable.cpp \
    src/cpp/CreateStructVariable.cpp \
	src/cpp/ConvertSciVarToMatVar.cpp \
    src/cpp/GetIntegerVariable.cpp \
    src/cpp/GetCharVariable.cpp \
	src/cpp/GetDoubleVariable.cpp \
    src/cpp/GetSparseVariable.cpp \
    src/cpp/GetMlistVariable.cpp

libscimatio_la_LDFLAGS = $(AM_LDFLAGS)

else

MATIO_C_SOURCES =
GATEWAY_CPP_SOURCES	=

endif

libscimatio_la_CPPFLAGS = \
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
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    $(MATIO_CFLAGS) \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscimatio.la
noinst_LTLIBRARIES = libscimatio-algo.la

libscimatio_algo_la_SOURCES = $(MATIO_C_SOURCES) $(MATIO_CPP_SOURCES)
libscimatio_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscimatio_algo_la_CPPFLAGS = $(libscimatio_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libscimatio_la_SOURCES)
INCLUDE_FLAGS = $(libscimatio_la_CPPFLAGS)

#### Target ######
modulename=matio

#### matio : Conf files ####
libscimatio_la_rootdir = $(mydatadir)
libscimatio_la_root_DATA =  license.txt

#### matio : init scripts ####
libscimatio_la_etcdir = $(mydatadir)/etc
libscimatio_la_etc_DATA = etc/matio.quit etc/matio.start

#### matio : gateway declaration ####
libscimatio_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscimatio_la_sci_gateway_DATA = sci_gateway/matio_gateway.xml

libscimatio_la_LIBADD = libscimatio-algo.la $(MATIO_LIBS)
