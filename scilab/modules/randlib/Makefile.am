##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

RANDLIB_C_SOURCES = src/c/fsultra.c \
src/c/mt.c \
src/c/igngeom.c \
src/c/kiss.c \
src/c/urand.c \
src/c/clcg2.c \
src/c/clcg4.c

RANDLIB_FORTRAN_SOURCES = \
src/fortran/gennf.f \
src/fortran/genchi.f \
src/fortran/setgmn.f \
src/fortran/gengam.f \
src/fortran/sexpo.f \
src/fortran/snorm.f \
src/fortran/gennch.f \
src/fortran/genmn.f \
src/fortran/genbet.f \
src/fortran/phrtsd.f \
src/fortran/sgamma.f \
src/fortran/ignbin.f \
src/fortran/spofa.f \
src/fortran/sdot.f \
src/fortran/ignnbn.f \
src/fortran/lennob.f \
src/fortran/genf.f \
src/fortran/genunf.f \
src/fortran/ignpoi.f \
src/fortran/genexp.f \
src/fortran/genmul.f \
src/fortran/genprm.f \
src/fortran/gennor.f

GATEWAY_C_SOURCES = sci_gateway/gw_randlib.c \
sci_gateway/sci_grand.c

librandlib_la_includedir=$(includedir)/randlib

librandlib_la_include_HEADERS = includes/grand.h \
includes/clcg4.h \
includes/others_generators.h


librandlib_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = librandlib.la

librandlib_la_SOURCES = $(RANDLIB_C_SOURCES) $(GATEWAY_C_SOURCES) $(RANDLIB_FORTRAN_SOURCES)

librandlib_la_LIBADD = \
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la

#	$(top_builddir)/libs/blas/libblas.la \
#	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 


librandlib_la_PKGCONFIG = randlib.pc
