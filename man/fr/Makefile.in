SHELL = /bin/sh

include ../../Path.incl
include ../../Makefile.incl

DIRS = arma control dcd elementary fileio functions graphics gui \
	linear metanet nonlinear polynomials programming robust \
	signal sound strings tdcs translation utilities \
	statistics mtlb sparse pvm tksci

QUIET= > /dev/null 
LANGUAGE=fr

#XSLFILE=html.xsl
XSLFILE=html-rev.xsl

man:
	@echo "Creating html help files" 
	$(SCIDIR)/bin/scilab -l $(LANGUAGE) \
		-nwni  -e "xmltohtml([],[],'$(XSLFILE)');quit" -args -nouserstartup

# running step by step 
# valid steps are whatis.step html.step index.step contents.step
#                 sciman.step all.step 

%.step: 
	@echo "Creating $*" 
	$(SCIDIR)/bin/scilab -l $(LANGUAGE) \
		-nwni  -e "xmltohtml([],[],'$(XSLFILE)','$*');quit" -args -nouserstartup



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
	@echo "Cleaning generated html files" 
	@for file in `find ./ -name '*.xml'`; do $(RM) $${file%xml}htm; done
	@for file in `find ./ -name '*.xml'`; do $(RM) $${file%xml}xml2; done
	@for dir in `ls -d ./ | grep -v scicos`; do $(RM) $${dir}/whatis.htm; done


