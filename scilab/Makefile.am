##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2008
##########

## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)

AUTOMAKE_OPTIONS = 1.9 dist-zip dist-bzip2

SUBDIRS= libs \
	modules \
	contrib

ACLOCAL_AMFLAGS = -I m4/

changelogs = CHANGES

bin_PROGRAMS		= scilab-bin

bin_SCRIPTS			= bin/scilab

scilab_bin_SOURCES		= $(top_srcdir)/modules/core/src/c/main_linux.c

scilab_bin_LDFLAGS 		=
scilab_bin_CFLAGS 		=
# --no-undefined --relax


# libshell is mandatory here since main_linux.c depends on it.
# Localization dep already exists because of the `exportLocaleToSystem' symbol
# (causing issues on some platform)
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
$(top_builddir)/modules/shell/libscishell.la \
$(top_builddir)/modules/localization/libscilocalization.la \
$(top_builddir)/modules/core/libscicore.la

if NEED_JAVA
	scilab_bin_LDADD += $(JAVA_JNI_LIBS)
#	$(X_EXTRA_LIBS) -lXt -lX11 -lSM -lGLU
#else
#	scilab_bin_LDADD +=	$(X_EXTRA_LIBS)
endif


if ENABLE_STATIC
scilab_static_bin_LDFLAGS 		= -static -static-libgcc --start-group
scilab_static_bin_CFLAGS =  -Wl,--start-group `$(CXX) -print-file-name=libstdc++.a`

#-static
bin_PROGRAMS	   		+= scilab-static-bin
scilab_static_bin_LDADD = $(top_builddir)/modules/libscilab.la

scilab_static_bin_SOURCES = $(scilab_bin_SOURCES)
endif


LANG_DOMAIN=scilab

TARGETS_ALL=macros
# doc

#  The user wants to compile the localization files
if GENERATE_LOCALIZATION_FILES
TARGETS_ALL+=localization
endif

all-local: $(TARGETS_ALL)

rootdir=$(pkgdatadir)/etc/
root_DATA = $(top_srcdir)/etc/scilab.start \
$(top_srcdir)/etc/scilab.quit \
$(top_srcdir)/etc/modules.xml \
$(top_srcdir)/etc/classpath.xml \
$(top_srcdir)/etc/librarypath.xml \
$(top_srcdir)/etc/toolboxes.xml


infofilesdir=$(pkgdatadir)/
infofiles_DATA = $(top_srcdir)/CHANGES \
$(top_srcdir)/ACKNOWLEDGEMENTS \
$(top_srcdir)/COPYING \
$(top_srcdir)/COPYING-FR \
$(top_srcdir)/README_Unix \
$(top_srcdir)/RELEASE_NOTES \
$(top_srcdir)/Readme_Visual.txt \
$(top_srcdir)/README_Windows.txt


profilingdir=$(pkgdatadir)/tools/profiling/
profiling_DATA = $(top_srcdir)/tools/profiling/valgrind.supp


macros:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	-@( cd $(top_builddir) ; \
	echo "$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce" ; \
	$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce)

doc:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
if BUILD_HELP
	-@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		echo "-- Building documentation (English) --"; \
		echo "$(top_builddir)/bin/scilab -nb -l en_US -nouserstartup -nw -e \"xmltojar();exit;\"";\
		DOCBOOK_ROOT=$(DOCBOOK_ROOT) $(top_builddir)/bin/scilab -nb -l en_US -nouserstartup -nw -e "xmltojar();exit;";\
		echo "-- Building documentation (French) --"; \
		echo "$(top_builddir)/bin/scilab -nb -l fr_FR -nouserstartup -nw -e \"xmltojar();exit;\""; \
		DOCBOOK_ROOT=$(DOCBOOK_ROOT) $(top_builddir)/bin/scilab -nb -l fr_FR -nouserstartup -nw -e "xmltojar();exit;"; \
	else \
		echo "Cannot find Scilab binary"; \
	fi
else
	@echo "Cannot buid help. Add --enable-build-help to the ./configure if you want to build it."
endif

check-help:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	@if test -x $(top_builddir)/scilab-bin; then \
		echo "-- Checking documentation (English) --"; \
		$(top_builddir)/bin/scilab -nwni -nb -l en -e 'check_help();exit;';\
		echo "-- Checking documentation (French) --"; \
		$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'check_help();exit;';\
	else \
		echo "Cannot find Scilab binary"; \
	fi

check-recursive: check-local
check-local:
	@if test -x $(top_builddir)/scilab-bin; then \
		COMMAND="test_run();quit";\
		export LANG=C;\
		$(top_builddir)/bin/scilab -nw -nb -e "$$COMMAND"; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

distclean-recursive: clean-recursive

clean-recursive: clean-doc clean-localization clean-macros

clean-macros:
	@echo "-- Cleanning macros (*.bin) --"; \
	find $(top_builddir) -name "*.bin" | xargs rm 2> /dev/null; \
	echo "-- Cleanning names files (names) --"; \
	find $(top_builddir)/modules/*/macros -name "names" | xargs rm 2> /dev/null; \
	echo "-- Cleanning lib files (lib) --"; \
	find $(top_builddir)/modules/*/macros -name "lib" | xargs rm 2> /dev/null


clean-doc:
	@if test -x $(top_builddir)/scilab-bin; then \
		$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'clean_help();exit;'; \
		$(top_builddir)/bin/scilab -nwni -nb -l en -e 'clean_help();exit;'; \
	else \
		echo "Cannot find Scilab binary"; \
	fi


clean-tests:
	@if test -x $(top_builddir)/scilab-bin; then \
		$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'test_clean();exit;'; \
	else \
		echo "Cannot find Scilab binary"; \
	fi


clean-localization:
	rm -rf $(top_srcdir)/locale/


# The user wants to compile the localization files
# Please note that ALL_LINGUAS is defined in configure.ac
if GENERATE_LOCALIZATION_FILES
localization:
	@echo "-- Build localization (.mo from .po) --"; \
	MASK="modules/*/locales/"; \
	LC=LC_MESSAGES; \
	MSGFMT_OPTS="$MSGFMT_OPTS --check"; \
	FILENAME="*.po*"; \
	rm modules/*/locales/*/*~ 2> /dev/null; \
	for LOCALE in $(ALL_LINGUAS); do \
		echo "        -- Building for $$LOCALE --"; \
		FILES=`ls $$MASK/$$LOCALE/$$FILENAME`; \
		PATHTO=$(top_builddir)/locale/$$LOCALE/$$LC/; \
		if test ! -d $$PATHTO; then \
			echo "        Creating $$PATHTO"; \
			mkdir -p $$PATHTO; \
		fi; \
		POFILE=$$PATHTO/$(LANG_DOMAIN).po; \
		$(MSGCAT) -o $$POFILE $$FILES; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics -o $$PATHTO/$(LANG_DOMAIN).mo $$POFILE; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics --locale $$LOCALE -r org.scilab.modules.localization.Messages --java2 -d $(top_builddir)/modules/localization/build/classes/ $$POFILE; \
	done; \
	(cd $(top_srcdir)/modules/localization/; $(ANT) jar);
endif

# Misc variable for the documentation installation

# How help files look like.
DOCMASK=scilab_*_help.jar

# Path to builded files
PATHTOHELPFILES = modules/helptools/jar/

if GENERATE_LOCALIZATION_FILES
install-data-local:
	@$(mkinstalldirs) $(DESTDIR)$(localedir); \
	echo $(mkinstalldirs) $(DESTDIR)$(localedir); \
	for lang in $(ALL_LINGUAS); do \
		LANG_PATH_DIST=$(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/; \
		echo $(mkinstalldirs) $$LANG_PATH_DIST; \
		$(mkinstalldirs) $$LANG_PATH_DIST; \
		echo $(INSTALL_DATA) $(top_builddir)/locale/$$lang/LC_MESSAGES/$(LANG_DOMAIN).mo $$LANG_PATH_DIST/; \
		$(INSTALL_DATA) $(top_builddir)/locale/$$lang/LC_MESSAGES/$(LANG_DOMAIN).mo $$LANG_PATH_DIST/; \
	done
endif

################ INSTALL DOCUMENTATION #################
# Starting from Scilab 5, documentation file are provided by Docbook
# In the Scilab application we are generating a JavaHelp file.
if BUILD_HELP
	@$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOHELPFILES); \
	echo $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOHELPFILES); \
	if ls -lLd $(top_builddir)/$(PATHTOHELPFILES)/$(DOCMASK) >/dev/null 2>&1; then \
		for file in $(top_builddir)/$(PATHTOHELPFILES)/$(DOCMASK); do \
		    echo "$(INSTALL_DATA) $$file $(DESTDIR)$(pkgdatadir)/$(PATHTOHELPFILES)" ; \
	    	$(INSTALL_DATA) "$$file" $(DESTDIR)$(pkgdatadir)/$(PATHTOHELPFILES) ; \
		done; \
	fi
endif

# Pkgconfig directory
pkgconfigdir = $(libdir)/pkgconfig

# Files to install in Pkgconfig directory
pkgconfig_DATA = scilab.pc

.PHONY: macros localization doc

.NOTPARALLEL: macros doc


