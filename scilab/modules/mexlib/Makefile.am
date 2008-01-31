##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

MEXLIB_C_SOURCES = src/c/mexlib.c

MEXLIB_FLAGS = -Iincludes/ -I$(top_builddir)/modules/output_stream/includes/

if STRING_V1
MEXLIB_FLAGS += -I$(top_srcdir)/modules/string/includes
else
MEXLIB_FLAGS += -I$(top_srcdir)/modules/STRING_V2/includes
endif					


MEXLIB_DEP = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/modules/dynamic_link/libscidynamic_link.la \
$(top_builddir)/modules/data_structures/libscidata_structures.la \
	$(top_builddir)/modules/output_stream/libscioutput_stream.la 
#	$(top_builddir)/libs/blas/libsciblas.la \
#	$(top_builddir)/modules/arnoldi/libsciarnoldi.la


if STRING_V1
MEXLIB_DEP += $(top_builddir)/modules/string/libscistring.la
else
MEXLIB_DEP += $(top_builddir)/modules/STRING_V2/libscistring.la
endif

pkglib_LTLIBRARIES = libmat.la libmex.la libmx.la

libmex_la_SOURCES = $(MEXLIB_C_SOURCES)

libmex_la_LIBADD = $(MEXLIB_DEP)

libmex_la_CFLAGS=  $(MEXLIB_FLAGS)

libmat_la_SOURCES = $(MEXLIB_C_SOURCES)

libmat_la_LIBADD = $(MEXLIB_DEP)

libmat_la_CFLAGS=  $(MEXLIB_FLAGS)

libmx_la_SOURCES = $(MEXLIB_C_SOURCES)

libmx_la_LIBADD = $(MEXLIB_DEP)

libmx_la_CFLAGS=  $(MEXLIB_FLAGS)


#### Target ######
modulename=mexlib


#### mexlib : Conf files ####
libmex_la_rootdir = $(mydatadir)
libmex_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### mexlib : init scripts ####
libmex_la_etcdir = $(mydatadir)/etc
libmex_la_etc_DATA = etc/mexlib.quit etc/mexlib.start


#### mexlib : include files ####
libmex_la_includedir=$(pkgincludedir)/mexlib
libmex_la_include_HEADERS = includes/mex.h

include $(top_srcdir)/Makefile.incl.am
