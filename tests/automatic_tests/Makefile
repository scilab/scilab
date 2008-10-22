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
	@$(RM) *.dia Makelib fun1.* *.c *.o *.obj libext1c.* libfoo* loader.sce *.dat asave foo fooc.* foof.* 
	@$(RM) -r .libs

distclean:: clean 
