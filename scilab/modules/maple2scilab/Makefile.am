##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/maple2scilab


#### maple2scilab : Conf files ####
libmaple2scilab_la_rootdir = $(target)
libmaple2scilab_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### maple2scilab : Scilab macros ###
libmaple2scilab_la_macrodir = $(target)/macros
libmaple2scilab_la_macro_DATA = macros/sci2map.sci macros/sci2map.bin macros/names macros/lib macros/buildmacros.sce


#### maple2scilab : init scripts #### 
libmaple2scilab_la_etcdir = $(target)/etc
libmaple2scilab_la_etc_DATA = etc/maple2scilab.quit etc/maple2scilab.start

#### maple2scilab : documentation FR #### 
libmaple2scilab_la_helpfrdir = $(target)/help/fr
libmaple2scilab_la_helpfr_DATA2 = help/fr/whatis.htm \
help/fr/sci2map.htm


#### maple2scilab : documentation EN #### 
libmaple2scilab_la_helpendir = $(target)/help/eng
libmaple2scilab_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/sci2map.htm

include $(top_srcdir)/Makefile.incl.am
