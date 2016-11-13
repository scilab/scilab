#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006-2008 - Sylvestre Ledru
# Copyright (C) INRIA   - 2007 - Vincent COUVERT
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# Copyright (C) DIGITEO - 2010 - Clément DAVID
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

#### Target ######
modulename=gui


#### gui : Conf files ####
libscigui_la_rootdir = $(mydatadir)
libscigui_la_root_DATA =  license.txt


#### gui : init scripts ####
libscigui_la_etcdir = $(mydatadir)/etc
libscigui_la_etc_DATA = \
	etc/gui.quit \
	etc/gui.start \
	etc/graphics_menubar.xml \
	etc/graphics_toolbar.xml \
	etc/helpbrowser_menubar.xml \
	etc/main_menubar.xml \
	etc/main_toolbar.xml \
	etc/flexdock-themes.xml \
	etc/integratedConfiguration.xml \
	etc/simpleConfiguration.xml \
	etc/XConfiguration-colors.xml \
	etc/XConfiguration-colors.xsl \
	etc/XConfiguration-font.xml \
	etc/XConfiguration-font.xsl \
	etc/XConfiguration-web.xml \
	etc/XConfiguration-web.xsl \
	etc/XConfiguration-toolbars.xml \
	etc/XConfiguration-toolbars.xsl

#### gui : tree functions ####
MACROSDIRSEXT = macros/tree

GUI_C_SOURCES = \
    src/c/buildMainWindowTitle.c \
    src/c/getMainWindowTitle.c \
    src/c/InitializeGUI.c \
    src/c/setMainWindowTitle.c \
    src/c/addmenu.c \
    src/c/CreateUimenu.c \
    src/c/checkColorRange.c \
    src/c/messageboxoptions.c \
    src/c/ScilabTermination.c \
    src/jni/ScilabTermination_wrap.c

GUI_CXX_SOURCES = \
    src/cpp/InitUIMenu.cpp \
    src/jni/CallScilabBridge.cpp \
    src/cpp/CreateUIControl.cpp \
    src/jni/Jxclick.cpp \
    src/jni/SwingView.cpp \
    src/cpp/CallJxclick.cpp \
    src/jni/Jxgetmouse.cpp \
    src/cpp/CallJxgetmouse.cpp \
    src/cpp/CallColorChooser.cpp \
    src/cpp/CallFontChooser.cpp \
    src/cpp/CallMessageBox.cpp \
    src/cpp/CallHelpBrowser.cpp \
    src/cpp/ContextMenu.cpp \
    src/cpp/Widget.cpp \
    src/cpp/SetUimenuLabel.cpp \
    src/cpp/GetUimenuLabel.cpp \
    src/cpp/GetUicontrolStyle.cpp \
    src/cpp/SetUicontrolString.cpp \
    src/cpp/GetUicontrolString.cpp \
    src/cpp/SetUicontrolTooltipString.cpp \
    src/cpp/GetUicontrolTooltipString.cpp \
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
    src/cpp/SetUicontrolIcon.cpp \
    src/cpp/GetUicontrolIcon.cpp \
    src/cpp/SetConsoleShowHiddenHandles.cpp \
    src/cpp/GetConsoleShowHiddenHandles.cpp \
    src/cpp/UnitsConversion.cpp \
    src/cpp/GetScreenProperty.cpp \
    src/cpp/CallClipboard.cpp \
    src/cpp/exportUserInterface.cpp \
    src/jni/LookAndFeelManager.cpp \
    src/jni/Juigetfile.cpp \
    src/cpp/CallJuigetfile.cpp \
    src/cpp/displaytree.cpp \
    src/jni/ScilabDisplayTree.cpp \
    src/jni/EditorManager.cpp \
    src/jni/StartGED.cpp \
    src/jni/StartPlotBrowser.cpp \
    src/jni/DatatipManager.cpp \
    src/jni/DatatipCreate.cpp \
    src/jni/DatatipDelete.cpp \
    src/jni/DatatipDrag.cpp \
    src/jni/DatatipSetStyle.cpp \
    src/jni/DatatipOrientation.cpp

GIWS_WRAPPERS = \
    src/jni/SwingView.giws.xml \
    src/jni/CallScilabBridge.giws.xml \
    src/jni/Jxclick.giws.xml \
    src/jni/Jxgetmouse.giws.xml \
    src/jni/LookAndFeelManager.giws.xml \
    src/jni/Juigetfile.giws.xml \
    src/jni/ScilabDisplayTree.giws.xml \
    src/jni/EditorManager.giws.xml \
    src/jni/Datatip.giws.xml \
    src/jni/StartGED.giws.xml \
    src/jni/StartPlotBrowser.giws.xml

SWIG_WRAPPERS = src/jni/ScilabTermination.i

BUILT_SOURCES=
if GIWS
BUILT_SOURCES+=giws
endif
if SWIG
BUILT_SOURCES+=swig
endif

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_x_choose.c \
    sci_gateway/c/sci_x_choose_modeless.c \
    sci_gateway/c/sci_unsetmenu.c \
    sci_gateway/c/sci_delmenu.c \
    sci_gateway/c/sci_setmenu.c \
    sci_gateway/c/sci_toolbar.c \
    sci_gateway/c/sci_uicontextmenu.c \
    sci_gateway/c/sci_uicontrol.c \
    sci_gateway/c/sci_uimenu.c \
    sci_gateway/c/sci_uiwait.c \
    sci_gateway/c/sci_mpopup.c \
    sci_gateway/c/sci_messagebox.c \
    sci_gateway/c/sci_waitbar.c \
    sci_gateway/c/sci_progressionbar.c \
    sci_gateway/c/sci_getcallbackobject.c \
    sci_gateway/c/sci_exportUI.c \
    sci_gateway/c/sci_figure.c

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_printsetupbox.cpp \
    sci_gateway/cpp/sci_toprint.cpp \
    sci_gateway/cpp/sci_printfigure.cpp \
    sci_gateway/cpp/sci_setlookandfeel.cpp \
    sci_gateway/cpp/sci_getinstalledlookandfeels.cpp \
    sci_gateway/cpp/sci_getlookandfeel.cpp \
    sci_gateway/cpp/sci_uigetdir.cpp \
    sci_gateway/cpp/sci_uigetfile.cpp \
    sci_gateway/cpp/sci_uigetfont.cpp \
    sci_gateway/cpp/sci_displaytree.cpp \
    sci_gateway/cpp/sci_uiputfile.cpp \
    sci_gateway/cpp/sci_about.cpp \
    sci_gateway/cpp/sci_raise_window.cpp \
    sci_gateway/cpp/sci_usecanvas.cpp \
    sci_gateway/cpp/sci_helpbrowser.cpp \
    sci_gateway/cpp/gui_gw.cpp \
    sci_gateway/cpp/sci_fire_closing_finished.cpp \
    sci_gateway/cpp/sci_useeditor.cpp \
    sci_gateway/cpp/sci_openged.cpp \
    sci_gateway/cpp/sci_datatipcreate.cpp \
    sci_gateway/cpp/sci_datatipremove.cpp \
    sci_gateway/cpp/sci_plotbrowser.cpp \
    sci_gateway/cpp/sci_datatip_manager_mode.cpp \
    sci_gateway/cpp/sci_datatip_toggle.cpp \
    sci_gateway/cpp/sci_datatipmove.cpp \
    sci_gateway/cpp/sci_datatip_set_style.cpp \
    sci_gateway/cpp/sci_datatip_set_display.cpp \
    sci_gateway/cpp/sci_datatip_set_interp.cpp \
    sci_gateway/cpp/sci_datatip_set_orient.cpp \
    sci_gateway/cpp/sci_saveGui.cpp \
    sci_gateway/cpp/sci_loadGui.cpp \
    sci_gateway/cpp/sci_x_choice.cpp \
    sci_gateway/cpp/sci_uigetcolor.cpp \
    sci_gateway/cpp/sci_ClipBoard.cpp \
    sci_gateway/cpp/sci_x_mdialog.cpp \
    sci_gateway/cpp/sci_x_dialog.cpp

libscigui_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/graphics/src/c/getHandleProperty/ \
    -I$(top_srcdir)/modules/graphic_objects/includes/ \
    -I$(top_srcdir)/modules/graphic_export/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(JAVA_JNI_INCLUDE) \
    $(AM_CPPFLAGS)

# Without the GUI module
libscigui_disable_la_CPPFLAGS = \
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


GUI_DISABLE_C_SOURCES = src/nogui/nogui.c
GUI_DISABLE_CXX_SOURCES = src/nogui/nogui_gw.cpp
libscigui_disable_la_SOURCES = $(GUI_DISABLE_C_SOURCES) $(GUI_DISABLE_CXX_SOURCES)


pkglib_LTLIBRARIES = libscigui-disable.la

if GUI
pkglib_LTLIBRARIES += libscigui.la
noinst_LTLIBRARIES = libscigui-algo.la
endif


libscigui_algo_la_SOURCES = $(GUI_C_SOURCES) $(GUI_CXX_SOURCES)
libscigui_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libscigui_algo_la_CPPFLAGS = $(libscigui_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(GUI_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigui_la_CPPFLAGS)

libscigui_la_LIBADD = libscigui-algo.la


#### gui : gateway declaration ####
libscigui_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigui_la_sci_gateway_DATA = sci_gateway/gui_gateway.xml


#### gui : icons files ####
#
# Generated with:
#    $ find images/ -type f -printf "%p \\\\\n" -name *.svg -or -name *.gif -or -name *.jpg -or -name *.png |sort -u
libscigui_la_iconsdir=$(mydatadir)/

nobase_libscigui_la_icons_DATA = \
images/icons/16x16/actions/code-navigator.png \
images/icons/16x16/actions/color-fill.png \
images/icons/16x16/actions/document-new.png \
images/icons/16x16/actions/document-open.png \
images/icons/16x16/actions/document-open-sci.png \
images/icons/16x16/actions/document-print.png \
images/icons/16x16/actions/document-print-preview.png \
images/icons/16x16/actions/document-save-as.png \
images/icons/16x16/actions/document-save.png \
images/icons/16x16/actions/dynamic-blue-up.png \
images/icons/16x16/actions/edit-clear.png \
images/icons/16x16/actions/edit-copy.png \
images/icons/16x16/actions/edit-cut.png \
images/icons/16x16/actions/edit-delete.png \
images/icons/16x16/actions/edit-find.png \
images/icons/16x16/actions/edit-find-replace.png \
images/icons/16x16/actions/edit-paste.png \
images/icons/16x16/actions/edit-redo.png \
images/icons/16x16/actions/edit-select-all.png \
images/icons/16x16/actions/edit-undo.png \
images/icons/16x16/actions/execute-all.png \
images/icons/16x16/actions/filter.png \
images/icons/16x16/actions/format-text-bold.png \
images/icons/16x16/actions/go-bottom.png \
images/icons/16x16/actions/go-next.png \
images/icons/16x16/actions/go-previous.png \
images/icons/16x16/actions/go-top.png \
images/icons/16x16/actions/list-add.png \
images/icons/16x16/actions/list-remove.png \
images/icons/16x16/actions/longe.png \
images/icons/16x16/actions/long.png \
images/icons/16x16/actions/media-playback-start.png \
images/icons/16x16/actions/media-playback-stop.png \
images/icons/16x16/actions/plot.png \
images/icons/16x16/actions/save-and-execute.png \
images/icons/16x16/actions/scilab_search.png \
images/icons/16x16/actions/shorte.png \
images/icons/16x16/actions/short.png \
images/icons/16x16/actions/SizeColumnsToFit.png \
images/icons/16x16/actions/system-search.png \
images/icons/16x16/actions/transform-rotate.png \
images/icons/16x16/actions/tree-collapse.png \
images/icons/16x16/actions/variable-from-selection.png \
images/icons/16x16/actions/variable-new.png \
images/icons/16x16/actions/view-refresh.png \
images/icons/16x16/actions/window-new.png \
images/icons/16x16/actions/zoom-area-cursor.png \
images/icons/16x16/actions/zoom-fit-drawing.png \
images/icons/16x16/actions/zoom-fit-selection.png \
images/icons/16x16/actions/zoom-in.png \
images/icons/16x16/actions/zoom-original.png \
images/icons/16x16/actions/zoom-out.png \
images/icons/16x16/actions/data_modify.png \
images/icons/16x16/animations/process-working.png \
images/icons/16x16/apps/accessories-text-editor.png \
images/icons/16x16/apps/datatips.png \
images/icons/16x16/apps/help-browser.png \
images/icons/16x16/apps/rrze_table.png \
images/icons/16x16/apps/scilab.png \
images/icons/16x16/apps/utilities-system-monitor.png \
images/icons/16x16/categories/applications-system.png \
images/icons/16x16/categories/preferences-system.png \
images/icons/16x16/devices/media-floppy.png \
images/icons/16x16/devices/printer.png \
images/icons/16x16/emblems/emblem-important.png \
images/icons/16x16/emblems/emblem-readonly.png \
images/icons/16x16/mimetypes/application-pdf.png \
images/icons/16x16/mimetypes/binary.png \
images/icons/16x16/mimetypes/image-x-generic.png \
images/icons/16x16/mimetypes/package-x-generic.png \
images/icons/16x16/mimetypes/text-csv.png \
images/icons/16x16/mimetypes/text-html.png \
images/icons/16x16/mimetypes/text-x-generic.png \
images/icons/16x16/mimetypes/text-xml.png \
images/icons/16x16/mimetypes/application-mat.png \
images/icons/16x16/places/folder.png \
images/icons/16x16/places/folder-saved-search.png \
images/icons/16x16/places/scilab-folder.png \
images/icons/16x16/places/user-home.png \
images/icons/16x16/status/dialog-information.png \
images/icons/16x16/status/dialog-warning.png \
images/icons/16x16/status/error.png \
images/icons/16x16/status/folder-open.png \
images/icons/16x16/status/line-found.png \
images/icons/16x16/status/scilab-folder-open.png \
images/icons/16x16/status/user-home-open.png \
images/icons/32x32/actions/edit-clear.png \
images/icons/32x32/actions/tree-diagramm.png \
images/icons/32x32/actions/tree-diagramm-delete.png \
images/icons/32x32/apps/accessories-text-editor.png \
images/icons/32x32/apps/graphic-window.png \
images/icons/32x32/apps/rrze_table.png \
images/icons/32x32/apps/scilab.png \
images/icons/32x32/apps/system-search.png \
images/icons/32x32/apps/utilities-system-monitor.png \
images/icons/32x32/apps/preferences-system.png \
images/icons/32x32/emblems/emblem-readonly.png \
images/icons/48x48/emblems/emblem-readonly.png \
images/icons/64x40/mimetypes/application-x-scilab-champ.png \
images/icons/64x40/mimetypes/application-x-scilab-contour2d.png \
images/icons/64x40/mimetypes/application-x-scilab-grayplot.png \
images/icons/64x40/mimetypes/application-x-scilab-hist3d.png \
images/icons/64x40/mimetypes/application-x-scilab-histplot.png \
images/icons/64x40/mimetypes/application-x-scilab-Matplot.png \
images/icons/64x40/mimetypes/application-x-scilab-mesh.png \
images/icons/64x40/mimetypes/application-x-scilab-pie.png \
images/icons/64x40/mimetypes/application-x-scilab-plot2d.png \
images/icons/64x40/mimetypes/application-x-scilab-plot3d.png \
images/icons/64x40/mimetypes/application-x-scilab-Sgrayplot.png \
images/icons/64x40/mimetypes/application-x-scilab-surf.png \
images/icons/256x256/apps/accessories-text-editor.png \
images/icons/256x256/apps/puffin.png \
images/icons/256x256/apps/rrze_table.png \
images/icons/256x256/apps/system-search.png \
images/icons/256x256/apps/utilities-system-monitor.png \
images/icons/aboutscilab.png \
images/icons/check-icon.png \
images/icons/close-tab-on.png \
images/icons/close-tab.png \
images/icons/dock_help_active.png \
images/icons/dock_help_default.png \
images/icons/puff.png \
images/icons/rotate.png \
images/icons/scilab_close_active.png \
images/icons/scilab_close_default.png \
images/icons/undock_active.png \
images/icons/undock_default.png \
images/preferences/integrated.png \
images/preferences/simple.png


if GUI
USEANT=1
endif

include $(top_srcdir)/Makefile.incl.am
