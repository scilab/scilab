##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

AM_CXXFLAGS = -I . -I $(top_srcdir)/src

YFLAGS = --no-lines -dvy -ra
LFLAGS = --noline --nounistd -o $(FLEX_SCANSCILAB_CPP)

FLEX_SCANSCILAB_LL	= src/flex/scanscilab.ll
FLEX_SCANSCILAB_CPP	= src/cpp/scanscilab.cpp

BISON_PARSESCILAB_YY	= src/bison/parsescilab.yy
BISON_PARSESCILAB_CPP	= src/cpp/parsescilab.cpp
BISON_PARSESCILAB_HXX	= includes/parsescilab.hxx
BISON_PARSESCILAB_OUTPUT= src/bison/parsescilab.output

PARSE_CPP_SOURCES 	= $(BISON_PARSESCILAB_CPP) \
			$(FLEX_SCANSCILAB_CPP) \
			src/cpp/parser.cpp \
			src/cpp/printerror.cpp

CLEANFILES = 	$(BISON_PARSESCILAB_CPP) \
		$(BISON_PARSESCILAB_HXX) \
		$(BISON_PARSESCILAB_OUTPUT) \
		$(FLEX_SCANSCILAB_CPP)

$(FLEX_SCANSCILAB_CPP) : $(FLEX_SCANSCILAB_LL)
	$(LEX) $(AM_FLAGS) $(LFLAGS) $(FLEX_SCANSCILAB_LL)

$(BISON_PARSESCILAB_CPP) : $(BISON_PARSESCILAB_YY)
	$(YACC) $(AM_FLAGS) $(YFLAGS) $(BISON_PARSESCILAB_YY) \
	&& mv y.tab.c $(BISON_PARSESCILAB_CPP) \
	&& mv y.tab.h $(BISON_PARSESCILAB_HXX) \
	&& mv y.output $(BISON_PARSESCILAB_OUTPUT)

pkglib_LTLIBRARIES = libsciParse.la

libsciParse_la_SOURCES 	= $(PARSE_CPP_SOURCES)

libsciParse_la_CXXFLAGS = -I$(top_builddir)/modules/core/includes \
			-I$(top_builddir)/modules/abstractSyntaxTree/includes \
			-I$(top_builddir)/modules/symbol/includes \
			-Iincludes \
			-Isrc/cpp