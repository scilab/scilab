#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006-2008 - Sylvestre Ledru
# Copyright (C) INRIA   - 2007 - Vincent COUVERT
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=gui


#### gui : Conf files ####
libscigui_la_rootdir = $(mydatadir)
libscigui_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### gui : init scripts ####
libscigui_la_etcdir = $(mydatadir)/etc
libscigui_la_etc_DATA = etc/gui.quit \
etc/gui.start \
etc/graphics_menubar.xml \
etc/graphics_toolbar.xml \
etc/helpbrowser_menubar.xml \
etc/main_menubar.xml \
etc/main_toolbar.xml \
etc/flexdock-themes.xml


include $(top_srcdir)/Makefile.incl.am

GUI_C_SOURCES = \
src/c/TerminateGui.c \
src/c/buildMainWindowTitle.c \
src/c/getMainWindowTitle.c \
src/c/InitializeGUI.c \
src/c/setMainWindowTitle.c \
src/c/addmenu.c \
src/c/CreateUIControl.c \
src/c/CreateUimenu.c \
src/c/InitWaitBar.c \
src/c/InitProgressionBar.c \
src/c/CreateUIContextMenu.c \
src/c/UicontrolStyleToString.c \
src/c/messageboxoptions.c 

GUI_CPP_SOURCES = src/cpp/InitUIMenu.cpp \
src/jni/CallScilabBridge.cpp \
src/jni/Jxclick.cpp \
src/cpp/DestroyUimenu.cpp \
src/cpp/DestroyUicontrol.cpp \
src/cpp/DestroyJavaUiobject.cpp \
src/cpp/DestroyWaitBar.cpp \
src/cpp/CallJxclick.cpp \
src/jni/Jxgetmouse.cpp \
src/cpp/CallJxgetmouse.cpp \
src/cpp/CallColorChooser.cpp \
src/cpp/CallFontChooser.cpp \
src/cpp/CallFigure.cpp \
src/cpp/CallMessageBox.cpp \
src/cpp/CallMainWindow.cpp \
src/cpp/CallWaitBar.cpp \
src/cpp/CallHelpBrowser.cpp \
src/cpp/PushButton.cpp \
src/cpp/EditBox.cpp \
src/cpp/Label.cpp \
src/cpp/CheckBox.cpp \
src/cpp/RadioButton.cpp \
src/cpp/Slider.cpp \
src/cpp/PopupMenu.cpp \
src/cpp/ContextMenu.cpp \
src/cpp/ListBox.cpp \
src/cpp/Frame.cpp \
src/cpp/Toolbar.cpp \
src/cpp/Widget.cpp \
src/cpp/SetUimenuLabel.cpp \
src/cpp/GetUimenuLabel.cpp \
src/cpp/GetUicontrolStyle.cpp \
src/cpp/SetUicontrolString.cpp \
src/cpp/GetUicontrolString.cpp \
src/cpp/SetUicontrolParent.cpp \
src/cpp/SetUicontrolBackgroundColor.cpp \
src/cpp/GetUicontrolBackgroundColor.cpp \
src/cpp/SetUiobjectForegroundColor.cpp \
src/cpp/GetUiobjectForegroundColor.cpp \
src/cpp/SetUicontrolPosition.cpp \
src/cpp/GetUicontrolPosition.cpp \
src/cpp/SetUicontrolFontName.cpp \
src/cpp/GetUicontrolFontName.cpp \
src/cpp/SetUicontrolFontWeight.cpp \
src/cpp/GetUicontrolFontWeight.cpp \
src/cpp/SetUicontrolFontUnits.cpp \
src/cpp/GetUicontrolFontUnits.cpp \
src/cpp/SetUicontrolFontSize.cpp \
src/cpp/GetUicontrolFontSize.cpp \
src/cpp/SetUicontrolFontAngle.cpp \
src/cpp/GetUicontrolFontAngle.cpp \
src/cpp/SetUiobjectCallback.cpp \
src/cpp/GetUiobjectCallback.cpp \
src/cpp/SetUiobjectTag.cpp \
src/cpp/GetUiobjectTag.cpp \
src/cpp/SetUiobjectEnable.cpp \
src/cpp/GetUiobjectEnable.cpp \
src/cpp/SetUimenuChecked.cpp \
src/cpp/GetUimenuChecked.cpp \
src/cpp/SetUicontrolMin.cpp \
src/cpp/GetUicontrolMin.cpp \
src/cpp/SetUicontrolMax.cpp \
src/cpp/GetUicontrolMax.cpp \
src/cpp/SetUicontrolValue.cpp \
src/cpp/GetUicontrolValue.cpp \
src/cpp/SetUicontrolListboxTop.cpp \
src/cpp/GetUicontrolListboxTop.cpp \
src/cpp/SetUicontrolUnits.cpp \
src/cpp/GetUicontrolUnits.cpp \
src/cpp/SetUicontrolRelief.cpp \
src/cpp/GetUicontrolRelief.cpp \
src/cpp/SetUicontrolHorizontalAlignment.cpp \
src/cpp/GetUicontrolHorizontalAlignment.cpp \
src/cpp/SetUicontrolVerticalAlignment.cpp \
src/cpp/GetUicontrolVerticalAlignment.cpp \
src/cpp/SetUicontrolSliderStep.cpp \
src/cpp/GetUicontrolSliderStep.cpp \
src/cpp/SetUiobjectVisible.cpp \
src/cpp/GetUiobjectVisible.cpp \
src/cpp/UnitsConversion.cpp \
src/cpp/GetScreenProperty.cpp \
src/cpp/CallClipboard.cpp \
src/cpp/exportUserInterface.cpp \
src/jni/LookAndFeelManager.cpp \
sci_gateway/cpp/sci_raise_window.cpp \
sci_gateway/cpp/sci_usecanvas.cpp \
src/jni/GiwsException.cpp \
src/jni/Juigetfile.cpp \
src/cpp/CallJuigetfile.cpp \
src/cpp/displaytree.cpp \
src/jni/ScilabDisplayTree.cpp


GIWS_WRAPPERS = \
src/jni/CallScilabBridge.giws.xml \
src/jni/Jxclick.giws.xml \
src/jni/Jxgetmouse.giws.xml \
src/jni/SwingScilabMenu.giws.xml \
src/jni/LookAndFeelManager.giws.xml \
src/jni/UIElementMapper.giws.xml \
src/jni/Juigetfile.giws.xml \
src/jni/ScilabDisplayTree.giws.xml


if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/sci_x_choice.c \
sci_gateway/c/gw_gui.c \
sci_gateway/c/sci_x_mdialog.c \
sci_gateway/c/sci_x_dialog.c \
sci_gateway/c/sci_x_choose.c \
sci_gateway/c/sci_x_choose_modeless.c \
sci_gateway/c/sci_unsetmenu.c \
sci_gateway/c/sci_delmenu.c \
sci_gateway/c/sci_setmenu.c \
sci_gateway/c/sci_ClipBoard.c \
sci_gateway/c/sci_toolbar.c \
sci_gateway/c/sci_uicontrol.c \
sci_gateway/c/sci_uicontextmenu.c \
sci_gateway/c/sci_uimenu.c \
sci_gateway/c/sci_uiwait.c \
sci_gateway/c/sci_mpopup.c \
sci_gateway/c/sci_messagebox.c \
sci_gateway/c/sci_waitbar.c \
sci_gateway/c/sci_progressionbar.c \
sci_gateway/c/sci_helpbrowser.c \
sci_gateway/c/sci_uigetfont.c \
sci_gateway/c/sci_uigetcolor.c \
sci_gateway/c/sci_getcallbackobject.c \
sci_gateway/c/sci_exportUI.c

GATEWAY_CPP_SOURCES = \
sci_gateway/cpp/sci_printsetupbox.cpp \
sci_gateway/cpp/sci_toprint.cpp \
sci_gateway/cpp/sci_printfigure.cpp \
sci_gateway/cpp/sci_setlookandfeel.cpp \
sci_gateway/cpp/sci_getinstalledlookandfeels.cpp \
sci_gateway/cpp/sci_getlookandfeel.cpp \
sci_gateway/cpp/sci_uigetdir.cpp \
sci_gateway/cpp/sci_uigetfile.cpp \
sci_gateway/cpp/sci_displaytree.cpp \
sci_gateway/cpp/sci_uiputfile.cpp

libscigui_la_CFLAGS=	-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/c/xsci/ \
			-I$(srcdir)/includes/ \
			-I$(top_srcdir)/modules/graphics/includes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/core/src/c/ \
			-I$(top_srcdir)/modules/string/includes/ \
			-I$(top_srcdir)/modules/tclsci/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/shell/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			$(TCL_INC_PATH) \
			$(Tk_INC_PATH) \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/libs/hashtable \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/dynamic_link/includes \
			-I$(srcdir)/src/cpp/ \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/

libscigui_la_CFLAGS += $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/renderer/includes/ \
			-I$(top_srcdir)/modules/string/includes/

libscigui_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/jni/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			-I$(top_srcdir)/modules/renderer/src/jni/ \
			-I$(top_srcdir)/modules/renderer/includes/ \
			-I$(top_srcdir)/modules/renderer/src/cpp/ \
			-I$(top_srcdir)/modules/renderer/src/cpp/figureDrawing/ \
			-I$(top_srcdir)/modules/graphic_export/includes/ \
			-I$(top_srcdir)/libs/doublylinkedlist/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/graphics/includes/ \
			-I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/ \
			-I$(top_srcdir)/modules/graphic_export/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/string/includes/


# Without the GUI module 
libscigui_disable_la_CFLAGS = -Iincludes/ \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(srcdir)/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes

GUI_DISABLE_C_SOURCES = src/nogui/nogui.c
libscigui_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscigui_disable_la_SOURCES = $(GUI_DISABLE_C_SOURCES)


pkglib_LTLIBRARIES = libscigui-disable.la

if GUI
pkglib_LTLIBRARIES += libscigui.la 
endif

libscigui_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscigui_la_SOURCES = $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(GUI_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigui_la_CFLAGS)

#  cyclic $(top_builddir)/modules/graphics/libscigraphics.la $(top_builddir)/modules/jvm/libscijvm.la
libscigui_la_LIBADD = $(top_builddir)/modules/renderer/libscirenderer.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la


#### gui : gateway declaration ####
libscigui_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigui_la_sci_gateway_DATA = sci_gateway/gui_gateway.xml


#### gui : icons files ####
libscigui_la_iconsdir=$(mydatadir)/
nobase_libscigui_la_icons_DATA = images/icons/accessories-text-editor.png \
images/icons/document-open.png \
images/icons/edit-copy.png \
images/icons/edit-cut.png \
images/icons/edit-paste.png \
images/icons/emblem-readonly.png \
images/icons/folder.png \
images/icons/format-text-bold.png \
images/icons/help-browser.png \
images/icons/original-view.png \
images/icons/printer.png \
images/icons/process-working.png \
images/icons/rotate.png \
images/icons/scilab_close_active.png \
images/icons/scilab_close_default.png \
images/icons/scilab.png \
images/icons/undock_active.png \
images/icons/undock_default.png \
images/icons/window-new.png \
images/icons/zoom-area-cursor.png \
images/icons/zoom-area.png \
images/icons/document-new.png \
images/icons/document-print-preview.png \
images/icons/document-print.png \
images/icons/document-save-as.png \
images/icons/document-save.png \
images/icons/edit-find-replace.png \
images/icons/edit-find.png \
images/icons/edit-redo.png \
images/icons/edit-undo.png \
images/icons/media-floppy.png


if GUI
USEANT=1
endif
