# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

# Install the contrib_skeleton directory (the template)

SKELETON_FILES=toolbox_skeleton/src/fortran/builder_fortran.sce \
toolbox_skeleton/src/fortran/fsum.f \
toolbox_skeleton/src/c/csum.c \
toolbox_skeleton/src/c/builder_c.sce \
toolbox_skeleton/src/c/multiplybypi.c \
toolbox_skeleton/src/c/csub.c \
toolbox_skeleton/src/c/multiplybypi.h \
toolbox_skeleton/src/c/csub.h \
toolbox_skeleton/src/c/csum.h \
toolbox_skeleton/src/cleaner_src.sce \
toolbox_skeleton/src/builder_src.sce \
toolbox_skeleton/etc/toolbox_skeleton.quit \
toolbox_skeleton/etc/toolbox_skeleton.start \
toolbox_skeleton/license.txt \
toolbox_skeleton/DESCRIPTION \
toolbox_skeleton/changelog.txt \
toolbox_skeleton/DESCRIPTION-FUNCTIONS \
toolbox_skeleton/help/builder_help.sce \
toolbox_skeleton/help/fr_FR/scilab_sum.xml \
toolbox_skeleton/help/fr_FR/c_sub.xml \
toolbox_skeleton/help/fr_FR/fortran_sum.xml \
toolbox_skeleton/help/fr_FR/c_sum.xml \
toolbox_skeleton/help/fr_FR/build_help.sce \
toolbox_skeleton/help/en_US/scilab_sum.xml \
toolbox_skeleton/help/en_US/c_sub.xml \
toolbox_skeleton/help/en_US/fortran_sum.xml \
toolbox_skeleton/help/en_US/c_sum.xml \
toolbox_skeleton/help/en_US/build_help.sce \
toolbox_skeleton/help/en_US/c_multiplybypi.xml \
toolbox_skeleton/demos/c_sum.dem.sce \
toolbox_skeleton/demos/toolbox_skeleton.dem.gateway.sce \
toolbox_skeleton/demos/cpp_find.dem.sce \
toolbox_skeleton/demos/scilab_sum.dem.sce \
toolbox_skeleton/demos/fortran_sum.dem.sce \
toolbox_skeleton/readme.txt \
toolbox_skeleton/tests/unit_tests/c_sum.tst \
toolbox_skeleton/tests/unit_tests/fortran_sum.tst \
toolbox_skeleton/tests/unit_tests/scilab_sum.dia.ref \
toolbox_skeleton/tests/unit_tests/fortran_sum.dia.ref \
toolbox_skeleton/tests/unit_tests/c_sum.dia.ref \
toolbox_skeleton/tests/unit_tests/scilab_sum.tst \
toolbox_skeleton/sci_gateway/fortran/builder_gateway_fortran.sce \
toolbox_skeleton/sci_gateway/fortran/sci_fsum.c \
toolbox_skeleton/sci_gateway/c/sci_csub.c \
toolbox_skeleton/sci_gateway/c/sci_multiplybypi.c \
toolbox_skeleton/sci_gateway/c/sci_foo.c \
toolbox_skeleton/sci_gateway/c/sci_csum.c \
toolbox_skeleton/sci_gateway/c/builder_gateway_c.sce \
toolbox_skeleton/sci_gateway/cpp/builder_gateway_cpp.sce \
toolbox_skeleton/sci_gateway/cpp/sci_cpp_find.cxx \
toolbox_skeleton/sci_gateway/builder_gateway.sce \
toolbox_skeleton/builder.sce \
toolbox_skeleton/macros/scilab_sum.sci \
toolbox_skeleton/macros/buildmacros.sce \
toolbox_skeleton/macros/cleanmacros.sce \
toolbox_skeleton/toolbox_skeleton_redist.iss \
toolbox_skeleton/toolbox_skeleton.iss \
xcos_toolbox_skeleton/builder.sce \
xcos_toolbox_skeleton/changelog.txt \
xcos_toolbox_skeleton/cleaner.sce \
xcos_toolbox_skeleton/demos/xcos_toolbox_skeleton.dem.gateway.sce \
xcos_toolbox_skeleton/DESCRIPTION \
xcos_toolbox_skeleton/DESCRIPTION-FUNCTIONS \
xcos_toolbox_skeleton/etc/xcos_toolbox_skeleton.quit \
xcos_toolbox_skeleton/etc/xcos_toolbox_skeleton.start \
xcos_toolbox_skeleton/help/builder_help.sce \
xcos_toolbox_skeleton/help/cleaner_help.sce \
xcos_toolbox_skeleton/help/en_US/build_help.sce \
xcos_toolbox_skeleton/help/en_US/TBX_SUM_c.xml \
xcos_toolbox_skeleton/help/en_US/tbx_sum.xml \
xcos_toolbox_skeleton/help/fr_FR/build_help.sce \
xcos_toolbox_skeleton/images/gif/TBX_MUT_STYLE.gif \
xcos_toolbox_skeleton/images/gif/TBX_SUM_c.gif \
xcos_toolbox_skeleton/images/svg/TBX_MUT_STYLE.svg \
xcos_toolbox_skeleton/images/svg/TBX_SUM_c.svg \
xcos_toolbox_skeleton/license.txt \
xcos_toolbox_skeleton/macros/buildmacros.sce \
xcos_toolbox_skeleton/macros/cleanmacros.sce \
xcos_toolbox_skeleton/macros/TBX_MUT_STYLE.sci \
xcos_toolbox_skeleton/macros/TBX_SUM_c.sci \
xcos_toolbox_skeleton/sci_gateway/builder_gateway.sce \
xcos_toolbox_skeleton/sci_gateway/c/builder_gateway_c.sce \
xcos_toolbox_skeleton/sci_gateway/c/sci_tbx_sum.c \
xcos_toolbox_skeleton/sci_gateway/loader_gateway.sce \
xcos_toolbox_skeleton/src/builder_src.sce \
xcos_toolbox_skeleton/src/c/block_sum.c \
xcos_toolbox_skeleton/src/c/builder_c.sce \
xcos_toolbox_skeleton/src/c/business.h \
xcos_toolbox_skeleton/src/c/business_sum.c \
xcos_toolbox_skeleton/src/cleaner_src.sce \
xcos_toolbox_skeleton/tests/unit_tests/tbx_sum.tst \
xcos_toolbox_skeleton/tests/unit_tests/tbx_sum.dia.ref \
loader.sce

install-data-local:
	$(mkinstalldirs) -m 755 $(DESTDIR)$(pkgdatadir)/contrib/; \
	$(mkinstalldirs) -m 755 $(DESTDIR)$(pkgdatadir)/contrib/toolbox_skeleton; \
	$(mkinstalldirs) -m 755 $(DESTDIR)$(pkgdatadir)/contrib/xcos_toolbox_skeleton; \
	for file in $(SKELETON_FILES) ; do \
	    filePath=`dirname $(DESTDIR)$(pkgdatadir)/contrib/$$file`; \
	    echo "$(mkinstalldirs) -m 755 $$filePath/"; \
	    $(mkinstalldirs) -m 755 $$filePath/; \
	    echo "$(INSTALL_DATA) $$file $$filePath" ; \
	    $(INSTALL_DATA) "$$file" $$filePath; \
	done

mostlyclean-local:
	rm -rf archives \
	toolbox_skeleton/cleaner.sce \
	toolbox_skeleton/help/*/master_help.xml \
	toolbox_skeleton/help/*/scilab_*_help \
	toolbox_skeleton/jar \
	toolbox_skeleton/loader.sce \
	toolbox_skeleton/macros/lib \
	toolbox_skeleton/macros/names \
	toolbox_skeleton/sci_gateway/cpp/cleaner.sce \
	toolbox_skeleton/sci_gateway/cpp/libskeleton_cpp.c \
	toolbox_skeleton/sci_gateway/cpp/libskeleton_cpp.* \
	toolbox_skeleton/sci_gateway/cpp/loader.sce \
	toolbox_skeleton/sci_gateway/c/cleaner.sce \
	toolbox_skeleton/sci_gateway/cleaner_gateway.sce \
	toolbox_skeleton/sci_gateway/c/libskeleton_c.c \
	toolbox_skeleton/sci_gateway/c/libskeleton_c.* \
	toolbox_skeleton/sci_gateway/c/loader.sce \
	toolbox_skeleton/sci_gateway/fortran/cleaner.sce \
	toolbox_skeleton/sci_gateway/fortran/libskeleton_fortran.c \
	toolbox_skeleton/sci_gateway/fortran/libskeleton_fortran.* \
	toolbox_skeleton/sci_gateway/fortran/loader.sce \
	toolbox_skeleton/sci_gateway/loader_gateway.sce \
	toolbox_skeleton/src/c/cleaner.sce \
	toolbox_skeleton/src/c/libcsum.* \
	toolbox_skeleton/src/c/loader.sce \
	toolbox_skeleton/src/fortran/cleaner.sce \
	toolbox_skeleton/src/fortran/libfsum.* \
	toolbox_skeleton/src/fortran/loader.sce \
	toolbox_skeleton/unloader.sce \
	xcos_toolbox_skeleton/cleaner.sce \
	xcos_toolbox_skeleton/help/*/master_help.xml \
	xcos_toolbox_skeleton/help/*/scilab_*_help \
	xcos_toolbox_skeleton/jar \
	xcos_toolbox_skeleton/loader.sce \
	xcos_toolbox_skeleton/macros/lib \
	xcos_toolbox_skeleton/macros/names \
	xcos_toolbox_skeleton/sci_gateway/cleaner.sce \
	xcos_toolbox_skeleton/sci_gateway/cleaner_gateway.sce \
	xcos_toolbox_skeleton/sci_gateway/c/lib \
	xcos_toolbox_skeleton/sci_gateway/c/libskeleton_c.c \
	xcos_toolbox_skeleton/sci_gateway/c/loader.sce \
	xcos_toolbox_skeleton/sci_gateway/c/cleaner.sce \
	xcos_toolbox_skeleton/sci_gateway/c/libxcos_toolbox_skeleton_c.c \
	xcos_toolbox_skeleton/sci_gateway/c/libxcos_toolbox_skeleton_c.* \
	xcos_toolbox_skeleton/src/c/cleaner.sce \
	xcos_toolbox_skeleton/src/c/libxcos_tbx_skel.* \
	xcos_toolbox_skeleton/src/c/loader.sce \
	xcos_toolbox_skeleton/unloader.sce
