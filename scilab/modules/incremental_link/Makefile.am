##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


#### Target ######
modulename=incremental_link


#### incremental_link : Conf files ####
libincremental_link_la_rootdir = $(mydatadir)
libincremental_link_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### incremental_link : init scripts #### 
libincremental_link_la_etcdir = $(mydatadir)/etc
libincremental_link_la_etc_DATA = etc/incremental_link.quit etc/incremental_link.start

#### incremental_link : compilation scripts #### 
libincremental_link_la_scriptdir = $(mydatadir)/src/scripts/
libincremental_link_la_script_DATA = src/scripts/Makefile.in \
src/scripts/configure.ac \
src/scripts/ltmain.sh \
src/scripts/Makefile.am \
src/scripts/aclocal.m4

libincremental_link_la_script_SCRIPTS = src/scripts/compilerDetection.sh \
src/scripts/config.guess \
src/scripts/config.sub \
src/scripts/configure \
src/scripts/depcomp \
src/scripts/install-sh \
src/scripts/missing \
src/scripts/scicompile.sh
include $(top_srcdir)/Makefile.incl.am
