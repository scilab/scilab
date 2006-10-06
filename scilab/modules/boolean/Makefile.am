##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

BOOLEAN_C_SOURCES = src/c/matboolean.c \
src/c/vect_or.c \
src/c/vect_and.c

BOOLEAN_FORTRAN_SOURCES = src/fortran/logic.f \
src/fortran/find.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_and.c \
sci_gateway/c/sci_or.c \
sci_gateway/c/gw_boolean.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_bool2s.f \
sci_gateway/fortran/sci_find.f

libboolean_la_includedir=$(includedir)/boolean

libboolean_la_include_HEADERS = includes/gw_boolean.h

libboolean_la_CFLAGS=  -Iincludes/ 

lib_LTLIBRARIES = libboolean.la

libboolean_la_SOURCES = $(BOOLEAN_C_SOURCES) $(BOOLEAN_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libboolean_la_LIBADD = \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/arnoldi/libarnoldi.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 

libboolean_la_PKGCONFIG = boolean.pc
