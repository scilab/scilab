##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/javasci

#### javasci : Conf files ####
libjavasci_la_rootdir = $(target)
libjavasci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### javasci : Scilab macros ###
libjavasci_la_macrodir = $(target)/macros
libjavasci_la_macro_DATA =  macros/buildmacros.sce


#### javasci : init scripts #### 
libjavasci_la_etcdir = $(target)/etc
libjavasci_la_etc_DATA = etc/javasci.quit etc/javasci.start


#### javasci : include files ####
libjavasci_la_includedir=$(includedir)/javasci
libjavasci_la_include_HEADERS = includes/javasci_Scilab.h \
includes/javasci_Scilab2.h \
includes/javasci_globals.h \
includes/javasci_SciStringArray.h \
includes/javasci_SciComplexArray.h \
includes/javasci_SciDoubleArray.h

include $(top_srcdir)/Makefile.incl.am
