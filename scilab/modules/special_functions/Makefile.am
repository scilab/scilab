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
libspecial_functions_la_LIBADD = \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la \
	$(top_builddir)/modules/statistics/libstatistics.la

#\
#	$(top_builddir)/libs/blas/libblas.la \
#	$(top_builddir)/modules/arnoldi/libarnoldi.la

libspecial_functions_la_PKGCONFIG = special_functions.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/special_functions


#### special_functions : Conf files ####
libspecial_functions_la_rootdir = $(target)
libspecial_functions_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### special_functions : Scilab macros ###
libspecial_functions_la_macrodir = $(target)/macros
libspecial_functions_la_macro_DATA =  macros/buildmacros.sce


#### special_functions : init scripts #### 
libspecial_functions_la_etcdir = $(target)/etc
libspecial_functions_la_etc_DATA = etc/special_functions.quit etc/special_functions.start

#### special_functions : gateway declaration #### 
libspecial_functions_la_sci_gatewaydir = $(target)/sci_gateway
libspecial_functions_la_sci_gateway_DATA = sci_gateway/special_functions


#### special_functions : documentation FR #### 
libspecial_functions_la_helpfrdir = $(target)/help/fr
libspecial_functions_la_helpfr_DATA2 = help/fr/legendre.htm \
help/fr/oldbessel.htm \
help/fr/calerf.htm \
help/fr/whatis.htm \
help/fr/beta.htm \
help/fr/bessel.htm \
help/fr/gamma.htm \
help/fr/dlgamma.htm \
help/fr/gammaln.htm


#### special_functions : documentation EN #### 
libspecial_functions_la_helpendir = $(target)/help/eng
libspecial_functions_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/legendre.htm \
help/eng/bessel.htm \
help/eng/beta.htm \
help/eng/calerf.htm \
help/eng/dlgamma.htm \
help/eng/gammaln.htm \
help/eng/gamma.htm \
help/eng/oldbessel.htm

include $(top_srcdir)/Makefile.incl.am
