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
src/c/GlobalTclInterp.c \
src/c/TCL_getErrorLine.c

GATEWAY_C_SOURCES =  sci_gateway/c/sci_opentk.c \
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

else

TCLSCI_C_SOURCES = src/notclsci/notclsci.c

GATEWAY_C_SOURCES =

endif

pkglib_LTLIBRARIES = libscitclsci.la

libscitclsci_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(TCLTK_LIBS)

libscitclsci_la_CFLAGS= -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c \
			-I$(srcdir)/sci_gateway/c/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			$(TCL_INC_PATH) \
			$(TK_INC_PATH) \
			-I$(top_srcdir)/modules/action_binding/includes \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/localization/includes

libscitclsci_la_SOURCES = $(TCLSCI_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscitclsci_la_SOURCES)
INCLUDE_FLAGS = $(libscitclsci_la_CFLAGS)

libscitclsci_la_LIBADD =  $(top_builddir)/modules/action_binding/libsciaction_binding.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Target ######
modulename=tclsci

#### tclsci : Conf files ####
libscitclsci_la_rootdir = $(mydatadir)
libscitclsci_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### tclsci : init scripts ####
libscitclsci_la_etcdir = $(mydatadir)/etc
libscitclsci_la_etc_DATA = etc/tclsci.quit etc/tclsci.start

### TCL macros ###
libscitclsci_la_tcldir = $(mydatadir)
nobase_libscitclsci_la_tcl_DATA = tcl/utils/tk_choose.tcl \
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
tcl/sciGUI/sciConfig.tcl \
tcl/sciGUI/sciBrowseVar.tcl \
tcl/sciGUI/sciEditVar.tcl \
tcl/BWidget-1.8.0/tests/entry.test \
tcl/BWidget-1.8.0/BWman/SelectColor.html \
tcl/BWidget-1.8.0/BWman/SpinBox.html \
tcl/BWidget-1.8.0/BWman/ButtonBox.html \
tcl/BWidget-1.8.0/BWman/ScrollView.html \
tcl/BWidget-1.8.0/BWman/LabelFrame.html \
tcl/BWidget-1.8.0/BWman/ProgressDlg.html \
tcl/BWidget-1.8.0/BWman/TitleFrame.html \
tcl/BWidget-1.8.0/BWman/Dialog.html \
tcl/BWidget-1.8.0/BWman/Separator.html \
tcl/BWidget-1.8.0/BWman/Entry.html \
tcl/BWidget-1.8.0/BWman/navtree.html \
tcl/BWidget-1.8.0/BWman/SelectFont.html \
tcl/BWidget-1.8.0/BWman/PanelFrame.html \
tcl/BWidget-1.8.0/BWman/Button.html \
tcl/BWidget-1.8.0/BWman/MessageDlg.html \
tcl/BWidget-1.8.0/BWman/PanedWindow.html \
tcl/BWidget-1.8.0/BWman/Label.html \
tcl/BWidget-1.8.0/BWman/options.htm \
tcl/BWidget-1.8.0/BWman/NoteBook.html \
tcl/BWidget-1.8.0/BWman/Widget.html \
tcl/BWidget-1.8.0/BWman/ListBox.html \
tcl/BWidget-1.8.0/BWman/BWidget.html \
tcl/BWidget-1.8.0/BWman/ArrowButton.html \
tcl/BWidget-1.8.0/BWman/PasswdDlg.html \
tcl/BWidget-1.8.0/BWman/ProgressBar.html \
tcl/BWidget-1.8.0/BWman/DropSite.html \
tcl/BWidget-1.8.0/BWman/PagesManager.html \
tcl/BWidget-1.8.0/BWman/ScrollableFrame.html \
tcl/BWidget-1.8.0/BWman/DynamicHelp.html \
tcl/BWidget-1.8.0/BWman/contents.html \
tcl/BWidget-1.8.0/BWman/ScrolledWindow.html \
tcl/BWidget-1.8.0/BWman/MainFrame.html \
tcl/BWidget-1.8.0/BWman/Tree.html \
tcl/BWidget-1.8.0/BWman/LabelEntry.html \
tcl/BWidget-1.8.0/BWman/DragSite.html \
tcl/BWidget-1.8.0/BWman/index.html \
tcl/BWidget-1.8.0/BWman/StatusBar.html \
tcl/BWidget-1.8.0/BWman/ComboBox.html \
tcl/BWidget-1.8.0/images/folder.gif \
tcl/BWidget-1.8.0/images/minus.xbm \
tcl/BWidget-1.8.0/images/paste.gif \
tcl/BWidget-1.8.0/images/dragfile.gif \
tcl/BWidget-1.8.0/images/oplink.xbm \
tcl/BWidget-1.8.0/images/file.gif \
tcl/BWidget-1.8.0/images/bold.gif \
tcl/BWidget-1.8.0/images/underline.gif \
tcl/BWidget-1.8.0/images/dragicon.gif \
tcl/BWidget-1.8.0/images/opmove.xbm \
tcl/BWidget-1.8.0/images/new.gif \
tcl/BWidget-1.8.0/images/error.gif \
tcl/BWidget-1.8.0/images/redo.gif \
tcl/BWidget-1.8.0/images/opcopy.xbm \
tcl/BWidget-1.8.0/images/info.gif \
tcl/BWidget-1.8.0/images/cut.gif \
tcl/BWidget-1.8.0/images/print.gif \
tcl/BWidget-1.8.0/images/overstrike.gif \
tcl/BWidget-1.8.0/images/palette.gif \
tcl/BWidget-1.8.0/images/save.gif \
tcl/BWidget-1.8.0/images/open.gif \
tcl/BWidget-1.8.0/images/passwd.gif \
tcl/BWidget-1.8.0/images/plus.xbm \
tcl/BWidget-1.8.0/images/italic.gif \
tcl/BWidget-1.8.0/images/undo.gif \
tcl/BWidget-1.8.0/images/warning.gif \
tcl/BWidget-1.8.0/images/openfold.gif \
tcl/BWidget-1.8.0/images/hourglass.gif \
tcl/BWidget-1.8.0/images/question.gif \
tcl/BWidget-1.8.0/images/target.xbm \
tcl/BWidget-1.8.0/images/copy.gif \
tcl/BWidget-1.8.0/lang/en.rc \
tcl/BWidget-1.8.0/lang/da.rc \
tcl/BWidget-1.8.0/lang/es.rc \
tcl/BWidget-1.8.0/lang/fr.rc \
tcl/BWidget-1.8.0/lang/de.rc \
tcl/BWidget-1.8.0/mainframe.tcl \
tcl/BWidget-1.8.0/dialog.tcl \
tcl/BWidget-1.8.0/utils.tcl \
tcl/BWidget-1.8.0/wizard.tcl \
tcl/BWidget-1.8.0/labelentry.tcl \
tcl/BWidget-1.8.0/entry.tcl \
tcl/BWidget-1.8.0/init.tcl \
tcl/BWidget-1.8.0/pagesmgr.tcl \
tcl/BWidget-1.8.0/font.tcl \
tcl/BWidget-1.8.0/pkgIndex.tcl \
tcl/BWidget-1.8.0/scrollframe.tcl \
tcl/BWidget-1.8.0/panelframe.tcl \
tcl/BWidget-1.8.0/button.tcl \
tcl/BWidget-1.8.0/panedw.tcl \
tcl/BWidget-1.8.0/label.tcl \
tcl/BWidget-1.8.0/notebook.tcl \
tcl/BWidget-1.8.0/widget.tcl \
tcl/BWidget-1.8.0/listbox.tcl \
tcl/BWidget-1.8.0/scrollw.tcl \
tcl/BWidget-1.8.0/passwddlg.tcl \
tcl/BWidget-1.8.0/scrollview.tcl \
tcl/BWidget-1.8.0/xpm2image.tcl \
tcl/BWidget-1.8.0/arrow.tcl \
tcl/BWidget-1.8.0/progressdlg.tcl \
tcl/BWidget-1.8.0/tree.tcl \
tcl/BWidget-1.8.0/separator.tcl \
tcl/BWidget-1.8.0/dragsite.tcl \
tcl/BWidget-1.8.0/dynhelp.tcl \
tcl/BWidget-1.8.0/CHANGES.txt \
tcl/BWidget-1.8.0/ChangeLog \
tcl/BWidget-1.8.0/combobox.tcl \
tcl/BWidget-1.8.0/statusbar.tcl \
tcl/BWidget-1.8.0/messagedlg.tcl \
tcl/BWidget-1.8.0/bitmap.tcl \
tcl/BWidget-1.8.0/color.tcl \
tcl/BWidget-1.8.0/LICENSE.txt \
tcl/BWidget-1.8.0/spinbox.tcl \
tcl/BWidget-1.8.0/buttonbox.tcl \
tcl/BWidget-1.8.0/dropsite.tcl \
tcl/BWidget-1.8.0/progressbar.tcl \
tcl/BWidget-1.8.0/labelframe.tcl \
tcl/BWidget-1.8.0/README.txt \
tcl/BWidget-1.8.0/titleframe.tcl \
tcl/TK_Scilab.tcl

#### tclsci : gateway declaration ####
libscitclsci_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitclsci_la_sci_gateway_DATA = sci_gateway/tclsci_gateway.xml

include $(top_srcdir)/Makefile.incl.am

