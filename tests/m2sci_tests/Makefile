SHELL = /bin/sh

SCIDIR = ../..

include ../../Makefile.incl

info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test M2SCI"
	@echo " SEE THE README FILE (IMPORTANT) "

tests:
	$(SCIDIR)/bin/scilab -nwni -nb -f m2sci.sce

clean ::
	$(SCIDIR)/bin/scilab -nwni -nb -f cleanTests.sce

distclean::
	$(SCIDIR)/bin/scilab -nwni -nb -f cleanTests.sce
