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
libinterpolation_la_macro_DATA = macros/buildmacros.sce


#### interpolation : init scripts #### 
libinterpolation_la_etcdir = $(target)/etc
libinterpolation_la_etc_DATA = etc/interpolation.quit etc/interpolation.start

#### interpolation : gateway declaration #### 
libinterpolation_la_sci_gatewaydir = $(target)/sci_gateway
libinterpolation_la_sci_gateway_DATA = sci_gateway/interpolation

#### interpolation : documentation FR #### 
libinterpolation_la_helpfrdir = $(target)/help/fr
libinterpolation_la_helpfr_DATA2 = help/fr/interp3d.htm \
help/fr/interp.htm \
help/fr/lsq_splin.htm \
help/fr/splin3d.htm \
help/fr/splin.htm \
help/fr/intsplin.htm \
help/fr/whatis.htm \
help/fr/cshep2d.htm \
help/fr/bsplin3val.htm \
help/fr/eval_cshep2d.htm \
help/fr/linear_interpn.htm


#### interpolation : documentation EN #### 
libinterpolation_la_helpendir = $(target)/help/eng
libinterpolation_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/eval_cshep2d.htm \
help/eng/bsplin3val.htm \
help/eng/cshep2d.htm \
help/eng/interp3d.htm \
help/eng/interp.htm \
help/eng/intsplin.htm \
help/eng/lsq_splin.htm \
help/eng/linear_interpn.htm \
help/eng/splin3d.htm \
help/eng/splin.htm


#### interpolation : include files ####
libinterpolation_la_includedir=$(includedir)/interpolation
libinterpolation_la_include_HEADERS = includes/gw_interpolation.h \
includes/interpolation.h \
includes/constinterp.h


#### interpolation : demos files ####
libinterpolation_la_demosdir=$(target)/demos/
libinterpolation_la_demos_DATA = demos/interp_demo.sci

include $(top_srcdir)/Makefile.incl.am
