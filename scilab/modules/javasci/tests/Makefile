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
SCILABBIN=$$SCI/bin/scilab
info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test the  distribution"
	@echo " SEE THE README FILE (IMPORTANT) "

tests: 
	@if test -z "$SCI"; then \
		echo "SCI must be defined"; \
		exit 1; \
	fi; \
	if test -x "$(SCILABBIN)"; then \
		$(SCILABBIN) -nwni -nb -f JavaTests.sce; \
	else \
		echo "Cannot find file $(SCILABBIN)"; \
	fi

clean ::
	$(RM) *.dia

distclean::
	$(RM) *.dia
