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
sci_gateway/fortran/intdiary.f \
sci_gateway/fortran/intread.f \
sci_gateway/fortran/intgetpid.f \
sci_gateway/fortran/inthost.f \
sci_gateway/fortran/intexecstr.f \
sci_gateway/fortran/intreadb.f \
sci_gateway/fortran/intfile.f

# Disable optimisation of the file intmgetl.f
noinst_LTLIBRARIES = libdummy-io.la
libdummy_io_la_SOURCES = sci_gateway/fortran/intmgetl.f
libdummy_io_la_FFLAGS = $(FFLAGS:-O%=)

libio_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libio.la

libio_la_SOURCES = $(IO_C_SOURCES) $(IO_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libio_la_LIBADD = libdummy-io.la \
	$(top_builddir)/modules/sparse/libsparse.la \
	$(top_builddir)/modules/fileio/libfileio.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libio_la_PKGCONFIG = io.pc

libdummy_io_la-intmgetl.lo: sci_gateway/fortran/intmgetl.f
	$(LIBTOOL) --tag=F77 --mode=compile $(F77) $(libdummy_io_la_FFLAGS) -c -o libdummy_io_la-intmgetl.lo `test -f 'sci_gateway/fortran/intmgetl.f' || echo '$(srcdir)/'`sci_gateway/fortran/intmgetl.f


datadir=${prefix}/modules
#### Target ######
target=$(datadir)/io


#### io : Conf files ####
libio_la_rootdir = $(target)
libio_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### io : Scilab macros ###
libio_la_macrodir = $(target)/macros
libio_la_macro_DATA = macros/printf.sci \
macros/ssprint.sci \
macros/getscilabkeywords.sci \
macros/sprintf.sci \
macros/unix_s.sci \
macros/fprintf.sci \
macros/halt.sci \
macros/unix_g.sci \
macros/unix_w.sci \
macros/unix_x.sci \
macros/readc_.sci \
macros/sscanf.sci \
macros/input.sci \
macros/fscanf.sci macros/fprintf.bin \
macros/fscanf.bin \
macros/getscilabkeywords.bin \
macros/halt.bin \
macros/input.bin \
macros/printf.bin \
macros/readc_.bin \
macros/sprintf.bin \
macros/sscanf.bin \
macros/ssprint.bin \
macros/unix_g.bin \
macros/unix_s.bin \
macros/unix_w.bin \
macros/unix_x.bin macros/names macros/lib macros/buildmacros.sce


#### io : init scripts #### 
libio_la_etcdir = $(target)/etc
libio_la_etc_DATA = etc/io.quit etc/io.start

#### io : gateway declaration #### 
libio_la_sci_gatewaydir = $(target)/sci_gateway
libio_la_sci_gateway_DATA = sci_gateway/io


#### io : documentation FR #### 
libio_la_helpfrdir = $(target)/help/fr
libio_la_helpfr_DATA = help/fr/fileinfo.htm \
help/fr/disp.htm \
help/fr/keyboard.htm \
help/fr/file.htm \
help/fr/halt.htm \
help/fr/load.htm \
help/fr/getscilabkeywords.htm \
help/fr/input.htm \
help/fr/lib.htm \
help/fr/mgetl.htm \
help/fr/newest.htm \
help/fr/oldload.htm \
help/fr/oldsave.htm \
help/fr/printf.htm \
help/fr/printf_conversion.htm \
help/fr/print.htm \
help/fr/read4b.htm \
help/fr/readb.htm \
help/fr/readc_.htm \
help/fr/read.htm \
help/fr/save.htm \
help/fr/writb.htm \
help/fr/write4b.htm \
help/fr/write.htm \
help/fr/execstr.htm \
help/fr/getenv.htm \
help/fr/getf.htm \
help/fr/whatis.htm \
help/fr/getio.htm \
help/fr/deff.htm \
help/fr/diary.htm \
help/fr/getrelativefilename.htm \
help/fr/getpid.htm \
help/fr/host.htm \
help/fr/setenv.htm \
help/fr/unix_g.htm \
help/fr/unix_s.htm \
help/fr/unix_w.htm \
help/fr/unix.htm \
help/fr/unix_x.htm


#### io : documentation EN #### 
libio_la_helpendir = $(target)/help/eng
libio_la_helpen_DATA = help/eng/whatis.htm \
help/eng/fileinfo.htm \
help/eng/deff.htm \
help/eng/diary.htm \
help/eng/disp.htm \
help/eng/execstr.htm \
help/eng/keyboard.htm \
help/eng/file.htm \
help/eng/getenv.htm \
help/eng/getf.htm \
help/eng/getio.htm \
help/eng/getpid.htm \
help/eng/halt.htm \
help/eng/host.htm \
help/eng/getrelativefilename.htm \
help/eng/getscilabkeywords.htm \
help/eng/input.htm \
help/eng/load.htm \
help/eng/lib.htm \
help/eng/mgetl.htm \
help/eng/newest.htm \
help/eng/oldload.htm \
help/eng/oldsave.htm \
help/eng/printf.htm \
help/eng/printf_conversion.htm \
help/eng/print.htm \
help/eng/read4b.htm \
help/eng/readb.htm \
help/eng/readc_.htm \
help/eng/read.htm \
help/eng/save.htm \
help/eng/setenv.htm \
help/eng/unix_g.htm \
help/eng/unix_s.htm \
help/eng/unix_w.htm \
help/eng/unix.htm \
help/eng/unix_x.htm \
help/eng/writb.htm \
help/eng/write4b.htm \
help/eng/write.htm


#### io : include files ####
libio_la_includedir=$(includedir)/io
libio_la_include_HEADERS = includes/sci_getrelativefilename.h \
includes/gw_io.h

