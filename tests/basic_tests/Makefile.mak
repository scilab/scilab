
SCIDIR = ../..
SCIDIR1 = ..\..

include ../../Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) -DFORDLL 
CFLAGS = $(CC_OPTIONS) -DFORDLL 

DUMPEXTS = $(SCIDIR1)\bin\dumpexts
SCIIMPLIB = $(SCIDIR)/bin/LibScilab.lib

OBJS = externals.obj

all :: externals.dll tests

tests: 
	$(SCIDIR1)\bin\scilex -nb -f maketests.sce

tests-clean:
	$(SCIDIR1)\bin\scilex -nb -f makecleantests.sce

clean ::
	@del externals.obj 
	@del externals.dll
	@del *.dia 
	@del examples.tst 
	@del get_examples.log 
	@del *.graph 
	@del test.wav 
	@del one_man.tst 
	@del test_macro_exec 
	@del asave 
	@del results 
	@del *.bin

distclean:: clean 

externals.dll : $(OBJS) 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(GUILIBS) $(SCIIMPLIB) \
	/subsystem:windows /dll /out:"$*.dll" /implib:"$*.lib" /def:$*.def 
















	
