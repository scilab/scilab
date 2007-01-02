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

libboolean_la_CFLAGS=  -Iincludes/ 

lib_LTLIBRARIES = libboolean.la

libboolean_la_SOURCES = $(BOOLEAN_C_SOURCES) $(BOOLEAN_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libboolean_la_LIBADD = \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/arnoldi/libarnoldi.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 
#$(top_builddir)/libs/blas/libblas.la:
#	(cd $(top_builddir)/libs/blas/; make libblas.la)
#SUFFIXES = .la
#.la:
#	echo "oh oh"
libboolean_la_PKGCONFIG = boolean.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/boolean


#### boolean : Conf files ####
libboolean_la_rootdir = $(target)
libboolean_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### boolean : Scilab macros ###
libboolean_la_macrodir = $(target)/macros
libboolean_la_macro_DATA =  macros/buildmacros.sce


#### boolean : init scripts #### 
libboolean_la_etcdir = $(target)/etc
libboolean_la_etc_DATA = etc/boolean.quit etc/boolean.start

#### boolean : gateway declaration #### 
libboolean_la_sci_gatewaydir = $(target)/sci_gateway
libboolean_la_sci_gateway_DATA = sci_gateway/boolean


#### boolean : documentation FR #### 
libboolean_la_helpfrdir = $(target)/help/fr
libboolean_la_helpfr_DATA2 = help/fr/whatis.htm \
help/fr/find.htm \
help/fr/bool2s.htm


#### boolean : documentation EN #### 
libboolean_la_helpendir = $(target)/help/eng
libboolean_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/bool2s.htm \
help/eng/find.htm


#### boolean : include files ####
libboolean_la_includedir=$(includedir)/boolean
libboolean_la_include_HEADERS = includes/gw_boolean.h

include $(top_srcdir)/Makefile.incl.am
