##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

FILEIO_SOURCES = src/FileExist.c \
src/files.c \
src/xls.c \
src/fileio.c \
src/olestream-unwrap.c \
src/pldstr.c \
src/ole.c \
src/bytedecoders.c \
src/logger.c \
src/ripole.c \
src/misc.c \
src/bt-int.c

GATEWAY_SOURCES = \
	sci_gateway/sci_xls_read.c \
	sci_gateway/gw_fileio.c \
	sci_gateway/sci_fileio.c \
	sci_gateway/sci_xls_open.c \
	sci_gateway/intreadxls.c

libfileio_la_CFLAGS=-Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/ 

lib_LTLIBRARIES = libfileio.la

libfileio_la_SOURCES = $(FILEIO_SOURCES) $(GATEWAY_SOURCES)

libfileio_la_DEPENDENCIES = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libfileio_la_LIBADD = $(libfileio_la_DEPENDENCIES)

libfileio_la_PKGCONFIG = fileio.pc
