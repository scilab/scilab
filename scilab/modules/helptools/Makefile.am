##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
target=$(datadir)/helptools

#SUFFIXES = .bin
#	SCISCRIPT=$<
#	$(top_builddir)/scilab-bin -ns -nwni -e "getf('$SCISCRIPT'); save('$SCISCRIPT.bin'); exit;"

#### helptools : Conf files ####
libscihelptools_la_rootdir = $(target)
libscihelptools_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### helptools : Scilab macros ###
libscihelptools_la_macrodir = $(target)/macros
dist_libscihelptools_la_macro_DATA22 = macros/add_help_chapter.sci \
macros/browsehelp.sci \
macros/help_menu.sci \
macros/make_help_index.sci \
macros/change_old_man.sci \
macros/find_links.sci \
macros/extract_help_examples.sci \
macros/manedit.sci \
macros/del_help_chapter.sci \
macros/apropos.sci \
macros/formatman.sci \
macros/dehtmlize.sci \
macros/check_help.sci \
macros/help.sci \
macros/gethelpfile.sci \
macros/loaddefaultbrowser.sci \
macros/xmlfiletohtml.sci \
macros/xmltohtml.sci \
macros/clean_help.sci \
macros/initial_help_chapters.sci \
macros/help_skeleton.sci \
macros/change_help_language.sci macros/add_help_chapter.bin \
macros/apropos.bin \
macros/browsehelp.bin \
macros/change_help_language.bin \
macros/change_old_man.bin \
macros/check_help.bin \
macros/clean_help.bin \
macros/dehtmlize.bin \
macros/del_help_chapter.bin \
macros/extract_help_examples.bin \
macros/find_links.bin \
macros/formatman.bin \
macros/gethelpfile.bin \
macros/help_menu.bin \
macros/help.bin \
macros/help_skeleton.bin \
macros/initial_help_chapters.bin \
macros/loaddefaultbrowser.bin \
macros/make_help_index.bin \
macros/manedit.bin \
macros/xmlfiletohtml.bin \
macros/xmltohtml.bin macros/names macros/lib macros/buildmacros.sce


#### helptools : init scripts #### 
libscihelptools_la_etcdir = $(target)/etc
libscihelptools_la_etc_DATA = etc/helptools.quit etc/helptools.start

include $(top_srcdir)/Makefile.incl.am
