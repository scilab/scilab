##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

MALLOC_C_SOURCES = src/sci_mem_alloc.c src/getmaxMALLOC.c

libscimalloc_la_includedir=$(pkgincludedir)/malloc

libscimalloc_la_include_HEADERS = includes/sci_mem_alloc.h \
includes/MALLOC.h \
includes/getmaxMALLOC.h \
includes/ExportImport.h

libscimalloc_la_CFLAGS= 

pkglib_LTLIBRARIES = libscimalloc.la

libscimalloc_la_SOURCES = $(MALLOC_C_SOURCES)

libscimalloc_la_LIBADD = 

#### Target ######
modulename=malloc

include $(top_srcdir)/Makefile.incl.am
