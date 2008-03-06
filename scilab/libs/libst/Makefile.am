##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LIBST_C_SOURCES = libst.c  misc.c


pkglib_LTLIBRARIES = libscilibst.la

libscilibst_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscilibst_la_SOURCES = $(LIBST_C_SOURCES)

libscilibst_la_LIBADD =

#### Target ######
modulename=libst

include $(top_srcdir)/Makefile.incl.am
