#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=javasci

#### javasci : Conf files ####
libjavasci_la_rootdir = $(mydatadir)
libjavasci_la_root_DATA =  license.txt version.xml

#### javasci : init scripts ####
libjavasci_la_etcdir = $(mydatadir)/etc
libjavasci_la_etc_DATA = etc/javasci.quit etc/javasci.start


if JAVASCI
### All the compilation process of java is deleguated to ant


#### Javasci : sources ####
JAVASCI_V1_C_SOURCES = src/c/javasci_Scilab2.c \
src/c/javasci_globals.c \
src/c/javasci_SciAbstractArray.c \
src/c/javasci_SciStringArray.c \
src/c/javasci_SciComplexArray.c \
src/c/fix.c \
src/c/javasci_SciDoubleArray.c \
src/c/javasci_Scilab.c \
src/c/javasci_SciBooleanArray.c \
src/c/javasci_SciIntegerArray.c

JAVASCI_V2_C_SOURCES = \
src/jni/call_scilab_wrap.c \
src/c/javasci2_helper.c

pkglib_LTLIBRARIES = libjavasci.la libjavasci2.la

libjavasci_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libjavasci_la_SOURCES = $(JAVASCI_V1_C_SOURCES)

libjavasci2_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libjavasci2_la_SOURCES = $(JAVASCI_V2_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(JAVASCI_C_SOURCES)
INCLUDE_FLAGS = $(libjavasci_la_CFLAGS)

libjavasci_la_CFLAGS = -I$(srcdir)/includes/ \
			$(JAVA_JNI_INCLUDE) \
			$(TCL_INC_PATH) \
			$(TK_INC_PATH) \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/modules/tclsci/includes \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/output_stream/includes

libjavasci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
	$(top_builddir)/modules/api_scilab/libsciapi_scilab.la \
	$(top_builddir)/modules/libscilab.la

libjavasci2_la_CFLAGS = -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			$(JAVA_JNI_INCLUDE) \
			$(TCL_INC_PATH) \
			$(TK_INC_PATH) \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/modules/tclsci/includes \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/output_stream/includes

libjavasci2_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
	$(top_builddir)/modules/api_scilab/libsciapi_scilab.la \
	$(top_builddir)/modules/libscilab.la

### javasci :  images ###
libsciscipad_la_imagesdir = $(mydatadir)/help/images/
libsciscipad_la_images_DATA = help/images/ScilabClass.gif \
help/images/SciAbstractDataTypeClass.gif \
help/images/SciDoubleClass.gif \
help/images/SciBooleanArrayClass.gif \
help/images/SciComplexClass.gif \
help/images/SciDoubleArrayClass.gif \
help/images/SciComplexArrayClass.gif \
help/images/SciStringArrayClass.gif \
help/images/SciAbstractArrayClass.gif \
help/images/SciStringClass.gif \
help/images/SciBooleanClass.gif

BUILT_SOURCES=
if SWIG
BUILT_SOURCES+=swig
endif

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/call_scilab.i

USEANT=1

include $(top_srcdir)/Makefile.incl.am

endif
