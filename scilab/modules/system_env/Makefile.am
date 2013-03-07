##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

SYSTEM_ENV_CPP_SOURCES = \
	src/cpp/setenvvar.cpp \
	src/cpp/sci_path.cpp \
	src/cpp/sci_home.cpp \
	src/cpp/home.cpp \
	src/cpp/sci_tmpdir.cpp \
	src/cpp/sci_warning.cpp \
	src/cpp/sci_mode.cpp \
	src/cpp/configvariable.cpp \
	src/cpp/lasterror.cpp \
	src/cpp/promptmode.cpp \
	src/cpp/formatmode.cpp \
	src/cpp/sci_ieee.cpp \
	src/cpp/sci_simpMode.cpp

pkglib_LTLIBRARIES = libscisystem_env.la

libscisystem_env_la_SOURCES = $(SYSTEM_ENV_CPP_SOURCES)

libscisystem_env_la_CPPFLAGS = \
	-I includes/ \
	-I src/cpp \
	-I $(top_srcdir)/libs/MALLOC/includes/ \
    -I $(top_srcdir)/modules/dynamic_link/includes \
	-I $(top_srcdir)/modules/api_scilab/includes/ \
	-I $(top_srcdir)/modules/ast/includes/ \
	-I $(top_srcdir)/modules/symbol/includes/ \
	-I $(top_srcdir)/modules/types/includes/ \
	-I $(top_srcdir)/modules/io/includes/ \
	-I $(top_srcdir)/modules/fileio/includes/ \
	-I $(top_srcdir)/modules/string/includes/ \
	-I $(top_srcdir)/modules/threads/includes/ \
	-I $(top_srcdir)/modules/operations/includes/ \
	-I $(top_srcdir)/modules/elementary_functions/includes/ \
	-I $(top_srcdir)/modules/differential_equations/includes/ \
	-I $(top_srcdir)/modules/output_stream/includes/ \
	-I $(top_srcdir)/modules/localization/includes/ \
	$(AM_CPPFLAGS)


libscisystem_env_la_includedir=$(pkgincludedir)/system_env

libscisystem_env_la_include_HEADERS = includes/setenvvar.hxx \
includes/sci_warning.h
