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

#### javasci : documentation FR #### 
libjavasci_la_helpfrdir = $(target)/help/fr
libjavasci_la_helpfr_DATA = help/fr/SciComplex.htm \
help/fr/SciDouble.htm \
help/fr/Scilab.htm \
help/fr/SciString.htm \
help/fr/whatis.htm \
help/fr/javasci.htm \
help/fr/SciStringArray.htm \
help/fr/SciComplexArray.htm \
help/fr/SciDoubleArray.htm


#### javasci : documentation EN #### 
libjavasci_la_helpendir = $(target)/help/eng
libjavasci_la_helpen_DATA = help/eng/whatis.htm \
help/eng/javasci.htm \
help/eng/Scilab.htm \
help/eng/SciComplexArray.htm \
help/eng/SciComplex.htm \
help/eng/SciDoubleArray.htm \
help/eng/SciDouble.htm \
help/eng/SciStringArray.htm \
help/eng/SciString.htm


#### javasci : include files ####
libjavasci_la_includedir=$(includedir)/javasci
libjavasci_la_include_HEADERS = includes/javasci_Scilab.h \
includes/javasci_Scilab2.h \
includes/javasci_globals.h \
includes/javasci_SciStringArray.h \
includes/javasci_SciComplexArray.h \
includes/javasci_SciDoubleArray.h

