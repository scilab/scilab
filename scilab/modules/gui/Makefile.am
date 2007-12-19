##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

include $(top_srcdir)/Makefile.incl.am

if GUI
GUI_C_SOURCES = \
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
src/c/addmenu.c \
src/c/CreateUIControl.c \
src/c/GetStyleProperty.c

GUI_CPP_SOURCES = src/cpp/InitUIMenu.cpp \
src/jni/CallScilabBridge.cpp \
src/jni/Jxclick.cpp \
src/cpp/CallJxclick.cpp \
src/jni/Jxgetmouse.cpp \
src/cpp/CallJxgetmouse.cpp \
src/cpp/CallFileChooser.cpp \
src/cpp/PushButton.cpp \
src/cpp/SetLabelProperty.cpp \
src/cpp/GetLabelProperty.cpp \
src/cpp/SetUicontrolParent.cpp \
src/cpp/SetUicontrolBackgroundColor.cpp \
src/cpp/GetUicontrolBackgroundColor.cpp \
src/cpp/SetUiobjectForegroundColor.cpp \
src/cpp/GetUiobjectForegroundColor.cpp \
src/cpp/SetUicontrolPosition.cpp \
src/cpp/GetUicontrolPosition.cpp \
src/cpp/SetUiobjectCallback.cpp \
src/cpp/GetUiobjectCallback.cpp

GIWS_WRAPPERS = \
src/jni/DrawableRectangleGL.giws.xml \
src/jni/Jxclick.giws.xml \
src/jni/Jxgetmouse.giws.xml

GATEWAY_C_SOURCES = sci_gateway/c/sci_x_choice.c \
sci_gateway/c/sci_x_message_modeless.c \
sci_gateway/c/gw_gui.c \
sci_gateway/c/sci_x_mdialog.c \
sci_gateway/c/sci_x_dialog.c \
sci_gateway/c/sci_raise_window.c \
sci_gateway/c/sci_x_choose.c \
sci_gateway/c/sci_xgetfile.c \
sci_gateway/c/sci_x_message.c \
sci_gateway/c/sci_unsetmenu.c \
sci_gateway/c/sci_delmenu.c \
sci_gateway/c/sci_setmenu.c \
sci_gateway/c/sci_getlookandfeel.c \
sci_gateway/c/sci_getinstalledlookandfeels.c \
sci_gateway/c/sci_setlookandfeel.c \
sci_gateway/c/sci_about.c \
sci_gateway/c/sci_ClipBoard.c \
sci_gateway/c/sci_hidetoolbar.c \
sci_gateway/c/sci_toolbar.c \
sci_gateway/c/sci_uigetdir.c \
sci_gateway/c/sci_uicontrol.c

libscigui_la_CFLAGS=	-Isrc/c/ \
			-Isrc/c/xsci/ \
			-Iincludes/ \
			-I$(top_srcdir)/modules/graphics/includes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/core/src/c/ \
			-I$(top_srcdir)/modules/string/includes/ \
			-I$(top_srcdir)/modules/tclsci/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/shell/includes/ \
			$(TCL_INC_PATH) \
			$(Tk_INC_PATH) \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/libs/hashtable \
			-I$(top_srcdir)/modules/output_stream/includes \
			-Isrc/cpp/ \
			-I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/

libscigui_la_CFLAGS += $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/modules/jvm/includes/

libscigui_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/cpp/ -Isrc/c/ -Isrc/jni/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/renderer/src/jni/ \
			-I$(top_srcdir)/modules/renderer/src/cpp/ \
			-I$(top_srcdir)/modules/renderer/src/cpp/figureDrawing/ \
			-I$(top_srcdir)/libs/doublylinkedlist/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/graphics/includes/ \
			-I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/ \
			-Isrc/jni/

else
libscigui_la_CFLAGS=			-Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

GUI_C_SOURCES = src/nogui/nogui.c

endif

libscigui_la_LDFLAGS = $(LD_FLAGS) $(X_LIBS) $(TERMCAP_LIB) $(X_EXTRA_LIBS)

pkglib_LTLIBRARIES = libscigui.la

libscigui_la_SOURCES = $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(GUI_CPP_SOURCES)

libscigui_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la \
    $(top_builddir)/modules/tclsci/libscitclsci.la

if STRING_V2
libscigui_la_LIBADD += $(top_builddir)/modules/STRING_V2/libscistring.la
else
libscigui_la_LIBADD += $(top_builddir)/modules/string/libscistring.la
endif


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
