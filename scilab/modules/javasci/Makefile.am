# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

#### Target ######
modulename=javasci

if JAVASCI
### All the compilation process of java is deleguated to ant


#### Javasci : sources ####
JAVASCI_C_SOURCES = src/c/javasci_Scilab2.c \
src/c/javasci_globals.c \
src/c/javasci_SciAbstractArray.c \
src/c/javasci_SciStringArray.c \
src/c/javasci_SciComplexArray.c \
src/c/fix.c \
src/c/javasci_SciDoubleArray.c \
src/c/javasci_Scilab.c \
src/c/javasci_SciBooleanArray.c

pkglib_LTLIBRARIES = libjavasci.la

libjavasci_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libjavasci_la_SOURCES = $(JAVASCI_C_SOURCES)

libjavasci_la_CFLAGS = -Iincludes/ \
			$(JAVA_JNI_INCLUDE) \
			$(TCL_INC_PATH) \
			$(TK_INC_PATH) \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/modules/tclsci/includes \
			-I$(top_srcdir)/modules/output_stream/includes

libjavasci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
	$(top_builddir)/modules/libscilab.la


#### javasci : Conf files ####
libjavasci_la_rootdir = $(mydatadir)
libjavasci_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### javasci : init scripts ####
libjavasci_la_etcdir = $(mydatadir)/etc
libjavasci_la_etc_DATA = etc/javasci.quit etc/javasci.start


#### javasci : include files ####
libjavasci_la_includedir=$(pkgincludedir)/javasci
libjavasci_la_include_HEADERS = includes/javasci_Scilab.h \
includes/javasci_Scilab2.h \
includes/javasci_globals.h \
includes/javasci_SciStringArray.h \
includes/javasci_SciComplexArray.h \
includes/javasci_SciDoubleArray.h \
includes/javasci_SciBooleanArray.h \
includes/javasci_SciAbstractArray.h


### javasci : special tests ###
libsciscipad_la_specialtestdir = $(mydatadir)/tests
libsciscipad_la_specialtest_DATA = tests/Boolean.java \
tests/Boolean.dia.ref \
tests/BooleanArray.java \
tests/BooleanArray.dia.ref \
tests/Clf.java \
tests/Clf.dia.ref \
tests/Disp.java \
tests/Disp.dia.ref \
tests/Exec.java  \
tests/Exec.dia.ref \
tests/Real1.java \
tests/Real1.dia.ref \
tests/Real2.java \
tests/Real2.dia.ref \
tests/Real3.dia.ref \
tests/Real3.java \
tests/String1.java \
tests/String1.dia.ref \
tests/Strings1.dia.ref \
tests/Strings1.java \
tests/Strings2.java \
tests/Strings2.dia.ref \
tests/JavaTests.sce \
tests/Makefile.mak \
tests/Makefile

USEANT=1

include $(top_srcdir)/Makefile.incl.am

endif
