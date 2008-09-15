##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### DIGITEO - Scilab 2008
##########

# Install the contrib_skeleton directory (the template)

SKELETON_FILES=toolbox_skeleton/builder.sce \
toolbox_skeleton/changelog.txt \
toolbox_skeleton/demos/c_sum.dem.sce \
toolbox_skeleton/demos/fortran_sum.dem.sce \
toolbox_skeleton/demos/scilab_sum.dem.sce \
toolbox_skeleton/demos/cpp_find.dem.sce \
toolbox_skeleton/demos/toolbox_skeleton.dem.gateway.sce \
toolbox_skeleton/etc/toolbox_skeleton.quit \
toolbox_skeleton/etc/toolbox_skeleton.start \
toolbox_skeleton/help/builder_help.sce \
toolbox_skeleton/help/en_US/build_help.sce \
toolbox_skeleton/help/en_US/c_sub.xml \
toolbox_skeleton/help/en_US/c_sum.xml \
toolbox_skeleton/help/en_US/fortran_sum.xml \
toolbox_skeleton/help/en_US/scilab_sum.xml \
toolbox_skeleton/help/en_US/cpp_find.xml \
toolbox_skeleton/help/fr_FR/build_help.sce \
toolbox_skeleton/help/fr_FR/c_sub.xml \
toolbox_skeleton/help/fr_FR/c_sum.xml \
toolbox_skeleton/help/fr_FR/fortran_sum.xml \
toolbox_skeleton/help/fr_FR/scilab_sum.xml \
toolbox_skeleton/license.txt \
toolbox_skeleton/macros/buildmacros.sce \
toolbox_skeleton/macros/scilab_sum.sci \
toolbox_skeleton/readme.txt \
toolbox_skeleton/sci_gateway/builder_gateway.sce \
toolbox_skeleton/sci_gateway/c/builder_gateway_c.sce \
toolbox_skeleton/sci_gateway/c/sci_csub.c \
toolbox_skeleton/sci_gateway/c/sci_csum.c \
toolbox_skeleton/sci_gateway/fortran/builder_gateway_fortran.sce \
toolbox_skeleton/sci_gateway/fortran/sci_fsum.c \
toolbox_skeleton/sci_gateway/cpp/builder_gateway_cpp.sce \
toolbox_skeleton/sci_gateway/cpp/sci_cpp_find.cxx \
toolbox_skeleton/src/builder_src.sce \
toolbox_skeleton/src/c/builder_c.sce \
toolbox_skeleton/src/c/csub.c \
toolbox_skeleton/src/c/csum.c \
toolbox_skeleton/src/fortran/builder_fortran.sce \
toolbox_skeleton/src/fortran/fsum.f \
toolbox_skeleton/tests/unit_tests/c_sum.tst \
toolbox_skeleton/tests/unit_tests/fortran_sum.tst \
toolbox_skeleton/tests/unit_tests/scilab_sum.tst \
toolbox_skeleton/toolbox_skeleton.iss \
toolbox_skeleton/toolbox_skeleton_redist.iss \
loader.sce


install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/contrib/toolbox_skeleton; \
	for file in $(SKELETON_FILES) ; do \
		echo "$(INSTALL_DATA) -D $$file $(DESTDIR)$(pkgdatadir)/contrib/$$file" ; \
		$(INSTALL_DATA) -D "$$file" $(DESTDIR)$(pkgdatadir)/contrib/$$file ; \
	done
