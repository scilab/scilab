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

# The order DOES matter !!!

# core is the main module of Scilab. All the compilation deps has been removed
# since it already linked almost the whole world
SUBDIRS = core

SUBDIRS += \
output_stream \
action_binding \
arnoldi \
elementary_functions \
string \
api_scilab \
dynamic_link \
fileio \
history_manager \
spreadsheet \
tclsci \
localization \
renderer \
shell \
completion \
console \
graphic_export \
gui \
jvm \
graphics \
optimization \
symbolic \
randlib \
statistics \
special_functions \
interpolation \
linear_algebra \
integer \
data_structures \
mexlib \
differential_equations \
cacsd \
polynomials \
boolean \
double \
pvm \
io \
intersci \
signal_processing \
core \
call_scilab \
interpolation \
sound \
time \
randlib \
sparse \
windows_tools \
functions \
overloading \
maple2scilab \
m2sci \
texmacs \
development_tools \
compatibility_functions \
helptools \
fftw \
umfpack \
demo_tools \
genetic_algorithms \
simulated_annealing \
parameters \
matio \
atoms \
hdf5 \
xcos \
xpad

#umfpack

#if MPI
#SUBDIRS += mpi
#endif

if SCICOS
SUBDIRS += scicos_blocks
endif

SUBDIRS += scicos

SUBDIRS += .

# javasci should be compile at the end because it needs libscilab.la
# Don't forget that the order in subdirs does matter !!!
if JAVASCI
SUBDIRS += javasci
endif

#### Basic files ####
libscilab_la_rootdir = $(mydatadir)

# libscilab is the full library with the GUI
# libscilab-cli is for scilab-cli (without GUI)
pkglib_LTLIBRARIES = libscilab.la libscilab-cli.la
libscilab_la_SOURCES =
libscilab_cli_la_SOURCES =

libscilab_la_LDFLAGS = $(LAPACK_LIBS) $(BLAS_LIBS)
libscilab_cli_la_LDFLAGS = $(LAPACK_LIBS) $(BLAS_LIBS)

EXTERNAL_LIBS = $(top_builddir)/libs/hashtable/libscihashtable.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la  \
$(top_builddir)/libs/doublylinkedlist/libscidoublylinkedlist.la  \
$(top_builddir)/libs/libst/libscilibst.la


GUI_LIBS = 	$(top_builddir)/modules/gui/libscigui.la \
	$(top_builddir)/modules/renderer/libscirenderer.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/modules/graphic_export/libscigraphic_export.la \
	$(top_builddir)/modules/console/libsciconsole.la \
	$(top_builddir)/modules/jvm/libscijvm.la \
	$(top_builddir)/modules/xcos/libscixcos.la \
	$(top_builddir)/modules/xpad/libscixpad.la


NO_GUI_LIBS = $(top_builddir)/modules/gui/libscigui-disable.la \
	$(top_builddir)/modules/renderer/libscirenderer-disable.la \
	$(top_builddir)/modules/graphics/libscigraphics-disable.la \
	$(top_builddir)/modules/graphic_export/libscigraphic_export-disable.la \
	$(top_builddir)/modules/console/libsciconsole-disable.la \
	$(top_builddir)/modules/jvm/libscijvm-disable.la \
	$(top_builddir)/modules/xcos/libscixcos-disable.la \
	$(top_builddir)/modules/xpad/libscixpad-disable.la

ENGINE_LIBS =	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/mexlib/libmex.la \
	$(top_builddir)/modules/boolean/libsciboolean.la \
	$(top_builddir)/modules/cacsd/libscicacsd.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/call_scilab/libscicall_scilab.la \
	$(top_builddir)/modules/api_scilab/libsciapi_scilab.la \
	$(top_builddir)/modules/data_structures/libscidata_structures.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/double/libscidouble.la \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/integer/libsciinteger.la \
	$(top_builddir)/modules/intersci/libsciintersci.la \
	$(top_builddir)/modules/io/libsciio.la \
	$(top_builddir)/modules/linear_algebra/libscilinear_algebra.la \
	$(top_builddir)/modules/localization/libscilocalization.la \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/modules/special_functions/libscispecial_functions.la \
	$(top_builddir)/modules/time/libscitime.la \
	$(top_builddir)/modules/windows_tools/libsciwindows_tools.la \
	$(top_builddir)/modules/tclsci/libscitclsci.la \
	$(top_builddir)/modules/pvm/libscipvm.la \
	$(top_builddir)/modules/sparse/libscisparse.la \
	$(top_builddir)/modules/shell/libscishell.la \
	$(top_builddir)/modules/completion/libscicompletion.la \
	$(top_builddir)/modules/history_manager/libscihistory_manager.la \
	$(top_builddir)/modules/action_binding/libsciaction_binding.la \
	$(top_builddir)/modules/output_stream/libscioutput_stream.la \
	$(top_builddir)/modules/string/libscistring.la \
	$(top_builddir)/modules/fftw/libscifftw.la \
	$(top_builddir)/modules/functions/libscifunctions.la \
	$(top_builddir)/modules/hdf5/libscihdf5.la


# These libraries are loaded dynamically from Scilab.
ENGINE_LIBS_DYNAMIC_LOAD = \
	$(top_builddir)/modules/optimization/libscioptimization.la \
	$(top_builddir)/modules/signal_processing/libscisignal_processing.la \
	$(top_builddir)/modules/interpolation/libsciinterpolation.la \
	$(top_builddir)/modules/symbolic/libscisymbolic.la \
	$(top_builddir)/modules/arnoldi/libsciarnoldi.la \
	$(top_builddir)/modules/statistics/libscistatistics.la \
	$(top_builddir)/modules/randlib/libscirandlib.la \
	$(top_builddir)/modules/scicos_blocks/libsciscicos_blocks.la \
	$(top_builddir)/modules/scicos/libsciscicos.la \
	$(top_builddir)/modules/dynamic_link/libscidynamic_link.la \
	$(top_builddir)/modules/sound/libscisound.la \
	$(top_builddir)/modules/spreadsheet/libscispreadsheet.la \
	$(top_builddir)/modules/umfpack/libsciumfpack.la \
	$(top_builddir)/modules/matio/libscimatio.la \
	$(top_builddir)/modules/helptools/libscihelptools.la

# Core modules
libscilab_la_LIBADD = $(ENGINE_LIBS) $(OTHER_LIBS) $(EXTERNAL_LIBS)

if GUI
libscilab_la_LIBADD += $(GUI_LIBS)
else
libscilab_la_LIBADD += $(NO_GUI_LIBS)
endif

# Core modules
libscilab_cli_la_LIBADD = $(ENGINE_LIBS) $(NO_GUI_LIBS) $(OTHER_LIBS) $(EXTERNAL_LIBS)

if ENABLE_STATIC
libscilab_la_LIBADD += $(ALL_LIBS)

endif

#if MPI
#libscilab_la_LIBADD += $(top_builddir)/modules/mpi/libscimpi.la
#endif
