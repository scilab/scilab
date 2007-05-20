##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

FILEIO_SOURCES = src/c/FileExist.c \
src/c/xls.c \
src/c/fileio.c \
src/c/olestream-unwrap.c \
src/c/pldstr.c \
src/c/ole.c \
src/c/bytedecoders.c \
src/c/logger.c \
src/c/ripole.c \
src/c/bt-int.c \
src/c/do_xxprintf.c \
src/c/do_xxscanf.c \
src/c/findfiles.c \
src/c/Status2Mode.c \
src/c/addfile.c \
src/c/delfile.c \
src/c/getfiledesc.c \
src/c/getfileinfo.c \
src/c/getfiletype.c \
src/c/mclearerr.c \
src/c/mclose.c \
src/c/meof.c \
src/c/merror.c \
src/c/mget.c \
src/c/mgetstr.c \
src/c/mopen.c \
src/c/mput.c \
src/c/mputstr.c \
src/c/mseek.c \
src/c/mtell.c \
src/c/diary.c \
src/c/readline.c \
src/c/getdrives.c


GATEWAY_SOURCES = \
sci_gateway/c/sci_xls_read.c \
sci_gateway/c/gw_fileio.c \
sci_gateway/c/sci_fileio.c \
sci_gateway/c/sci_xls_open.c \
sci_gateway/c/intreadxls.c \
sci_gateway/c/sci_mfprintf.c \
sci_gateway/c/sci_mprintf.c \
sci_gateway/c/sci_msprintf.c \
sci_gateway/c/sci_mscanf.c \
sci_gateway/c/sci_msscanf.c \
sci_gateway/c/sci_mfscanf.c \
sci_gateway/c/sci_fprintfMat.c \
sci_gateway/c/sci_NumTokens.c \
sci_gateway/c/sci_fscanfMat.c \
sci_gateway/c/sci_findfiles.c \
sci_gateway/c/sci_maxfiles.c \
sci_gateway/c/sci_getdrives.c \
sci_gateway/c/sci_fullpath.c

libscifileio_la_CFLAGS=-Iincludes/ -Isrc/c/ -I$(top_srcdir)/libs/MALLOC/includes/ 

pkglib_LTLIBRARIES = libscifileio.la

libscifileio_la_SOURCES = $(FILEIO_SOURCES) $(GATEWAY_SOURCES)

libscifileio_la_LIBADD = \
	$(top_builddir)/libs/MALLOC/libscimalloc.la \
    $(top_builddir)/libs/libst/libscilibst.la

# cyclic deps
#	$(top_builddir)/modules/core/libscicore.la




#### Target ######
modulename=fileio


#### fileio : Conf files ####
libscifileio_la_rootdir = $(mydatadir)
libscifileio_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


#### fileio : init scripts #### 
libscifileio_la_etcdir = $(mydatadir)/etc
libscifileio_la_etc_DATA = etc/fileio.quit etc/fileio.start

#### fileio : gateway declaration #### 
libscifileio_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifileio_la_sci_gateway_DATA = sci_gateway/fileio_gateway.xml

#### fileio : include files ####
libscifileio_la_includedir=$(pkgincludedir)/fileio
libscifileio_la_include_HEADERS = includes/FileExist.h \
includes/gw_fileio.h \
includes/sci_fileio.h


#### fileio : demos files ####
libscifileio_la_demosdir=$(mydatadir)
nobase_libscifileio_la_demos_DATA = demos/excel/Testo1.xls \
demos/excel/o1.xls \
demos/excel/Test1.xls \
demos/excel/Testobig.xls \
demos/excel/Testbig.xls \
demos/excel/t1.xls \
demos/excel/README

include $(top_srcdir)/Makefile.incl.am
