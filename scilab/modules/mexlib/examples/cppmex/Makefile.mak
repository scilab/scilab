SCIDIR=../../../..
SCIDIR1=..\..\..\..

MAKE=nmake /nologo

all	: Makelib.mak  message 

Makelib.mak : builder.sce
	@echo running builder BE PATIENT
	type builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

clean  	: 
	del Makelib.mak 
	del libmex.dia
	del libmex.dll
	del libmex.lib
	del *.obj
	del libmex.exp
	del libmex.def
	del libmex.c
	del loader.sce
		

distclean::
	del Makelib.mak 
	del libmex.dia
	del libmex.dll
	del libmex.lib
	del libmex.obj
	del libmex.exp
	del libmex.def
	del libmex.c
	del loader.sce


tests	: 
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e exec('builder.sce',-1);quit; >NUL
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e scitest('libmex.tst',%t);quit;

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec libmex.tst" to test 
	@echo ------------------------------------------;
	@echo Type nmake /nologo /f Makefile.mak tests 
	@echo to run all tests 
	@echo ------------------------------------------;

