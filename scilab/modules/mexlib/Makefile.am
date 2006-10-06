##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

MEXLIB_C_SOURCES = src/mexlib.c

libmexlib_la_includedir=$(includedir)/mexlib

libmexlib_la_include_HEADERS = includes/mex.h

libmexlib_la_CFLAGS=  -Iincludes/ 

lib_LTLIBRARIES = libmexlib.la

libmexlib_la_SOURCES = $(MEXLIB_C_SOURCES)

libmexlib_la_DEPENDENCIES = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/data_structures/libdata_structures.la 
#	$(top_builddir)/libs/blas/libblas.la \
#	$(top_builddir)/modules/arnoldi/libarnoldi.la


libmexlib_la_LIBADD = $(libmexlib_la_DEPENDENCIES)

libmexlib_la_PKGCONFIG = mexlib.pc
