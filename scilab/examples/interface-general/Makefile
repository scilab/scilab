SCIDIR=../../

include $(SCIDIR)/Makefile.incl


all	:: Makelib job 
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
	@$(RM) -f -r *.o *.lo *.la *.so *.sl *.a *.dia .job.sce so_locations
	@$(RM) -f -r .libs f3cloader.sce f3cmake f3floader.sce f3fmake
	@cd lib ; make clean

distclean:: clean 
	@$(RM) -f *.dia  *_gateway.c 
	@$(RM) -f Makelib 
	@cd lib ; make distclean 


message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec loader.sce";
	@echo "------------------------------------------";

