include ../../Path.incl

MAKE=nmake /nologo

all	:  builder.sce
	@echo running builder
	cat builder.sce > job.sce
	echo quit >> job.sce 
	"$(SCIDIR1)\bin\scilex.exe" -nwni -f job.sce 
	del job.sce 

tests   :  Makefile 
	@echo "No tests to do in man-examples"

distclean::
	$del *\*.cat *\*.xml *\*.xml2 *\*.htm *.htm sciman.hh*

clean::






