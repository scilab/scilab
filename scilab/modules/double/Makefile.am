
GATEWAY_C_SOURCES = sci_gateway/c/gw_double.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/vecldiv.f \
sci_gateway/fortran/vecmul.f \
sci_gateway/fortran/matxpow.f \
sci_gateway/fortran/vecimpl.f \
sci_gateway/fortran/matldiv.f \
sci_gateway/fortran/matsubst.f \
sci_gateway/fortran/vecrdiv.f \
sci_gateway/fortran/matchsgn.f \
sci_gateway/fortran/matrdiv.f \
sci_gateway/fortran/matrc.f \
sci_gateway/fortran/matpow.f \
sci_gateway/fortran/matins1.f \
sci_gateway/fortran/matins2.f \
sci_gateway/fortran/matcmp.f \
sci_gateway/fortran/matmult.f \
sci_gateway/fortran/matext1.f \
sci_gateway/fortran/matext2.f \
sci_gateway/fortran/mattr.f \
sci_gateway/fortran/matcc.f \
sci_gateway/fortran/mattrc.f \
sci_gateway/fortran/matadd.f

lib_LTLIBRARIES = libdouble.la

libdouble_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libdouble_la_LIBADD = \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/polynomials/libpolynomials.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la  \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/intersci/libintersci.la

libdouble_la_PKGCONFIG = double.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/double


#### double : Conf files ####
libdouble_la_rootdir = $(target)
libdouble_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### double : Scilab macros ###
libdouble_la_macrodir = $(target)/macros
libdouble_la_macro_DATA = macros/buildmacros.sce


#### double : init scripts #### 
libdouble_la_etcdir = $(target)/etc
libdouble_la_etc_DATA = etc/double.quit etc/double.start

#### double : documentation FR #### 
libdouble_la_helpfrdir = $(target)/help/fr
libdouble_la_helpfr_DATA2 = 


#### double : documentation EN #### 
libdouble_la_helpendir = $(target)/help/eng
libdouble_la_helpen_DATA2 = 

include $(top_srcdir)/Makefile.incl.am
