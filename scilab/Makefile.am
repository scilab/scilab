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

bin_SCRIPTS			= bin/scilab

scilab_bin_SOURCES		= $(top_srcdir)/modules/core/src/c/main_linux.c


if GUI
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
	$(X_EXTRA_LIBS) -lXt -lX11 -lSM -lGLU
else
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
	$(X_EXTRA_LIBS)
endif


rootdir=$(pkgdatadir)/etc/
root_DATA = $(top_srcdir)/etc/scilab.start \
$(top_srcdir)/etc/scilab.quit \
$(top_srcdir)/etc/modules.xml \
$(top_srcdir)/etc/classpath.xml \
$(top_srcdir)/etc/librarypath.xml \
$(top_srcdir)/etc/toolboxes.xml

all: macros
 ### Crappy code ... Need some clean up

macros:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	-@( cd $(top_builddir) ; \
	LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/i386/:$(JAVA_HOME)/jre/lib/i386/server/:$(JAVA_HOME)/jre/lib/i386/native_threads/:$(LD_LIBRARY_PATH)"; 	export LD_LIBRARY_PATH; \
	echo "$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce" ; \
	$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce)

doc:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	-@( cd $(top_builddir) ; \
	LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/i386/:$(JAVA_HOME)/jre/lib/i386/server/:$(JAVA_HOME)/jre/lib/i386/native_threads/:$(LD_LIBRARY_PATH)"; 	export LD_LIBRARY_PATH; \
	$(top_builddir)/bin/scilab -nb -l en -nouserstartup -nwni -e "xmltohtml();exit;")

	-@( cd $(top_builddir) ; \
	LD_LIBRARY_PATH="$(JAVA_HOME)/jre/lib/i386/:$(JAVA_HOME)/jre/lib/i386/server/:$(JAVA_HOME)/jre/lib/i386/native_threads/:$(LD_LIBRARY_PATH)"; 	export LD_LIBRARY_PATH; \
	$(top_builddir)/bin/scilab -nb -l fr -nouserstartup -nwni -e "xmltohtml();exit;")


check-help:
	$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'check_help();exit;'
	$(top_builddir)/bin/scilab -nwni -nb -l en -e 'check_help();exit;'

distclean-local::
clean-local:
#	@..\..\..\bin\scilex -nb -l en -nouserstartup -nwni -e clean_help();exit;
#	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e clean_help();exit;


# Pkgconfig directory
pkgconfigdir = $(libdir)/pkgconfig

# Files to install in Pkgconfig directory
pkgconfig_DATA =  scilab.pc
