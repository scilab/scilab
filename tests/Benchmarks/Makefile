SHELL = /bin/sh

SCIDIR = ../..

include ../../Makefile.incl

info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test the  distribution"
	@echo " SEE THE README FILE (IMPORTANT) "

tests: 
	$(SCIDIR)/bin/scilab -nwni -nb -f benchmark.sce

clean ::
	$(RM) $(SCIDIR)/benchmark.ini

distclean::
	$(RM) $(SCIDIR)/benchmark.ini
