##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LOCALIZATION_C_SOURCES = src/hashtable_localization.c \
src/localization.c

liblocalization_la_includedir=$(includedir)/localization

liblocalization_la_include_HEADERS = includes/localization.h

liblocalization_la_CFLAGS=  -Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/libs/hashtable/ $(XML_FLAGS)
liblocalization_la_LDFLAGS= $(XML_LIBS)

lib_LTLIBRARIES = liblocalization.la

liblocalization_la_SOURCES = $(LOCALIZATION_C_SOURCES) 

liblocalization_la_DEPENDENCIES = 	$(top_builddir)/libs/MALLOC/libmalloc.la \
$(top_builddir)/libs/hashtable/libhashtable.la

liblocalization_la_LIBADD = $(liblocalization_la_DEPENDENCIES)

liblocalization_la_PKGCONFIG = localization.pc
