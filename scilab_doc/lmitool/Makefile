SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

TEXFILES = lmidoc.tex

all:: $(SCIDOC)/Lmi.ps

$(SCIDOC)/Lmi.ps: $(TEXFILES)
	latex lmidoc
	latex lmidoc
	$(RM) $@
	dvips -o $(SCIDOC)/Lmi.ps lmidoc.dvi

clean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	lmidoc_pdf.* *.tmp Lmi_pdf.ps
distclean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	lmidoc_pdf.* *.tmp Lmi_pdf.ps

pdf: $(TEXFILES)
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\\usepackage{times}\n",$$1} else {print $$0} }' \
	lmidoc.tex > lmidoc_pdf.tex
	latex lmidoc_pdf
	latex lmidoc_pdf
	dvips -z -o Lmi_pdf.ps lmidoc_pdf.dvi
