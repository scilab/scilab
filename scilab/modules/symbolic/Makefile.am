##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

GATEWAY_C_SOURCES = sci_gateway/c/gw_symbolic.c \
sci_gateway/c/sci_mulf.c \
sci_gateway/c/sci_ldivf.c \
sci_gateway/c/sci_addsubf.c \
sci_gateway/c/sci_rdivf.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_addsubf.f \
sci_gateway/fortran/sci_mulf.f \
sci_gateway/fortran/sci_ldivf.f \
sci_gateway/fortran/sci_rdivf.f

libsymbolic_la_includedir=$(includedir)/symbolic

libsymbolic_la_include_HEADERS = includes/gw_symbolic.h

libsymbolic_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libsymbolic.la

libsymbolic_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsymbolic_la_LIBADD = \
$(top_builddir)/modules/core/libcore.la

libsymbolic_la_PKGCONFIG = symbolic.pc
