##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

FILEIO_SOURCES = src/c/FileExist.c \
src/c/fileio.c \
src/c/do_xxprintf.c \
src/c/do_xxscanf.c \
src/c/findfiles.c \
src/c/Status2Mode.c \
src/c/addfile.c \
src/c/delfile.c \
src/c/filesmanagement.c \
src/c/getfiledesc.c \
src/c/getfileinfo.c \
src/c/getfiletype.c \
src/c/mclearerr.c \
src/c/mclose.c \
src/c/meof.c \
src/c/merror.c \
src/c/mget.c \
src/c/mgeti.c \
src/c/mgetstr.c \
src/c/mopen.c \
src/c/mput.c \
src/c/mputi.c \
src/c/mputstr.c \
src/c/mseek.c \
src/c/mtell.c \
src/c/diary.c \
src/c/readline.c \
src/c/getdrives.c \
src/c/deleteafile.c \
src/c/getshortpathname.c \
src/c/getlongpathname.c \
src/c/FindFileExtension.c \
src/c/URIFileToFilename.c \
src/c/createdirectory.c \
src/c/isdir.c \
src/c/isDrive.c \
src/c/removedir.c \
src/c/set_xxprintf.c 


GATEWAY_SOURCES = \
sci_gateway/c/gw_fileio.c \
sci_gateway/c/sci_fileio.c \
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
sci_gateway/c/sci_fullpath.c \
sci_gateway/c/sci_deletefile.c \
sci_gateway/c/sci_pathsep.c \
sci_gateway/c/sci_filesep.c \
sci_gateway/c/sci_getshortpathname.c \
sci_gateway/c/sci_getlongpathname.c \
sci_gateway/c/sci_fileext.c \
sci_gateway/c/sci_isdir.c \
sci_gateway/c/sci_removedir.c \
sci_gateway/c/sci_createdir.c

libscifileio_la_CFLAGS=		-Iincludes/ \
				-Isrc/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
                -I$(top_srcdir)/modules/string/includes

pkglib_LTLIBRARIES = libscifileio.la

libscifileio_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscifileio_la_SOURCES = $(FILEIO_SOURCES) $(GATEWAY_SOURCES)

# pb of order $(top_builddir)/modules/shell/libscishell.la
libscifileio_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la  $(top_builddir)/modules/string/libscistring.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/libs/libst/libscilibst.la
#### Target ######
modulename=fileio


#### fileio : Conf files ####
libscifileio_la_rootdir = $(mydatadir)
libscifileio_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### fileio : init scripts ####
libscifileio_la_etcdir = $(mydatadir)/etc
libscifileio_la_etc_DATA = etc/fileio.quit etc/fileio.start

#### fileio : gateway declaration ####
libscifileio_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifileio_la_sci_gateway_DATA = sci_gateway/fileio_gateway.xml

#### fileio : include files ####
libscifileio_la_includedir=$(pkgincludedir)/fileio
libscifileio_la_include_HEADERS = includes/FileExist.h \
includes/gw_fileio.h \
includes/findfiles.h \
includes/FindFileExtension.h \
includes/filesmanagement.h \
includes/URIFileToFilename.h \
includes/createdirectory.h \
includes/isdir.h \
includes/removedir.h \
includes/mopen.h \
includes/mtell.h \
includes/addfile.h \
includes/delfile.h \
includes/diary.h \
includes/getfiledesc.h \
includes/getfileinfo.h \
includes/getfiletype.h \
includes/mclose.h \
includes/mget.h \
includes/mput.h \
includes/mseek.h \
includes/deleteafile.h \
includes/readline.h

#### fileio : demos files ####
libscifileio_la_demosdir=$(mydatadir)
nobase_libscifileio_la_demos_DATA =

include $(top_srcdir)/Makefile.incl.am
