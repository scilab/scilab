#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# 
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

#### Target ######
modulename=javasci

#### javasci : Conf files ####
libjavasci2_la_rootdir = $(mydatadir)
libjavasci2_la_root_DATA =  license.txt 

#### javasci : init scripts ####
libjavasci2_la_etcdir = $(mydatadir)/etc
libjavasci2_la_etc_DATA = \
    etc/javasci.quit \
    etc/javasci.start


if JAVASCI
### All the compilation process of java is deleguated to ant


#### Javasci : sources ####
JAVASCI_V2_C_SOURCES = \
    src/jni/call_scilab_wrap.c \
    src/jni/putLists.c \
    src/c/javasci2_helper.c

pkglib_LTLIBRARIES = libjavasci2.la


libjavasci2_la_SOURCES = $(JAVASCI_V2_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(JAVASCI_C_SOURCES)
INCLUDE_FLAGS = $(libjavasci2_la_CPPFLAGS)

libjavasci2_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    $(JAVA_JNI_INCLUDE) \
    $(TCL_INC_PATH) \
    $(TK_INC_PATH) \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/tclsci/includes \
    -I$(top_srcdir)/modules/graphic_objects/includes \
    -I$(top_srcdir)/modules/call_scilab/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    $(AM_CPPFLAGS)

libjavasci2_la_LIBADD = \
	$(top_builddir)/modules/api_scilab/libsciapi_scilab.la \
	$(top_builddir)/modules/libscilab.la

BUILT_SOURCES=
if SWIG
BUILT_SOURCES+=swig
endif

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/call_scilab.i

USEANT=1

include $(top_srcdir)/Makefile.incl.am

endif

