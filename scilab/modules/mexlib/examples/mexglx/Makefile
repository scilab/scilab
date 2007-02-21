SCIDIR=../../../

include $(SCIDIR)/Makefile.incl


all	:: 
	@echo "See the README file" 

Makelib : builder.sce
	@echo running builder: BE PATIENT ...
	@$(SCIDIR)/util/scidem $(SCIDIR) builder.sce builder.dia

job	: 
	make -f Makelib

tests	: 
	@$(SCIDIR)/util/scidem $(SCIDIR) test.sce test.dia

clean	::
	@echo Cleaning 
	@$(RM) -f -r *.o *.lo *.la *.so *.dll *.sl *.a *.dia so_locations
	@$(RM) -f -r .libs foo.mexglx Makelib 

distclean:: clean 
	@$(RM) -f *.dia  *_gateway.c 
	@$(RM) -f Makelib 

message:
	@echo "------------------------------------------";
	@echo "At Scilab prompt, enter:";
	@echo "-->exec loader.sce";
	@echo "------------------------------------------";


