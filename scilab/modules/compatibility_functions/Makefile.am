# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
# Copyright (C) 2008 - INRIA - Vincent Couvert <vincent.couvert@inria.fr>
# 
# This file is distributed under the same license as the Scilab package.

include $(top_srcdir)/Makefile.incl.am

#### List of the gateway files ####
GATEWAY_C_SOURCES = sci_gateway/c/gw_matio.c \
		    sci_gateway/c/sci_matfile_open.c \
		    sci_gateway/c/sci_matfile_close.c \
		    sci_gateway/c/sci_matfile_listvar.c \
		    sci_gateway/c/sci_matfile_varreadnext.c

#### List of the C files ####
COMPATIBILITY_FUNCTIONS_C_SOURCES = src/c/matfile_manager.c \
				    src/c/CreateMatlabVariable.c \
				    src/c/CreateCharVariable.c \
				    src/c/CreateDoubleVariable.c \
				    src/c/CreateIntegerVariable.c \
				    src/c/CreateHyperMatrixVariable.c \
				    src/c/CreateStructVariable.c \
				    src/c/CreateSparseVariable.c \
				    src/c/CreateCellVariable.c

libscicompatibility_functions_la_CFLAGS = -Iincludes/ \
					-Isrc/c/ \
					-Isrc/cpp/ \
					-Isrc/matio/ \
					-I$(top_srcdir)/modules/output_stream/includes/ \
					-I$(top_srcdir)/modules/string/includes/ \
					$(MATIO_INC_PATH)

libscicompatibility_functions_la_LDFLAGS = $(LD_FLAGS) $(X_LIBS) $(X_EXTRA_LIBS)  $(MATIO_LIB)


pkglib_LTLIBRARIES = libscicompatibility_functions.la

libscicompatibility_functions_la_SOURCES = $(COMPATIBILITY_FUNCTIONS_C_SOURCES) $(GATEWAY_C_SOURCES)

#### Target ######
modulename=compatibility_functions

#### compatibility_functions : Conf files ####
libscicompatibility_functions_la_rootdir = $(mydatadir)
libscicompatibility_functions_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### compatibility_functions : init scripts #### 
libscicompatibility_functions_la_etcdir = $(mydatadir)/etc
libscicompatibility_functions_la_etc_DATA = etc/compatibility_functions.quit etc/compatibility_functions.start

