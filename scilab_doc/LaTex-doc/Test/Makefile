SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

TR2TEX = ../tr2latex/tr2tex

EX = binary

all:: Docu.dvi

Docu.dvi: example.tex
	$(RM) example.1
	latex Docu.tex
	makeindex -s Docu.isty Docu
	latex Docu

$(TR2TEX):
	cd ../tr2latex ; $(MAKE) $(MFLAGS)

example.tex : $(TR2TEX) example.1
	$(TR2TEX)  -m example.1 > /tmp/example.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' -e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/example.tex > example.tex
	$(RM) /tmp/example.tex

example.1:
	cp $(SCIDIR)/man/*/$(EX).man example.1

clean::
	$(RM) *.aux *.log  example.tex *.dvi *.ilg *.ind *.idx example.1
distclean::
	$(RM) *.aux *.log  example.tex *.dvi *.ilg *.ind *.idx example.1
