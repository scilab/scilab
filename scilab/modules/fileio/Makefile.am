# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


FILEIO_SOURCES = src/c/FileExist.c \
src/c/scanf_functions.c \
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
src/c/mgetl.c \
src/c/mgeti.c \
src/c/mgetstr.c \
src/c/mopen.c \
src/c/mput.c \
src/c/mputi.c \
src/c/mputstr.c \
src/c/mseek.c \
src/c/mtell.c \
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
src/c/cluni0.c \
src/c/scicurdir.c \
src/c/fullpath.c \
src/c/fileinfo.c \
src/c/getrelativefilename.c \
src/c/splitpath.c \
src/c/movefile.c \
src/c/expandPathVariable.c \
src/c/basename.c \
src/c/pathconvert.c \
src/c/copyfile_others.c \
src/c/getFullFilename.c \
src/c/mputl.c \
src/c/fscanfMat.c \
src/c/fprintfMat.c \
src/c/createtempfilename.c \
src/c/convert_tools.c


GATEWAY_SOURCES = \
sci_gateway/c/gw_fileio.c \
sci_gateway/c/sci_mfprintf.c \
sci_gateway/c/sci_mscanf.c \
sci_gateway/c/sci_msscanf.c \
sci_gateway/c/sci_mfscanf.c \
sci_gateway/c/sci_fprintfMat.c \
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
sci_gateway/c/sci_createdir.c \
sci_gateway/c/sci_chdir.c \
sci_gateway/c/sci_pwd.c \
sci_gateway/c/sci_mclearerr.c \
sci_gateway/c/sci_mclose.c \
sci_gateway/c/sci_merror.c \
sci_gateway/c/sci_mget.c \
sci_gateway/c/sci_mgetl.c \
sci_gateway/c/sci_mgetstr.c \
sci_gateway/c/sci_moef.c \
sci_gateway/c/sci_mopen.c \
sci_gateway/c/sci_mput.c \
sci_gateway/c/sci_mputstr.c \
sci_gateway/c/sci_mseek.c \
sci_gateway/c/sci_mtell.c \
sci_gateway/c/sci_fileinfo.c \
sci_gateway/c/sci_newest.c \
sci_gateway/c/sci_get_absolute_file_path.c  \
sci_gateway/c/sci_getrelativefilename.c \
sci_gateway/c/sci_copyfile.c \
sci_gateway/c/sci_fileparts.c \
sci_gateway/c/sci_movefile.c \
sci_gateway/c/sci_basename.c \
sci_gateway/c/sci_pathconvert.c \
sci_gateway/c/sci_isfile.c \
sci_gateway/c/sci_mputl.c \
sci_gateway/c/sci_tempname.c


libscifileio_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/io/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/string/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscifileio.la
noinst_LTLIBRARIES = libscifileio-algo.la
else
noinst_LTLIBRARIES = libscifileio-algo.la libscifileio.la
endif



libscifileio_algo_la_SOURCES = $(FILEIO_SOURCES)
libscifileio_la_SOURCES = $(GATEWAY_SOURCES) $(GATEWAY_CXX_SOURCES)
libscifileio_algo_la_CPPFLAGS = $(libscifileio_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(FILEIO_SOURCES) $(GATEWAY_SOURCES)
INCLUDE_FLAGS = $(libscifileio_la_CPPFLAGS)

# pb of order $(top_builddir)/modules/console/libsciconsole.la
libscifileio_la_LIBADD = libscifileio-algo.la

#### Target ######
modulename=fileio


#### fileio : Conf files ####
libscifileio_la_rootdir = $(mydatadir)
libscifileio_la_root_DATA =  license.txt 


#### fileio : init scripts ####
libscifileio_la_etcdir = $(mydatadir)/etc
libscifileio_la_etc_DATA = etc/fileio.quit etc/fileio.start

#### fileio : gateway declaration ####
libscifileio_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifileio_la_sci_gateway_DATA = sci_gateway/fileio_gateway.xml

#### fileio : include files ####
libscifileio_la_includedir=$(pkgincludedir)
libscifileio_la_include_HEADERS = includes/getFullFilename.h \
includes/dynlib_fileio.h

include $(top_srcdir)/Makefile.incl.am
