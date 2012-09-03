# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Allan CORNET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

if GUI
if XCOS
# No GUI. Disable the Java build
USEANT=1
endif
endif


HDF5_SOURCES = src/c/h5_fileManagement.c \
src/c/h5_writeDataToFile.c \
src/c/h5_readDataFromFile.c \
src/c/h5_readDataFromFile_v1.c \
src/cpp/import_from_hdf5_v1.cpp \
src/cpp/listvar_in_hdf5_v1.cpp



GATEWAY_C_SOURCES = sci_gateway/c/gw_hdf5.c

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_export_to_hdf5.cpp \
sci_gateway/cpp/sci_import_from_hdf5.cpp \
sci_gateway/cpp/sci_is_hdf5_file.cpp \
sci_gateway/cpp/sci_listvar_in_hdf5.cpp

if GIWS
BUILT_SOURCES=giws
endif


#### hdf5 : include files ####

libscihdf5_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
                -I$(top_srcdir)/modules/jvm/includes/ \
                -I$(top_srcdir)/modules/commons/src/jni/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/fileio/includes/ \
				$(JAVA_JNI_INCLUDE) \
				$(HDF5_CFLAGS) \
				$(AM_CPPFLAGS)


#### hdf5 : gateway declaration ####
libscihdf5_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihdf5_la_sci_gateway_DATA = sci_gateway/hdf5_gateway.xml

pkglib_LTLIBRARIES = libscihdf5.la
noinst_LTLIBRARIES =

noinst_LTLIBRARIES += libscihdf5-algo.la

# When Scilab is built with HDF5, we need some libraries for a workaround

libscihdf5_la_LDFLAGS = $(HDF5_LIBS) $(AM_LDFLAGS)

libscihdf5_algo_la_SOURCES = $(HDF5_SOURCES)
libscihdf5_la_SOURCES = $(GATEWAY_CXX_SOURCES) $(GATEWAY_C_SOURCES)
libscihdf5_algo_la_CPPFLAGS = $(libscihdf5_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(HDF5_SOURCES) $(GATEWAY_CXX_SOURCES)
INCLUDE_FLAGS = $(libscihdf5_la_CPPFLAGS)

libscihdf5_la_LIBADD = libscihdf5-algo.la

#### Target ######
modulename=hdf5


#### hdf5 : Conf files ####
libscihdf5_la_rootdir = $(mydatadir)
libscihdf5_la_root_DATA =  license.txt


#### hdf5 : init scripts ####
libscihdf5_la_etcdir = $(mydatadir)/etc
libscihdf5_la_etc_DATA = etc/hdf5.quit etc/hdf5.start

#### hdf5 : test file ####
libscihdf5_la_testsampledatadir = $(mydatadir)/tests/sample_scilab_data
libscihdf5_la_testsampledata_DATA = \
tests/sample_scilab_data/booleanmatrix.sod \
tests/sample_scilab_data/booleanmatrix_v1.sod \
tests/sample_scilab_data/booleanscalar.sod \
tests/sample_scilab_data/booleanscalar_v1.sod \
tests/sample_scilab_data/booleansparse.sod \
tests/sample_scilab_data/booleansparse_v1.sod \
tests/sample_scilab_data/emptymatrix.sod \
tests/sample_scilab_data/emptysparse.sod \
tests/sample_scilab_data/emptysparse_v1.sod \
tests/sample_scilab_data/hypermatrixcomplex.sod \
tests/sample_scilab_data/hypermatrixcomplex_v1.sod \
tests/sample_scilab_data/hypermatrix.sod \
tests/sample_scilab_data/hypermatrix_v1.sod \
tests/sample_scilab_data/int16.sod \
tests/sample_scilab_data/int16_v1.sod \
tests/sample_scilab_data/int32.sod \
tests/sample_scilab_data/int32_v1.sod \
tests/sample_scilab_data/int8.sod \
tests/sample_scilab_data/int8_v1.sod \
tests/sample_scilab_data/list.sod \
tests/sample_scilab_data/list_v1.sod \
tests/sample_scilab_data/listnested.sod \
tests/sample_scilab_data/listnested_v1.sod \
tests/sample_scilab_data/matricedoublecomplex.sod \
tests/sample_scilab_data/matricedoublecomplex_v1.sod \
tests/sample_scilab_data/matricedoublecomplexscalar.sod \
tests/sample_scilab_data/matricedoublecomplexscalar_v1.sod \
tests/sample_scilab_data/matricedouble.sod \
tests/sample_scilab_data/matricedouble_v1.sod \
tests/sample_scilab_data/matricedoublescalar.sod \
tests/sample_scilab_data/matricedoublescalar_v1.sod \
tests/sample_scilab_data/matricestring.sod \
tests/sample_scilab_data/matricestring_v1.sod \
tests/sample_scilab_data/matricestringscalar.sod \
tests/sample_scilab_data/matricestringscalar_v1.sod \
tests/sample_scilab_data/mlist.sod \
tests/sample_scilab_data/mlist_v1.sod \
tests/sample_scilab_data/polynomialscoef.sod \
tests/sample_scilab_data/polynomialscoef_v1.sod \
tests/sample_scilab_data/polynomials.sod \
tests/sample_scilab_data/polynomials_v1.sod \
tests/sample_scilab_data/sparsematrix.sod \
tests/sample_scilab_data/sparsematrix_v1.sod \
tests/sample_scilab_data/tlist.sod \
tests/sample_scilab_data/tlist_v1.sod \
tests/sample_scilab_data/uint16.sod \
tests/sample_scilab_data/uint16_v1.sod \
tests/sample_scilab_data/uint32.sod \
tests/sample_scilab_data/uint32_v1.sod \
tests/sample_scilab_data/uint8.sod \
tests/sample_scilab_data/uint8_v1.sod \
tests/sample_scilab_data/undefinedelement.sod \
tests/sample_scilab_data/undefinedelement_v1.sod \
tests/sample_scilab_data/voidelement.sod \
tests/sample_scilab_data/voidelement_v1.sod

include $(top_srcdir)/Makefile.incl.am
