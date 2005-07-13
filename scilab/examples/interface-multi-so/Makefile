SHELL = /bin/sh
SCIDIR=../..

include Path.incl 

include $(SCIDIR)/Makefile.incl

all	:: Makelib 
	cd lib ; make all
	make -f Makelib

Makelib : builder.sce
	@echo running builder 
	@$(SCIDIR)/util/scidem $(SCIDIR) builder.sce builder.dia

job	: 
	make -f Makelib

tests	: all
	make -f Makelib tests 

clean	::
	@echo Cleaning 
	@$(RM) -f -r *.o *.lo *.la *.so *.dll *.sl *.a *.dia so_locations
	@$(RM) -f -r .libs libmulti[0-9]*.c 
	@cd lib ; make clean

distclean:: clean 
	@$(RM) -f *.dia 
	@$(RM) -f Makelib 
	@cd lib ; make distclean 


message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec loader.sce";
	@echo "------------------------------------------";

