#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)

AUTOMAKE_OPTIONS = 1.11 dist-zip dist-bzip2

SUBDIRS= libs \
	modules \
	contrib

ACLOCAL_AMFLAGS = -I m4/

changelogs = CHANGES_5.1.X CHANGES_5.2.X CHANGES_5.3.X

bin_PROGRAMS		= scilab-bin scilab-cli-bin

bin_SCRIPTS			= bin/scilab bin/scilab-adv-cli bin/scilab-cli

if IS_MACOSX
bin_SCRIPTS 		+= bin/checkmacosx.applescript
endif

scilab_bin_LDFLAGS 		=
scilab_cli_bin_LDFLAGS		=

if IS_MACOSX
scilab_bin_LDFLAGS 		+= -framework Cocoa
endif

scilab_bin_CFLAGS 		= -I$(top_srcdir)/modules/io/includes
# --no-undefined --relax

# libshell is mandatory here since main_linux.c depends on it.
# Localization dep already exists because of the `exportLocaleToSystem' symbol
# (causing issues on some platform)
COMMON_LIBS = $(top_builddir)/modules/shell/libscishell.la \
$(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/modules/completion/libscicompletion.la \
$(top_builddir)/modules/localization/libscilocalization.la \
$(top_builddir)/modules/io/libsciio.la \
$(LTLIBINTL)

SCILAB_LIB=$(top_builddir)/modules/libscilab.la
SCILAB_CLI_LIB=$(top_builddir)/modules/libscilab-cli.la


scilab_bin_SOURCES		= $(top_srcdir)/modules/shell/src/c/others/mainscic.c

scilab_bin_LDADD 		= $(COMMON_LIBS) $(SCILAB_LIB)

# Match scilab -nwni (ie command line without graphics)
scilab_cli_bin_SOURCES	= $(top_srcdir)/modules/shell/src/c/others/mainscic.c

scilab_cli_bin_CFLAGS 		= -DWITHOUT_GUI -I$(top_srcdir)/modules/io/includes
scilab_cli_bin_LDADD 		= $(COMMON_LIBS) $(SCILAB_CLI_LIB)


if NEED_JAVA
# Don't do it for scilab_cli_bin. We want to be able to call it without
# the java libs
	scilab_bin_LDADD += $(JAVA_JNI_LIBS)
endif

if IS_SOLARIS
# The binary itself needs this dep
	scilab_bin_LDADD += $(BLAS_LIBS)
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

#  The user wants to compile the localization files
if GENERATE_LOCALIZATION_FILES
TARGETS_ALL+=localization
endif

if COMPILATION_TESTS
TARGETS_ALL+=check-java
endif

all-local: $(TARGETS_ALL)

rootdir=$(pkgdatadir)/etc/
root_DATA = $(top_srcdir)/etc/scilab.start \
$(top_srcdir)/etc/scilab.quit \
$(top_srcdir)/etc/toolboxes.xml \
$(top_builddir)/etc/jvm_options.xml \
$(top_builddir)/etc/modules.xml \
$(top_builddir)/etc/classpath.xml \
$(top_builddir)/etc/logging.properties \
$(top_builddir)/etc/librarypath.xml

infofilesdir=$(pkgdatadir)/
infofiles_DATA = $(top_srcdir)/CHANGES_2.X \
$(top_srcdir)/CHANGES_3.X \
$(top_srcdir)/CHANGES_4.X \
$(top_srcdir)/CHANGES_5.0.X \
$(top_srcdir)/CHANGES_5.1.X \
$(top_srcdir)/CHANGES_5.2.X \
$(top_srcdir)/CHANGES_5.3.X \
$(top_srcdir)/ACKNOWLEDGEMENTS \
$(top_srcdir)/COPYING \
$(top_srcdir)/COPYING-BSD \
$(top_srcdir)/COPYING-FR \
$(top_srcdir)/README_Unix \
$(top_srcdir)/RELEASE_NOTES_5.0.X \
$(top_srcdir)/RELEASE_NOTES_5.1.X \
$(top_srcdir)/RELEASE_NOTES_5.2.X \
$(top_srcdir)/RELEASE_NOTES_5.3.X \
$(top_srcdir)/Readme_Visual.txt \
$(top_srcdir)/README_Windows.txt \
$(top_builddir)/Version.incl

iconsdir=$(pkgdatadir)/icons/
icons_DATA = $(top_srcdir)/desktop/scilab.xpm \
$(top_srcdir)/desktop/puffin_128.xpm \
$(top_srcdir)/desktop/puffin_256.xpm \
$(top_srcdir)/desktop/puffin_32.xpm \
$(top_srcdir)/desktop/puffin_48.xpm \
$(top_srcdir)/desktop/puffin.png \
$(top_srcdir)/desktop/puffin.icns \
$(top_srcdir)/desktop/scilab.icns

valgrinddir=$(pkgdatadir)/tools/profiling
valgrind_DATA = $(top_srcdir)/tools/profiling/valgrind.supp

# Desktop file
# Note that here, it is datadir and NOT pkgdatadir
desktopdir = $(datadir)/applications
desktop_DATA = $(top_srcdir)/desktop/scilab-adv-cli.desktop \
$(top_srcdir)/desktop/scilab-cli.desktop \
$(top_srcdir)/desktop/scilab.desktop

macros:$(top_builddir)/scilab-cli-bin $(top_builddir)/bin/scilab-cli
	-@( cd $(top_builddir) ; \
	echo "$(top_builddir)/bin/scilab-cli -ns -f modules/functions/scripts/buildmacros/buildmacros.sce" ; \
	$(top_builddir)/bin/scilab-cli -ns -f modules/functions/scripts/buildmacros/buildmacros.sce) || exit 1


# Build the public Javadoc (javasci v2 and Java/Scilab types)
# This is not done in the module itself because its needs also the "types"
# module
javadoc:
	$(JAVADOC) -noindex -nohelp -nonavbar -notree \
	-d $(top_builddir)/modules/javasci/javadoc \
	-sourcepath $(top_srcdir)/modules/javasci/src/java:$(top_srcdir)/modules/types/src/java \
	org.scilab.modules.javasci org.scilab.modules.types

LANGDOC=en_US fr_FR pt_BR ja_JP

doc: javadoc $(top_builddir)/scilab-bin $(top_builddir)/bin/scilab-adv-cli
if BUILD_HELP
	@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		for l in $(LANGDOC); do \
			echo "-- Building documentation ($$l) --"; \
			COMMAND="try xmltojar([],[],'$$l');catch exit(-1);end;exit(0);"; \
			echo "LANG=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 $(top_builddir)/bin/scilab-adv-cli -nb -nouserstartup -e \"$$COMMAND\"";\
			LANG=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 $(top_builddir)/bin/scilab-adv-cli -nb -l $$l -nouserstartup -e "$$COMMAND" || exit 1;\
		done; \
	else \
		echo "Cannot find Scilab-adv-cli binary"; \
	fi
else
	@echo "Cannot buid help. Add --enable-build-help to the ./configure if you want to build it."
endif

doc-web:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab-adv-cli
if BUILD_HELP
	@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		for l in $(LANGDOC); do \
			echo "-- Building documentation ($$l) --"; \
			COMMAND="try xmltoweb([],[],'$$l');catch exit(-1);end;exit(0);";\
			echo "LANG=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 $(top_builddir)/bin/scilab-adv-cli -nb -nouserstartup -e \"$$COMMAND\"";\
			LANG=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 $(top_builddir)/bin/scilab-adv-cli -nb -l $$l -nouserstartup -e "$$COMMAND" || exit 1;\
			JAVADOC_TARGET=$(top_builddir)/modules/helptools/web/$$l/javasci; \
			if test ! -d $$JAVADOC_TARGET; then \
				mkdir $$JAVADOC_TARGET; \
				ln -s ../../../../javasci/javadoc/ $$JAVADOC_TARGET/; \
			fi; \
		done; \
	else \
		echo "Cannot find Scilab-adv-cli binary"; \
	fi
else
	@echo "Cannot buid help. Add --enable-build-help to the ./configure if you want to build it."
endif

check-help:$(top_builddir)/scilab-cli-bin $(top_builddir)/bin/scilab-cli
	@if test -x $(top_builddir)/scilab-cli-bin; then \
		for l in $(LANGDOC); do \
			echo "-- Checking documentation ($$l) --"; \
			COMMAND="try check_help();catch exit(-1);end;exit(0);";\
			echo "$(top_builddir)/bin/scilab-cli -nb -l $$l -e \"$$COMMAND\"";\
			$(top_builddir)/bin/scilab-cli -nb -l $$l -e "$$COMMAND" || exit 1;\
		done; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

check-recursive: check-local
check-local:
	@if test -x $(top_builddir)/scilab-bin; then \
		COMMAND="try test_run();catch exit(-1);end;exit(0)";\
		export LANG=C;\
		$(top_builddir)/bin/scilab-adv-cli -nb -e "$$COMMAND"; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

code-coverage:
if CODE_COVERAGE
	@if test ! -d $(top_builddir)/coverage/; then \
		mkdir $(top_builddir)/coverage/; \
	fi; \
	$(LCOV) --directory $(top_builddir) --capture --ignore-errors source --output-file $(top_builddir)/coverage/scilab-code-coverage.info; \
	$(GENHTML) -o $(top_builddir)/coverage/results/ --show-details --highlight --legend $(top_builddir)/coverage/scilab-code-coverage.info; \
	echo "To open the result: firefox  $(top_builddir)/coverage/results/index.html"
#--frames is disable because of : http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=607206
else
	@echo "Cannot start the code coverage. Add --enable-code-coverage to the ./configure and 'make clean all'."
endif

distclean-recursive: distclean-generated clean-recursive

clean-recursive: clean-help clean-localization clean-macros clean-generated

clean-macros:
# Find is necessary because of subdirs...
	-@echo "-- Cleaning macros (*.bin) --"; \
	find $(top_builddir) -name "*.bin" | xargs --no-run-if-empty rm 2> /dev/null; \
	echo "-- Cleaning names files (names) --"; \
	find $(top_builddir)/modules/*/macros -name "names" | xargs --no-run-if-empty rm 2> /dev/null; \
	echo "-- Cleaning lib files (lib) --"; \
	find $(top_builddir)/modules/*/macros -name "lib" | xargs --no-run-if-empty rm 2> /dev/null

# To ensure previous compatibility
clean-doc: clean-help

clean-help:
	@if test -x $(top_builddir)/scilab-cli-bin; then \
		COMMAND="try clean_help();catch exit(-1);end;exit(0)"; \
		for l in $(LANGDOC); do \
			echo "$(top_builddir)/bin/scilab-cli -nb -l $$l -e '$$COMMAND'"; \
			$(top_builddir)/bin/scilab-cli -nb -l $$l -e "$$COMMAND" || exit 1; \
		done; \
	else \
		echo "Cannot find Scilab binary"; \
	fi; \
	echo "rmdir $(top_builddir)/modules/helptools/javaHelp/"; \
	rm -rf $(top_builddir)/modules/helptools/javaHelp/; \
	echo "rm -rf $(top_builddir)/modules/scicos/help/*/scilab_*_help"; \
	rm -rf $(top_builddir)/modules/scicos/help/*/scilab_*_help; \
	echo "rm -f $(top_builddir)/modules/scicos/help/en_US/master_help.xml"; \
	rm -f $(top_builddir)/modules/scicos/help/en_US/master_help.xml; \
	echo "rm -f $(top_builddir)/modules/scicos/jar/"; \
	rm -rf $(top_builddir)/modules/scicos/jar/; \
	echo "rm -rf $(top_builddir)/modules/javasci/javadoc"; \
	rm -rf $(top_builddir)/modules/javasci/javadoc;

clean-tests:
	@if test -x $(top_builddir)/scilab-cli-bin; then \
		$(top_builddir)/bin/scilab-cli -nb -l fr -e 'try test_clean();catch exit(-1);end;exit(0);' || exit 1; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

clean-localization:
	rm -rf $(top_srcdir)/locale/

clean-generated:
	rm -rf $(top_builddir)/modules/scicos_blocks/build/ 2> /dev/null

distclean-generated:
	rm -f $(top_builddir)/modules/core/includes/stack.h 2> /dev/null


if IS_MACOSX
# Used for mac os x because it needs to remove the charset=UTF-8 because it is breaking the accents.
MACOSX=1
endif

# For each module, build and call Java unitary tests
# If fails, stops the call
check-java:
	@echo "-- Launch all TestNG (Java) unitary tests"; \
	MASK="$(top_srcdir)/modules/*/tests/java/"; \
	MODULES=`ls -d $$MASK`; \
	for M in $$MODULES; do \
		BASE=`echo $$M|sed -e "s|tests/java/$$||g"`; \
		$(ANT) -buildfile $$BASE/build.xml test || exit $$?; \
	done

# The user wants to compile the localization files
# Please note that ALL_LINGUAS is defined in configure.ac
if GENERATE_LOCALIZATION_FILES
localization:
	@echo "-- Build localization (.mo from .po) --"; \
	MASK="modules/*/locales/"; \
	LC="LC_MESSAGES"; \
	MSGFMT_OPTS="$MSGFMT_OPTS --check"; \
	EXTENSION="*.po"; \
	rm modules/*/locales/*~ 2> /dev/null; \
	for LOCALE in $(ALL_LINGUAS); do \
		echo "        -- Building for $$LOCALE --"; \
		FILES=`ls $$MASK/$$LOCALE$$EXTENSION`; \
		PATHTO=$(top_builddir)/locale/$$LOCALE/$$LC/; \
		if test ! -d $$PATHTO; then \
			echo "        Creating $$PATHTO"; \
			mkdir -p $$PATHTO; \
		fi; \
		POFILE=$$PATHTO/$(LANG_DOMAIN).po; \
		$(MSGCAT) --use-first -o $$POFILE $$FILES; \
		if test "$(MACOSX)" = "1"; then sed -i -e '/charset=UTF-8/d' $$POFILE; fi; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics -o $$PATHTO/$(LANG_DOMAIN).mo $$POFILE; \
	done;
endif

# Misc variable for the documentation installation

# How help files look like.
DOCMASK=scilab_*_help.jar

# Path to builded files
PATHTOHELPFILES = modules/helptools/jar/

# Path javasci to builded files
PATHTOJAVASCIDOC = modules/javasci/javadoc/
JAVASCI_DIRS = . resources \
	org/scilab/modules/javasci/ \
	org/scilab/modules/types/

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
## Install Javasci documentation (javadoc)
## See target javadoc in this file to see how it is build
	@if test -d $(PATHTOJAVASCIDOC); then \
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOJAVASCIDOC); \
	echo $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOJAVASCIDOC); \
	for dir in $(JAVASCI_DIRS) ; do \
		$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOJAVASCIDOC)/$$dir; \
		echo $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOJAVASCIDOC)/$$dir; \
			for file in $(top_builddir)/$(PATHTOJAVASCIDOC)/$(dir)/*; do \
			    echo "$(INSTALL_DATA) $$file $(DESTDIR)$(pkgdatadir)/$(PATHTOJAVASCIDOC)/$$dir" ; \
				$(INSTALL_DATA) "$$file" $(DESTDIR)$(pkgdatadir)/$(PATHTOJAVASCIDOC)/$$dir ; \
			done; \
	done; \
	fi
endif

# Pkgconfig directory
pkgconfigdir = $(libdir)/pkgconfig

# Files to install in Pkgconfig directory
pkgconfig_DATA = scilab.pc

.PHONY: macros localization doc

.NOTPARALLEL: macros doc

include $(top_srcdir)/Makefile.call_scilab.am
