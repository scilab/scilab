SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

INTERNALFILES = compl0.tex internals.tex

all:: $(SCIDOC)/Internals.ps

$(SCIDOC)/Internals.ps: $(INTERNALFILES)
	latex compl
	makeindex compl
	latex compl
	$(RM) $@
	dvips -o $(SCIDOC)/Internals.ps compl.dvi

clean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	compl_pdf.* *.tmp Internals_pdf.ps
distclean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	compl_pdf.* *.tmp Internals_pdf.ps

pdf: $(INTERNALFILES)
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\\usepackage{times}\n",$$1} else {print $$0} }' \
	compl.tex > compl_pdf.tex
	latex compl_pdf
	makeindex compl_pdf
	latex compl_pdf
	dvips -z -o Internals_pdf.ps compl_pdf.dvi
