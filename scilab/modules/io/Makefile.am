##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

IO_C_SOURCES = src/c/getpidc.c \
src/c/setenvc.c \
src/c/systemc.c \
src/c/getenvc.c \
src/c/GetenvB.c

IO_FORTRAN_SOURCES = src/fortran/v2unit.f \
src/fortran/newsave.f \
src/fortran/v2cunit.f \
src/fortran/writebuf.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_read4b.c \
sci_gateway/c/sci_getio.c \
sci_gateway/c/sci_readb.c \
sci_gateway/c/sci_host.c \
sci_gateway/c/sci_file.c \
sci_gateway/c/sci_rat.c \
sci_gateway/c/sci_getenv.c \
sci_gateway/c/sci_write.c \
sci_gateway/c/sci_writeb.c \
sci_gateway/c/sci_setenv.c \
sci_gateway/c/gw_io.c \
sci_gateway/c/sci_mgetl.c \
sci_gateway/c/sci_read.c \
sci_gateway/c/sci_getpid.c \
sci_gateway/c/sci_write4b.c \
sci_gateway/c/sci_load.c \
sci_gateway/c/sci_unix.c \
sci_gateway/c/sci_save.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/intwrite4b.f \
sci_gateway/fortran/intrat.f \
sci_gateway/fortran/intwritb.f \
sci_gateway/fortran/intwrite.f \
sci_gateway/fortran/intread4b.f \
sci_gateway/fortran/intread.f \
sci_gateway/fortran/intreadb.f \
sci_gateway/fortran/intfile.f

# Disable optimisation of the file intmgetl.f
noinst_LTLIBRARIES = libdummy-io.la
libdummy_io_la_SOURCES = sci_gateway/fortran/intmgetl.f
libdummy_io_la_FFLAGS = $(FFLAGS:-O%=)

libsciio_la_CFLAGS=  	-I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/

libsciio_la_CPPFLAGS= 	-I$(srcdir)/includes/ \
			-I$(top_srcdir)/modules/core/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/output_stream/includes/


pkglib_LTLIBRARIES = libsciio.la

libsciio_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libsciio_la_SOURCES = $(IO_C_SOURCES) $(IO_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(IO_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciio_la_CFLAGS)

libsciio_la_LIBADD = libdummy-io.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/tclsci/libscitclsci.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/integer/libsciinteger.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la


libdummy_io_la-intmgetl.lo: sci_gateway/fortran/intmgetl.f
	$(LIBTOOL) --tag=F77 --mode=compile $(F77) $(libdummy_io_la_FFLAGS) -c -o libdummy_io_la-intmgetl.lo `test -f 'sci_gateway/fortran/intmgetl.f' || echo '$(srcdir)/'`sci_gateway/fortran/intmgetl.f


#### Target ######
modulename=io

#### io : Conf files ####
libsciio_la_rootdir = $(mydatadir)
libsciio_la_root_DATA = changelog.txt license.txt readme.txt version.xml


####�io : init scripts ####
libsciio_la_etcdir = $(mydatadir)/etc
libsciio_la_etc_DATA = etc/io.quit etc/io.start

####�io : gateway declaration ####
libsciio_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciio_la_sci_gateway_DATA = sci_gateway/io_gateway.xml

include $(top_srcdir)/Makefile.incl.am
