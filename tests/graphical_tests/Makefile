SHELL = /bin/sh

SCIDIR=../../

include ../../Makefile.incl


all:: info

info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test the  distribution"
	@echo " SEE THE README FILE (IMPORTANT) "

tests: 
	$(SCIDIR)/bin/scilab -nw -f maketests.sce

clean ::
	@$(RM) *.dia  *.c *.o *.obj 
	@$(RM) -r .libs

distclean:: clean 

