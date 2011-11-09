# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Allan CORNET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

if HDF5

if GUI
if XCOS
# No GUI. Disable the Java build
USEANT=1
endif
endif



if NEED_JAVA

# This is due to an ugly hack for bug #5481
FORCE_LOAD_SOURCES = src/cpp/forceJHDF5load.cpp \
src/jni/jhdf5load.hxx \
src/jni/jhdf5load.cpp
else

# This is due to an ugly hack for bug #5481
FORCE_LOAD_SOURCES =
endif

GATEWAY_C_SOURCES = sci_gateway/c/gw_hdf5.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_export_to_hdf5.cpp \
    sci_gateway/cpp/sci_import_from_hdf5.cpp


HDF5_CPP_SOURCES = \
    src/cpp/h5_fileManagement.cpp \
    src/cpp/h5_writeDataToFile.cpp \
    src/cpp/dynhdf5.cpp \
    src/cpp/h5_readDataFromFile.cpp

FORCE_LOAD_SOURCES_DISABLE = src/cpp/forceJHDF5load-jvmdisable.cpp

libscihdf5_forceload_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS) $(HDF5_LIBS)
libscihdf5_forceload_la_SOURCES = $(FORCE_LOAD_SOURCES)
libscihdf5_forceload_la_CPPFLAGS=$(libscihdf5_la_CPPFLAGS)
libscihdf5_forceload_la_CFLAGS=$(libscihdf5_la_CFLAGS)

libscihdf5_forceload_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS) $(HDF5_LIBS)
libscihdf5_forceload_disable_la_SOURCES = $(FORCE_LOAD_SOURCES_DISABLE)
libscihdf5_forceload_disable_la_CPPFLAGS=$(libscihdf5_la_CPPFLAGS)
libscihdf5_forceload_disable_la_CFLAGS=$(libscihdf5_la_CFLAGS)

else

GATEWAY_C_SOURCES =
GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_no_hdf5.cpp

endif

GIWS_WRAPPERS = src/jni/jhdf5load.giws.xml

if GIWS
BUILT_SOURCES=giws
endif


#### hdf5 : include files ####

libscihdf5_la_CPPFLAGS=	-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/operations/includes \
                -I$(top_srcdir)/modules/jvm/includes/ \
                -I$(top_srcdir)/modules/commons/src/jni/ \
				-I$(top_srcdir)/modules/fileio/includes \
				-I$(top_srcdir)/modules/system_env/includes \
				-I$(top_srcdir)/modules/string/includes \
				-I$(top_srcdir)/modules/types/includes \
				-I$(top_srcdir)/modules/functions_manager/includes \
				-I$(top_srcdir)/modules/symbol/includes \
				-I$(top_srcdir)/modules/ast/includes \
                -I$(top_srcdir)/libs/dynamiclibrary/includes \
				$(JAVA_JNI_INCLUDE) \
				$(HDF5_CFLAGS)

libscihdf5_la_CFLAGS=		-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/localization/includes \
				-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/system_env/includes \
				-I$(top_srcdir)/modules/fileio/includes \
				-I$(top_srcdir)/modules/string/includes \
				$(HDF5_CFLAGS)


#### hdf5 : gateway declaration ####
libscihdf5_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihdf5_la_sci_gateway_DATA = sci_gateway/hdf5_gateway.xml


pkglib_LTLIBRARIES = libscihdf5.la
if HDF5
noinst_LTLIBRARIES = libscihdf5-algo.la
endif

if HDF5
# When Scilab is built with HDF5, we need some libraries for a workaround

if NEED_JAVA
pkglib_LTLIBRARIES += libscihdf5-forceload.la
endif

pkglib_LTLIBRARIES += libscihdf5-forceload-disable.la
endif

libscihdf5_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(HDF5_LIBS)

libscihdf5_algo_la_SOURCES = $(HDF5_CPP_SOURCES)
libscihdf5_la_SOURCES = $(GATEWAY_CPP_SOURCES) $(GATEWAY_C_SOURCES)
libscihdf5_algo_la_CFLAGS = $(libscihdf5_la_CFLAGS)
libscihdf5_algo_la_CPPFLAGS = $(libscihdf5_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(HDF5_CPP_SOURCES) $(GATEWAY_CPP_SOURCES)
INCLUDE_FLAGS = $(libscihdf5_la_CFLAGS)

# pb of order
if HDF5
libscihdf5_la_LIBADD = libscihdf5-algo.la \
				$(top_builddir)/libs/MALLOC/libscimalloc.la \
				$(top_builddir)/modules/output_stream/libscioutput_stream.la
else
libscihdf5_la_LIBADD =
endif

#### Target ######
modulename=hdf5


#### hdf5 : Conf files ####
libscihdf5_la_rootdir = $(mydatadir)
libscihdf5_la_root_DATA =  license.txt


#### hdf5 : init scripts ####
libscihdf5_la_etcdir = $(mydatadir)/etc
libscihdf5_la_etc_DATA = etc/hdf5.quit etc/hdf5.start

include $(top_srcdir)/Makefile.incl.am
