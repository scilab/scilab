#!/bin/sh

RAPPORT = scilabstats

LT = pdflatex

pdf:
	$(LT) $(RAPPORT)
	bibtex $(RAPPORT)
	$(LT) $(RAPPORT)
	$(LT) $(RAPPORT)


dvi:
	$(LT) ${RAPPORT}
	bibtex ${RAPPORT}
	$(LT) ${RAPPORT}

clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.out
	rm -f *.toc

spell:
	ispell -t ${RAPPORT}.tex

distclean:
	make clean
	rm -f ${RAPPORT}.pdf
	rm -f ${RAPPORT}.dvi
	
