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


SUBDIRS = \
prebuildjava \
fileio \
mexlib \
functions \
parameters \
boolean \
cacsd \
signal_processing \
core \
call_scilab \
api_scilab \
data_structures \
differential_equations \
double \
elementary_functions \
integer \
intersci \
io \
linear_algebra \
localization \
polynomials \
special_functions \
time \
windows_tools \
sparse \
completion \
history_manager \
output_stream \
string \
hdf5 \
fftw \
statistics \
dynamic_link \
console \
arnoldi \
spreadsheet \
tclsci \
commons \
jvm \
action_binding \
graphic_objects \
renderer \
graphic_export \
gui \
graphics \
optimization \
symbolic \
randlib \
interpolation \
sound \
overloading \
m2sci \
development_tools \
compatibility_functions \
helptools \
umfpack \
demo_tools \
genetic_algorithms \
simulated_annealing \
matio \
atoms \
hdf5 \
graph\
xcos \
scinotes \
parallel \
modules_manager \
ui_data \
history_browser \
preferences \
xml \
types \
external_objects \
external_objects_java


if XCOS
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
pkglib_LTLIBRARIES = libscilab-cli.la libscilab.la
libscilab_la_SOURCES =

libscilab_la_LDFLAGS = $(LAPACK_LIBS) $(BLAS_LIBS) $(AM_LDFLAGS)


GUI_LIBS = $(top_builddir)/modules/gui/libscigui.la \
	$(top_builddir)/modules/renderer/libscirenderer.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/modules/graphic_export/libscigraphic_export.la \
	$(top_builddir)/modules/console/libsciconsole.la \
	$(top_builddir)/modules/action_binding/libsciaction_binding.la \
	$(top_builddir)/modules/jvm/libscijvm.la \
	$(top_builddir)/modules/graphic_objects/libscigraphic_objects.la \
	$(top_builddir)/modules/scinotes/libsciscinotes.la \
	$(top_builddir)/modules/ui_data/libsciui_data.la \
	$(top_builddir)/modules/history_browser/libscihistory_browser.la \
	$(top_builddir)/modules/commons/libscicommons.la \
	$(top_builddir)/modules/tclsci/libscitclsci.la \
	$(top_builddir)/modules/preferences/libscipreferences.la \
	$(top_builddir)/modules/external_objects_java/libsciexternal_objects_java.la 

NO_GUI_LIBS = $(top_builddir)/modules/gui/libscigui-disable.la \
	$(top_builddir)/modules/graphics/libscigraphics-disable.la \
	$(top_builddir)/modules/graphic_export/libscigraphic_export-disable.la \
	$(top_builddir)/modules/console/libsciconsole-minimal.la \
	$(top_builddir)/modules/action_binding/libsciaction_binding-disable.la \
	$(top_builddir)/modules/jvm/libscijvm-disable.la \
	$(top_builddir)/modules/xcos/libscixcos-disable.la \
	$(top_builddir)/modules/graphic_objects/libscigraphic_objects-disable.la \
	$(top_builddir)/modules/scinotes/libsciscinotes-disable.la \
	$(top_builddir)/modules/ui_data/libsciui_data-disable.la \
	$(top_builddir)/modules/history_browser/libscihistory_browser-disable.la \
	$(top_builddir)/modules/commons/libscicommons-disable.la


ENGINE_LIBS =	$(top_builddir)/modules/fileio/libscifileio.la \
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
	$(top_builddir)/modules/xml/libscixml.la \
	$(top_builddir)/modules/external_objects/libsciexternal_objects.la \
	$(top_builddir)/modules/preferences/libscipreferences-cli.la

# These libraries are loaded dynamically from Scilab.
ENGINE_LIBS_DYNAMIC_LOAD = \
	$(top_builddir)/modules/optimization/libscioptimization.la \
	$(top_builddir)/modules/signal_processing/libscisignal_processing.la \
	$(top_builddir)/modules/interpolation/libsciinterpolation.la \
	$(top_builddir)/modules/symbolic/libscisymbolic.la \
	$(top_builddir)/modules/arnoldi/libsciarnoldi.la \
	$(top_builddir)/modules/randlib/libscirandlib.la \
	$(top_builddir)/modules/xcos/libscixcos.la \
	$(top_builddir)/modules/scicos_blocks/libsciscicos_blocks.la \
	$(top_builddir)/modules/scicos/libsciscicos.la \
	$(top_builddir)/modules/sound/libscisound.la \
	$(top_builddir)/modules/spreadsheet/libscispreadsheet.la \
	$(top_builddir)/modules/umfpack/libsciumfpack.la \
	$(top_builddir)/modules/matio/libscimatio.la \
	$(top_builddir)/modules/helptools/libscihelptools.la \
	$(top_builddir)/modules/parallel/libsciparallel.la


# Core modules

libscilab_la_LIBADD =

if GUI
libscilab_la_LIBADD += $(GUI_LIBS)	$(top_builddir)/modules/types/libscitypes.la
else
libscilab_la_LIBADD += $(NO_GUI_LIBS)
endif

# Core modules
libscilab_la_LIBADD += $(OTHER_LIBS) $(EXTERNAL_LIBS)

if FORCE_LINK
# This should never be used in production. It forces the explicit link
# between libscilab and some "on-the-fly" loaded libraries.
# It has been added to make sure that the full link is correctly done
# and all symbols resolved.
libscilab_la_LIBADD += $(ENGINE_LIBS_DYNAMIC_LOAD)
endif

libscilab_la_LIBADD += $(top_builddir)/modules/libscilab-cli.la

################## libscilab-cli  ##################

libscilab_cli_la_SOURCES =

libscilab_cli_la_LDFLAGS = $(LAPACK_LIBS) $(BLAS_LIBS) $(AM_LDFLAGS)

# Core modules
libscilab_cli_la_LIBADD = $(ENGINE_LIBS) $(NO_GUI_LIBS) $(OTHER_LIBS) $(EXTERNAL_LIBS)

.NOTPARALLEL: libscilab-cli.la libscilab.la
