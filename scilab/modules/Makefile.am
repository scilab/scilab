##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
##########

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
dynamic_link \
history_manager \
fileio \
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
metanet \
signal_processing \
core \
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
scipad \
scilab2fortran \
development_tools \
compatibility_functions \
helptools \
fftw \
umfpack \
demo_tools \
genetic_algorithms \
simulated_annealing \
parameters \
matio

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

#
pkglib_LTLIBRARIES = libscilab.la
libscilab_la_SOURCES =

libscilab_la_LDFLAGS =

if ! USE_EMBEDDED_LAPACK
libscilab_la_LDFLAGS += $(LAPACK_LIBS)
endif

if ! USE_EMBEDDED_BLAS
libscilab_la_LDFLAGS += $(BLAS_LIBS)
endif

ALL_LIBS=	$(top_builddir)/modules/fileio/libscifileio.la \
$(top_builddir)/modules/mexlib/libmex.la \
	$(top_builddir)/modules/boolean/libsciboolean.la \
	$(top_builddir)/modules/cacsd/libscicacsd.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/data_structures/libscidata_structures.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/double/libscidouble.la \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/modules/graphic_export/libscigraphic_export.la \
	$(top_builddir)/modules/renderer/libscirenderer.la \
	$(top_builddir)/modules/gui/libscigui.la \
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
	$(top_builddir)/modules/console/libsciconsole.la \
	$(top_builddir)/modules/completion/libscicompletion.la \
	$(top_builddir)/modules/history_manager/libscihistory_manager.la \
	$(top_builddir)/modules/action_binding/libsciaction_binding.la \
	$(top_builddir)/modules/dynamic_link/libscidynamic_link.la \
	$(top_builddir)/modules/output_stream/libscioutput_stream.la \
	$(top_builddir)/modules/sound/libscisound.la \
	$(top_builddir)/modules/helptools/libscihelptools.la \
	$(top_builddir)/modules/matio/libscimatio.la \
	$(top_builddir)/modules/randlib/libscirandlib.la \
	$(top_builddir)/modules/optimization/libscioptimization.la \
	$(top_builddir)/modules/arnoldi/libsciarnoldi.la \
	$(top_builddir)/modules/symbolic/libscisymbolic.la \
	$(top_builddir)/modules/metanet/libscimetanet.la \
	$(top_builddir)/modules/spreadsheet/libscispreadsheet.la \
	$(top_builddir)/modules/statistics/libscistatistics.la \
	$(top_builddir)/modules/signal_processing/libscisignal_processing.la \
	$(top_builddir)/modules/interpolation/libsciinterpolation.la

# Dev in progress

# Core modules
libscilab_la_LIBADD = $(ALL_LIBS)

if ENABLE_STATIC
libscilab_la_LIBADD += $(ALL_LIBS)

endif
# Temp stuff
libscilab_la_LIBADD +=	$(top_builddir)/modules/string/libscistring.la

#if MPI
#libscilab_la_LIBADD += $(top_builddir)/modules/mpi/libscimpi.la
#endif

libscilab_la_LIBADD += $(top_builddir)/modules/fftw/libscifftw.la

libscilab_la_LIBADD += $(top_builddir)/modules/umfpack/libsciumfpack.la

libscilab_la_LIBADD += $(top_builddir)/modules/jvm/libscijvm.la

libscilab_la_LIBADD += $(top_builddir)/modules/scicos/libsciscicos.la
if SCICOS
libscilab_la_LIBADD += $(top_builddir)/modules/scicos_blocks/libsciscicos_blocks.la
endif

## Libs
libscilab_la_LIBADD += \
$(top_builddir)/libs/hashtable/libscihashtable.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la  \
$(top_builddir)/libs/doublylinkedlist/libscidoublylinkedlist.la  \
$(top_builddir)/libs/libst/libscilibst.la

if USE_EMBEDDED_BLAS
# Use the blas provided into the Scilab tarball
libscilab_la_LIBADD += $(top_builddir)/libs/blas/libsciblas.la
endif

if USE_EMBEDDED_LAPACK
# Use the lapack provided into the Scilab tarball
libscilab_la_LIBADD += $(top_builddir)/libs/lapack/libscilapack.la
endif
