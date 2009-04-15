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

changelogs = CHANGES CHANGES_5.1.X

bin_PROGRAMS		= scilab-bin

bin_SCRIPTS			= bin/scilab

scilab_bin_SOURCES		= $(top_srcdir)/modules/shell/src/c/others/mainscic.c

scilab_bin_LDFLAGS 		= 
if IS_MACOSX
scilab_bin_LDFLAGS 		+= -framework Cocoa
endif

scilab_bin_CFLAGS 		=
# --no-undefined --relax


# libshell is mandatory here since main_linux.c depends on it.
# Localization dep already exists because of the `exportLocaleToSystem' symbol
# (causing issues on some platform)
scilab_bin_LDADD = $(top_builddir)/modules/libscilab.la \
$(top_builddir)/modules/shell/libscishell.la \
$(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/modules/completion/libscicompletion.la \
$(top_builddir)/modules/localization/libscilocalization.la \
$(top_builddir)/modules/io/libsciio.la

if NEED_JAVA
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
$(top_srcdir)/CHANGES_2.X \
$(top_srcdir)/CHANGES_3.X \
$(top_srcdir)/CHANGES_4.X \
$(top_srcdir)/CHANGES_5.0.X \
$(top_srcdir)/CHANGES_5.1.X \
$(top_srcdir)/ACKNOWLEDGEMENTS \
$(top_srcdir)/COPYING \
$(top_srcdir)/COPYING-FR \
$(top_srcdir)/README_Unix \
$(top_srcdir)/RELEASE_NOTES \
$(top_srcdir)/RELEASE_NOTES_5.0.X \
$(top_srcdir)/RELEASE_NOTES_5.1.X \
$(top_srcdir)/Readme_Visual.txt \
$(top_srcdir)/README_Windows.txt

iconsdir=$(pkgdatadir)/icons/
icons_DATA = $(top_srcdir)/icons/scilab.xpm \
icons/puffin_128.xpm \
icons/puffin_256.xpm \
icons/puffin_32.xpm \
icons/puffin_48.xpm \
icons/puffin.png \
icons/puffin.icns \
icons/scilab.icns

macros:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
	-@( cd $(top_builddir) ; \
	echo "$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce" ; \
	$(top_builddir)/bin/scilab -ns -nwni -f modules/functions/scripts/buildmacros/buildmacros.sce)

doc:$(top_builddir)/scilab-bin $(top_builddir)/bin/scilab
if BUILD_HELP
	-@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		echo "-- Building documentation (English) --"; \
		echo "LANG=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 DOCBOOK_ROOT=$(DOCBOOK_ROOT) $(top_builddir)/bin/scilab -nb -nouserstartup -nw -e \"xmltojar([],[],'en_US');exit;\"";\
		LANG=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 DOCBOOK_ROOT=$(DOCBOOK_ROOT) $(top_builddir)/bin/scilab -nb -l en_US -nouserstartup -nw -e "xmltojar([],[],'en_US');exit;";\
		echo "-- Building documentation (French) --"; \
		echo "SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 DOCBOOK_ROOT=$(DOCBOOK_ROOT) $(top_builddir)/bin/scilab -nb -nouserstartup -nw -e \"xmltojar([],[],'fr_FR');exit;\""; \
		SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 DOCBOOK_ROOT=$(DOCBOOK_ROOT) $(top_builddir)/bin/scilab -nb -nouserstartup -nw -e "xmltojar([],[],'fr_FR');exit;"; \
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

distclean-recursive: distclean-generated clean-recursive

clean-recursive: clean-doc clean-localization clean-macros clean-generated

clean-macros:
# Find is necessary because of subdirs...
	@echo "-- Cleaning macros (*.bin) --"; \
	find $(top_builddir) -name "*.bin" | xargs --no-run-if-empty rm 2> /dev/null; \
	echo "-- Cleaning names files (names) --"; \
	find $(top_builddir)/modules/*/macros -name "names" | xargs --no-run-if-empty rm 2> /dev/null; \
	echo "-- Cleaning lib files (lib) --"; \
	find $(top_builddir)/modules/*/macros -name "lib" | xargs --no-run-if-empty rm 2> /dev/null


clean-doc:
	@if test -x $(top_builddir)/scilab-bin; then \
		echo "$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'clean_help();exit;'"; \
		$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'clean_help();exit;'; \
		echo "$(top_builddir)/bin/scilab -nwni -nb -l en -e 'clean_help();exit;'"; \
		$(top_builddir)/bin/scilab -nwni -nb -l en -e 'clean_help();exit;'; \
	else \
		echo "Cannot find Scilab binary"; \
	fi; \
	echo "rmdir $(top_builddir)/modules/helptools/javaHelp/"; \
	rmdir $(top_builddir)/modules/helptools/javaHelp/; \
	echo "rm -rf $(top_builddir)/modules/scicos/help/*/scilab_*_help"; \
	rm -rf $(top_builddir)/modules/scicos/help/*/scilab_*_help; \
	echo "rm -f $(top_builddir)/modules/scicos/help/en_US/master_help.xml"; \
	rm -f $(top_builddir)/modules/scicos/help/en_US/master_help.xml; \
	echo "rm -f $(top_builddir)/modules/scicos/jar/"; \
	rm -rf $(top_builddir)/modules/scicos/jar/

clean-tests:
	@if test -x $(top_builddir)/scilab-bin; then \
		$(top_builddir)/bin/scilab -nwni -nb -l fr -e 'test_clean();exit;'; \
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

# The user wants to compile the localization files
# Please note that ALL_LINGUAS is defined in configure.ac
if GENERATE_LOCALIZATION_FILES
localization:
	@echo "-- Build localization (.mo from .po) --"; \
	MASK="modules/*/locales/"; \
	LC="LC_MESSAGES"; \
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
		if test "$(MACOSX)" = "1"; then sed -i -e '/charset=UTF-8/d' $$POFILE; fi; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics -o $$PATHTO/$(LANG_DOMAIN).mo $$POFILE; \
	done;
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

CALL_SCILAB_CFLAGS_COMMON = -I$(top_srcdir)/modules/call_scilab/includes/ -I$(top_srcdir)/modules/core/includes/
CALL_SCILAB_LDADD_COMMON = $(top_builddir)/modules/libscilab.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/call_scilab/libscicall_scilab.la

check_PROGRAMS = call_scilab_c_simple call_scilab_c call_scilab_c_getlastjob call_scilab_c_DisableInteractiveMode call_scilab_readwriteboolean call_scilab_readwritecomplexmatrix call_scilab_readwritematrix call_scilab_readwritestring

call_scilab_c_SOURCES = $(top_srcdir)/modules/call_scilab/examples/call_scilab/c/common/myprog.c
call_scilab_c_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_c_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)


call_scilab_c_simple_SOURCES = $(top_srcdir)/modules/call_scilab/examples/call_scilab/c/common/simple_call_scilab.c
call_scilab_c_simple_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_c_simple_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)

call_scilab_c_getlastjob_SOURCES = $(top_srcdir)/modules/call_scilab/examples/basicExamples/GetLastJob.c
call_scilab_c_getlastjob_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_c_getlastjob_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)

call_scilab_c_DisableInteractiveMode_SOURCES = $(top_srcdir)/modules/call_scilab/examples/basicExamples/DisableInteractiveMode.c
call_scilab_c_DisableInteractiveMode_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_c_DisableInteractiveMode_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)

call_scilab_readwriteboolean_SOURCES = $(top_srcdir)/modules/call_scilab/examples/basicExamples/readwriteboolean.c
call_scilab_readwriteboolean_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_readwriteboolean_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)

call_scilab_readwritecomplexmatrix_SOURCES = $(top_srcdir)/modules/call_scilab/examples/basicExamples/readwritecomplexmatrix.c
call_scilab_readwritecomplexmatrix_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_readwritecomplexmatrix_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)

call_scilab_readwritematrix_SOURCES = $(top_srcdir)/modules/call_scilab/examples/basicExamples/readwritematrix.c
call_scilab_readwritematrix_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_readwritematrix_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)

call_scilab_readwritestring_SOURCES = $(top_srcdir)/modules/call_scilab/examples/basicExamples/readwritestring.c
call_scilab_readwritestring_CFLAGS = $(CALL_SCILAB_CFLAGS_COMMON)
call_scilab_readwritestring_LDFLAGS = $(CALL_SCILAB_LDADD_COMMON)
