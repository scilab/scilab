SHELL = /bin/sh

include ../../Path.incl
include ../../Makefile.incl

LANGUAGE=eng 

XSLFILE=html-rev.xsl

man: 	
	@echo "Creating html help files" 
	..\..\bin\scilex.exe -l $(LANGUAGE) \
		-nwni -e xmltohtml([],[],'$(XSLFILE)');quit

distclean::
	$(RM)  $(FILES_TO_CLEAN)

manclean: distclean 
	$del  *\whatis *\*.htm index.htm contents.htm
	$del  sciman.hh*
	$del  sciman.chm
 

 

