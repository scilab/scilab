##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

include $(top_srcdir)/Makefile.incl.am


libscigui_la_CFLAGS=  -Isrc/c/ -Isrc/c/xsci/ -Iincludes/ -I$(top_srcdir)/modules/graphics/includes/ -I$(top_srcdir)/libs/MALLOC/includes/ -I$(top_srcdir)/modules/core/src/c/ -I$(top_srcdir)/modules/string/includes/ -I$(top_srcdir)/modules/tclsci/includes/ -I$(top_srcdir)/modules/jvm/includes/ -I$(top_srcdir)/modules/shell/includes/ $(TCL_INC_PATH) $(Tk_INC_PATH) \
-I$(top_srcdir)/libs/doublylinkedlist/includes

GUI_C_SOURCES = \
src/c/IsNoInteractiveWindow.c \
src/c/xsci/IsConsoleMode.c \
src/c/xsci/xscisrn.c \
src/c/xsci/mainscic.c 

if GUI
GUI_C_SOURCES += \
src/c/getlookandfeel.c \
src/c/setlookandfeel.c \
src/c/getinstalledlookandfeels.c \
src/c/TerminateGui.c \
src/c/guitable.c \
src/c/buildMainWindowTitle.c \
src/c/getMainWindowTitle.c \
src/c/getScreenProperties.c \
src/c/InitializeGUI.c \
src/c/setMainWindowTitle.c \
src/c/addmenu.c


GATEWAY_C_SOURCES = sci_gateway/c/sci_x_choice.c \
sci_gateway/c/sci_x_message_modeless.c \
sci_gateway/c/gw_gui.c \
sci_gateway/c/sci_x_mdialog.c \
sci_gateway/c/sci_x_dialog.c \
sci_gateway/c/sci_raise_window.c \
sci_gateway/c/sci_x_choose.c \
sci_gateway/c/sci_xgetfile.c \
sci_gateway/c/sci_x_message.c \
sci_gateway/c/sci_unsetmenu_old.c \
sci_gateway/c/sci_delmenu_old.c \
sci_gateway/c/sci_addmenu_old.c \
sci_gateway/c/sci_setmenu_old.c \
sci_gateway/c/sci_getlookandfeel.c \
sci_gateway/c/sci_getinstalledlookandfeels.c \
sci_gateway/c/sci_setlookandfeel.c \
sci_gateway/c/sci_about.c \
sci_gateway/c/sci_ClipBoard.c \
sci_gateway/c/sci_hidetoolbar.c \
sci_gateway/c/sci_toolbar.c 




#BUILT_SOURCES = $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h

#$(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h: $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad src/c/xsci/x_main.c
#	$(top_srcdir)/modules/gui/src/c/xsci/ad2c.sh $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad > $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h

#CLEANFILES = $(top_srcdir)/modules/gui/src/c/xsci/Xscilab.ad.h

libscigui_la_CFLAGS += $(JAVA_JNI_INCLUDE)

else

GUI_C_SOURCES += src/nogui/nogui.c 

endif

libscigui_la_LDFLAGS = $(LD_FLAGS) $(X_LIBS) $(TERMCAP_LIB) $(X_EXTRA_LIBS)

pkglib_LTLIBRARIES = libscigui.la

libscigui_la_SOURCES = $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libscigui_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/string/libscistring.la \
    $(top_builddir)/modules/tclsci/libscitclsci.la


# Cyclic dep
#	$(top_builddir)/modules/graphics/libscigraphics.la

#### Target ######
modulename=gui

#### gui : Conf files ####
libscigui_la_rootdir = $(mydatadir)
libscigui_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

####�gui : init scripts #### 
libscigui_la_etcdir = $(mydatadir)/etc
libscigui_la_etc_DATA = etc/gui.quit etc/gui.start

####�gui : gateway declaration #### 
libscigui_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigui_la_sci_gateway_DATA = sci_gateway/gui_gateway.xml


####�gui : include files ####
libscigui_la_includedir=$(pkgincludedir)/gui
libscigui_la_include_HEADERS = includes/men_choice.h \
includes/men_scilab.h \
includes/gw_gui.h \
includes/wmen_scilab.h \
includes/sci_raise_window.h \
includes/GraphicWindow.h \
includes/InitializeGUI.h



####�gui : demos files ####
libscigui_la_demosdir=$(mydatadir)/demos/
libscigui_la_demos_DATA = demos/dialog.dem


if GUI

USEANT=1

endif
