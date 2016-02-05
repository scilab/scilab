# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

if TCLTK

TCLSCI_C_SOURCES = \
    src/c/TCL_Global.c \
    src/c/InitTclTk.c \
    src/c/setenvtcl.c \
    src/c/InitializeTclTk.c \
    src/c/TerminateTclTk.c \
    src/c/setvar.c \
    src/c/TCL_ArrayDim.c \
    src/c/TCL_ArrayExist.c \
    src/c/TCL_ArrayGetVar.c \
    src/c/TCL_ArraySize.c \
    src/c/TCL_Command.c \
    src/c/GlobalTclInterp.c \
    src/c/TCL_getErrorLine.c \
    src/c/LoadUnloadTclsci.c

TCLSCI_CPP_SOURCES = \
    src/cpp/ScilabEval.cpp

GATEWAY_CPP_SOURCES = \
	sci_gateway/cpp/tclsci_gw.cpp

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_opentk.c \
    sci_gateway/c/sci_TCL_SetVar.c \
    sci_gateway/c/sci_TCL_ExistVar.c \
    sci_gateway/c/sci_TCL_DoOneEvent.c \
    sci_gateway/c/sci_TCL_UnsetVar.c \
    sci_gateway/c/sci_TCL_ExistInterp.c \
    sci_gateway/c/sci_TCL_EvalStr.c \
    sci_gateway/c/sci_TCL_DeleteInterp.c \
    sci_gateway/c/sci_TCL_GetVersion.c \
    sci_gateway/c/sci_TCL_EvalFile.c \
    sci_gateway/c/sci_TCL_GetVar.c \
    sci_gateway/c/sci_TCL_ExistArray.c \
    sci_gateway/c/sci_TCL_UpVar.c

else

TCLSCI_C_SOURCES =
TCLSCI_CPP_SOURCES = src/notclsci/notclsci.cpp
GATEWAY_CPP_SOURCES =
GATEWAY_C_SOURCES =

endif

libscitclsci_la_LDFLAGS = $(AM_LDFLAGS)

libscitclsci_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c \
    -I$(srcdir)/sci_gateway/c/ \
    $(TCL_INC_PATH) \
    $(TK_INC_PATH) \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/action_binding/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/call_scilab/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    $(AM_CPPFLAGS)

# Without the TCLSCI module
libscitclsci_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    $(AM_CPPFLAGS)

libscitclsci_algo_la_SOURCES = $(TCLSCI_CPP_SOURCES) $(TCLSCI_C_SOURCES)
libscitclsci_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscitclsci_algo_la_CPPFLAGS = $(libscitclsci_la_CPPFLAGS)

TCLSCI_DISABLE_CPP_SOURCES = src/notclsci/notclsci.cpp
libscitclsci_disable_la_SOURCES = $(TCLSCI_DISABLE_CPP_SOURCES)

pkglib_LTLIBRARIES = libscitclsci-disable.la libscitclsci.la
noinst_LTLIBRARIES = libscitclsci-algo.la


# For the code check (splint)
CHECK_SRC= $(libscitclsci_la_SOURCES)
INCLUDE_FLAGS = $(libscitclsci_la_CPPFLAGS)

libscitclsci_la_LIBADD = libscitclsci-algo.la
if TCLTK
libscitclsci_la_LIBADD += $(TCLTK_LIBS)
endif

#### Target ######
modulename=tclsci

#### tclsci : Conf files ####
libscitclsci_la_rootdir = $(mydatadir)
libscitclsci_la_root_DATA =  license.txt

#### tclsci : init scripts ####
libscitclsci_la_etcdir = $(mydatadir)/etc
libscitclsci_la_etc_DATA = \
    etc/tclsci.quit \
    etc/tclsci.start

### TCL macros ###
libscitclsci_la_tcldir = $(mydatadir)
nobase_libscitclsci_la_tcl_DATA = \
tcl/utils/Notebook.tcl \
tcl/utils/Balloon.tcl \
tcl/utils/Combobox.tcl \
tcl/sciGUI/data/initial.help \
tcl/sciGUI/data/dontshow.txt \
tcl/sciGUI/data/tp00.gif \
tcl/sciGUI/data/tp01.gif \
tcl/sciGUI/data/tp10.gif \
tcl/sciGUI/data/tp02.gif \
tcl/sciGUI/data/initial \
tcl/sciGUI/data/tp04.gif \
tcl/sciGUI/data/giftable.txt \
tcl/sciGUI/data/ndemo_tx01.txt \
tcl/sciGUI/data/tp05.gif \
tcl/sciGUI/data/error.gif \
tcl/sciGUI/data/tp06.gif \
tcl/sciGUI/data/tp15.gif \
tcl/sciGUI/data/tp16.gif \
tcl/sciGUI/data/info.gif \
tcl/sciGUI/data/tp08.gif \
tcl/sciGUI/data/tp17.gif \
tcl/sciGUI/data/scilab01.gif \
tcl/sciGUI/data/passwd.gif \
tcl/sciGUI/data/warning.gif \
tcl/sciGUI/data/question.gif \
tcl/sciGUI/data/hourglass.gif \
tcl/sciGUI/data/icontable.txt \
tcl/sciGUI/local_extra/combobox-2.3/ANNOUNCE.txt \
tcl/sciGUI/local_extra/combobox-2.3/combobox.tcl \
tcl/sciGUI/local_extra/combobox-2.3/README.txt \
tcl/sciGUI/local_extra/combobox-2.3/combobox.html \
tcl/sciGUI/local_extra/combobox-2.3/example.tcl \
tcl/sciGUI/local_extra/balloonhelp.tcl \
tcl/sciGUI/local_extra/rnotebook.tcl \
tcl/sciGUI/local_extra/spinbutton.tcl \
tcl/sciGUI/license.txt \
tcl/sciGUI/sciGUI.tcl \
tcl/sciGUI/sciBrowseVar.tcl \
tcl/sciGUI/sciEditVar.tcl \
tcl/TK_Scilab.tcl

#### tclsci : gateway declaration ####
libscitclsci_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitclsci_la_sci_gateway_DATA = sci_gateway/tclsci_gateway.xml

include $(top_srcdir)/Makefile.incl.am

