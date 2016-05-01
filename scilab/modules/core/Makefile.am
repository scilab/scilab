# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006-2008 - INRIA - Sylvestre LEDRU
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.


CORE_C_SOURCES = \
    src/c/csignal.c \
    src/c/md5.c \
    src/c/texmacs.c \
    src/c/getmodules.c \
    src/c/getmemory.c \
    src/c/isanan.c \
    src/c/returnanan.c \
    src/c/loadversion.c \
    src/c/version.c \
    src/c/GetXmlFileEncoding.c \
    src/c/islittleendian.c \
    src/c/getcommandlineargs.c \
    src/c/commandwords.c \
    src/c/freeArrayOfString.c \
    src/c/getstaticdebuginfo.c \
    src/c/getdynamicdebuginfo.c \
    src/c/dynamic_gateways.c \
    src/c/setPrecisionFPU.c \
    src/c/getos.c \
    src/c/transposeMatrix.c \
    src/c/getversion.c \
    src/c/backtrace.c \
    src/c/signal_mgmt.c \
    src/c/suspendProcess.c \
    src/c/saveCWDInPreferences.c \
    src/c/getmaxMALLOC.c \
    src/c/sci_mem_alloc.c \
    src/c/hashtable/hashtable.c \
    src/c/hashtable/hashtable_utility.c \
    src/c/InitializePreferences.c

CORE_CPP_SOURCES = \
    src/cpp/demangle.cpp \
    src/cpp/backtrace_print.cpp \
    src/cpp/storeCommand.cpp \
    src/cpp/banner.cpp \
    src/cpp/with_module.cpp \
    src/cpp/runner.cpp \
    src/cpp/tasks.cpp \
    src/cpp/getvariablesname.cpp \
    src/cpp/getmacrosname.cpp \
    src/cpp/getfunctionsname.cpp \
    src/cpp/InitScilab.cpp \
    src/cpp/setformat.cpp

CORE_FORTRAN_SOURCES = \
    src/fortran/clunit.f \
    src/fortran/folhp.f \
    src/fortran/xerbla.f \
    src/fortran/expsum.f \
    src/fortran/readinter.f \
    src/fortran/rewindinter.f \
    src/fortran/backspaceinter.f \
	src/fortran/write_inter.f \
	src/fortran/read_inter.f

GATEWAY_C_SOURCES = \
	sci_gateway/c/sci_getdebuginfo.c

GATEWAY_CPP_SOURCES = \
	sci_gateway/cpp/core_gw.cpp \
	sci_gateway/cpp/sci_getmodules.cpp \
	sci_gateway/cpp/sci_clear.cpp \
	sci_gateway/cpp/sci_clearfun.cpp \
	sci_gateway/cpp/sci_newfun.cpp \
	sci_gateway/cpp/sci_funptr.cpp \
	sci_gateway/cpp/sci_banner.cpp \
	sci_gateway/cpp/sci_quit.cpp \
	sci_gateway/cpp/sci_exit.cpp \
	sci_gateway/cpp/sci_argn.cpp \
	sci_gateway/cpp/sci_clearglobal.cpp \
	sci_gateway/cpp/sci_global.cpp \
	sci_gateway/cpp/sci_isglobal.cpp \
	sci_gateway/cpp/sci_getscilabmode.cpp \
	sci_gateway/cpp/sci_getos.cpp \
	sci_gateway/cpp/sci_exists.cpp \
	sci_gateway/cpp/sci_with_module.cpp \
	sci_gateway/cpp/sci_lasterror.cpp \
	sci_gateway/cpp/sci_getversion.cpp \
	sci_gateway/cpp/sci_warning.cpp \
	sci_gateway/cpp/sci_getmd5.cpp \
	sci_gateway/cpp/sci_error.cpp \
	sci_gateway/cpp/sci_pause.cpp \
	sci_gateway/cpp/sci_abort.cpp \
	sci_gateway/cpp/sci_who.cpp \
	sci_gateway/cpp/sci_format.cpp \
	sci_gateway/cpp/sci_sciargs.cpp \
	sci_gateway/cpp/sci_ieee.cpp \
	sci_gateway/cpp/sci_funcprot.cpp \
	sci_gateway/cpp/sci_mode.cpp \
	sci_gateway/cpp/sci_typename.cpp \
	sci_gateway/cpp/sci_what.cpp \
	sci_gateway/cpp/sci_where.cpp \
	sci_gateway/cpp/sci_getmemory.cpp \
	sci_gateway/cpp/sci_errclear.cpp \
	sci_gateway/cpp/sci_intppty.cpp \
	sci_gateway/cpp/sci_analyzeroptions.cpp \
	sci_gateway/cpp/sci_macr2tree.cpp \
	sci_gateway/cpp/sci_predef.cpp \
	sci_gateway/cpp/sci_debug.cpp \
	sci_gateway/cpp/sci_recursionlimit.cpp \
	sci_gateway/cpp/sci_type.cpp \
	sci_gateway/cpp/sci_list.cpp \
	sci_gateway/cpp/sci_struct.cpp \
	sci_gateway/cpp/sci_insert.cpp \
	sci_gateway/cpp/sci_null.cpp \
	sci_gateway/cpp/sci_cell.cpp \
	sci_gateway/cpp/sci_makecell.cpp \
	sci_gateway/cpp/sci_typeof.cpp \
	sci_gateway/cpp/sci_tlist.cpp \
	sci_gateway/cpp/sci_mlist.cpp \
	sci_gateway/cpp/sci_isfield.cpp \
	sci_gateway/cpp/sci_oldEmptyBehaviour.cpp \
	sci_gateway/cpp/sci_fieldnames.cpp

if ENABLE_DEBUG
GATEWAY_CPP_SOURCES += \
    sci_gateway/cpp/sci_inspectorDeleteUnreferencedItems.cpp \
	sci_gateway/cpp/sci_inspectorGetItem.cpp \
	sci_gateway/cpp/sci_inspectorGetItemCount.cpp \
	sci_gateway/cpp/sci_inspectorGetUnreferencedItem.cpp \
	sci_gateway/cpp/sci_inspectorShowItem.cpp \
	sci_gateway/cpp/sci_inspectorShowUnreferencedItem.cpp \
	sci_gateway/cpp/sci_inspectorGetFunctionList.cpp
endif

libscicore_la_CPPFLAGS = \
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
    -I$(top_srcdir)/modules/call_scilab/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/action_binding/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/io/includes/ \
    -I$(top_srcdir)/modules/tclsci/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/history_manager/includes/ \
    -I$(top_srcdir)/modules/preferences/includes/ \
    -I$(top_srcdir)/modules/external_objects/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/jvm/src/c/ \
    -I$(top_srcdir)/modules/gui/includes/ \
    -I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/hdf5/includes/ \
    -I$(top_srcdir)/modules/fftw/includes/ \
    -I$(top_srcdir)/modules/ui_data/includes/ \
    $(XML_FLAGS) \
    $(AM_CPPFLAGS)

# Used by sci_getdebuginfo:
if TCLTK
libscicore_la_CPPFLAGS += $(TCL_INC_PATH) \
		                  $(TK_INC_PATH)
endif

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscicore.la
noinst_LTLIBRARIES = libscicore-algo.la
else
noinst_LTLIBRARIES = libscicore-algo.la libscicore.la
endif


libscicore_la_LDFLAGS = $(AM_LDFLAGS)
#-Wl,--version-script,LibScilab_Import.def

# For the code check (splint)
CHECK_SRC= $(CORE_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscicore_la_CPPFLAGS)

libscicore_algo_la_SOURCES = $(CORE_C_SOURCES) $(CORE_CPP_SOURCES) $(CORE_FORTRAN_SOURCES)
libscicore_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscicore_algo_la_CPPFLAGS = $(libscicore_la_CPPFLAGS)

# Commented because it is easier to comment first the core module
libscicore_la_LIBADD = libscicore-algo.la $(XML_LIBS)

#### Target ######
modulename=core

#### core : Conf files ####
libscicore_la_rootdir = $(mydatadir)
libscicore_la_root_DATA =  license.txt


#### core : init scripts ####
libscicore_la_etcdir = $(mydatadir)/etc
libscicore_la_etc_DATA = etc/core.quit etc/core.start

#### core : gateway declaration ####
libscicore_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicore_la_sci_gateway_DATA = sci_gateway/core_gateway.xml

#### core :  dtd files ####
libscicore_la_xmldir = $(mydatadir)/xml
libscicore_la_xml_DATA = xml/modules.dtd xml/version.dtd

#### core : include files ####
libscicore_la_includedir=$(pkgincludedir)
libscicore_la_include_HEADERS = \
includes/backtrace_print.h \
includes/BOOL.h \
includes/core_math.h \
includes/existfunction.h \
includes/exit_status.hxx \
includes/freeArrayOfString.h \
includes/getcommandlineargs.h \
includes/getmemory.h \
includes/getos.h \
includes/getversion.h \
includes/hashtable.h \
includes/hashtable_utility.h \
includes/machine.h \
includes/PATH_MAX.h \
includes/scilabDefaults.h \
includes/sci_malloc.h \
includes/sci_mem_alloc.h \
includes/scisparse.h \
includes/sci_types.h \
includes/storeCommand.h \
includes/timer.hxx \
includes/version.h


if GUI
USEANT=1
endif


include $(top_srcdir)/Makefile.incl.am
