SHELL = /bin/sh

include ../../Path.incl
include ../../Makefile.incl

DIRS = arma control dcd elementary fileio functions graphics gui \
	linear metanet nonlinear polynomials programming pvm robust \
	signal sound strings tdcs tksci translation utilities mtlb
#scicos 
QUIET= > /dev/null 
LANGUAGE=fr

#XSLFILE=html.xsl
XSLFILE=html-rev.xsl

man: 	
	@echo "Creating html help files" 
	$(SCIDIR)/bin/scilab -l $(LANGUAGE) \
		-nwni -e "xmltohtml([],[],'$(XSLFILE)');quit"

# running step by step 
# valid steps are whatis.step html.step index.step contents.step
#                 sciman.step all.step 

%.step: 
	@echo "Creating $*" 
	$(SCIDIR)/bin/scilab -l $(LANGUAGE) \
		-nwni -e "xmltohtml([],[],'$(XSLFILE)','$*');quit"



check:
	@for i in $(DIRS) ;\
	do \
		(cd $$i ; echo "Checking xml in $$i"; \
		for j in *.xml; \
		do xmllint --noout --valid $$j 2> xmllint.tmp; \
			if (grep error xmllint.tmp $(QUIET)) then \
				echo "   bad syntax in file: $$j"; \
			fi; \
			rm -f xmllint.tmp; \
		done); \
	done

distclean::
	$(RM)  $(FILES_TO_CLEAN)

manclean: distclean 
	$(RM)  */whatis */*.htm index.htm contents.htm
	$(RM)  sciman.hh* sciman.chm

 

