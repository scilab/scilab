#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
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

# The order DOES matter !!!

# core is the main module of Scilab. All the compilation deps has been removed
# since it already linked almost the whole world

SUBDIRS = \
prebuildjava \
threads \
output_stream \
arnoldi \
elementary_functions \
functions_manager \
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
elementary_functions \
integer \
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
types \
action_binding \
graphic_objects \
renderer \
gui \
graphic_export \
graphics \
optimization \
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
parameters \
matio \
atoms \
hdf5 \
graph \
ast \
scinotes \
parallel \
modules_manager \
ui_data \
history_browser \
preferences \
xml \
external_objects \
slint \
coverage

if GUI
SUBDIRS += external_objects_java
endif

if XCOS
SUBDIRS += scicos_blocks scicos xcos
else
SUBDIRS += xcos
endif

if MPI
SUBDIRS += mpi
endif

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

AM_CPPFLAGS = $(SCI_CPPFLAGS)
AM_CFLAGS   = $(SCI_CFLAGS)
AM_CXXFLAGS = $(SCI_CXXFLAGS)
AM_FFLAGS   = $(SCI_FFLAGS)
AM_LDFLAGS  = $(SCI_LDFLAGS)

# Tag shared libraries with the Scilab version
AM_LDFLAGS += -version-number $(SCILAB_LIBRARY_VERSION)

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
	$(top_builddir)/modules/types/libscitypes-java.la \
	$(top_builddir)/modules/preferences/libscipreferences.la \
	$(top_builddir)/modules/external_objects_java/libsciexternal_objects_java.la \
	$(top_builddir)/modules/helptools/libscihelptools.la \
	$(top_builddir)/modules/tclsci/libscitclsci.la

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
	$(top_builddir)/modules/commons/libscicommons-disable.la \
	$(top_builddir)/modules/preferences/libscipreferences-cli.la \
	$(top_builddir)/modules/tclsci/libscitclsci-disable.la


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
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
	$(top_builddir)/modules/integer/libsciinteger.la \
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
	$(top_builddir)/modules/threads/libscithreads.la \
	$(top_builddir)/modules/ast/libsciast.la \
	$(top_builddir)/modules/functions_manager/libscifunctions_manager.la \
	$(top_builddir)/modules/xml/libscixml.la \
	$(top_builddir)/modules/external_objects/libsciexternal_objects.la \
	$(top_builddir)/modules/slint/libscislint.la \
	$(top_builddir)/modules/coverage/libscicoverage.la

# These libraries are loaded dynamically from Scilab.
ENGINE_LIBS_DYNAMIC_LOAD = \
	$(top_builddir)/modules/optimization/libscioptimization.la \
	$(top_builddir)/modules/signal_processing/libscisignal_processing.la \
	$(top_builddir)/modules/interpolation/libsciinterpolation.la \
	$(top_builddir)/modules/arnoldi/libsciarnoldi.la \
	$(top_builddir)/modules/randlib/libscirandlib.la \
	$(top_builddir)/modules/xcos/libscixcos.la \
	$(top_builddir)/modules/scicos_blocks/libsciscicos_blocks.la \
	$(top_builddir)/modules/scicos/libsciscicos.la \
	$(top_builddir)/modules/sound/libscisound.la \
	$(top_builddir)/modules/spreadsheet/libscispreadsheet.la \
	$(top_builddir)/modules/umfpack/libsciumfpack.la \
	$(top_builddir)/modules/matio/libscimatio.la \
	$(top_builddir)/modules/parallel/libsciparallel.la

if MPI
ENGINE_LIBS += $(top_builddir)/modules/mpi/libscimpi.la
endif

# Core modules

libscilab_la_LIBADD =

if GUI
# Force the libscijvm to be linked first. Under Debian, this avoid some reordering issues during the link
libscilab_la_LIBADD += $(top_builddir)/modules/jvm/libscijvm.la $(GUI_LIBS)
else
libscilab_la_LIBADD += $(NO_GUI_LIBS)
endif

libscilab_la_LIBADD += $(top_builddir)/modules/libscilab-cli.la

# Core modules
libscilab_la_LIBADD += $(OTHER_LIBS) $(EXTERNAL_LIBS)

if FORCE_LINK
# This should never be used in production. It forces the explicit link
# between libscilab and some "on-the-fly" loaded libraries.
# It has been added to make sure that the full link is correctly done
# and all symbols resolved.
libscilab_la_LIBADD += $(ENGINE_LIBS_DYNAMIC_LOAD)
endif

libscilab_la_LIBADD += $(LAPACK_LIBS) $(BLAS_LIBS)

################## libscilab-cli  ##################

libscilab_cli_la_SOURCES =

libscilab_cli_la_LDFLAGS = $(AM_LDFLAGS) $(SCI_LDFLAGS)

# Core modules
libscilab_cli_la_LIBADD = $(ENGINE_LIBS) $(NO_GUI_LIBS) $(OTHER_LIBS) $(EXTERNAL_LIBS) $(LAPACK_LIBS) $(BLAS_LIBS)

.NOTPARALLEL: libscilab-cli.la libscilab.la
