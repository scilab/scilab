##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

MEXLIB_C_SOURCES = src/mexlib.c

libmexlib_la_CFLAGS=  -Iincludes/ 

lib_LTLIBRARIES = libmexlib.la

libmexlib_la_SOURCES = $(MEXLIB_C_SOURCES)

libmexlib_la_LIBADD = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/data_structures/libdata_structures.la 
#	$(top_builddir)/libs/blas/libblas.la \
#	$(top_builddir)/modules/arnoldi/libarnoldi.la

libmexlib_la_PKGCONFIG = mexlib.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/mexlib


#### mexlib : Conf files ####
libmexlib_la_rootdir = $(target)
libmexlib_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### mexlib : Scilab macros ###
libmexlib_la_macrodir = $(target)/macros
libmexlib_la_macro_DATA = macros/buildmacros.sce


#### mexlib : init scripts #### 
libmexlib_la_etcdir = $(target)/etc
libmexlib_la_etc_DATA = etc/mexlib.quit etc/mexlib.start

#### mexlib : documentation FR #### 
libmexlib_la_helpfrdir = $(target)/help/fr
libmexlib_la_helpfr_DATA = 


#### mexlib : documentation EN #### 
libmexlib_la_helpendir = $(target)/help/eng
libmexlib_la_helpen_DATA = 


#### mexlib : include files ####
libmexlib_la_includedir=$(includedir)/mexlib
libmexlib_la_include_HEADERS = includes/mex.h

