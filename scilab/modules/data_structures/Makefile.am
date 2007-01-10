##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


DATA_STRUCTURES_C_SOURCES = src/c/stcreate.c \
src/c/hmops.c \
src/c/DoublyLinkedList.c

DATA_STRUCTURES_FORTRAN_SOURCES = src/fortran/extractfields.f \
src/fortran/forcerhs.f \
src/fortran/intl_i.f \
src/fortran/lsstyp.f \
src/fortran/udptr.f \
src/fortran/followpath.f \
src/fortran/rattyp.f \
src/fortran/lstops.f \
src/fortran/gratyp.f \
src/fortran/mlist.f \
src/fortran/insertfield.f \
src/fortran/intl_e.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_lsslist.c \
sci_gateway/c/sci_glist.c \
sci_gateway/c/sci_mlist.c \
sci_gateway/c/sci_definedfields.c \
sci_gateway/c/sci_lstcat.c \
sci_gateway/c/sci_list.c \
sci_gateway/c/gw_data_structures1.c \
sci_gateway/c/gw_data_structures2.c \
sci_gateway/c/sci_lstsize.c \
sci_gateway/c/sci_rlist.c \
sci_gateway/c/sci_tlist.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_definedfields.f \
sci_gateway/fortran/sci_lstsize.f \
sci_gateway/fortran/sci_setfield.f \
sci_gateway/fortran/sci_getfield.f \
sci_gateway/fortran/sci_lstcat.f \
sci_gateway/fortran/sci_list.f

libdata_structures_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/
pkglib_LTLIBRARIES = libdata_structures.la

libdata_structures_la_SOURCES = $(DATA_STRUCTURES_C_SOURCES) $(DATA_STRUCTURES_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libdata_structures_la_LIBADD = \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/integer/libsciinteger.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

libdata_structures_la_PKGCONFIG = data_structures.pc

#SUFFIXES = .sci
#.sci:
#	$(top_builddir)/scilab-bin -nb -l eng -nouserstartup -nwni -e "xmltohtml();exit;"


datadir=${prefix}/modules
#### Target ######
target=$(datadir)/data_structures


#### data_structures : Conf files ####
libdata_structures_la_rootdir = $(target)
libdata_structures_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### data_structures : Scilab macros ###
libdata_structures_la_macrodir = $(target)/macros
libdata_structures_la_macro_DATA22 = macros/struct.sci \
macros/iscell.sci \
macros/cell.sci \
macros/isfield.sci \
macros/isstruct.sci macros/cell.bin \
macros/iscell.bin \
macros/isfield.bin \
macros/isstruct.bin \
macros/struct.bin macros/names macros/lib macros/buildmacros.sce


#### data_structures : init scripts #### 
libdata_structures_la_etcdir = $(target)/etc
libdata_structures_la_etc_DATA = etc/data_structures.quit etc/data_structures.start

#### data_structures : gateway declaration #### 
libdata_structures_la_sci_gatewaydir = $(target)/sci_gateway
libdata_structures_la_sci_gateway_DATA = sci_gateway/data_structures



#### data_structures : include files ####
libdata_structures_la_includedir=$(includedir)/data_structures
libdata_structures_la_include_HEADERS = includes/gw_data_structures1.h \
includes/gw_data_structures2.h \
includes/DoublyLinkedList.h

include $(top_srcdir)/Makefile.incl.am
