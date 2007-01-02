##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/functions


#### functions : Conf files ####
libfunctions_la_rootdir = $(target)
libfunctions_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### functions : Scilab macros ###
libfunctions_la_macrodir = $(target)/macros
libfunctions_la_macro_DATA = macros/getd.sci \
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
libfunctions_la_etcdir = $(target)/etc
libfunctions_la_etc_DATA = etc/functions.quit etc/functions.start

#### functions : documentation FR #### 
libfunctions_la_helpfrdir = $(target)/help/fr
libfunctions_la_helpfr_DATA2 = help/fr/macrovar.htm \
help/fr/library.htm \
help/fr/varargin.htm \
help/fr/macro.htm \
help/fr/profile.htm \
help/fr/plotprofile.htm \
help/fr/showprofile.htm \
help/fr/varargout.htm \
help/fr/functions.htm \
help/fr/function.htm \
help/fr/whatis.htm \
help/fr/getd.htm \
help/fr/genlib.htm \
help/fr/head_comments.htm \
help/fr/get_function_path.htm


#### functions : documentation EN #### 
libfunctions_la_helpendir = $(target)/help/eng
libfunctions_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/functions.htm \
help/eng/function.htm \
help/eng/genlib.htm \
help/eng/getd.htm \
help/eng/library.htm \
help/eng/get_function_path.htm \
help/eng/head_comments.htm \
help/eng/macrovar.htm \
help/eng/macro.htm \
help/eng/plotprofile.htm \
help/eng/profile.htm \
help/eng/showprofile.htm \
help/eng/varargin.htm \
help/eng/varargout.htm

include $(top_srcdir)/Makefile.incl.am
