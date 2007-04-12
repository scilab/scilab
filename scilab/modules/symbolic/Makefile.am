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

libscisymbolic_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
pkglib_LTLIBRARIES = libscisymbolic.la

libscisymbolic_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libscisymbolic_la_LIBADD = \
$(top_builddir)/modules/core/libscicore.la




#### Target ######
target=$(datadir)/symbolic


#### symbolic : Conf files ####
libscisymbolic_la_rootdir = $(target)
libscisymbolic_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


#### symbolic : init scripts #### 
libscisymbolic_la_etcdir = $(target)/etc
libscisymbolic_la_etc_DATA = etc/symbolic.quit etc/symbolic.start

#### symbolic : gateway declaration #### 
libscisymbolic_la_sci_gatewaydir = $(target)/sci_gateway
libscisymbolic_la_sci_gateway_DATA = sci_gateway/symbolic_gateway.xml 


#### symbolic : include files ####
libscisymbolic_la_includedir=$(sciincludedir)/symbolic
libscisymbolic_la_include_HEADERS = includes/gw_symbolic.h

include $(top_srcdir)/Makefile.incl.am
