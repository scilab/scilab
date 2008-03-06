##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

HASHTABLE_C_SOURCES = hashtable_utility.c \
hashtable_itr.c \
hashtable.c


libscihashtable_la_includedir=$(pkgincludedir)/hashtable

libscihashtable_la_include_HEADERS = ./hashtable_itr.h \
./hashtable.h \
./hashtable_utility.h

#libscihashtable_la_CFLAGS=  

pkglib_LTLIBRARIES = libscihashtable.la

libscihashtable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscihashtable_la_SOURCES = $(HASHTABLE_C_SOURCES)

libscihashtable_la_LIBADD = 

#### Target ######
modulename=hashtable

include $(top_srcdir)/Makefile.incl.am