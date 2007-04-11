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

libscispecial_functions_la_includedir=$(sciincludedir)/special_functions

libscispecial_functions_la_include_HEADERS = includes/gw_special_functions1.h \
includes/gw_special_functions2.h

libscispecial_functions_la_CFLAGS=  -Iincludes/ 

pkglib_LTLIBRARIES = libscispecial_functions.la

libscispecial_functions_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libscispecial_functions_la_LIBADD = \
	$(top_builddir)/modules/elementaries_functions/libscielementaries_functions.la \
	$(top_builddir)/modules/statistics/libscistatistics.la

#\
#	$(top_builddir)/libs/blas/libsciblas.la \
#	$(top_builddir)/modules/arnoldi/libsciarnoldi.la




#### Target ######
target=$(datadir)/special_functions


#### special_functions : Conf files ####
libscispecial_functions_la_rootdir = $(target)
libscispecial_functions_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### special_functions : init scripts #### 
libscispecial_functions_la_etcdir = $(target)/etc
libscispecial_functions_la_etc_DATA = etc/special_functions.quit etc/special_functions.start

#### special_functions : gateway declaration #### 
libscispecial_functions_la_sci_gatewaydir = $(target)/sci_gateway
libscispecial_functions_la_sci_gateway_DATA = sci_gateway/special_functions_gateway.xml

include $(top_srcdir)/Makefile.incl.am
