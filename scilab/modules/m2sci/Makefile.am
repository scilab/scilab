##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
target=$(datadir)/modules/m2sci


#### m2sci : Conf files ####
libm2sci_la_rootdir = $(target)
libm2sci_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml

#### m2sci : Macros ####
MACROSDIRSEXT =  macros/m2sci/kernel/ \
macros/m2sci/percent/ \
macros/m2sci/sci_files/

#### m2sci : init scripts #### 
libm2sci_la_etcdir = $(target)/etc
libm2sci_la_etc_DATA = etc/m2sci.quit etc/m2sci.start

include $(top_srcdir)/Makefile.incl.am
