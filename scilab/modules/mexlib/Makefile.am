##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

MEXLIB_C_SOURCES = src/mexlib.c

libscimexlib_la_CFLAGS=  -Iincludes/ 

pkglib_LTLIBRARIES = libscimexlib.la

libscimexlib_la_SOURCES = $(MEXLIB_C_SOURCES)

libscimexlib_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/data_structures/libscidata_structures.la 
#	$(top_builddir)/libs/blas/libsciblas.la \
#	$(top_builddir)/modules/arnoldi/libsciarnoldi.la




#### Target ######
target=$(datadir)/mexlib


#### mexlib : Conf files ####
libscimexlib_la_rootdir = $(target)
libscimexlib_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### mexlib : init scripts #### 
libscimexlib_la_etcdir = $(target)/etc
libscimexlib_la_etc_DATA = etc/mexlib.quit etc/mexlib.start


#### mexlib : include files ####
libscimexlib_la_includedir=$(sciincludedir)/mexlib
libscimexlib_la_include_HEADERS = includes/mex.h

include $(top_srcdir)/Makefile.incl.am
