SCIDIR=../../../

include $(SCIDIR)/Makefile.incl

all	:: 
	@echo "See the README file" 

Makelib : builder.sce
	@echo running builder: BE PATIENT ...
	@$(SCIDIR)/util/scidem $(SCIDIR) builder.sce builder.dia

job	: 
	make -f Makelib

tests	: all

#	make -f Makelib tests 

clean	::
	@echo Cleaning 
	@$(RM) -f -r *.o *.lo *.la *.so *.dll *.sl *.a *.dia so_locations
	@$(RM) -f -r .libs 

distclean:: clean 
	@$(RM) -f *.dia  *_gateway.c 
	@$(RM) -f Makelib 

message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec loader.sce";
	@echo "------------------------------------------";


