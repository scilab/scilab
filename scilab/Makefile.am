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
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la 
#	$(X_EXTRA_LIBS) -lXt -lX11 -lSM -lGLU
else
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
	$(X_EXTRA_LIBS)
endif

TARGETS_ALL=macros

LANG_DOMAIN=scilab

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
	-@( cd $(top_builddir) ; \
	$(top_builddir)/bin/scilab -nb -l en -nouserstartup -nwni -e "xmltohtml();exit;")

	-@( cd $(top_builddir) ; \
	$(top_builddir)/bin/scilab -nb -l fr -nouserstartup -nwni -e "xmltohtml();exit;")


check-help:
	$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'check_help();exit;'
	$(top_builddir)/bin/scilab -nwni -nb -l en -e 'check_help();exit;'

check-recursive: check-local
check-local:
	@COMMAND="test_run();quit";\
	export LANG=C;\
	$(top_builddir)/bin/scilab -nw -nb -e "$$COMMAND"

distclean-recursive: clean-help clean-localization

clean-recursive: clean-help clean-localization

clean-help:
	$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'clean_help();exit;'
	$(top_builddir)/bin/scilab -nwni -nb -l en -e 'clean_help();exit;'

clean-localization:
	rm -rf $(top_srcdir)/locale/

localization:
	@echo "-- Build localization (.mo from .po) --"; \
	MASK="modules/*/locales/"; \
	LC=LC_MESSAGES; \
	FILENAME="messages.po*"; \
	rm modules/*/locales/*/*~ 2> /dev/null; \
	for LOCALE in $(ALL_LINGUAS); do \
		FILES=`ls $$MASK/$$LOCALE/$$FILENAME`; \
		PATHTO=$(top_srcdir)/locale/$$LOCALE/$$LC/; \
		if test ! -d $$PATHTO; then \
			echo "Creating $$PATHTO"; \
			mkdir -p $$PATHTO; \
		fi; \
		POFILE=$$PATHTO/$$LANG_DOMAIN.po; \
		$(MSGCAT) -o $$POFILE $$FILES; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics -o $$PATHTO/$$LANG_DOMAIN.mo $$POFILE; \
	done

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


