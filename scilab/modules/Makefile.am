##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

# The order DOES matter !!!
SUBDIRS = \
intersci \
polynomials \
cacsd \
elementaries_functions \
arnoldi \
boolean \
linear_algebra \
core \
metanet \
optimization \
differential_equations \
interpolation \
localization \
sound \
statistics \
time \
pvm \
fileio \
integer \
special_functions \
double \
randlib \
data_structures \
symbolic \
tclsci \
sparse \
string \
io \
gui \
graphics \
scicos \
wintools

if JAVA
#SUBDIRS += Javasci
endif

SUBDIRS += .

# 
lib_LTLIBRARIES = libscilab.la
libscilab_la_SOURCES = 

# Core modules
libscilab_la_DEPENDENCIES = \
	$(top_builddir)/modules/arnoldi/libarnoldi.la \
	$(top_builddir)/modules/boolean/libboolean.la \
	$(top_builddir)/modules/cacsd/libcacsd.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/data_structures/libdata_structures.la \
	$(top_builddir)/modules/differential_equations/libdifferential_equations.la \
	$(top_builddir)/modules/double/libdouble.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la \
	$(top_builddir)/modules/fileio/libfileio.la \
	$(top_builddir)/modules/graphics/libgraphics.la \
	$(top_builddir)/modules/gui/libgui.la \
	$(top_builddir)/modules/integer/libinteger.la \
	$(top_builddir)/modules/interpolation/libinterpolation.la \
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/io/libio.la \
	$(top_builddir)/modules/linear_algebra/liblinear_algebra.la \
	$(top_builddir)/modules/localization/liblocalization.la \
	$(top_builddir)/modules/metanet/libmetanet.la \
	$(top_builddir)/modules/optimization/liboptimization.la \
	$(top_builddir)/modules/pvm/libscipvm.la \
	$(top_builddir)/modules/polynomials/libpolynomials.la \
	$(top_builddir)/modules/randlib/librandlib.la \
	$(top_builddir)/modules/symbolic/libsymbolic.la \
	$(top_builddir)/modules/signal_processing/libsignal_processing.la \
	$(top_builddir)/modules/special_functions/libspecial_functions.la \
	$(top_builddir)/modules/scicos/libscicos.la \
	$(top_builddir)/modules/sound/libsound.la \
	$(top_builddir)/modules/tclsci/libtclsci.la \
	$(top_builddir)/modules/time/libtime.la \
	$(top_builddir)/modules/wintools/libwintools.la \
	$(top_builddir)/modules/jvm/libscijvm.la

## Libs 
libscilab_la_DEPENDENCIES += \
$(top_builddir)/libs/blas/libblas.la \
$(top_builddir)/libs/hashtable/libhashtable.la \
$(top_builddir)/libs/lapack/liblapack.la \
$(top_builddir)/libs/MALLOC/libmalloc.la 

libscilab_la_LIBADD = $(libscilab_la_DEPENDENCIES)
