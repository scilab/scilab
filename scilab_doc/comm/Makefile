SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

all:: $(SCIDOC)/Comm.ps

$(SCIDOC)/Comm.ps: comm.tex
	latex comm
	latex comm
	$(RM) $@
	dvips -o $(SCIDOC)/Comm.ps comm.dvi

clean::
	$(RM) *.aux *.log  *.dvi *.idx *.ind *.ilg *.lof *.lot *.toc \
	comm_pdf.* *.tmp Comm_pdf.ps
distclean::
	$(RM) *.aux *.log  *.dvi *.idx *.ind *.ilg *.lof *.lot *.toc \
	comm_pdf.* *.tmp Comm_pdf.ps 

CFLAGS = $(CC_OPTIONS)

examples: alpha beta

alpha: alpha.c
	$(RM) alpha
	$(CC) -o alpha alpha.c $(CC_LDFLAGS) $(SCIDIR)/libs/libcomm.a

beta: beta.c
	$(RM) beta
	$(CC) -o beta beta.c $(CC_LDFLAGS) $(SCIDIR)/libs/libcomm.a

distclean::
	$(RM) alpha beta

pdf: comm.tex
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\\usepackage{times}\n",$$1} else {print $$0} }' \
	comm.tex > comm_pdf.tex
	latex comm_pdf
	latex comm_pdf
	dvips -z -o Comm_pdf.ps comm_pdf.dvi
