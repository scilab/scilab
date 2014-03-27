# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006-2007 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

STRING_C_SOURCES = src/c/code2str.c \
src/c/stripblanks.c \
src/c/convstr.c \
src/c/men_Sutils.c \
src/c/tokens.c \
src/c/strsplit.c \
src/c/partfunction.c \
src/c/InitializeString.c \
src/c/setScilabCharactersCodes.c \
src/c/cvstr.c \
src/c/getfastcode.c \
src/c/pcre_private.c \
src/c/pcre_error.c \
src/c/isletter.c \
src/c/isdigit.c \
src/c/strsubst.c \
src/c/stristr.c \
src/c/stringsstrrchr.c \
src/c/stringsstrrev.c \
src/c/stringsCompare.c \
src/c/isalphanum.c \
src/c/StringConvert.c \
src/c/scistrtostr.c \
src/c/stringToDouble.c \
src/c/isnum.c \
src/c/stringToComplex.c \
src/c/complex_array.c

STRING_CPP_SOURCES = \
    src/cpp/sort_inter.cpp
    
STRING_FORTRAN_SOURCES =

GATEWAY_C_SOURCES = sci_gateway/c/gw_string.c \
sci_gateway/c/sci_ascii.c \
sci_gateway/c/sci_code2str.c \
sci_gateway/c/sci_convstr.c \
sci_gateway/c/sci_emptystr.c \
sci_gateway/c/sci_grep.c \
sci_gateway/c/sci_length.c \
sci_gateway/c/sci_part.c \
sci_gateway/c/sci_str2code.c \
sci_gateway/c/sci_strcat.c \
sci_gateway/c/sci_strindex.c \
sci_gateway/c/sci_string.c \
sci_gateway/c/sci_stripblanks.c \
sci_gateway/c/sci_strsplit.c \
sci_gateway/c/sci_strsubst.c \
sci_gateway/c/sci_tokens.c \
sci_gateway/c/sci_strcmp.c \
sci_gateway/c/sci_isletter.c \
sci_gateway/c/sci_isdigit.c \
sci_gateway/c/sci_isalphanum.c \
sci_gateway/c/sci_isascii.c \
sci_gateway/c/sci_strchr.c \
sci_gateway/c/sci_strcspn.c \
sci_gateway/c/sci_strncpy.c \
sci_gateway/c/sci_strrev.c \
sci_gateway/c/sci_strspn.c \
sci_gateway/c/sci_strstr.c \
sci_gateway/c/sci_strtod.c \
sci_gateway/c/sci_strtok.c \
sci_gateway/c/sci_regexp.c \
sci_gateway/c/sci_isnum.c



GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_string.f \
sci_gateway/fortran/strops.f

libscistring_la_CPPFLAGS = -I$(srcdir)/src/c \
	-I$(srcdir)/includes/ \
	-I$(srcdir)/src/cpp/ \
	-I$(top_srcdir)/modules/functions/includes/ \
	-I$(top_srcdir)/modules/api_scilab/includes/ \
	-I$(top_srcdir)/modules/localization/includes/ \
        -I$(top_srcdir)/modules/spreadsheet/src/c/ \
	-I$(top_srcdir)/modules/output_stream/includes/ \
	$(PCRE_CFLAGS) \
	$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscistring.la
noinst_LTLIBRARIES = libscistring-algo.la
else
noinst_LTLIBRARIES = libscistring-algo.la libscistring.la
endif


libscistring_la_LDFLAGS = $(AM_LDFLAGS)

libscistring_algo_la_SOURCES = $(STRING_C_SOURCES) $(STRING_CPP_SOURCES) $(STRING_FORTRAN_SOURCES)
libscistring_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libscistring_algo_la_CPPFLAGS = $(libscistring_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(STRING_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscistring_la_CPPFLAGS)

# commented since it is the first module to be processed
libscistring_la_LIBADD = libscistring-algo.la $(PCRE_LIBS)

#### Target ######
modulename=string


#### string : Conf files ####
libscistring_la_rootdir = $(mydatadir)
libscistring_la_root_DATA =  license.txt

#### string : init scripts ####
libscistring_la_etcdir = $(mydatadir)/etc
libscistring_la_etc_DATA = etc/string.quit etc/string.start

#### string : gateway declaration ####
libscistring_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscistring_la_sci_gateway_DATA = sci_gateway/string_gateway.xml

include $(top_srcdir)/Makefile.incl.am

