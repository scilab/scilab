SHELL = /bin/sh

include ../../Path.incl
include ../../Makefile.incl

DIRS = arma control dcd elementary fileio functions graphics gui \
	linear metanet nonlinear polynomials programming pvm robust \
	scicos signal sound strings tdcs tksci translation utilities

QUIET= > /dev/null 
LANGUAGE=eng 

XSLFILE=../html-jpc.xsl
#XSLFILE=../html.xsl

man: whatis html index contents

# whatis must be done before html
whatis:
	@for i in $(DIRS) ;\
	do \
		(cd $$i ;\
		echo "Creating whatis.html in $$i"; \
		$(SCIDIR)/bin/scilab -nw -f "../make-whatis.sce" $(QUIET)) \
	done

html:
	@for i in $(DIRS) ;\
	do \
		(cd $$i ;\
		echo "Processing chapter $$i"; \
		$(SCIDIR)/bin/scilab -nw -l $(LANGUAGE) -f ../make-links.sce  $(QUIET); \
		for j in *.xml2; \
		do if [ $$j != "*.xml2" ]; then \
			echo "   processing $${j%.*}.xml"; \
			sabcmd $(XSLFILE) $$j $${j%.*}.htm 2> sabcmd.tmp; \
			if (grep Error sabcmd.tmp $(QUIET)) then \
				echo "   bad syntax in file: $$j"; \
			fi; \
			rm -f sabcmd.tmp; \
		fi; done; \
		rm -f *.xml2); \
	done

index:
	@echo "Creating index.html"
	@$(SCIDIR)/bin/scilab -nw -l $(LANGUAGE) -f "make-index.sce" $(QUIET)


contents: 
	@echo "Creating contents.html"
	@$(SCIDIR)/bin/scilab -nw -l $(LANGUAGE) -f "make-contents.sce" $(QUIET)

check:
	@for i in $(DIRS) ;\
	do \
		(cd $$i ; echo "Checking xlm in $$i"; \
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

manclean:
	$(RM)  $(FILES_TO_CLEAN) */whatis */*.htm index.htm contents.htm
