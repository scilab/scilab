SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/../$(SCIVERSION)/doc
include ../../Makefile.incl

INTROFILES = chap4.tex chap0.tex chap1.tex chap2.tex \
	chap5.tex intro.tex chap3.tex \
	appendices.tex chap6.tex intro.tex

all:: $(SCIDOC)/Intro.ps

$(SCIDOC)/Intro.ps: subs  $(INTROFILES)
	latex intro
	makeindex -s intro.isty intro
	latex intro
	$(RM) $@
	dvips -o $(SCIDOC)/Intro.ps intro.dvi

intro-html.html: subs  $(INTROFILES) intro-html.tex
	hevea	intro-html.tex   
	imagen intro-html 

clean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	intro_pdf.* *.tmp Intro_pdf.ps *.hind *.haux *.html intro-html*.gif \
	intro-html.image.tex *_motif.gif
distclean::
	$(RM) *.aux *.log  *.dvi *.ind *.idx *.ilg *.lof *.lot *.toc \
	intro_pdf.* *.tmp Intro_pdf.ps *.hind *.haux *.html intro-html*.gif\
	intro-html.image.tex *_motif.gif

SUBDIRS = diary figures macros

subs:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making" subs "in intro/$$i..."; \
			$(MAKE) $(MFLAGS) all); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making" clean "in intro/$$i..."; \
			$(MAKE) $(MFLAGS) clean); \
	done

distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making" distclean "in intro/$$i..."; \
			$(MAKE) $(MFLAGS) distclean); \
	done
html: subs  $(INTROFILES)
	hevea intro-html.tex;imagen intro-html;hacha intro-html.html	
	@\rm -rf ../Intro-html
	@mkdir ../Intro-html
	@mv *.html ../Intro-html
	@mv *.gif ../Intro-html

pdf: subs  $(INTROFILES)  
	awk '{if ($$1~"documentclass") \
	{printf "%s\n\\usepackage[dvips,colorlinks=true]{hyperref}\\usepackage{times}\n",$$1} else {print $$0} }' \
	intro.tex > intro_pdf.tex
	latex intro_pdf
	makeindex -s intro.isty intro_pdf
	latex intro_pdf
	dvips -z -o Intro_pdf.ps intro_pdf.dvi
	ps2pdf Intro_pdf.ps
	mv  Intro_pdf.ps $(SCIDOC)/Intro.pdf
