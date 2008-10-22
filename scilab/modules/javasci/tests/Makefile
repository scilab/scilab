########### 
## Unitary tests of Scilab features
## Java tests
## INRIA
## Authors : 
## * Pierre MARECHAL
## * Allan CORNET
##
## Update Sylvestre LEDRU 2007
##
###########

SHELL = /bin/sh

info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test the  distribution"
	@echo " SEE THE README FILE (IMPORTANT) "

tests: 
	@if test -z "$SCI"; then \
		echo "SCI must be defined"; \
		exit 1; \
	fi; \
	$(SCI)/bin/scilab -nwni -nb -f JavaTests.sce

clean ::
	$(RM) *.dia

distclean::
	$(RM) *.dia
