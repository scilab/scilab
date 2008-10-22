SHELL = /bin/sh

include ../../../Makefile.incl

TR2TEX = ../tr2latex/tr2tex

all:: Readme.dvi

Readme.dvi: example.tex
	latex Readme.tex

$(TR2TEX):
	cd ../tr2latex ; $(MAKE) $(MFLAGS)

example.tex: $(TR2TEX) example.1
	$(TR2TEX)  -m example.1 > /tmp/example.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' -e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/example.tex > example.tex
	$(RM) /tmp/example.tex

clean::
	$(RM) *.aux *.log  example.tex *.dvi
distclean::
	$(RM) *.aux *.log  example.tex *.dvi
