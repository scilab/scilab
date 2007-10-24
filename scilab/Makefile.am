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

TARGETS_ALL=macros

#  The user wants to compile the localization files
if GENERATE_LOCALIZATION_FILES
TARGETS_ALL+=build-localization
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

distclean-local::
clean-local:
	$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'clean_help();exit;'
	$(top_builddir)/bin/scilab -nwni -nb -l en -e 'clean_help();exit;'

build-localization:
	@echo "-- Build localization (.mo from .po) --"; \
	MASK="modules/*/locales/"; \
	LC=LC_MESSAGES; \
	FILENAME="messages.po*"; \
	LANG_DOMAIN=scilab; \
	for LOCALE in $(ALL_LINGUAS); do \
		FILES=`ls $$MASK/$$LOCALE/$$FILENAME`; \
		PATHTO=$(top_srcdir)/locale/$$LOCALE/$$LC/; \
		if test ! -d $$PATHTO; then \
			echo "$$PATHTO is not existing"; \
			mkdir -p $$PATHTO; \
		fi; \
		POFILE=$$PATHTO/$$LANG_DOMAIN.po; \
		$(MSGCAT) -o $$POFILE $$FILES; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics -o $$PATHTO/$$LANG_DOMAIN.mo $$POFILE; \
	done

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(localedir); \
	for lang in $(LANGS); do \
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


