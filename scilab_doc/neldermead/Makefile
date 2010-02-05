#!/bin/sh

RAPPORT = neldermead

pdf:
	pdflatex ${RAPPORT}
	#bibtex ${RAPPORT}
	#pdflatex ${RAPPORT}
	#pdflatex ${RAPPORT}

dvi:
	latex ${RAPPORT}
	bibtex ${RAPPORT}
	latex ${RAPPORT}

clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.out
	rm -f *.toc
	rm -f *.idx

ortho:
	ispell -t ${RAPPORT}.tex

distclean:
	make clean
	rm -f ${RAPPORT}.pdf
	rm -f ${RAPPORT}.dvi

