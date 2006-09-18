##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


GATEWAY_C_SOURCES = sci_gateway/c/sci_dlgamma.c \
sci_gateway/c/sci_oldbessely.c \
sci_gateway/c/sci_bessely.c \
sci_gateway/c/sci_beta.c \
sci_gateway/c/sci_gamma.c \
sci_gateway/c/sci_oldbesseli.c \
sci_gateway/c/sci_legendre.c \
sci_gateway/c/sci_besselh.c \
sci_gateway/c/sci_oldbesselj.c \
sci_gateway/c/sci_besseli.c \
sci_gateway/c/sci_besselj.c \
sci_gateway/c/sci_oldbesselk.c \
sci_gateway/c/sci_besselk.c \
sci_gateway/c/sci_calerf.c \
sci_gateway/c/gw_special_functions1.c \
sci_gateway/c/gw_special_functions2.c \
sci_gateway/c/sci_lgamma.c

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/sci_f_dlgamma.f \
sci_gateway/fortran/sci_f_lgamma.f \
sci_gateway/fortran/sci_f_oldbesseli.f \
sci_gateway/fortran/sci_f_gamma.f \
sci_gateway/fortran/sci_f_oldbessely.f \
sci_gateway/fortran/sci_f_oldbesselj.f \
sci_gateway/fortran/sci_f_oldbesselk.f \
sci_gateway/fortran/sci_f_calerf.f

libspecial_functions_la_includedir=$(includedir)/special_functions

#libspecial_functions_la_include_HEADERS = includes/gw_special_functions.h

#libspecial_functions_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libspecial_functions.la

libspecial_functions_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libspecial_functions_la_DEPENDENCIES = \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la \
	$(top_builddir)/modules/statistics/libstatistics.la

#\
#	$(top_builddir)/libs/blas/libblas.la \
#	$(top_builddir)/modules/arnoldi/libarnoldi.la




libspecial_functions_la_LIBADD = $(libspecial_functions_la_DEPENDENCIES)

libspecial_functions_la_PKGCONFIG = special_functions.pc
