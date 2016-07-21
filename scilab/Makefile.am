#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2006-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

## Process this file with automake to produce Makefile.in (which will be changed by the ./configure)

AUTOMAKE_OPTIONS = 1.11 dist-zip dist-bzip2

SUBDIRS= modules \
	contrib \
	desktop

ACLOCAL_AMFLAGS = -I m4

# Set AM_* SCI detected settings
AM_CPPFLAGS = $(SCI_CPPFLAGS)
AM_CFLAGS   = $(SCI_CFLAGS)
AM_CXXFLAGS = $(SCI_CXXFLAGS)
AM_FFLAGS   = $(SCI_FFLAGS)
AM_LDFLAGS  = $(SCI_LDFLAGS)

bin_PROGRAMS		= scilab-bin scilab-cli-bin

bin_SCRIPTS			= bin/scilab bin/scilab-adv-cli bin/scilab-cli \
bin/scinotes bin/xcos

if IS_MACOSX
bin_SCRIPTS 		+= bin/checkmacosx.applescript
endif

scilab_bin_LDFLAGS      = $(AM_LDFLAGS) $(OPENMPI_LIBS)
scilab_cli_bin_LDFLAGS  = $(AM_LDFLAGS) $(OPENMPI_LIBS)

#
# using the static system libs might produce linkage issue (eg.
# std::nothrow for instance) but it seems to be the only acceptable
# solution for now.
#
if USE_STATIC_SYSTEM_LIB
scilab_bin_LDFLAGS += -static-libstdc++ -static-libgcc -static-libgfortran
scilab_cli_bin_LDFLAGS += -static-libstdc++ -static-libgcc -static-libgfortran
else
# Clang (for example) needs an explicit reference to the lib
scilab_bin_LDFLAGS += -lstdc++ -lgfortran
scilab_cli_bin_LDFLAGS += -lstdc++ -lgfortran
endif


if IS_MACOSX
scilab_bin_LDFLAGS 		+= -framework Cocoa
endif

scilab_bin_CPPFLAGS = \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/history_manager/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/core/src/c/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/startup/src/c/ \
    $(AM_CPPFLAGS)

# --no-undefined --relax

# Localization dep already exists because of the `exportLocaleToSystem' symbol
# (causing issues on some platform)
COMMON_LIBS = $(LTLIBINTL)

# Link against valgrind library as on some systems (eg. Fedora) it should be
# link on the final binary
COMMON_LIBS += $(VALGRIND_LIBS)

scilab_bin_SOURCES = modules/startup/src/cpp/scilab.cpp

# the scilab-bin object file itself uses symbols from at least libscilab-cli.la,
# libsciconsole-minimal.la and libscijvm-disable.la, so we have to link with these
# libraries explicitly unless we link with -Wl,--copy-dt-needed-entries
scilab_bin_LDADD 	= \
			$(top_builddir)/modules/libscilab.la \
			$(top_builddir)/modules/libscilab-cli.la \
			$(OPENMPI_LIBS) $(XML_LIBS) \
			$(COMMON_LIBS) $(FLIBS)

if NEED_JAVA
scilab_bin_LDADD += \
			$(top_builddir)/modules/console/libsciconsole.la \
			$(top_builddir)/modules/jvm/libscijvm.la \
			$(top_builddir)/modules/commons/libscicommons.la
else
scilab_bin_LDADD += \
			$(top_builddir)/modules/console/libsciconsole-minimal.la \
			$(top_builddir)/modules/jvm/libscijvm-disable.la
endif

# BLAS_LIBS & LAPACK_LIBS are explicitly linked at the end. Unlike Scilab 5 in which the binary
# is in C, Scilab 6 main is in C++. For some reasons (ie, I don't know why), libtool places blas and
# lapack at the beginning of the order of the link. This caused some issues with the overload of the
# xerbla function
scilab_bin_LDADD += $(top_builddir)/modules/libscilab.la $(BLAS_LIBS) $(LAPACK_LIBS) $(FLIBS)

# Match scilab -nwni (ie command line without graphics)

scilab_cli_bin_SOURCES	= modules/startup/src/cpp/scilab.cpp

scilab_cli_bin_CPPFLAGS = -DWITHOUT_GUI $(scilab_bin_CPPFLAGS)

# See the comment about scilab_bin_LDADD (relative to BLAS_LIBS & LAPACK_LIBS)
scilab_cli_bin_LDADD 	= $(COMMON_LIBS) $(top_builddir)/modules/libscilab-cli.la $(BLAS_LIBS) $(LAPACK_LIBS) $(FLIBS)

if MPI
CXX = $(OPENMPI_CXX)
scilab_bin_SOURCES += modules/startup/src/c/initMPI.c
scilab_bin_CPPFLAGS += $(OPENMPI_CXXFLAGS)
scilab_cli_bin_SOURCES += modules/startup/src/c/initMPI.c
scilab_cli_bin_CPPFLAGS += $(OPENMPI_CXXFLAGS)
endif


if NEED_JAVA
# Don't do it for scilab_cli_bin. We want to be able to call it without
# the java libs
	scilab_bin_LDADD += $(JAVA_JNI_LIBS)
endif

if IS_SOLARIS
# The binary itself needs this dep
	scilab_bin_LDADD += $(BLAS_LIBS)
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

# check is performed with scilab, so use the serial-tests on purpose
TESTS_ENVIRONMENT = LANG=C $(top_builddir)/bin/scilab-cli -nb -noatomsautoload -f
TESTS = $(top_builddir)/modules/development_tools/tests/unit_tests/test_run_level_1.tst

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
infofiles_DATA = $(top_srcdir)/CHANGES.md \
$(top_srcdir)/ACKNOWLEDGEMENTS \
$(top_srcdir)/COPYING \
$(top_srcdir)/COPYING-BSD \
$(top_srcdir)/README.md \
$(top_builddir)/Version.incl

valgrinddir=$(pkgdatadir)/tools/profiling
valgrind_DATA = $(top_srcdir)/tools/profiling/valgrind.supp

macros: $(top_builddir)/scilab-cli-bin $(top_builddir)/bin/scilab-cli check-jvm-dep check-libstdcpp-dep
	-@( cd $(top_builddir) ; \
	echo "$(top_builddir)/bin/scilab-cli -ns -noatomsautoload -quit -f modules/functions/scripts/buildmacros/buildmacros.sce" ; \
	$(top_builddir)/bin/scilab-cli -ns -noatomsautoload -quit -f modules/functions/scripts/buildmacros/buildmacros.sce) || exit 1

strip-debug:
	find $(top_builddir) -name '*.so.?.?.?' | while read file ; \
	do \
	    if [ ! -f $$file.debug ]; then \
	        objcopy --only-keep-debug $$file $$file.debug; \
	        objcopy --strip-debug $$file; \
	        objcopy --add-gnu-debuglink=$$file.debug $$file; \
	    fi \
	done;

check-jvm-dep:
# Check that scilab-cli does not have a dependency on libjvm
if !IS_MACOSX
	@ldd .libs/scilab-cli-bin|grep libjvm > /dev/null; \
	if test $$? == 0; then \
		echo "Found dependency from scilab-cli-bin to libjvm found"; exit 2;\
	fi
endif


check-libstdcpp-dep:
if USE_STATIC_SYSTEM_LIB
# Check that scilab-cli does not have a dependency on libstdc++ (should be static)
	@ldd .libs/scilab-bin .libs/scilab-cli-bin modules/.libs/libscilab*0 |grep libstdc++ > /dev/null; \
	if test $$? == 0; then \
		echo "Found dependency from scilab-cli-bin to libstdc++ found"; exit 2;\
	fi
else
	echo "libstdc++ presence test skipped"
endif

# Build the public Javadoc (javasci v2 and Java/Scilab types)
# This is not done in the module itself because its needs also the "types"
# module
javadoc:
	$(JAVADOC) -noindex -nohelp -nonavbar -notree \
	-d $(top_builddir)/modules/javasci/javadoc \
	-sourcepath $(top_srcdir)/modules/javasci/src/java:$(top_srcdir)/modules/types/src/java::$(top_srcdir)/modules/localization/src/java \
	org.scilab.modules.javasci org.scilab.modules.types

javadoc-all:
	MASK="$(top_srcdir)/modules/*/src/java/"; \
	MODULES=`ls -d $$MASK`; \
	for M in $$MODULES; do \
		LISTMODULES="$$M:$$LISTMODULES"; \
		MODULE=$$(echo $$M|cut -d/ -f3); \
		LISTPKG="$$LISTPKG org.scilab.modules.$$MODULE"; \
	done; \
	$(JAVADOC) \
	-d $(top_builddir)/javadocall \
	-sourcepath $$LISTMODULES \
	$$LISTPKG

doc-fr_FR:
	@$(MAKE) $(AM_MAKEFLAGS) doc ALL_LINGUAS_DOC=fr_FR

doc-en_US:
	@$(MAKE) $(AM_MAKEFLAGS) doc ALL_LINGUAS_DOC=en_US

doc-pt_BR:
	@$(MAKE) $(AM_MAKEFLAGS) doc ALL_LINGUAS_DOC=pt_BR

doc-ru_RU:
	@$(MAKE) $(AM_MAKEFLAGS) doc ALL_LINGUAS_DOC=ru_RU

doc-ja_JP:
	@$(MAKE) $(AM_MAKEFLAGS) doc ALL_LINGUAS_DOC=ja_JP

# Regenerate all images
doc-images:
	rm -rf $(top_srcdir)/modules/helptools/images/
	mkdir $(top_srcdir)/modules/helptools/images/
	$(MAKE) doc

doc: javadoc $(top_builddir)/scilab-bin $(top_builddir)/bin/scilab-adv-cli
if BUILD_HELP
	@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		for l in $(ALL_LINGUAS_DOC); do \
			echo "-- Building documentation ($$l) --"; \
			COMMAND="try xmltojar([],[],'$$l');catch disp(lasterror()); exit(-1);end;exit(0);"; \
			echo "LANG=$$l.UTF-8 LC_ALL=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 _JAVA_OPTIONS='-Djava.awt.headless=true' $(top_builddir)/bin/scilab-adv-cli -noatomsautoload -nb -l $$l -nouserstartup -e \"$$COMMAND\"";\
			LANG=$$l.UTF-8 LC_ALL=C SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 _JAVA_OPTIONS='-Djava.awt.headless=true' $(top_builddir)/bin/scilab-adv-cli -noatomsautoload -nb -l $$l -nouserstartup -e "$$COMMAND" || exit 1;\
		done; \
	else \
		echo "Cannot find Scilab-adv-cli binary"; \
	fi
else
	@echo "Cannot buid help. Add --enable-build-help to the ./configure if you want to build it."
endif

doc-web: javadoc $(top_builddir)/scilab-bin $(top_builddir)/bin/scilab-adv-cli
if BUILD_HELP
	@if test -x $(top_builddir)/scilab-bin; then \
		cd $(top_builddir);\
		for l in $(ALL_LINGUAS_DOC); do \
			echo "-- Building documentation ($$l) --"; \
			COMMAND="try xmltoweb([],[],'$$l');catch exit(-1);end;exit(0);";\
			echo "LANG=$$l.UTF-8 SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 $(top_builddir)/bin/scilab-adv-cli -nb -l $$l -noatomsautoload -nouserstartup -e \"$$COMMAND\"";\
			LANG=$$l.UTF-8 SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1 $(top_builddir)/bin/scilab-adv-cli -nb -l $$l -noatomsautoload -nouserstartup -e "$$COMMAND" || exit 1;\
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

doxygen: modules/development_tools/etc/doxyfile
if DOXYGEN
	mkdir $(top_builddir)/reports/
	$(DOXYGEN_BIN) modules/development_tools/etc/doxyfile
endif

check-help:$(top_builddir)/scilab-cli-bin $(top_builddir)/bin/scilab-cli
	@if test -x $(top_builddir)/scilab-cli-bin; then \
		for l in $(ALL_LINGUAS_DOC); do \
			echo "-- Checking documentation ($$l) --"; \
			COMMAND="try check_help();catch exit(-1);end;exit(0);";\
			echo "$(top_builddir)/bin/scilab-cli -nb -l $$l -noatomsautoload -e \"$$COMMAND\"";\
			$(top_builddir)/bin/scilab-cli -nb -l $$l -noatomsautoload -e "$$COMMAND" || exit 1;\
		done; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

code-coverage:
if CODE_COVERAGE
	@if test ! -d $(top_builddir)/coverage/; then \
		mkdir $(top_builddir)/coverage/; \
	fi; \
	$(LCOV) --directory $(top_builddir) --capture --ignore-errors source --output-file $(top_builddir)/coverage/scilab-code-coverage.info; \
	if test ! -f ~/.lcovrc; then echo -e "genhtml_hi_limit = 85\ngenhtml_med_limit = 70" > ~/.lcovrc; fi; \
	$(GENHTML) -o $(top_builddir)/coverage/results/ --show-details --highlight --legend $(top_builddir)/coverage/scilab-code-coverage.info; \
	rm -f $(top_builddir)/coverage/lcovrc; \
	echo "To open the result: firefox  $(top_builddir)/coverage/results/index.html";
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
	echo "-- Cleaning lib files (lib) --"; \
	find $(top_builddir)/modules/*/macros -name "lib" | xargs --no-run-if-empty rm 2> /dev/null

# To ensure previous compatibility
clean-doc: clean-help

clean-help:
	-@if test -x $(top_builddir)/scilab-cli-bin; then \
		COMMAND="try clean_help();catch exit(-1);end;exit(0)"; \
		for l in $(ALL_LINGUAS_DOC); do \
			echo "$(top_builddir)/bin/scilab-cli -nb -l $$l -noatomsautoload -e '$$COMMAND'"; \
			$(top_builddir)/bin/scilab-cli -nb -l $$l -noatomsautoload -e "$$COMMAND" || exit 1; \
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
	rm -rf $(top_builddir)/modules/javasci/javadoc; \
	echo "rm -rf $(top_builddir)/html"; \
	rm -rf $(top_builddir)/html;

clean-tests:
	@if test -x $(top_builddir)/scilab-cli-bin; then \
		$(top_builddir)/bin/scilab-cli -nb -noatomsautoload -l fr -e 'try test_clean();catch exit(-1);end;exit(0);' || exit 1; \
	else \
		echo "Cannot find Scilab binary"; \
	fi

clean-localization:
	rm -rf $(top_srcdir)/locale/*

clean-generated:
# Deleted junit + code coverage
	rm -rf $(top_builddir)/reports/
	rm -rf $(top_builddir)/modules/scicos_blocks/build/ 2> /dev/null

distclean-generated:
	rm -f $(top_builddir)/modules/core/includes/stack.h 2> /dev/null


if IS_MACOSX
# Used for mac os x because it needs to remove the charset=UTF-8 because it is breaking the accents.
MACOSX=1
endif

clean-reports:
	rm -f reports/xml/TEST-org.scilab.*.xml

# For each module, build and call Java unitary tests
# If fails, stops the call
check-java: clean-reports
	@echo "-- Launch all Junit (Java) unitary tests"; \
	MASK="$(top_srcdir)/modules/*/tests/java/"; \
	MODULES=`ls -d $$MASK`; \
	for M in $$MODULES; do \
		BASE=`echo $$M|sed -e "s|tests/java/$$||g"`; \
		$(ANT) -buildfile $$BASE/build.xml test || exit $$?; \
	done
	rm -f reports/xml/TEST-org.scilab.*.xml

codecoverage-java: clean-reports
	@echo "Check if --enable-debug-java has been set"; \
	grep "build.debug=on" $(top_srcdir)/scilab.properties > /dev/null || exit 10; \
	echo "-- Launch all Junit (Java) unitary tests + code coverage"; \
	MASK="$(top_srcdir)/modules/*/tests/java/"; \
	MODULES=`ls -d $$MASK`; \
	for M in $$MODULES; do \
		BASE=`echo $$M|sed -e "s|tests/java/$$||g"`; \
		$(ANT) -buildfile $$BASE/build.xml codecoverage || exit $$?; \
	done
	rm -f reports/xml/TEST-org.scilab.*.xml

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
		if test "$$LOCALE" == "en_US"; then \
			LOCFILE=$$MASK/*.pot; \
		else \
			LOCFILE=$$MASK/$$LOCALE$$EXTENSION; \
		fi; \
		FILES=`ls $$LOCFILE`; \
		PATHTO=$(top_builddir)/locale/$$LOCALE/$$LC/; \
		if test ! -d $$PATHTO; then \
			echo "        Creating $$PATHTO"; \
			mkdir -p $$PATHTO; \
		fi; \
		POFILE=$$PATHTO/$(LANG_DOMAIN).po; \
		$(MSGCAT) --use-first -o $$POFILE $$FILES || exit 2; \
		if test "$(MACOSX)" = "1"; then sed -i -e '/charset=UTF-8/d' $$POFILE; fi; \
		$(MSGFMT) $(MSGFMT_OPTS) --statistics -o $$PATHTO/$(LANG_DOMAIN).mo $$POFILE || exit 2; \
	done;


localization-status:
	@for LOCALE in $(ALL_LINGUAS_DOC); do \
		if test "$$LOCALE" != "en_US"; then \
			PATHTO=$(top_builddir)/locale-status/$$LOCALE/; \
			if test ! -d $$PATHTO; then \
				echo "        Creating $$PATHTO"; \
				mkdir -p $$PATHTO; \
			fi; \
			echo "Building status for $$LOCALE"; \
			echo "SCI=$(top_srcdir) php tools/localization/revcheck.php $$LOCALE > $$PATHTO/index.html"; \
			SCI=$(top_srcdir) php tools/localization/revcheck.php $$LOCALE > $$PATHTO/index.html; \
		fi; \
	done;

endif

# Misc variable for the documentation installation

# How help files look like.
DOCMASK=scilab_*_help.jar
DOCIMAGES=scilab_images.jar

# Path to built files
PATHTOHELPFILES = modules/helptools/jar/

# Path to scirenderer jar
PATHTOSCIRENDERER = modules/scirenderer/jar/
SCIRENDERERJAR = scirenderer.jar

# Path javasci to built files
PATHTOJAVASCIDOC = modules/javasci/javadoc/
JAVASCI_DIRS = . resources \
	org/scilab/modules/javasci/ \
	org/scilab/modules/types/

install-data-local:
if GENERATE_LOCALIZATION_FILES
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
		for file in $(top_builddir)/$(PATHTOHELPFILES)/$(DOCMASK) $(top_builddir)/$(PATHTOHELPFILES)/$(DOCIMAGES); do \
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
if !EXTERNAL_SCIRENDERER
if GUI
# Also install scirenderer.jar
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$(PATHTOSCIRENDERER)
	$(INSTALL_DATA) $(top_builddir)/$(PATHTOSCIRENDERER)/$(SCIRENDERERJAR) $(DESTDIR)$(pkgdatadir)/$(PATHTOSCIRENDERER)/
endif
endif

# Pkgconfig directory
pkgconfigdir = $(libdir)/pkgconfig

# Files to install in Pkgconfig directory
pkgconfig_DATA = scilab.pc

.PHONY: macros localization doc strip-debug

.NOTPARALLEL: macros doc

include $(top_srcdir)/Makefile.call_scilab.am
