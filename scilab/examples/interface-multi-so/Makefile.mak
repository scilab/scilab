SCIDIR=../../
SCIDIR1=..\..\

all	: Makelib.mak job message 
	nmake -f Makelib.mak

Makelib.mak : builder.sce
	@echo running builder 
	type builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

job	: 
	Makesubdirs.bat lib
	nmake -f Makelib.mak

clean  	: 
	nmake -f Makelib.mak clean 
	Makesubdirs.bat lib-clean 

distclean:: clean 
	Makesubdirs.bat lib-distclean 

tests	: all
	nmake -f Makelib.mak tests 

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec loader.sce " to load the shared libraries
	@echo Type nmake /f Makefile.mak tests 
	@echo to run a test
	@echo ------------------------------------------;


