##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########


INCREMENTAL_LINK_C_SOURCES = src/c/addinter.c \
src/c/link.c


#src/c/link_SYSV.c 
#src/c/link_std.c 
#src/c/link_W95.c

libsciincremental_link_la_CFLAGS = \
-I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/libs/doublylinkedlist/includes \
-Isrc/c/ \
-Iincludes/

pkglib_LTLIBRARIES = libsciincremental_link.la
libsciincremental_link_la_SOURCES = $(INCREMENTAL_LINK_C_SOURCES)



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
