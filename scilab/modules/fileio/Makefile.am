# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


FILEIO_C_SOURCES = \
    src/c/FileExist.c \
    src/c/scanf_functions.c \
    src/c/do_xxscanf.c \
    src/c/findfiles.c \
    src/c/Status2Mode.c \
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
    src/c/basename.c \
    src/c/pathconvert.c \
    src/c/copyfile_others.c \
    src/c/getFullFilename.c \
    src/c/os_wfopen.c \
    src/c/fscanfMat.c \
    src/c/fprintfMat.c \
    src/c/createtempfilename.c \
	src/c/dlManager.c \
	src/c/convert_tools.c

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_fprintfMat.c \
    sci_gateway/c/sci_fscanfMat.c \
    sci_gateway/c/sci_chdir.c \
    sci_gateway/c/sci_pwd.c \
    sci_gateway/c/sci_copyfile.c \
    sci_gateway/c/sci_tempname.c \
    sci_gateway/c/sci_movefile.c \
	sci_gateway/c/sci_getURL.c \
	sci_gateway/c/sci_splitURL.c

GATEWAY_CPP_SOURCES = \
	sci_gateway/cpp/fileio_gw.cpp \
	sci_gateway/cpp/sci_mopen.cpp \
	sci_gateway/cpp/sci_mclose.cpp \
	sci_gateway/cpp/sci_mget.cpp \
	sci_gateway/cpp/sci_mput.cpp \
	sci_gateway/cpp/sci_mputl.cpp \
	sci_gateway/cpp/sci_mgetl.cpp \
	sci_gateway/cpp/sci_pathconvert.cpp \
	sci_gateway/cpp/sci_isdir.cpp \
	sci_gateway/cpp/sci_filesep.cpp \
	sci_gateway/cpp/sci_fileparts.cpp \
	sci_gateway/cpp/sci_findfiles.cpp \
	sci_gateway/cpp/sci_basename.cpp \
	sci_gateway/cpp/sci_fileinfo.cpp \
	sci_gateway/cpp/sci_isfile.cpp \
	sci_gateway/cpp/sci_deletefile.cpp \
	sci_gateway/cpp/sci_getshortpathname.cpp \
	sci_gateway/cpp/sci_getlongpathname.cpp \
	sci_gateway/cpp/sci_fileext.cpp \
	sci_gateway/cpp/sci_createdir.cpp \
	sci_gateway/cpp/sci_fullpath.cpp \
	sci_gateway/cpp/sci_removedir.cpp \
	sci_gateway/cpp/sci_meof.cpp \
	sci_gateway/cpp/sci_mputstr.cpp \
	sci_gateway/cpp/sci_mgetstr.cpp \
	sci_gateway/cpp/sci_mseek.cpp \
	sci_gateway/cpp/sci_mtell.cpp \
	sci_gateway/cpp/sci_newest.cpp \
	sci_gateway/cpp/sci_merror.cpp \
	sci_gateway/cpp/sci_mclearerr.cpp \
	sci_gateway/cpp/sci_get_absolute_file_path.cpp \
	sci_gateway/cpp/sci_getrelativefilename.cpp \
	sci_gateway/cpp/sci_pathsep.cpp \
	sci_gateway/cpp/sci_mfprintf.cpp \
	sci_gateway/cpp/sci_mfscanf.cpp \
	sci_gateway/cpp/sci_mscanf.cpp \
	sci_gateway/cpp/sci_msscanf.cpp \
	sci_gateway/cpp/sci_getdrives.cpp

FILEIO_CPP_SOURCES = \
	src/cpp/expandPathVariable.cpp \
	src/cpp/filemanager_interface.cpp \
	src/cpp/mopen.cpp \
	src/cpp/mclose.cpp \
	src/cpp/mput.cpp \
	src/cpp/mget.cpp \
	src/cpp/mputl.cpp \
	src/cpp/mgetl.cpp \
	src/cpp/mgetstr.cpp \
	src/cpp/filemanager.cpp \
	src/cpp/mseek.cpp \
	src/cpp/mtell.cpp \
	src/cpp/newest.cpp \
	src/cpp/scilab_sscanf.cpp \
    src/cpp/addfile.cpp \
    src/cpp/delfile.cpp \
    src/cpp/getfiledesc.cpp \
    src/cpp/getfileinfo.cpp \
    src/cpp/getfiletype.cpp

libscifileio_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
	-I$(srcdir)/src/c/ \
	-I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/functions_manager/includes \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/io/includes \
	-I$(top_srcdir)/modules/threads/includes \
	-I$(top_srcdir)/modules/console/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/dynamic_link/includes \
	-I$(top_srcdir)/modules/preferences/includes \
	$(HDF5_CFLAGS) \
	$(XML_FLAGS) \
	$(CURL_CFLAGS) \
	$(AM_CPPFLAGS)


if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscifileio.la
noinst_LTLIBRARIES = libscifileio-algo.la
else
noinst_LTLIBRARIES = libscifileio-algo.la libscifileio.la
endif



libscifileio_algo_la_SOURCES = $(FILEIO_C_SOURCES) $(FILEIO_CPP_SOURCES) $(FILEIO_FORTRAN_SOURCES)
libscifileio_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscifileio_algo_la_CPPFLAGS = $(libscifileio_la_CPPFLAGS)
libscifileio_algo_la_LDFLAGS = $(CURL_LIBS)

# For the code check (splint)
CHECK_SRC= $(FILEIO_C_SOURCES) $(FILEIO_CPP_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
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
