SCIDIR=../../
SCIDIR1=..\..\

MAKE=nmake /nologo

all	: lib Makelib.mak  message 

lib	: 
	Makesubdirs.bat lib

Makelib.mak : builder.sce
	@echo running builder BE PATIENT
	type builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

clean  	: 
	$(MAKE) -f Makelib.mak clean 
	del Makelib.mak 
	Makesubdirs.bat lib-clean 

distclean::
	del libf3c*
	del libf3f*
	del f3cloader.sce 
	del f3floader.sce 
	del f3cmake.mak 
	del f3fmake.mak 
	del make.exe.stackdump
	del libexamples.ilk
	del libexamples.pdb

distclean::
	$(MAKE) -f Makelib.mak distclean
	del Makelib.mak 
	Makesubdirs.bat lib-distclean 

tests	: Makelib.mak 
	$(MAKE) -f Makelib.mak tests 

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec libmex.tst 
	@echo to run a test
	@echo ------------------------------------------;


