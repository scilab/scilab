SCIDIR=../../../

include $(SCIDIR)/Makefile.incl

all	:: Makelib
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
	@$(RM) -f -r *.o *.obj *.dll *.lo *.la *.so *.sl *.a *.dia .job.sce so_locations
	@$(RM) -f -r .libs 

distclean:: clean 
	@$(RM) -f *.dia  *_gateway.c 
	@$(RM) -f Makelib 

message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec loader.sce";
	@echo "------------------------------------------";

