##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########



#### Target ######
target=$(datadir)/texmacs


#### texmacs : Conf files ####
libscitexmacs_la_rootdir = $(target)
libscitexmacs_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### texmacs : Scilab macros ###
libscitexmacs_la_macrodir = $(target)/macros
MACROS = macros/%p_p.sci \
macros/%b_p.sci \
macros/%r_p.sci \
macros/%c_p.sci \
macros/%s_p.sci \
macros/plotout.sci \
macros/texout.sci \
macros/%i_p.sci \
macros/texprint.sci \
macros/%l_p.sci \
macros/addmenus.sci macros/addmenus.bin \
macros/%b_p.bin \
macros/%c_p.bin \
macros/%i_p.bin \
macros/%l_p.bin \
macros/plotout.bin \
macros/%p_p.bin \
macros/%r_p.bin \
macros/%s_p.bin \
macros/texout.bin \
macros/texprint.bin macros/names macros/lib macros/buildmacros.sce


#### texmacs : init scripts #### 
libscitexmacs_la_etcdir = $(target)/etc
libscitexmacs_la_etc_DATA = etc/texmacs.quit etc/texmacs.start

include $(top_srcdir)/Makefile.incl.am
