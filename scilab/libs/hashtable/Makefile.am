##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

HASHTABLE_C_SOURCES = hashtable_utility.c \
hashtable_itr.c \
hashtable.c


libhashtable_la_includedir=$(includedir)/hashtable

libhashtable_la_include_HEADERS = ./hashtable_itr.h \
./hashtable.h \
./hashtable_utility.h


#libhashtable_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libhashtable.la

libhashtable_la_SOURCES = $(HASHTABLE_C_SOURCES)

libhashtable_la_DEPENDENCIES = 



libhashtable_la_LIBADD = $(libhashtable_la_DEPENDENCIES)

libhashtable_la_PKGCONFIG = hashtable.pc
