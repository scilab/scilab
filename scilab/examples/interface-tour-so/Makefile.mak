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
	nmake -f Makelib.mak

clean  	: 
	nmake -f Makelib.mak clean 
	del Makelib.mak 

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
	nmake -f Makelib.mak distclean
	del Makelib.mak 

tests	: all
	$(SCIDIR1)\bin\scilex.exe  -f  libexamples.tst

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter
	@echo "-->exec exXX.sce" to test one example 
	@echo "-->exec libexamples.tst" to test all examples
	@echo ------------------------------------------;
	@echo Type nmake /f Makefile.mak tests 
	@echo to run all tests 
	@echo ------------------------------------------;

