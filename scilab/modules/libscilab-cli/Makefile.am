#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#
#### Basic files ####
libscilab_la_rootdir = $(mydatadir)

# libscilab is the full library with the GUI
# libscilab-cli is for scilab-cli (without GUI)
pkglib_LTLIBRARIES = libscilab-cli.la
#libscilab_la_SOURCES =
libscilab_cli_la_SOURCES =

#libscilab_la_LDFLAGS = $(LAPACK_LIBS) $(BLAS_LIBS)
libscilab_cli_la_LDFLAGS = $(LAPACK_LIBS) $(BLAS_LIBS) -lstdc++

EXTERNAL_LIBS = $(top_builddir)/libs/hashtable/libscihashtable.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la  \
$(top_builddir)/libs/doublylinkedlist/libscidoublylinkedlist.la  \
$(top_builddir)/libs/libst/libscilibst.la

ENGINE_LIBS = \
	$(top_builddir)/modules/types/libscitypes.la \
	$(top_builddir)/modules/ast/libsciast.la \
	$(top_builddir)/modules/symbol/libscisymbol.la \
	$(top_builddir)/modules/parse/libsciparse.la \
	$(top_builddir)/modules/system_env/libscisystem_env.la \
	$(top_builddir)/modules/threads/libscithreads.la \
	$(top_builddir)/modules/operations/libscioperations.la \
	$(top_builddir)/modules/functions_manager/libscifunctions_manager.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/mexlib/libmex.la \
	$(top_builddir)/modules/functions/libscifunctions.la \
	$(top_builddir)/modules/parameters/libsciparameters.la \
	$(top_builddir)/modules/boolean/libsciboolean.la \
	$(top_builddir)/modules/cacsd/libscicacsd.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/call_scilab/libscicall_scilab.la \
	$(top_builddir)/modules/api_scilab/libsciapi_scilab.la \
	$(top_builddir)/modules/data_structures/libscidata_structures.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/double/libscidouble.la \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
	$(top_builddir)/modules/integer/libsciinteger.la \
	$(top_builddir)/modules/intersci/libsciintersci.la \
	$(top_builddir)/modules/io/libsciio.la \
	$(top_builddir)/modules/linear_algebra/libscilinear_algebra.la \
	$(top_builddir)/modules/localization/libscilocalization.la \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/modules/special_functions/libscispecial_functions.la \
	$(top_builddir)/modules/time/libscitime.la \
	$(top_builddir)/modules/windows_tools/libsciwindows_tools.la \
	$(top_builddir)/modules/sparse/libscisparse.la \
	$(top_builddir)/modules/completion/libscicompletion.la \
	$(top_builddir)/modules/history_manager/libscihistory_manager.la \
	$(top_builddir)/modules/output_stream/libscioutput_stream.la \
	$(top_builddir)/modules/string/libscistring.la \
	$(top_builddir)/modules/hdf5/libscihdf5.la \
	$(top_builddir)/modules/fftw/libscifftw.la \
    $(top_builddir)/modules/statistics/libscistatistics.la \
    $(top_builddir)/modules/dynamic_link/libscidynamic_link.la \
	$(top_builddir)/libs/dynamiclibrary/libscidynamiclibrary.la

NO_GUI_LIBS = $(top_builddir)/modules/gui/libscigui-disable.la \
	$(top_builddir)/modules/graphics/libscigraphics-disable.la \
	$(top_builddir)/modules/graphic_export/libscigraphic_export-disable.la \
	$(top_builddir)/modules/console/libsciconsole-disable.la \
	$(top_builddir)/modules/action_binding/libsciaction_binding-disable.la \
	$(top_builddir)/modules/jvm/libscijvm-disable.la \
	$(top_builddir)/modules/xcos/libscixcos-disable.la \
	$(top_builddir)/modules/scinotes/libsciscinotes-disable.la \
	$(top_builddir)/modules/ui_data/libsciui_data-disable.la \
	$(top_builddir)/modules/history_browser/libscihistory_browser-disable.la \
	$(top_builddir)/modules/commons/libscicommons-disable.la

if HDF5
NO_GUI_LIBS += $(top_builddir)/modules/hdf5/libscihdf5-forceload-disable.la
# HDF5 is here because it needs the JVM (see bug #5481)
endif


# Core modules
libscilab_cli_la_LIBADD = $(ENGINE_LIBS) $(NO_GUI_LIBS) $(OTHER_LIBS) $(EXTERNAL_LIBS)
