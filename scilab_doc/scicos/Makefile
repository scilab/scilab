SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

TEXFILES = scicos.tex 

all:: $(SCIDOC)/Scicos.ps


$(SCIDOC)/Scicos.ps: $(TEXFILES)
	latex scicos
	latex scicos
	$(RM) $(SCIDOC)/Scicos.ps
	dvips -o $(SCIDOC)/Scicos.ps scicos.dvi

clean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	scicos_pdf.* *.tmp Scicos_pdf.ps
distclean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	scicos_pdf.* *.tmp Scicos_pdf.ps

pdf: $(TEXFILES)
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\n",$$1} else {print $$0} }' \
	scicos.tex > scicos_pdf.tex
	latex scicos_pdf
	latex scicos_pdf
	dvips -z -o Scicos_pdf.ps scicos_pdf.dvi
