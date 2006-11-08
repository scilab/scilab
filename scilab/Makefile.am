##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)

AUTOMAKE_OPTIONS = 1.9 dist-zip dist-bzip2

SUBDIRS= libs \
	modules

ACLOCAL_AMFLAGS = -I m4/

changelogs = CHANGES

bin_PROGRAMS		= scilab-bin

bin_SCRIPTS			= scripts/scilab

scilab_bin_SOURCES		= $(top_srcdir)/modules/core/src/fortran/mainsci.f

scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
	$(X_EXTRA_LIBS) -lXt -lX11 -lSM

sysconf_DATA = $(top_srcdir)/etc/scilab.start \
$(top_srcdir)/etc/scilab.quit

all-local: ### Crappy code ... Need some clean up

macros:
	-@( cd $(top_builddir) ; SCI=`pwd` export SCI; \
	LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/i386/:$(JAVA_HOME)/jre/lib/i386/server/:$(JAVA_HOME)/jre/lib/i386/native_threads/"; 	export LD_LIBRARY_PATH; \
	./scilab-bin -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce)

scihelp:
	-@( cd $(top_builddir) ; SCI=`pwd` export SCI; \
	LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/i386/:$(JAVA_HOME)/jre/lib/i386/server/:$(JAVA_HOME)/jre/lib/i386/native_threads/"; 	export LD_LIBRARY_PATH; \
	./scilab-bin -nb -l eng -nouserstartup -nwni -e "xmltohtml()";exit;)

	-@( cd $(top_builddir) ; SCI=`pwd` export SCI; \
	LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/i386/:$(JAVA_HOME)/jre/lib/i386/server/:$(JAVA_HOME)/jre/lib/i386/native_threads/"; 	export LD_LIBRARY_PATH; \
	./scilab-bin -nb -l fr -nouserstartup -nwni -e "xmltohtml()";exit;)


check-local:
#	@..\..\..\bin\scilex -nb -l fr -e check_help();exit;
#	@..\..\..\bin\scilex -nb -l eng -e check_help();exit;

distclean-local::	
clean-local:
#	@..\..\..\bin\scilex -nb -l eng -nouserstartup -nwni -e clean_help();exit;
#	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e clean_help();exit;
