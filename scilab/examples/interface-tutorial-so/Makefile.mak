SCIDIR=../../
SCIDIR1=..\..\

all	: Makelib.mak job message
	nmake -f Makelib.mak

Makelib.mak : builder.sce
	@echo running builder 
	cat builder.sce > job.sce
	echo quit >> job.sce 
	$(SCIDIR1)\bin\scilex.exe -nwni -f job.sce 
	del job.sce 

job	: 
	nmake /c -f Makelib.mak

clean  	: 

distclean  	:
	nmake /c -f Makelib.mak clean 
	del Makelib.mak 
	del libtutorial.ilk 
	del libtutorial.pdb 

tests	: all
	$(SCIDIR1)\bin\scilex.exe  -f zall.sce

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec libtutorial.tst "
	@echo ------------------------------------------;
	@echo Type nmake /f Makefile.mak tests 
	@echo to run all tests 
	@echo ------------------------------------------;

