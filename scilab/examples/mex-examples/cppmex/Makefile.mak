SCIDIR=../../../
SCIDIR1=..\..\..\

MAKE=nmake /nologo

all	: Makelib.mak  message 

Makelib.mak : builder.sce
	@echo running builder BE PATIENT
	type builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

clean  	: 
	$(MAKE) -f Makelib.mak clean 
	del Makelib.mak 

distclean::
	del libmex.c
	del make.exe.stackdump
	del libexamples.ilk
	del libexamples.pdb

distclean::
	$(MAKE) -f Makelib.mak distclean
	del Makelib.mak 

tests	: Makelib.mak 
	$(MAKE) -f Makelib.mak tests 

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec libmex.tst" to test 
	@echo ------------------------------------------;
	@echo Type nmake /nologo /f Makefile.mak tests 
	@echo to run all tests 
	@echo ------------------------------------------;

