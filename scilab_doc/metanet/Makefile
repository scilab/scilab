SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

all:: $(SCIDOC)/Metanet.ps

$(SCIDOC)/Metanet.ps: metanet.tex
	touch metanet.ind
	latex metanet
	makeindex metanet
	latex metanet
	$(RM) $(SCIDOC)/Metanet.ps
	dvips -o $(SCIDOC)/Metanet.ps metanet.dvi

clean::
	$(RM) *.aux *.log  *.dvi *.idx *.ind *.ilg *.lof *.lot *.toc \
	metanet_pdf.* *.tmp Metanet_pdf.ps
distclean::
	$(RM) *.aux *.log  *.dvi *.idx *.ind *.ilg *.lof *.lot *.toc \
	metanet_pdf.* *.tmp Metanet_pdf.ps

pdf: metanet.tex
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\\usepackage{times}\n",$$1} else {print $$0} }' \
	metanet.tex > metanet_pdf.tex
	touch metanet.ind
	latex metanet_pdf
	makeindex metanet_pdf
	latex metanet_pdf
	dvips -z -o Metanet_pdf.ps metanet_pdf.dvi
