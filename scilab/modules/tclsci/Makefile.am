##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

TCLSCI_C_SOURCES = src/GedManagement.c \
src/TclUiGet.c \
src/TCL_Global.c \
src/tksynchro.c \
src/InitTclTk.c \
src/ScilabEval.c \
src/TclEvents.c \
src/setenvtcl.c \
src/with_tk.c \
src/EnvTclTk.c \
src/TclUiSet.c

GATEWAY_C_SOURCES =  sci_gateway/sci_TCL_CreateSlave.c \
sci_gateway/sci_opentk.c \
sci_gateway/sci_TCL_gcf.c \
sci_gateway/sci_close.c \
sci_gateway/sci_TCL_SetVar.c \
sci_gateway/sci_TCL_ExistVar.c \
sci_gateway/sci_TCL_DoOneEvent.c \
sci_gateway/sci_TCL_UnsetVar.c \
sci_gateway/sci_findobj.c \
sci_gateway/sci_get.c \
sci_gateway/sci_TCL_ExistInterp.c \
sci_gateway/gw_tclsci.c \
sci_gateway/sci_TCL_EvalStr.c \
sci_gateway/sci_TCL_DeleteInterp.c \
sci_gateway/sci_TCL_GetVersion.c \
sci_gateway/sci_TCL_EvalFile.c \
sci_gateway/sci_TCL_GetVar.c \
sci_gateway/sci_set.c \
sci_gateway/sci_TCL_ExistArray.c \
sci_gateway/sci_TCL_UpVar.c

libtclsci_la_includedir=$(includedir)/tclsci

libtclsci_la_include_HEADERS = includes/GedManagement.h \
includes/sci_set.h \
includes/TclUiSet.h \
includes/sci_TCL_ExistArray.h \
includes/sci_TCL_ExistVar.h \
includes/sci_TCL_UpVar.h \
includes/sci_opentk.h \
includes/Errors.h \
includes/sci_TCL_CreateSlave.h \
includes/sci_TCL_gcf.h \
includes/TCL_Global.h \
includes/sci_close.h \
includes/sci_TCL_SetVar.h \
includes/ScilabEval.h \
includes/InitTclTk.h \
includes/sci_TCL_ExistInterp.h \
includes/sci_TCL_DoOneEvent.h \
includes/TclEvents.h \
includes/Messages.h \
includes/sci_TCL_UnsetVar.h \
includes/sci_findobj.h \
includes/sci_get.h \
includes/gw_tclsci.h \
includes/sci_TCL_DeleteInterp.h \
includes/sci_TCL_EvalStr.h \
includes/TclUiGet.h \
includes/sci_TCL_GetVersion.h \
includes/sci_TCL_EvalFile.h \
includes/Warnings.h \
includes/sci_TCL_GetVar.h


libtclsci_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/ $(TCL_INC_PATH) $(TK_INC_PATH)
libtclsci_la_LDFLAGS= $(TCLTK_LIBS)
lib_LTLIBRARIES = libtclsci.la

libtclsci_la_SOURCES = $(TCLSCI_C_SOURCES) $(GATEWAY_C_SOURCES)

libtclsci_la_DEPENDENCIES = 
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la


libtclsci_la_LIBADD = $(libtclsci_la_DEPENDENCIES)

libtclsci_la_PKGCONFIG = tclsci.pc
