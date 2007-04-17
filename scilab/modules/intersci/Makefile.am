##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


INTERSCI_LIB_C_SOURCES = \
src/lib/libinter.c \
src/lib/erro.c \
src/lib/out.c

INTERSCI_EXE_C_SOURCES = \
src/exe/intersci-n.c \
src/exe/getrhs.c \
src/exe/crerhs.c \
src/exe/variables.c \
src/exe/read.c \
src/exe/fornames.c \
src/exe/out-e.c \
src/exe/outext.c \
src/exe/declare.c \
src/exe/opt.c \
src/exe/check.c

libsciintersci_la_includedir=$(sciincludedir)/intersci


pkglib_LTLIBRARIES = libsciintersci.la

bin_PROGRAMS = intersci
intersci_SOURCES = $(INTERSCI_EXE_C_SOURCES)

libsciintersci_la_SOURCES = $(INTERSCI_LIB_C_SOURCES)

libsciintersci_la_CFLAGS = -I$(top_srcdir)/libs/MALLOC/includes/

libsciintersci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Target ######
target=$(datadir)/modules/intersci


#### intersci : Conf files ####
libsciintersci_la_rootdir = $(target)
libsciintersci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


#### intersci : init scripts #### 
libsciintersci_la_etcdir = $(target)/etc
libsciintersci_la_etc_DATA = etc/intersci.quit etc/intersci.start

include $(top_srcdir)/Makefile.incl.am

