##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


datadir=${prefix}/modules
#### Target ######
target=$(datadir)/development_tools


#### development_tools : Conf files ####
libdevelopment_tools_la_rootdir = $(target)
libdevelopment_tools_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### development_tools : Scilab macros ###
libdevelopment_tools_la_macrodir = $(target)/macros
libdevelopment_tools_la_macro_DATA = macros/scitest.sci \
macros/testexamples.sci macros/scitest.bin \
macros/testexamples.bin macros/names macros/lib macros/buildmacros.sce


#### development_tools : init scripts #### 
libdevelopment_tools_la_etcdir = $(target)/etc
libdevelopment_tools_la_etc_DATA = etc/development_tools.quit etc/development_tools.start


#### development_tools : documentation FR #### 
libdevelopment_tools_la_helpfrdir = $(target)/help/fr
libdevelopment_tools_la_helpfr_DATA2 = 


#### development_tools : documentation EN #### 
libdevelopment_tools_la_helpendir = $(target)/help/eng
libdevelopment_tools_la_helpen_DATA2 = 

include $(top_srcdir)/Makefile.incl.am
