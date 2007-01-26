##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
target=$(datadir)/scipad


#### scipad : Conf files ####
libsciscipad_la_rootdir = $(target)
libsciscipad_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### scipad : Scilab macros ###
libsciscipad_la_macrodir = $(target)
nobase_MACROS = macros/scipad_internals/lib \
macros/scipad_internals/LocalizeForScipad.sci \
macros/scipad_internals/ext_exists.sci \
macros/scipad_internals/msgdiff.sci \
macros/scipad_internals/FormatStringsForWatch.sci \
macros/scipad_internals/FormatWhereForWatch.sci \
macros/scipad.sci \
macros/editor.sci \
macros/edit_error.sci macros/scipad_internals/ext_exists.bin \
macros/scipad_internals/FormatStringsForWatch.bin \
macros/scipad_internals/FormatWhereForWatch.bin \
macros/scipad_internals/LocalizeForScipad.bin \
macros/scipad_internals/msgdiff.bin \
macros/edit_error.bin \
macros/editor.bin \
macros/scipad.bin macros/names macros/lib macros/buildmacros.sce


#### scipad : init scripts #### 
libsciscipad_la_etcdir = $(target)/etc
libsciscipad_la_etc_DATA = etc/scipad.quit etc/scipad.start


include $(top_srcdir)/Makefile.incl.am
