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
src/bt-int.c \
src/do_xxprintf.c \
src/do_xxscanf.c

GATEWAY_SOURCES = \
	sci_gateway/sci_xls_read.c \
	sci_gateway/gw_fileio.c \
	sci_gateway/sci_fileio.c \
	sci_gateway/sci_xls_open.c \
	sci_gateway/intreadxls.c \
	sci_gateway/sci_mfprintf.c \
	sci_gateway/sci_mprintf.c \
	sci_gateway/sci_msprintf.c \
	sci_gateway/sci_mscanf.c \
	sci_gateway/sci_msscanf.c \
	sci_gateway/sci_mfscanf.c \
	sci_gateway/sci_fprintfMat.c \
	sci_gateway/sci_NumTokens.c \
	sci_gateway/sci_fscanfMat.c


libfileio_la_CFLAGS=-Iincludes/ -Isrc/ -I$(top_srcdir)/libs/MALLOC/includes/ 

lib_LTLIBRARIES = libfileio.la

libfileio_la_SOURCES = $(FILEIO_SOURCES) $(GATEWAY_SOURCES)

libfileio_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libmalloc.la
# cyclic deps
#	$(top_builddir)/modules/core/libcore.la

libfileio_la_PKGCONFIG = fileio.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/fileio


#### fileio : Conf files ####
libfileio_la_rootdir = $(target)
libfileio_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### fileio : Scilab macros ###
libfileio_la_macrodir = $(target)/macros
libfileio_la_macro_DATA22 = macros/fullfile.sci \
macros/readxls.sci \
macros/mkdir.sci \
macros/isdir.sci \
macros/copyfile.sci \
macros/listvarinfile.sci \
macros/basename.sci \
macros/rmdir.sci \
macros/dir.sci \
macros/ls.sci \
macros/dirname.sci \
macros/mdelete.sci \
macros/excel2sci.sci \
macros/sci2excel.sci \
macros/dispfiles.sci \
macros/cd.sci \
macros/format_txt.sci \
macros/get_absolute_file_path.sci \
macros/fileparts.sci \
macros/get_file_path.sci \
macros/scanf.sci \
macros/pathconvert.sci \
macros/listfiles.sci \
macros/savematfile.sci \
macros/mputl.sci macros/basename.bin \
macros/cd.bin \
macros/copyfile.bin \
macros/dirname.bin \
macros/dir.bin \
macros/dispfiles.bin \
macros/excel2sci.bin \
macros/fileparts.bin \
macros/format_txt.bin \
macros/fullfile.bin \
macros/get_absolute_file_path.bin \
macros/get_file_path.bin \
macros/isdir.bin \
macros/listfiles.bin \
macros/listvarinfile.bin \
macros/ls.bin \
macros/mdelete.bin \
macros/mkdir.bin \
macros/mputl.bin \
macros/pathconvert.bin \
macros/readxls.bin \
macros/rmdir.bin \
macros/savematfile.bin \
macros/scanf.bin \
macros/sci2excel.bin macros/names macros/lib macros/buildmacros.sce


#### fileio : init scripts #### 
libfileio_la_etcdir = $(target)/etc
libfileio_la_etc_DATA = etc/fileio.quit etc/fileio.start

#### fileio : gateway declaration #### 
libfileio_la_sci_gatewaydir = $(target)/sci_gateway
libfileio_la_sci_gateway_DATA = sci_gateway/fileio

#### fileio : include files ####
libfileio_la_includedir=$(includedir)/fileio
libfileio_la_include_HEADERS = includes/FileExist.h \
includes/gw_fileio.h \
includes/sci_fileio.h


#### fileio : demos files ####
libfileio_la_demosdir=$(target)
nobase_libfileio_la_demos_DATA = demos/excel/Testo1.xls \
demos/excel/o1.xls \
demos/excel/Test1.xls \
demos/excel/Testobig.xls \
demos/excel/Testbig.xls \
demos/excel/t1.xls \
demos/excel/README

include $(top_srcdir)/Makefile.incl.am
