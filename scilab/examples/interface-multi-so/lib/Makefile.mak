SCIDIR=../../../
SCIDIR1=..\..\..\

all	: Makelib.mak job 
	nmake -f Makelib.mak

Makelib.mak : builder.sce
	@echo running builder 
	cat builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

job	: 
	nmake -f Makelib.mak

clean  	: 
	nmake -f Makelib.mak clean 

distclean:: clean 




