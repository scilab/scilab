##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2007
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
differential_equations \
string \
string_V2 \
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
wintools \
functions \
incremental_link \
others \
overloading \
maple2scilab \
m2sci \
texmacs \
scipad \
scilab2fortran \
development_tools \
compatibility_functions \
helptools

SUBDIRS += scicos 

if JAVA
SUBDIRS += jvm
endif

if FFTW
SUBDIRS += fftw
endif

if PVM
SUBDIRS += pvm
endif

SUBDIRS += .

# javasci should be compile at the end because it needs libscilab.la
# Don't forget that the order in subdirs does matter !!!
if JAVA
SUBDIRS += javasci
endif



#### Target ######
target=$(datadir)/

#### Basic files ####
libscilab_la_rootdir = $(target)
libscilab_la_root_DATA = modules

# 
pkglib_LTLIBRARIES = libscilab.la
libscilab_la_SOURCES = 

# Core modules
libscilab_la_LIBADD = \
	$(top_builddir)/modules/mexlib/libmex.la \
	$(top_builddir)/modules/arnoldi/libsciarnoldi.la \
	$(top_builddir)/modules/boolean/libsciboolean.la \
	$(top_builddir)/modules/cacsd/libscicacsd.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/data_structures/libscidata_structures.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/double/libscidouble.la \
	$(top_builddir)/modules/elementaries_functions/libscielementaries_functions.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/modules/gui/libscigui.la \
	$(top_builddir)/modules/integer/libsciinteger.la \
	$(top_builddir)/modules/interpolation/libsciinterpolation.la \
	$(top_builddir)/modules/intersci/libsciintersci.la \
	$(top_builddir)/modules/io/libsciio.la \
	$(top_builddir)/modules/linear_algebra/libscilinear_algebra.la \
	$(top_builddir)/modules/localization/libscilocalization.la \
	$(top_builddir)/modules/metanet/libscimetanet.la \
	$(top_builddir)/modules/optimization/libscioptimization.la \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/modules/randlib/libscirandlib.la \
	$(top_builddir)/modules/symbolic/libscisymbolic.la \
	$(top_builddir)/modules/signal_processing/libscisignal_processing.la \
	$(top_builddir)/modules/special_functions/libscispecial_functions.la \
	$(top_builddir)/modules/sound/libscisound.la \
	$(top_builddir)/modules/tclsci/libscitclsci.la \
	$(top_builddir)/modules/statistics/libscistatistics.la \
	$(top_builddir)/modules/time/libscitime.la \
	$(top_builddir)/modules/wintools/libsciwintools.la

if STRINGV2
libscilab_la_LIBADD += $(top_builddir)/modules/string_V2/libscistring.la
else
libscilab_la_LIBADD += $(top_builddir)/modules/string/libscistring.la
endif

if PVM
libscilab_la_LIBADD += $(top_builddir)/modules/pvm/libscipvm.la
endif

if FFTW
libscilab_la_LIBADD += $(top_builddir)/modules/fftw/libscifftw.la
endif

if JAVA
libscilab_la_LIBADD += $(top_builddir)/modules/jvm/libscijvm.la
endif

libscilab_la_LIBADD += $(top_builddir)/modules/scicos/libsciscicos.la

## Libs 
libscilab_la_LIBADD += \
$(top_builddir)/libs/blas/libsciblas.la \
$(top_builddir)/libs/hashtable/libscihashtable.la \
$(top_builddir)/libs/lapack/libscilapack.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la  \
$(top_builddir)/libs/libst/libscilibst.la


#BUILT_SOURCES = modules

# Generate the module list from the SUBDIRS
# COMMENTED : because it must be hardcoded (the order does matter)
#modules:
#	@for module in $(SUBDIRS); do if test $$module != "."; then echo $$module; fi;	done > modules

include $(top_srcdir)/Makefile.incl.am
