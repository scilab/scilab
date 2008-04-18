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
scilab_bin_LDFLAGS 		= 
#-static
# libshell is mandatory here since main_linux.c depends on it.
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
$(top_builddir)/modules/shell/libscishell.la

if GUI
	scilab_bin_LDADD += $(JAVA_JNI_LIBS)
#	$(X_EXTRA_LIBS) -lXt -lX11 -lSM -lGLU
#else
#	scilab_bin_LDADD +=	$(X_EXTRA_LIBS)
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


macros:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	-@( cd $(top_builddir) ; \
	echo "$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce" ; \
	$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce)

doc:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	-@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		echo "-- Building documentation (English) --"; \
		echo "$(top_builddir)/bin/scilab -nb -l en_US -nouserstartup -nw -e \"xmltojar();exit;\"";\
		$(top_builddir)/bin/scilab -nb -l en_US -nouserstartup -nw -e "xmltojar();exit;";\
		echo "-- Building documentation (French) --"; \
		echo "$(top_builddir)/bin/scilab -nb -l fr_FR -nouserstartup -nw -e \"xmltojar();exit;\""; \
		$(top_builddir)/bin/scilab -nb -l fr_FR -nouserstartup -nw -e "xmltojar();exit;"; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

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


#  The user wants to compile the localization files
if GENERATE_LOCALIZATION_FILES
localization:
	@echo "-- Build localization (.mo from .po) --"; \
	MASK="modules/*/locales/"; \
	LC=LC_MESSAGES; \
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
		(cd $(top_srcdir)/modules/localization/; $(ANT) jar); \
	done
endif

install-data-local:
	@$(mkinstalldirs) $(DESTDIR)$(localedir); \
	echo $(mkinstalldirs) $(DESTDIR)$(localedir); \
	for lang in $(ALL_LINGUAS); do \
		LANG_PATH_DIST=$(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/; \
		echo $(mkinstalldirs) $$LANG_PATH_DIST; \
		$(mkinstalldirs) $$LANG_PATH_DIST; \
		echo $(INSTALL_DATA) locale/$$lang/LC_MESSAGES/$(LANG_DOMAIN).mo $$LANG_PATH_DIST/; \
		$(INSTALL_DATA) locale/$$lang/LC_MESSAGES/$(LANG_DOMAIN).mo $$LANG_PATH_DIST/; \
	done

# Pkgconfig directory
pkgconfigdir = $(libdir)/pkgconfig

# Files to install in Pkgconfig directory
pkgconfig_DATA = scilab.pc

.PHONY: macros localization doc

.NOTPARALLEL: macros doc


