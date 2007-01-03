##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

INTERPOLATION_FORTRAN_SOURCES = src/fortran/somespline.f \
src/fortran/dspfit.f \
src/fortran/cshep2d.f \
src/fortran/dtensbs.f \
src/fortran/pchim.f

INTERPOLATION_C_SOURCES = src/c/interpolation.c \
src/c/someinterp.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_lsq_splin.c \
sci_gateway/c/sci_interp.c \
sci_gateway/c/sci_interp2d.c \
sci_gateway/c/sci_splin.c \
sci_gateway/c/gw_interpolation.c \
sci_gateway/c/sci_cshep2d.c \
sci_gateway/c/sci_interp3d.c \
sci_gateway/c/sci_linear_interpn.c \
sci_gateway/c/sci_splin2d.c \
sci_gateway/c/sci_splin3d.c \
sci_gateway/c/sci_bsplin3val.c \
sci_gateway/c/sci_eval_cshep2d.c

libinterpolation_la_FFLAGS=  -Iincludes/
libinterpolation_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libinterpolation.la

libinterpolation_la_SOURCES = $(INTERPOLATION_C_SOURCES) $(INTERPOLATION_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) 

libinterpolation_la_LIBADD = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 

libinterpolation_la_PKGCONFIG = interpolation.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/interpolation


#### interpolation : Conf files ####
libinterpolation_la_rootdir = $(target)
libinterpolation_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### interpolation : Scilab macros ###
libinterpolation_la_macrodir = $(target)/macros
libinterpolation_la_macro_DATA22 = macros/buildmacros.sce


#### interpolation : init scripts #### 
libinterpolation_la_etcdir = $(target)/etc
libinterpolation_la_etc_DATA = etc/interpolation.quit etc/interpolation.start

#### interpolation : gateway declaration #### 
libinterpolation_la_sci_gatewaydir = $(target)/sci_gateway
libinterpolation_la_sci_gateway_DATA = sci_gateway/interpolation

#### interpolation : include files ####
libinterpolation_la_includedir=$(includedir)/interpolation
libinterpolation_la_include_HEADERS = includes/gw_interpolation.h \
includes/interpolation.h \
includes/constinterp.h


#### interpolation : demos files ####
libinterpolation_la_demosdir=$(target)/demos/
libinterpolation_la_demos_DATA = demos/interp_demo.sci

include $(top_srcdir)/Makefile.incl.am
