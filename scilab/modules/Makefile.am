##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

# The order DOES matter !!!
SUBDIRS = \
helptools \
intersci \
polynomials \
cacsd \
elementaries_functions \
arnoldi \
boolean \
linear_algebra \
differential_equations \
string \
signal_processing \
fileio \
core \
metanet \
optimization \
interpolation \
localization \
sound \
statistics \
time \
integer \
special_functions \
double \
randlib \
data_structures \
mexlib \
symbolic \
tclsci \
sparse \
io \
gui \
graphics \
scicos \
wintools \
functions \
incremental_link \
others \
overloading \
javasci \
maple2scilab \
m2sci \
texmacs \
tfds \
scipad \
scilab2fortran \
development_tools


if JAVA
SUBDIRS += jvm 
endif

if PVM
SUBDIRS += pvm
endif

SUBDIRS += .


datadir=${prefix}/modules
#### Target ######
target=$(datadir)

#### Basic files ####
libscilab_la_rootdir = $(target)
libscilab_la_root_DATA = modules

# 
lib_LTLIBRARIES = libscilab.la
libscilab_la_SOURCES = 

# Core modules
libscilab_la_LIBADD = \
	$(top_builddir)/modules/mexlib/libmexlib.la \
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
	$(top_builddir)/modules/polynomials/libpolynomials.la \
	$(top_builddir)/modules/randlib/librandlib.la \
	$(top_builddir)/modules/symbolic/libsymbolic.la \
	$(top_builddir)/modules/signal_processing/libsignal_processing.la \
	$(top_builddir)/modules/special_functions/libspecial_functions.la \
	$(top_builddir)/modules/scicos/libscicos.la \
	$(top_builddir)/modules/sound/libsound.la \
	$(top_builddir)/modules/tclsci/libtclsci.la \
	$(top_builddir)/modules/statistics/libstatistics.la \
	$(top_builddir)/modules/time/libtime.la \
	$(top_builddir)/modules/wintools/libwintools.la

if PVM
libscilab_la_LIBADD += $(top_builddir)/modules/pvm/libscipvm.la
endif

if JAVA
libscilab_la_LIBADD += $(top_builddir)/modules/jvm/libscijvm.la
endif
## Libs 
libscilab_la_LIBADD += \
$(top_builddir)/libs/blas/libblas.la \
$(top_builddir)/libs/hashtable/libhashtable.la \
$(top_builddir)/libs/lapack/liblapack.la \
$(top_builddir)/libs/MALLOC/libmalloc.la 

BUILT_SOURCES = modules

# Generate the module list from the SUBDIRS
modules:
	@for module in $(SUBDIRS); do if test $$module != "."; then echo $$module; fi;	done > modules