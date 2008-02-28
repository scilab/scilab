##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

if TCLTK

TCLSCI_C_SOURCES = src/c/GedManagement.c \
src/c/TCL_Global.c \
src/c/InitTclTk.c \
src/c/ScilabEval.c \
src/c/setenvtcl.c \
src/c/InitializeTclTk.c \
src/c/TerminateTclTk.c \
src/c/withtk.c \
src/c/setvar.c \
src/c/TCL_ArrayDim.c \
src/c/TCL_ArrayExist.c \
src/c/TCL_ArrayGetVar.c \
src/c/TCL_ArraySize.c \
src/c/TCL_Command.c \
src/c/GlobalTclInterp.c

GATEWAY_C_SOURCES =  sci_gateway/c/sci_TCL_CreateSlave.c \
sci_gateway/c/sci_opentk.c \
sci_gateway/c/sci_TCL_SetVar.c \
sci_gateway/c/sci_TCL_ExistVar.c \
sci_gateway/c/sci_TCL_DoOneEvent.c \
sci_gateway/c/sci_TCL_UnsetVar.c \
sci_gateway/c/sci_TCL_ExistInterp.c \
sci_gateway/c/gw_tclsci.c \
sci_gateway/c/sci_TCL_EvalStr.c \
sci_gateway/c/sci_TCL_DeleteInterp.c \
sci_gateway/c/sci_TCL_GetVersion.c \
sci_gateway/c/sci_TCL_EvalFile.c \
sci_gateway/c/sci_TCL_GetVar.c \
sci_gateway/c/sci_TCL_ExistArray.c \
sci_gateway/c/sci_TCL_UpVar.c

pkglib_LTLIBRARIES = libscitclsci.la

libscitclsci_la_CFLAGS= -Iincludes/ \
			-Isrc/c \
			-Isci_gateway/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			$(TCL_INC_PATH) \
			$(TK_INC_PATH) \
			-I$(top_srcdir)/modules/action_binding/includes \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes

libscitclsci_la_LDFLAGS= $(TCLTK_LIBS)

libscitclsci_la_SOURCES = $(TCLSCI_C_SOURCES) $(GATEWAY_C_SOURCES)

libscitclsci_la_LIBADD =  $(top_builddir)/modules/action_binding/libsciaction_binding.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Target ######
modulename=tclsci


#### tclsci : Conf files ####
libscitclsci_la_rootdir = $(mydatadir)

#### tclsci : init scripts ####
libscitclsci_la_etcdir = $(mydatadir)/etc
libscitclsci_la_etc_DATA = etc/tclsci.quit etc/tclsci.start

include $(top_srcdir)/Makefile.incl.am
endif

