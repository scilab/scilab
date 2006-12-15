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
src/TclUiSet.c

# a priori utilisé seulement par windows
# src/EnvTclTk.c

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

lib_LTLIBRARIES = libtclsci.la

libtclsci_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/ $(TCL_INC_PATH) $(TK_INC_PATH)
libtclsci_la_LDFLAGS= $(TCLTK_LIBS)

libtclsci_la_SOURCES = $(TCLSCI_C_SOURCES) $(GATEWAY_C_SOURCES)

libtclsci_la_LIBADD = 
	$(top_builddir)/modules/intersci/libintersci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libtclsci_la_PKGCONFIG = tclsci.pc



datadir=${prefix}/modules
#### Target ######
target=$(datadir)/tclsci


#### tclsci : Conf files ####
libtclsci_la_rootdir = $(target)
libtclsci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### tclsci : Scilab macros ###
libtclsci_la_macrodir = $(target)/macros
libtclsci_la_macro_DATA = macros/TK_SetVar.sci \
macros/IsAScalar.sci \
macros/uimenu.sci \
macros/config.sci \
macros/gettklib.sci \
macros/sciGUI_init.sci \
macros/demoplay.sci \
macros/browsevar.sci \
macros/tk_getdir.sci \
macros/GetUiControlValueType.sci \
macros/tk_getfile.sci \
macros/TK_EvalStr.sci \
macros/tk_choose.sci \
macros/uicontrol.sci \
macros/figure.sci \
macros/winclose.sci \
macros/GetUiControlPropertyName.sci \
macros/TK_EvalFile.sci \
macros/TK_GetVar.sci \
macros/waitbar.sci \
macros/winlist.sci \
macros/buttondialog.sci \
macros/sciGUIhelp.sci \
macros/tk_savefile.sci \
macros/editvar_get.sci \
macros/editvar.sci \
macros/progressionbar.sci \
macros/GetUiControlStyleName.sci macros/browsevar.bin \
macros/buttondialog.bin \
macros/config.bin \
macros/demoplay.bin \
macros/editvar_get.bin \
macros/editvar.bin \
macros/figure.bin \
macros/gettklib.bin \
macros/GetUiControlPropertyName.bin \
macros/GetUiControlStyleName.bin \
macros/GetUiControlValueType.bin \
macros/IsAScalar.bin \
macros/progressionbar.bin \
macros/sciGUIhelp.bin \
macros/sciGUI_init.bin \
macros/tk_choose.bin \
macros/TK_EvalFile.bin \
macros/TK_EvalStr.bin \
macros/tk_getdir.bin \
macros/tk_getfile.bin \
macros/TK_GetVar.bin \
macros/tk_savefile.bin \
macros/TK_SetVar.bin \
macros/uicontrol.bin \
macros/uimenu.bin \
macros/waitbar.bin \
macros/winclose.bin \
macros/winlist.bin macros/names macros/lib macros/buildmacros.sce


### TCL macros ###
libtclsci_la_tcldir = $(target)/tcl/
libtclsci_la_tcl_DATA = tcl/uicontrol.tcl \
tcl/callbacks.tcl \
tcl/get.tcl \
tcl/figure.tcl \
tcl/tkcontrols.tcl \
tcl/uimenu.tcl \
tcl/html_library.tcl \
tcl/browsehelp.tcl \
tcl/TK_Scilab.tcl \
tcl/misc.tcl \
tcl/set.tcl

#### tclsci : init scripts #### 
libtclsci_la_etcdir = $(target)/etc
libtclsci_la_etc_DATA = etc/tclsci.quit etc/tclsci.start

#### tclsci : gateway declaration #### 
libtclsci_la_sci_gatewaydir = $(target)/sci_gateway
libtclsci_la_sci_gateway_DATA = sci_gateway/tclsci


#### tclsci : documentation FR #### 
libtclsci_la_helpfrdir = $(target)/help/fr
libtclsci_la_helpfr_DATA = help/fr/buttondialog.htm \
help/fr/demoplay.htm \
help/fr/config.htm \
help/fr/winclose.htm \
help/fr/editvar.htm \
help/fr/figure.htm \
help/fr/findobj.htm \
help/fr/TCL_ExistVar.htm \
help/fr/ScilabEval.htm \
help/fr/uimenu.htm \
help/fr/TCL_ExistArray.htm \
help/fr/TCL_ExistInterp.htm \
help/fr/TCL_GetVar.htm \
help/fr/TCL_GetVersion.htm \
help/fr/TCL_SetVar.htm \
help/fr/TCL_UnsetVar.htm \
help/fr/TCL_UpVar.htm \
help/fr/TK_EvalFile.htm \
help/fr/TK_EvalStr.htm \
help/fr/tk_getdir.htm \
help/fr/tk_getfile.htm \
help/fr/TK_GetVar.htm \
help/fr/tk_savefile.htm \
help/fr/TK_SetVar.htm \
help/fr/uicontrol.htm \
help/fr/waitbar.htm \
help/fr/winlist.htm \
help/fr/whatis.htm \
help/fr/close.htm \
help/fr/browsevar.htm \
help/fr/TCL_CreateSlave.htm \
help/fr/progressionbar.htm \
help/fr/TCL_DeleteInterp.htm


#### tclsci : documentation EN #### 
libtclsci_la_helpendir = $(target)/help/eng
libtclsci_la_helpen_DATA = help/eng/whatis.htm \
help/eng/buttondialog.htm \
help/eng/browsevar.htm \
help/eng/demoplay.htm \
help/eng/close.htm \
help/eng/config.htm \
help/eng/winclose.htm \
help/eng/editvar.htm \
help/eng/figure.htm \
help/eng/findobj.htm \
help/eng/TCL_DeleteInterp.htm \
help/eng/progressionbar.htm \
help/eng/ScilabEval.htm \
help/eng/TCL_CreateSlave.htm \
help/eng/uimenu.htm \
help/eng/TCL_ExistArray.htm \
help/eng/TCL_ExistInterp.htm \
help/eng/TCL_ExistVar.htm \
help/eng/TCL_GetVar.htm \
help/eng/TCL_GetVersion.htm \
help/eng/TCL_SetVar.htm \
help/eng/TCL_UnsetVar.htm \
help/eng/TCL_UpVar.htm \
help/eng/TK_EvalFile.htm \
help/eng/TK_EvalStr.htm \
help/eng/tk_getdir.htm \
help/eng/tk_getfile.htm \
help/eng/TK_GetVar.htm \
help/eng/tk_savefile.htm \
help/eng/TK_SetVar.htm \
help/eng/uicontrol.htm \
help/eng/waitbar.htm \
help/eng/winlist.htm


#### tclsci : include files ####
libtclsci_la_includedir=$(includedir)/tclsci
libtclsci_la_include_HEADERS = includes/sci_set.h \
includes/sci_TCL_ExistArray.h \
includes/TclUiSet.h \
includes/GedManagement.h \
includes/sci_TCL_UpVar.h \
includes/sci_TCL_CreateSlave.h \
includes/sci_opentk.h \
includes/sci_TCL_gcf.h \
includes/TCL_Global.h \
includes/sci_close.h \
includes/sci_TCL_SetVar.h \
includes/ScilabEval.h \
includes/InitTclTk.h \
includes/sci_TCL_ExistVar.h \
includes/sci_TCL_DoOneEvent.h \
includes/TclEvents.h \
includes/Messages.h \
includes/sci_TCL_UnsetVar.h \
includes/sci_findobj.h \
includes/Errors.h \
includes/sci_get.h \
includes/gw_tclsci.h \
includes/sci_TCL_ExistInterp.h \
includes/sci_TCL_EvalStr.h \
includes/TclUiGet.h \
includes/sci_TCL_DeleteInterp.h \
includes/sci_TCL_GetVersion.h \
includes/sci_TCL_EvalFile.h \
includes/Warnings.h \
includes/sci_TCL_GetVar.h


#### tclsci : demos files ####
libtclsci_la_demosdir=$(target)/demos/
libtclsci_la_demos_DATA = demos/sciGUI/sciGUIdemo.dpf \
demos/sciGUI/test_demoplay.dpf \
demos/sciGUI/scilab01.gif \
demos/sciGUI/logo01.gif \
demos/tk/timer \
demos/tk/tk.dem \
demos/tk/rolodex.sce \
demos/tk/demredraw.sci \
demos/tk/rgb.txt \
demos/tk/timer.sce \
demos/tk/puzzle \
demos/tk/uicontrol.sci \
demos/tk/scale.sce \
demos/tk/cscroll \
demos/tk/tcolor \
demos/tk/puzzle.sce \
demos/tk/scroll.sce \
demos/tk/uicontrol1.sce \
demos/tk/tcolor.sce \
demos/tk/rolodex \
demos/tk/vscale

