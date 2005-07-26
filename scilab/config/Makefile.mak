FFLAGS = $(FC_OPTIONS)
CFLAGS = $(CC_OPTIONS)
RESOURCES= routines/wsci/Rscilab.res 


bin/LibScilab.dll: $(MAINSCI) $(DEFAULTS) $(LIBRSCI)
	@echo Creation of $*.dll and import lib $*.lib
	@$(LINKER) -subsystem:windows $(LINKER_FLAGS) $(RESOURCES) $(MAINSCI) $(DEFAULTS) $(LIBR) $(XLIBS) \
	 /dll /out:"$*.dll" /implib:"$*.lib" /def:"$*.def" 

bin/WScilex.exe : bin/LibScilab.dll
	@$(LINKER)  $(LINKER_FLAGS) -OUT:"$*.exe"  $(RESOURCES) \
	routines/wsci/WScilex/WScilex_f2c.obj bin/LibScilab.lib $(XLIBS) 

bin/scilex.exe : bin/LibScilab.dll
	@$(LINKER)  $(LINKER_FLAGS) -OUT:"$*.exe"  $(RESOURCES) \
	routines/wsci/CScilex/CScilex_f2c.obj bin/LibScilab.lib $(XLIBS) 
