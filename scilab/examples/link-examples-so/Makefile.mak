SCIDIR=../../
SCIDIR1=..\..\

all	: message 

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter:;
	@echo -->exec exXX.sce; 
	@echo to execute example XX 
	@echo ------------------------------------------;
	@echo Type nmake /f Makefile.mak tests 
	@echo to run all tests 
	@echo ------------------------------------------;

tests	: 
	$(SCIDIR1)\bin\scilex.exe  -f zall.sce


distclean	::
	@del *.obj 
	@del *.dll
	@del *.ilib 
	@del *.pdk
	@del *.pdb
	@del *.ilk 
	@del *.def
	@del *.exp 
	@del libext*


