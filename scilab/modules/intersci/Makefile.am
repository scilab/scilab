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

libintersci_la_includedir=$(includedir)/intersci


lib_LTLIBRARIES = libintersci.la

bin_PROGRAMS = intersci
intersci_SOURCES = $(INTERSCI_EXE_C_SOURCES)

libintersci_la_SOURCES = $(INTERSCI_LIB_C_SOURCES)

libintersci_la_CFLAGS = -I$(top_srcdir)/libs/MALLOC/includes/

libintersci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libintersci_la_PKGCONFIG = intersci.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/intersci


#### intersci : Conf files ####
libintersci_la_rootdir = $(target)
libintersci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### intersci : Scilab macros ###
libintersci_la_macrodir = $(target)/macros
libintersci_la_macro_DATA =   macros/buildmacros.sce


#### intersci : init scripts #### 
libintersci_la_etcdir = $(target)/etc
libintersci_la_etc_DATA = etc/intersci.quit etc/intersci.start

include $(top_srcdir)/Makefile.incl.am

