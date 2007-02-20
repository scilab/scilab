##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
target=$(datadir)/javasci

#### javasci : Conf files ####
libscijavasci_la_rootdir = $(target)
libscijavasci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION

#### javasci : init scripts #### 
libscijavasci_la_etcdir = $(target)/etc
libscijavasci_la_etc_DATA = etc/javasci.quit etc/javasci.start


#### javasci : include files ####
libscijavasci_la_includedir=$(sciincludedir)/javasci
libscijavasci_la_include_HEADERS = includes/javasci_Scilab.h \
includes/javasci_Scilab2.h \
includes/javasci_globals.h \
includes/javasci_SciStringArray.h \
includes/javasci_SciComplexArray.h \
includes/javasci_SciDoubleArray.h \
includes/javasci_SciBooleanArray.h


include $(top_srcdir)/Makefile.incl.am
