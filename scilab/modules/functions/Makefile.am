##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
target=$(datadir)/functions


#### functions : Conf files ####
libscifunctions_la_rootdir = $(target)
libscifunctions_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### functions : Scilab macros ###
libscifunctions_la_macrodir = $(target)/macros
libscifunctions_la_macro_DATA22 = macros/getd.sci \
macros/head_comments.sci \
macros/macrovar.sci \
macros/old2newinstr.sci \
macros/createfun.sci \
macros/genlib_old.sci \
macros/get_profile.sci \
macros/plotprofile.sci \
macros/genlib.sci \
macros/get_function_path.sci \
macros/profile.sci \
macros/showprofile.sci macros/createfun.bin \
macros/genlib_old.bin \
macros/genlib.bin \
macros/getd.bin \
macros/get_function_path.bin \
macros/get_profile.bin \
macros/head_comments.bin \
macros/macrovar.bin \
macros/old2newinstr.bin \
macros/plotprofile.bin \
macros/profile.bin \
macros/showprofile.bin macros/names macros/lib macros/buildmacros.sce


#### functions : init scripts #### 
libscifunctions_la_etcdir = $(target)/etc
libscifunctions_la_etc_DATA = etc/functions.quit etc/functions.start

include $(top_srcdir)/Makefile.incl.am
