SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

INTERSCIFILES = intersci.tex calc.desc  contr.desc  csom.desc  ext.desc myex_builder.sce   myex_builder_orig.sce fcalc.f csom.c 

all:: $(SCIDOC)/Intersci.ps

$(SCIDOC)/Intersci.ps:  $(INTERSCIFILES)
	latex intersci
	makeindex intersci
	latex intersci
	$(RM) $@
	dvips -o $(SCIDOC)/Intersci.ps intersci.dvi

clean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	intersci_pdf.* *.tmp Intersci_pdf.ps *motif.gif *.html \
	intersci.image.tex *.hind intersci_pdf.*
distclean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	intersci_pdf.* *.tmp Intersci_pdf.ps *motif.gif *.html \
	intersci.image.tex *.hind intersci_pdf.*

html::   $(INTERSCIFILES)
	hevea intersci.tex;hacha intersci.html

pdf::   $(INTERSCIFILES) 
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\\usepackage{times}\n",$$1} else {print $$0} }' \
	intersci.tex > intersci_pdf.tex
	latex intersci_pdf
	makeindex -s intersci.isty intersci_pdf
	latex intersci_pdf
	dvips -z -o Intersci_pdf.ps intersci_pdf.dvi
	ps2pdf Intersci_pdf.ps Intersci.pdf
	mv Intersci.pdf $(SCIDOC)/Intersci.pdf
