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
	del *.dia
  del *.lib
  del *.exp
  del *.def
  del *.dll
  del *.obj
  del libmex.c
  del makelib.mak
  del loader.sce
  del path.incl

distclean::
	del *.dia
  del *.lib
  del *.exp
  del *.def
  del *.dll
  del *.obj
  del libmex.c
  del makelib.mak
  del loader.sce
  del path.incl

tests	: 
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e exec('builder.sce',-1);quit; >NUL
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e scitest('libmex.tst',%t);quit; 

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec libmex.tst 
	@echo to run a test
	@echo ------------------------------------------;


