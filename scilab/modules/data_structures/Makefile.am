##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########


DATA_STRUCTURES_C_SOURCES = src/c/stcreate.c \
src/c/hmops.c

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
sci_gateway/c/sci_getfield.c \
sci_gateway/c/sci_setfield.c \
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

libscidata_structures_la_CFLAGS=  	-Iincludes/ \
-I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(top_srcdir)/modules/output_stream/includes \
                    -I$(top_srcdir)/modules/string/includes

pkglib_LTLIBRARIES = libscidata_structures.la

libscidata_structures_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscidata_structures_la_SOURCES = $(DATA_STRUCTURES_C_SOURCES) $(DATA_STRUCTURES_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# cyclic deps $(top_builddir)/modules/polynomials/libscipolynomials.la 
libscidata_structures_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/integer/libsciinteger.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/modules/output_stream/libscioutput_stream.la

#### Target ######
modulename=data_structures


#### data_structures : Conf files ####
libscidata_structures_la_rootdir = $(mydatadir)
libscidata_structures_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### data_structures : init scripts ####
libscidata_structures_la_etcdir = $(mydatadir)/etc
libscidata_structures_la_etc_DATA = etc/data_structures.quit etc/data_structures.start

#### data_structures : gateway declaration ####
libscidata_structures_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscidata_structures_la_sci_gateway_DATA = sci_gateway/data_structures_gateway.xml



#### data_structures : include files ####
libscidata_structures_la_includedir=$(pkgincludedir)/data_structures
libscidata_structures_la_include_HEADERS = includes/gw_data_structures1.h \
includes/gw_data_structures2.h

include $(top_srcdir)/Makefile.incl.am
