all	:: genlib names lib

lib 	: $(MACROS) 
	$(SCIDIR1)\macros\Lib @<< 
$(MACROS)
<<
	@del *.bin
	$(SCIDIR1)\bin\scilex.exe -nwni -ns -f tmp_Macros
	$(SCIDIR1)\bin\scilex.exe -nwni -ns -f genlib
	@del tmp_*

clean::


distclean::
	@del *.bin 
	@del lib 
	@del names 
	@del genlib 

# generates the names file and a Scilab script genlib 
# for building lib 

names genlib : 	$(MACROS) Makefile 
	$(SCIDIR1)\macros\Name @<<
$(NAM) $(NAME) $(MACROS)
<<


