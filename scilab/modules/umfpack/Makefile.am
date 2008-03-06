##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Allan Cornet <allan.cornet@inria.fr>
### INRIA - Scilab 2007
##########

modulename=umfpack

pkglib_LTLIBRARIES = libsciumfpack.la

libsciumfpack_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(UMFPACK3_LIB)

#### umfpack : Conf files ####
libsciumfpack_la_rootdir = $(mydatadir)

#### umfpack : init scripts ####
libsciumfpack_la_etcdir = $(mydatadir)/etc
libsciumfpack_la_etc_DATA = etc/umfpack.quit etc/umfpack.start

#if UMFPACK

UMFPACK_C_SOURCES = src/c/intscispt.c \
src/c/taucs_scilab.c 

#GATEWAY_C_SOURCES = 


#### umfpack : Conf files ####
libsciumfpack_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### umfpack : gateway declaration ####
libsciumfpack_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciumfpack_la_sci_gateway_DATA = sci_gateway/umfpack_gateway.xml

libsciumfpack_la_SOURCES = $(UMFPACK_C_SOURCES) 
#$(GATEWAY_C_SOURCES)

libsciumfpack_la_LIBADD =  $(top_builddir)/modules/libmx/libmx.la $(top_builddir)/modules/libmat/libmat.la $(top_builddir)/modules/sparse/libscisparse.la $(top_builddir)/modules/libmex/libmex.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 

if USE_EMBEDDED_LAPACK
libsciumfpack_la_LIBADD += $(top_builddir)/libs/lapack/libscilapack.la
endif

if USE_EMBEDDED_BLAS
libsciumfpack_la_LIBADD += $(top_builddir)/libs/blas/libsciblas.la 
endif

libsciumfpack_la_CFLAGS=  	-Iincludes/ \
			-Isrc/c/ \
			-I$(top_srcdir)/libs/dynamiclibrary/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/mexlib/includes

#### umfpack : include files ####
libsciumfpack_la_includedir=$(pkgincludedir)/umfpack
libsciumfpack_la_include_HEADERS = 

#else

#UMFPACK_C_SOURCES = src/noumfpack/noumfpack.c

#libsciumfpack_la_SOURCES = $(UMFPACK_C_SOURCES)

#libsciumfpack_la_CFLAGS= -I$(top_srcdir)/modules/output_stream/includes
#endif

include $(top_srcdir)/Makefile.incl.am