# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006-2007 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

STRING_CPP_SOURCES = \
    src/cpp/stripblanks.cpp \
    src/cpp/sort_inter.cpp \
    src/cpp/os_wtoi.cpp

STRING_C_SOURCES = \
    src/c/convstr.c \
    src/c/men_Sutils.c \
    src/c/tokens.c \
    src/c/strsplit.c \
    src/c/partfunction.c \
    src/c/cvstr.c \
    src/c/getfastcode.c \
    src/c/pcre_private.c \
    src/c/pcre_error.c \
    src/c/isletter.c \
    src/c/isdigit.c \
    src/c/strsubst.c \
    src/c/stristr.c \
    src/c/stringsstrrev.c \
    src/c/stringsCompare.c \
    src/c/isalphanum.c \
    src/c/StringConvert.c \
    src/c/os_wcsdup.c \
    src/c/os_wcstok.c \
    src/c/scistrtostr.c \
    src/c/stringToDouble.c \
    src/c/stringToComplex.c \
    src/c/complex_array.c \
    src/c/isascii.c \
    src/c/isnum.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_stripblanks.cpp \
    sci_gateway/cpp/sci_grep.cpp \
    sci_gateway/cpp/string_gw.cpp \
    sci_gateway/cpp/sci_regexp.cpp \
    sci_gateway/cpp/sci_part.cpp \
    sci_gateway/cpp/sci_length.cpp \
    sci_gateway/cpp/sci_strindex.cpp \
    sci_gateway/cpp/sci_strsubst.cpp \
    sci_gateway/cpp/sci_ascii.cpp \
    sci_gateway/cpp/sci_strcat.cpp \
    sci_gateway/cpp/sci_string.cpp \
    sci_gateway/cpp/sci_convstr.cpp \
    sci_gateway/cpp/sci_strncpy.cpp \
    sci_gateway/cpp/sci_strrchr.cpp \
    sci_gateway/cpp/sci_strchr.cpp \
    sci_gateway/cpp/sci_strstr.cpp \
    sci_gateway/cpp/sci_strrev.cpp \
    sci_gateway/cpp/sci_strtod.cpp \
    sci_gateway/cpp/sci_tokens.cpp \
    sci_gateway/cpp/sci_strcmp.cpp \
    sci_gateway/cpp/sci_isletter.cpp \
    sci_gateway/cpp/sci_strcspn.cpp \
    sci_gateway/cpp/sci_strtok.cpp \
    sci_gateway/cpp/sci_strspn.cpp \
    sci_gateway/cpp/sci_isalphanum.cpp \
    sci_gateway/cpp/sci_isdigit.cpp \
    sci_gateway/cpp/sci_isascii.cpp \
    sci_gateway/cpp/sci_strsplit.cpp \
    sci_gateway/cpp/sci_isnum.cpp

libscistring_la_CFLAGS= \
    $(PCRE_CFLAGS) \
	-I$(srcdir)/src/c \
	-I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
	-I$(top_srcdir)/modules/functions/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/output_stream/includes

libscistring_la_CPPFLAGS= \
    $(PCRE_CFLAGS) \
	-I$(srcdir)/src/c \
	-I$(srcdir)/src/cpp \
	-I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/functions_manager/includes \
	-I$(top_srcdir)/modules/core/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/console/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes

libscistring_algo_la_CPPFLAGS = $(libscistring_la_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscistring.la
noinst_LTLIBRARIES = libscistring-algo.la
else
noinst_LTLIBRARIES = libscistring-algo.la libscistring.la
endif


libscistring_la_LDFLAGS = $(AM_LDFLAGS)

libscistring_algo_la_SOURCES = $(STRING_C_SOURCES) $(STRING_CPP_SOURCES)
libscistring_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscistring_algo_la_CFLAGS = $(libscistring_la_CFLAGS)

# For the code check (splint)
CHECK_SRC= $(STRING_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscistring_la_CFLAGS)

# commented since it is the first module to be processed
libscistring_la_LIBADD = libscistring-algo.la $(PCRE_LIBS)

#### Target ######
modulename=string


#### string : Conf files ####
libscistring_la_rootdir = $(mydatadir)
libscistring_la_root_DATA =  license.txt

####�string : init scripts ####
libscistring_la_etcdir = $(mydatadir)/etc
libscistring_la_etc_DATA = \
    etc/string.quit \
    etc/string.start

####�string : gateway declaration ####
libscistring_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscistring_la_sci_gateway_DATA = sci_gateway/string_gateway.xml

#### string : include files ####
libscistring_la_includedir=$(pkgincludedir)
libscistring_la_include_HEADERS = \
includes/complex_array.h \
includes/convstr.h \
includes/cvstr.h \
includes/dynlib_string_gw.h \
includes/dynlib_string.h \
includes/getfastcode.h \
includes/men_Sutils.h \
includes/os_string.h \
includes/os_wcstok.h \
includes/pcre_private.h \
includes/scistrtostr.h \
includes/StringConvert.h \
includes/string_gw.hxx \
includes/stringToComplex.h \
includes/stringToDouble.h \
includes/stristr.h \
includes/strsubst.h



include $(top_srcdir)/Makefile.incl.am

