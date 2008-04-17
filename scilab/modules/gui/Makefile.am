##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
### Vincent COUVERT <vincent.couvert@inria.fr>
### INRIA - Scilab 2008
##########

include $(top_srcdir)/Makefile.incl.am

if GUI
GUI_C_SOURCES = \
src/c/getlookandfeel.c \
src/c/setlookandfeel.c \
src/c/getinstalledlookandfeels.c \
src/c/TerminateGui.c \
src/c/buildMainWindowTitle.c \
src/c/getMainWindowTitle.c \
src/c/InitializeGUI.c \
src/c/setMainWindowTitle.c \
src/c/addmenu.c \
src/c/CreateUIControl.c \
src/c/DestroyUicontrol.c \
src/c/CreateUimenu.c \
src/c/DestroyUimenu.c \
src/c/InitWaitBar.c \
src/c/InitProgressionBar.c \
src/c/CreateUIContextMenu.c \
src/c/UicontrolStyleToString.c \
src/c/messageboxoptions.c

GUI_CPP_SOURCES = src/cpp/InitUIMenu.cpp \
src/jni/CallScilabBridge.cpp \
src/jni/Jxclick.cpp \
src/cpp/DestroyUIControl.cpp \
src/cpp/DestroyWaitBar.cpp \
src/cpp/CallJxclick.cpp \
src/jni/Jxgetmouse.cpp \
src/cpp/CallJxgetmouse.cpp \
src/cpp/CallColorChooser.cpp \
src/cpp/CallFileChooser.cpp \
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
src/cpp/GetScreenProperty.cpp

GIWS_WRAPPERS = \
src/jni/CallScilabBridge.giws.xml \
src/jni/FileChooser.giws.xml \
src/jni/Jxclick.giws.xml \
src/jni/Jxgetmouse.giws.xml \
src/jni/SwingScilabMenu.giws.xml \
src/jni/UIElementMapper.giws.xml

GATEWAY_C_SOURCES = sci_gateway/c/sci_x_choice.c \
sci_gateway/c/gw_gui.c \
sci_gateway/c/sci_x_mdialog.c \
sci_gateway/c/sci_x_dialog.c \
sci_gateway/c/sci_raise_window.c \
sci_gateway/c/sci_x_choose.c \
sci_gateway/c/sci_x_choose_modeless.c \
sci_gateway/c/sci_xgetfile.c \
sci_gateway/c/sci_unsetmenu.c \
sci_gateway/c/sci_delmenu.c \
sci_gateway/c/sci_setmenu.c \
sci_gateway/c/sci_getlookandfeel.c \
sci_gateway/c/sci_getinstalledlookandfeels.c \
sci_gateway/c/sci_setlookandfeel.c \
sci_gateway/c/sci_ClipBoard.c \
sci_gateway/c/sci_toolbar.c \
sci_gateway/c/sci_uigetdir.c \
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
sci_gateway/c/sci_getcallbackobject.c

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
			-I$(top_srcdir)/modules/dynamic_link/includes \
			-Isrc/cpp/ \
			-I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/

libscigui_la_CFLAGS += $(JAVA_JNI_INCLUDE) -I$(top_srcdir)/modules/jvm/includes/ -I$(top_srcdir)/modules/string/includes/

libscigui_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/cpp/ -Isrc/c/ -Isrc/jni/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/renderer/src/jni/ \
			-I$(top_srcdir)/modules/renderer/includes/ \
			-I$(top_srcdir)/modules/renderer/src/cpp/ \
			-I$(top_srcdir)/modules/renderer/src/cpp/figureDrawing/ \
			-I$(top_srcdir)/modules/graphic_export/includes/ \
			-I$(top_srcdir)/libs/doublylinkedlist/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/graphics/includes/ \
			-I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/ \
			-Isrc/jni/ \
			-I$(top_srcdir)/modules/graphic_export/includes/ \
			-I$(top_srcdir)/modules/string/includes/

else
libscigui_la_CFLAGS=			-Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

GUI_C_SOURCES = src/nogui/nogui.c

endif

pkglib_LTLIBRARIES = libscigui.la

libscigui_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS) $(X_LIBS) $(X_EXTRA_LIBS)

libscigui_la_SOURCES = $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(GUI_CPP_SOURCES)

#  cyclic $(top_builddir)/modules/graphics/libscigraphics.la $(top_builddir)/modules/jvm/libscijvm.la
libscigui_la_LIBADD = $(top_builddir)/modules/renderer/libscirenderer.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
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


#### gui : gateway declaration ####
libscigui_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigui_la_sci_gateway_DATA = sci_gateway/gui_gateway.xml


#### gui : include files ####
libscigui_la_includedir=$(pkgincludedir)/gui
libscigui_la_include_HEADERS = includes/gw_gui.h \
includes/sci_raise_window.h \
includes/GraphicWindow.h \
includes/InitializeGUI.h \
includes/CallColorChooser.h \
includes/CallFileChooser.h \
includes/CallFontChooser.h \
includes/CallFigure.h \
includes/CallJxclick.h \
includes/CallJxgetmouse.h \
includes/CallMessageBox.h \
includes/CheckBox.h \
includes/ContextMenu.h \
includes/DestroyJavaUicontrol.h \
includes/EditBox.h \
includes/Frame.h \
includes/GetScreenProperty.h \
includes/GetUicontrolBackgroundColor.h \
includes/GetUicontrolFontAngle.h \
includes/GetUicontrolFontName.h \
includes/GetUicontrolFontSize.h \
includes/GetUicontrolFontUnits.h \
includes/GetUicontrolFontWeight.h \
includes/GetUicontrolHorizontalAlignment.h \
includes/GetUicontrolListboxTop.h \
includes/GetUicontrolMax.h \
includes/GetUicontrolMin.h \
includes/GetUicontrolPosition.h \
includes/GetUicontrolRelief.h \
includes/GetUicontrolSliderStep.h \
includes/GetUicontrolString.h \
includes/GetUicontrolStyle.h \
includes/GetUicontrolUnits.h \
includes/GetUicontrolValue.h \
includes/GetUicontrolVerticalAlignment.h \
includes/GetUimenuLabel.h \
includes/GetUiobjectCallback.h \
includes/GetUiobjectForegroundColor.h \
includes/GetUiobjectTag.h \
includes/GetUiobjectVisible.h \
includes/InitUIMenu.h \
includes/Label.h \
includes/ListBox.h \
includes/PopupMenu.h \
includes/PushButton.h \
includes/RadioButton.h \
includes/sci_mpopup.h \
includes/sci_uicontextmenu.h \
includes/sci_uicontrol.h \
includes/sci_uimenu.h \
includes/sci_uiwait.h \
includes/SetUicontrolBackgroundColor.h \
includes/SetUicontrolFontAngle.h \
includes/SetUicontrolFontName.h \
includes/SetUicontrolFontSize.h \
includes/SetUicontrolFontUnits.h \
includes/SetUicontrolFontWeight.h \
includes/SetUicontrolHorizontalAlignment.h \
includes/SetUicontrolListboxTop.h \
includes/SetUicontrolMax.h \
includes/SetUicontrolMin.h \
includes/SetUicontrolParent.h \
includes/SetUicontrolPosition.h \
includes/SetUicontrolRelief.h \
includes/SetUicontrolSliderStep.h \
includes/SetUicontrolString.h \
includes/SetUicontrolUnits.h \
includes/SetUicontrolValue.h \
includes/SetUicontrolVerticalAlignment.h \
includes/SetUimenuLabel.h \
includes/SetUiobjectCallback.h \
includes/SetUiobjectForegroundColor.h \
includes/SetUiobjectTag.h \
includes/SetUiobjectVisible.h \
includes/Slider.h \
includes/TerminateGui.h \
includes/write_scilab.h \
includes/Toolbar.h \
includes/DestroyUicontrol.h \
includes/DestroyUimenu.h

#### gui : demos files ####
libscigui_la_demosdir=$(mydatadir)/demos/
libscigui_la_demos_DATA = demos/dialog.dem


#### graphics : icons files ####
libscigui_la_iconsdir=$(mydatadir)/
nobase_libscigui_la_icons_DATA = images/icons/accessories-text-editor.png \
images/icons/document-open.png \
images/icons/edit-copy.png \
images/icons/edit-paste.png \
images/icons/folder.png \
images/icons/format-text-bold.png \
images/icons/help-browser.png \
images/icons/list-add.png \
images/icons/list-remove.png \
images/icons/printer.png \
images/icons/scilab.png \
images/icons/view-refresh.png \
images/icons/window-new.png \
images/icons/edit-cut.png


if GUI

USEANT=1

endif
