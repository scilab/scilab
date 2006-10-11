##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

IO_C_SOURCES = src/c/getpidc.c \
src/c/setenvc.c \
src/c/systemc.c \
src/c/getenvc.c

IO_FORTRAN_SOURCES = src/fortran/v2unit.f \
src/fortran/newsave.f \
src/fortran/v2cunit.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_execstr.c \
sci_gateway/c/sci_read4b.c \
sci_gateway/c/sci_deff.c \
sci_gateway/c/sci_lib.c \
sci_gateway/c/sci_getio.c \
sci_gateway/c/sci_readb.c \
sci_gateway/c/sci_host.c \
sci_gateway/c/sci_getrelativefilename.c \
sci_gateway/c/sci_fileinfo.c \
sci_gateway/c/sci_file.c \
sci_gateway/c/intsetenv.c \
sci_gateway/c/sci_exec.c \
sci_gateway/c/sci_rat.c \
sci_gateway/c/sci_getenv.c \
sci_gateway/c/sci_write.c \
sci_gateway/c/sci_oldload.c \
sci_gateway/c/sci_writeb.c \
sci_gateway/c/sci_print.c \
sci_gateway/c/sci_setenv.c \
sci_gateway/c/gw_io.c \
sci_gateway/c/sci_mgetl.c \
sci_gateway/c/sci_diary.c \
sci_gateway/c/sci_read.c \
sci_gateway/c/sci_getpid.c \
sci_gateway/c/sci_write4b.c \
sci_gateway/c/sci_load.c \
sci_gateway/c/intfilestat.c \
sci_gateway/c/sci_unix.c \
sci_gateway/c/sci_getf.c \
sci_gateway/c/sci_oldsave.c \
sci_gateway/c/sci_save.c \
sci_gateway/c/sci_disp.c \
sci_gateway/c/sci_newest.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/oldloadsave.f \
sci_gateway/fortran/intwrite4b.f \
sci_gateway/fortran/intexec.f \
sci_gateway/fortran/intgetf.f \
sci_gateway/fortran/intrat.f \
sci_gateway/fortran/intwritb.f \
sci_gateway/fortran/intgetenv.f \
sci_gateway/fortran/intwrite.f \
sci_gateway/fortran/intprint.f \
sci_gateway/fortran/intdisp.f \
sci_gateway/fortran/intread4b.f \
sci_gateway/fortran/intdeff.f \
sci_gateway/fortran/intlib.f \
sci_gateway/fortran/intgetio.f \
sci_gateway/fortran/intmgetl.f \
sci_gateway/fortran/intdiary.f \
sci_gateway/fortran/intread.f \
sci_gateway/fortran/intgetpid.f \
sci_gateway/fortran/inthost.f \
sci_gateway/fortran/intexecstr.f \
sci_gateway/fortran/intreadb.f \
sci_gateway/fortran/intfile.f

libio_la_includedir=$(includedir)/io

libio_la_include_HEADERS = includes/gw_io.h \
includes/sci_getrelativefilename.h

libio_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libio.la

libio_la_SOURCES = $(IO_C_SOURCES) $(IO_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libio_la_LIBADD = \
	$(top_builddir)/modules/sparse/libsparse.la \
	$(top_builddir)/modules/fileio/libfileio.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libio_la_PKGCONFIG = io.pc
