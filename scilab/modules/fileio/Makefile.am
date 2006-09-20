
FILEIO_SOURCES =  src/misc.c src/fileio.c src/xls.c
RIPOLE_SOURCES = src/bt-int.c src/bytedecoders.c src/logger.c src/ole.c src/olestream-unwrap.c src/pldstr.c src/ripole.c

GATEWAY_SOURCES = sci_gateway/sci_xls_read.c sci_gateway/gw_fileio.c \
	sci_gateway/sci_fileio.c sci_gateway/sci_xls_open.c \
	sci_gateway/intreadxls.c

libfileio_la_CFLAGS=-Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/ 

lib_LTLIBRARIES = libfileio.la

libfileio_la_SOURCES = $(FILEIO_SOURCES) $(RIPOLE_SOURCES) $(GATEWAY_SOURCES)

libfileio_la_DEPENDENCIES = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libfileio_la_LIBADD = $(libfileio_la_DEPENDENCIES)

libfileio_la_PKGCONFIG = fileio.pc
