all	:: genlib names lib

lib 	: $(MACROS) 
	$(SCIDIR1)\macros\Lib @<< 
$(MACROS)
<<
	$(SCIDIR1)\bin\scilex.exe -nb -nwni -ns -f tmp_Macros
	$(SCIDIR1)\bin\scilex.exe -nb -nwni -ns -f genlib
	@del tmp_*

clean::
	@del *.bin
	@del lib 
	@del names 
	@del genlib 

distclean::
	 
	@del lib 
	@del names 
	@del genlib 

# generates the names file and a Scilab script genlib 
# for building lib 

names genlib : 	$(MACROS) Makefile 
	$(SCIDIR1)\macros\Name @<<
$(NAM) $(NAME) $(MACROS)
<<


