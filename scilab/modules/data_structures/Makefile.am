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

libdata_structures_la_includedir=$(includedir)/data_structures

libdata_structures_la_include_HEADERS = includes/gw_data_structures1.h  \
includes/gw_data_structures2.h \
includes/DoublyLinkedList.h


libdata_structures_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libdata_structures.la

libdata_structures_la_SOURCES = $(DATA_STRUCTURES_C_SOURCES) $(DATA_STRUCTURES_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libdata_structures_la_DEPENDENCIES = \
	$(top_builddir)/modules/polynomials/libpolynomials.la \
	$(top_builddir)/modules/core/libcore.la
#	$(top_builddir)/modules/arnoldi/libarnoldi.la \
#	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 


libdata_structures_la_LIBADD = $(libdata_structures_la_DEPENDENCIES)

libdata_structures_la_PKGCONFIG = data_structures.pc
