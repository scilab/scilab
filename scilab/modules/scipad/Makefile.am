##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

if TCLTK

#### scipad : Target ######
modulename=scipad

#### scipad : Conf files ####
libsciscipad_la_rootdir = $(mydatadir)
libsciscipad_la_root_DATA = changelog.txt license.txt readme.txt version.xml BUGS

MACROSDIRSEXT = macros/scipad_internals

#### scipad : init scripts ####
libsciscipad_la_etcdir = $(mydatadir)/etc
libsciscipad_la_etc_DATA = etc/scipad.quit etc/scipad.start

### scipad : TCL macros ###

libsciscipad_la_tcldir = $(mydatadir)/tcl
libsciscipad_la_tcl_DATA = tcl/buffernavigation.tcl \
tcl/colorize.tcl \
tcl/commonbindings.tcl \
tcl/completion.tcl \
tcl/cutcopypaste.tcl \
tcl/db_actbptmanagement.tcl \
tcl/db_argsmanagement.tcl \
tcl/db_bptsgui.tcl \
tcl/db_bptsmanagement.tcl \
tcl/db_configureexec.tcl \
tcl/db_debugsession.tcl \
tcl/db_init.tcl \
tcl/db_states.tcl \
tcl/db_watch.tcl \
tcl/debugger.tcl \
tcl/defaults.tcl \
tcl/filecommands.tcl \
tcl/findinfiles.tcl \
tcl/findreplace.tcl \
tcl/fonts.tcl \
tcl/helps.tcl \
tcl/infomessages.tcl \
tcl/inputtext.tcl \
tcl/localetools.tcl \
tcl/mainwindow.tcl \
tcl/menues.tcl \
tcl/modselection.tcl \
tcl/MRUlist.tcl \
tcl/platformbind.tcl \
tcl/popupmenus.tcl \
tcl/print.tcl \
tcl/progressbar.tcl \
tcl/scilabexec.tcl \
tcl/scipad.tcl \
tcl/scipaddebug1.tcl \
tcl/scipaddebug2.tcl \
tcl/scrollableframe.tcl \
tcl/textarea.tcl \
tcl/tkdndtools.tcl \
tcl/tooltips.tcl \
tcl/undoredo.tcl \
tcl/whichfun.tcl

### scipad : Bindings ###

libsciscipad_la_bindingsdir = $(mydatadir)/tcl/bindings
libsciscipad_la_bindings_DATA = tcl/bindings/emacs.tcl \
tcl/bindings/issues.txt \
tcl/bindings/mac-pc.tcl

### scipad : localization ###

libsciscipad_la_msgfilesdir = $(mydatadir)/tcl/msg_files
libsciscipad_la_msgfiles_DATA = tcl/msg_files/AddingTranslations.txt \
tcl/msg_files/da_dk.msg \
tcl/msg_files/de_de.msg \
tcl/msg_files/en_us.msg \
tcl/msg_files/es_es.msg \
tcl/msg_files/fr_fr.msg \
tcl/msg_files/it_it.msg \
tcl/msg_files/localenames.tcl \
tcl/msg_files/no.msg \
tcl/msg_files/pl.msg \
tcl/msg_files/se.msg \
tcl/msg_files/zh_cn.msg \
tcl/msg_files/zh_tw.msg

### scipad : icons ###
libsciscipad_la_iconsdir = $(mydatadir)/tcl/icons
libsciscipad_la_icons_DATA = tcl/icons/arrowcursor.gif \
tcl/icons/arrowend.gif \
tcl/icons/arrowend2.gif \
tcl/icons/arrowenter.gif \
tcl/icons/arrowexit.gif \
tcl/icons/arrowframe.gif \
tcl/icons/arrowignore.gif \
tcl/icons/arrowin.gif \
tcl/icons/arrowout.gif \
tcl/icons/arrowover.gif \
tcl/icons/arrowstop.gif \
tcl/icons/arrowtoreturn.gif \
tcl/icons/arrowtoreturn2.gif \
tcl/icons/arrowwatch.gif \
tcl/icons/breakpointreturn.gif \
tcl/icons/dbs_DebugInProgress_48x24.gif \
tcl/icons/dbs_NoDebug_48x24.gif \
tcl/icons/dbs_ReadyForDebug_48x24.gif \
tcl/icons/prompt_busy_48x24.gif \
tcl/icons/prompt_ready_48x24.gif \
tcl/icons/removeallbp.gif \
tcl/icons/setbp.gif \
tcl/icons/stepstep.gif \
tcl/icons/scipad-editor.gif \
tcl/icons/editbp.gif \
tcl/icons/questionbp.gif \
tcl/icons/questionbpbig.gif

include $(top_srcdir)/Makefile.incl.am

else

#### Target ######
modulename=scipad

#### scipad : Conf files ####
libsciscipad_la_rootdir = $(mydatadir)
libsciscipad_la_root_DATA = changelog.txt license.txt readme.txt version.xml BUGS

MACROSDIRSEXT = macros/scipad_internals

#### scipad : init scripts ####
libsciscipad_la_etcdir = $(mydatadir)/etc
libsciscipad_la_etc_DATA = etc/scipad.quit etc/scipad.start

include $(top_srcdir)/Makefile.incl.am

endif
