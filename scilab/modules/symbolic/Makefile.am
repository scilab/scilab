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

libsymbolic_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libsymbolic.la

libsymbolic_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsymbolic_la_LIBADD = \
$(top_builddir)/modules/core/libcore.la

libsymbolic_la_PKGCONFIG = symbolic.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/symbolic


#### symbolic : Conf files ####
libsymbolic_la_rootdir = $(target)
libsymbolic_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### symbolic : Scilab macros ###
libsymbolic_la_macrodir = $(target)/macros
libsymbolic_la_macro_DATA22 =  macros/buildmacros.sce


#### symbolic : init scripts #### 
libsymbolic_la_etcdir = $(target)/etc
libsymbolic_la_etc_DATA = etc/symbolic.quit etc/symbolic.start

#### symbolic : gateway declaration #### 
libsymbolic_la_sci_gatewaydir = $(target)/sci_gateway
libsymbolic_la_sci_gateway_DATA = sci_gateway/symbolic


#### symbolic : include files ####
libsymbolic_la_includedir=$(includedir)/symbolic
libsymbolic_la_include_HEADERS = includes/gw_symbolic.h

include $(top_srcdir)/Makefile.incl.am
