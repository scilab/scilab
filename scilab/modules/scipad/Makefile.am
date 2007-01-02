##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/scipad


#### scipad : Conf files ####
libscipad_la_rootdir = $(target)
libscipad_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### scipad : Scilab macros ###
libscipad_la_macrodir = $(target)
nobase_libscipad_la_macro_DATA = macros/scipad_internals/lib \
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
libscipad_la_etcdir = $(target)/etc
libscipad_la_etc_DATA = etc/scipad.quit etc/scipad.start


#### scipad : documentation FR #### 
libscipad_la_helpfrdir = $(target)/help/fr
libscipad_la_helpfr_DATA2 = help/fr/whatis.htm \
help/fr/scipad.htm \
help/fr/edit_error.htm


#### scipad : documentation EN #### 
libscipad_la_helpendir = $(target)/help/eng
libscipad_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/scipad.htm \
help/eng/edit_error.htm

include $(top_srcdir)/Makefile.incl.am
