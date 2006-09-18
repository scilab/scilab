##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LOCALIZATION_C_SOURCES = src/hashtable_localization.c \
src/localization.c


liblocalization_la_includedir=$(includedir)/localization

liblocalization_la_include_HEADERS = includes/localization.h

liblocalization_la_CFLAGS=  -Iincludes/ -Isrc/

lib_LTLIBRARIES = liblocalization.la

liblocalization_la_SOURCES = $(LOCALIZATION_C_SOURCES) 

liblocalization_la_DEPENDENCIES = 

liblocalization_la_LIBADD = $(liblocalization_la_DEPENDENCIES)

liblocalization_la_PKGCONFIG = localization.pc
