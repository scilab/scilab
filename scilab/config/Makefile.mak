FFLAGS = $(FC_OPTIONS)
CFLAGS = $(CC_OPTIONS)
RESOURCES= routines/wsci/Rscilab.res 


bin/LibScilab.dll: $(DEFAULTS) $(LIBRSCI)
	@echo Creation of $*.dll and import lib $*.lib
	@$(LINKER) -subsystem:windows $(LINKER_FLAGS) $(RESOURCES) $(DEFAULTS) $(LIBR) $(XLIBS) \
	 /dll /out:"$*.dll" /implib:"$*.lib" /def:"$*.def" 

bin/WScilex.exe : bin/LibScilab.dll
	@$(LINKER)  $(LINKER_FLAGS) -OUT:"$*.exe"  $(RESOURCES) \
	routines/wsci/WScilex/WScilex.obj bin/LibScilab.lib $(XLIBS) 

bin/scilex.exe : bin/LibScilab.dll
	@$(LINKER)  $(LINKER_FLAGS) -OUT:"$*.exe"  $(RESOURCES) \
	routines/wsci/CScilex/CScilex.obj bin/LibScilab.lib $(XLIBS) 
