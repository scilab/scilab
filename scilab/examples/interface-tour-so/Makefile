SCIDIR=../../

include $(SCIDIR)/Makefile.incl

all	:: Makelib
	make -f Makelib

Makelib : builder.sce
	@echo running builder: BE PATIENT ...
	@$(SCIDIR)/util/scidem $(SCIDIR) builder.sce builder.dia

tests	: all
	make -f Makelib tests 

clean	::
	@echo Cleaning 
	@$(RM) -f -r *.o *.lo *.la *.so *.dll *.sl *.a *.dia .job.sce so_locations
	@$(RM) -f -r .libs f3cloader.sce f3cmake f3floader.sce f3fmake

distclean:: clean 
	@$(RM) -f *.dia  *_gateway.c 
	@$(RM) -f Makelib 

message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec loader.sce";
	@echo "------------------------------------------";



