SCIDIR=../../../
SCIDIR1=..\..\..\

MAKE=nmake /nologo

all	: Makelib.mak  

Makelib.mak : builder.sce
	@echo running builder BE PATIENT
	cat builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

clean  	: 
	$(MAKE) -f Makelib.mak clean 
	del Makelib.mak 

distclean::
	del foo.obj
	del bar.obj 
	del libutil.*

distclean::
	$(MAKE) -f Makelib.mak distclean
	del Makelib.mak 





