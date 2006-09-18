INTERSCI_LIB_C_SOURCES = \
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

libintersci_la_DEPENDENCIES = \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libintersci_la_LIBADD = $(libintersci_la_DEPENDENCIES)

libintersci_la_PKGCONFIG = intersci.pc
