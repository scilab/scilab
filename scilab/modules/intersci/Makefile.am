##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########


INTERSCI_LIB_C_SOURCES = \
src/lib/libinter.c \
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

libsciintersci_la_includedir=$(pkgincludedir)/intersci

pkglib_LTLIBRARIES = libsciintersci.la

libsciintersci_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

bin_PROGRAMS = intersci
intersci_SOURCES = $(INTERSCI_EXE_C_SOURCES)

libsciintersci_la_SOURCES = $(INTERSCI_LIB_C_SOURCES)

libsciintersci_la_CFLAGS = 	-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(srcdir)/includes \
		        -I$(top_srcdir)/modules/string/includes	\
		        -I$(top_srcdir)/modules/operations/includes

# For the code check (splint)
CHECK_SRC= $(libsciintersci_la_SOURCES)
INCLUDE_FLAGS = $(libsciintersci_la_CFLAGS)

libsciintersci_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
	$(top_builddir)/modules/output_stream/libscioutput_stream.la \
    $(top_builddir)/modules/string/libscistring.la

#### Target ######
modulename=intersci


#### intersci : Conf files ####
libsciintersci_la_rootdir = $(mydatadir)
libsciintersci_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### intersci : init scripts ####
libsciintersci_la_etcdir = $(mydatadir)/etc
libsciintersci_la_etc_DATA = etc/intersci.quit etc/intersci.start

include $(top_srcdir)/Makefile.incl.am

