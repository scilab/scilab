##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

GUI_C_SOURCES = src/c/SciGUIConsole/SciGUIConsole.c \
src/c/xsci/wf_w_util.c \
src/c/xsci/utils.c \
src/c/xsci/x_cursor.c \
src/c/xsci/x_main.c \
src/c/xsci/wf_f_read.c \
src/c/xsci/wf_f_util.c \
src/c/xsci/x_tabs.c \
src/c/xsci/x_misc.c \
src/c/xsci/wf_e_edit.c \
src/c/xsci/windows.c \
src/c/xsci/wf_w_cursor.c \
src/c/xsci/wf_w_dir.c \
src/c/xsci/x_util.c \
src/c/xsci/x_screen.c \
src/c/xsci/x_scrollbar.c \
src/c/xsci/zzledt.c \
src/c/xsci/SGraph.c \
src/c/xsci/x_menu.c \
src/c/xsci/xwidgets.c \
src/c/xsci/x_data.c \
src/c/xsci/x_VTPrsTbl.c \
src/c/xsci/inter.c \
src/c/xsci/coloredit.c \
src/c/xsci/x_input.c \
src/c/xsci/x_charproc.c \
src/c/xsci/wf_w_msgpanel.c \
src/c/xsci/wf_w_file.c \
src/c/xsci/x_button.c \
src/c/xsci/command.c \
src/c/xsci/wf_w_setup.c \
src/c/xsci/wf_w_init.c \
src/c/men_dialog.c \
src/c/xmen_getfile.c \
src/c/xmen_choose.c \
src/c/xmen_message.c \
src/c/xmen_mdial.c \
src/c/men_getfile.c \
src/c/xmen_madial.c \
src/c/men_choose.c \
src/c/xmen_choice.c \
src/c/men_message.c \
src/c/xmen_print.c \
src/c/men_mdial.c \
src/c/men_madial.c \
src/c/xmen_dialog.c \
src/c/xmen_Utils.c \
src/c/men_choice.c \
src/c/men_print.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_x_choice.c \
sci_gateway/c/sci_clc.c \
sci_gateway/c/sci_x_message_modeless.c \
sci_gateway/c/sci_unsetmenu_old.c \
sci_gateway/c/gw_gui.c \
sci_gateway/c/sci_x_mdialog.c \
sci_gateway/c/sci_x_dialog.c \
sci_gateway/c/sci_delmenu_old.c \
sci_gateway/c/sci_show_window.c \
sci_gateway/c/sci_x_choose.c \
sci_gateway/c/sci_xgetfile.c \
sci_gateway/c/sci_addmenu_old.c \
sci_gateway/c/sci_tohome.c \
sci_gateway/c/sci_x_message.c \
sci_gateway/c/sci_setmenu_old.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_setmenu_old.f \
sci_gateway/fortran/sci_choices.f \
sci_gateway/fortran/sci_delmenu_old.f \
sci_gateway/fortran/sci_x_mdialog.f \
sci_gateway/fortran/sci_x_dialog.f \
sci_gateway/fortran/sci_x_message_modeless.f \
sci_gateway/fortran/sci_x_choose.f \
sci_gateway/fortran/sci_xgetfile.f \
sci_gateway/fortran/sci_addmenu_old.f \
sci_gateway/fortran/sci_x_message.f

libgui_la_includedir=$(includedir)/gui

libgui_la_include_HEADERS = includes/men_choice.h \
includes/sci_tohome.h \
includes/men_scilab.h \
includes/sci_clc.h \
includes/gw_gui.h \
includes/sci_show_window.h

BUILT_SOURCES = $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h

$(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h: $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad src/c/xsci/x_main.c
	$(top_srcdir)/modules/gui/src/c/xsci/ad2c.sh $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad > $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h

CLEANFILES=$(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h

libgui_la_CFLAGS=  -Iincludes/  $(JAVA_JNI_INCLUDE)  -I$(top_srcdir)/modules/graphics/includes/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/core/src/c/ -I$(top_srcdir)/modules/string/includes/ -I$(top_srcdir)/modules/tclsci/includes/ $(TCL_INC_PATH) $(Tk_INC_PATH) 

libgui_la_LDFLAGS = $(X_LIBS) $(TERMCAP_LIB)

lib_LTLIBRARIES = libgui.la

libgui_la_SOURCES = $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libgui_la_DEPENDENCIES = \
	$(top_builddir)/modules/tclsci/libtclsci.la \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/modules/string/libstring.la \
	$(top_builddir)/modules/jvm/libscijvm.la

# Cyclic dep
#	$(top_builddir)/modules/graphics/libgraphics.la

libgui_la_LIBADD = $(libgui_la_DEPENDENCIES)

libgui_la_PKGCONFIG = gui.pc
