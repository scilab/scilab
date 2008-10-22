SHELL = /bin/sh

include ../../Version.incl
SCIDIR=../..
SCIDOC=$(SCIDIR)/doc
include ../../Makefile.incl

FILES = Chap1.tex Chap2.tex Chap3.tex Chap4.tex Chap5.tex \
	Chap6.tex Chap7.tex Chap8.tex Chap9.tex Chap10.tex \
	Chap11.tex Chap12.tex Chap13.tex Chap14.tex Chap15.tex \
	Chap16.tex Chap17.tex  Chap19.tex Chap20.tex \
	Chap21.tex Chap5-1.tex Chap5-2.tex  Chap5-3.tex  Chap5-4.tex  

MAN = $(SCIDIR)/man/


TR2TEX = tr2latex/translate
LATEX = latex

all:: $(SCIDOC)/Manual.ps

SUBDIRS = tr2latex

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making" clean "in LaTeX-doc/$$i..."; \
			$(MAKE) $(MFLAGS)  clean); \
	done

distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making" distclean "in LaTeX-doc/$$i..."; \
			$(MAKE) $(MFLAGS)  distclean); \
	done

$(SCIDOC)/Manual.ps: Tr2tex chapters couvert.tex couv2.ps
	$(LATEX) Docu.tex
	$(LATEX) Docu.tex
	$(LATEX) Docu.tex
	makeindex -s Docu.isty Docu.idx
	$(LATEX) Docu.tex
	$(RM) $@
	dvips -o $@ Docu.dvi

pdf: Tr2tex chapters couvert.tex couv2.ps
	awk '{if ($$1~"documentstyle") \
	{printf "\\documentstyle[verbatim,tr2latex/troffman,makeidx,hyperref,times]{book}\n"} else {print $$0} }' \
	Docu.tex > Docu_pdf.tex
	$(LATEX) Docu_pdf.tex
	$(LATEX) Docu_pdf.tex
	$(LATEX) Docu_pdf.tex
	makeindex -s Docu.isty Docu_pdf.idx
	$(LATEX) Docu_pdf.tex
	dvips -z -o Manual_pdf.ps Docu_pdf.dvi
	ps2pdf Manual_pdf.ps
	mv  Manual_pdf.pdf ../Manual.pdf
html: 
	build-html

couv2.ps: couv2.tex
	$(LATEX) couv2
	dvips -o couv2.ps couv2.dvi

chapters: $(FILES)

Tr2tex : tr2latex/*.c tr2latex/*.h
	cd tr2latex ; $(MAKE) $(MFLAGS)

Chap1.tex : $(TR2TEX) $(MAN)/programming/*.man
	$(TR2TEX)  -m $(MAN)/programming/?*.man > /tmp/Chap1.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap1.tex > Chap1.tex
	$(RM) /tmp/Chap1.tex

Chap2.tex : $(TR2TEX) $(MAN)/graphics/?*.man
	$(TR2TEX)  -m $(MAN)/graphics/?*.man > /tmp/Chap2.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap2.tex > Chap2.tex
	$(RM) /tmp/Chap2.tex

Chap3.tex : $(TR2TEX) $(MAN)/elementary/?*.man
	$(TR2TEX)  -m $(MAN)/elementary/?*.man > /tmp/Chap3.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap3.tex > Chap3.tex
	$(RM) /tmp/Chap3.tex

Chap4.tex : $(TR2TEX) $(MAN)/fileio/?*.man
	$(TR2TEX)  -m $(MAN)/fileio/?*.man > /tmp/Chap4.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap4.tex > Chap4.tex
	$(RM) /tmp/Chap4.tex

Chap5.tex : $(TR2TEX) $(MAN)/functions/?*.man
	$(TR2TEX)  -m $(MAN)/functions/?*.man > /tmp/Chap5.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap5.tex > Chap5.tex
	$(RM) /tmp/Chap5.tex

Chap5-1.tex : $(TR2TEX) $(MAN)/strings/?*.man
	$(TR2TEX)  -m $(MAN)/strings/?*.man > /tmp/Chap5-1.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap5-1.tex > Chap5-1.tex
	$(RM) /tmp/Chap5-1.tex

Chap5-2.tex : $(TR2TEX) $(MAN)/gui/?*.man
	$(TR2TEX)  -m $(MAN)/gui/?*.man > /tmp/Chap5-2.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap5-2.tex > Chap5-2.tex
	$(RM) /tmp/Chap5-2.tex

Chap5-3.tex : $(TR2TEX) $(MAN)/utilities/?*.man
	$(TR2TEX)  -m $(MAN)/utilities/?*.man > /tmp/Chap5-3.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap5-3.tex > Chap5-3.tex
	$(RM) /tmp/Chap5-3.tex

Chap5-4.tex : $(TR2TEX) $(MAN)/time-date/?*.man
	$(TR2TEX)  -m $(MAN)/time-date/?*.man > /tmp/Chap5-4.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap5-4.tex > Chap5-4.tex
	$(RM) /tmp/Chap5-4.tex

Chap6.tex : $(TR2TEX) $(MAN)/control/?*.man
	$(TR2TEX)  -m $(MAN)/control/?*.man > /tmp/Chap6.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap6.tex > Chap6.tex
	$(RM) /tmp/Chap6.tex

Chap7.tex : $(TR2TEX) $(MAN)/robust/?*.man
	$(TR2TEX)  -m $(MAN)/robust/?*.man > /tmp/Chap7.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap7.tex > Chap7.tex
	$(RM) /tmp/Chap7.tex

Chap8.tex : $(TR2TEX) $(MAN)/nonlinear/?*.man
	$(TR2TEX)  -m $(MAN)/nonlinear/?*.man > /tmp/Chap8.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap8.tex > Chap8.tex
	$(RM) /tmp/Chap8.tex

Chap9.tex : $(TR2TEX) $(MAN)/signal/?*.man
	$(TR2TEX)  -m $(MAN)/signal/?*.man > /tmp/Chap9.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap9.tex > Chap9.tex
	$(RM) /tmp/Chap9.tex

Chap10.tex : $(TR2TEX) $(MAN)/polynomials/?*.man
	$(TR2TEX)  -m $(MAN)/polynomials/?*.man > /tmp/Chap10.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap10.tex > Chap10.tex
	$(RM) /tmp/Chap10.tex

Chap11.tex : $(TR2TEX) $(MAN)/linear/?*.man
	$(TR2TEX)  -m $(MAN)/linear/?*.man > /tmp/Chap11.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap11.tex > Chap11.tex
	$(RM) /tmp/Chap11.tex

Chap12.tex : $(TR2TEX) $(MAN)/metanet/?*.man
	$(TR2TEX)  -m $(MAN)/metanet/?*.man > /tmp/Chap12.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap12.tex > Chap12.tex
	$(RM) /tmp/Chap12.tex

Chap13.tex : $(TR2TEX) $(MAN)/arma/?*.man
	$(TR2TEX)  -m $(MAN)/arma/?*.man > /tmp/Chap13.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap13.tex > Chap13.tex
	$(RM) /tmp/Chap13.tex

Chap14.tex : $(TR2TEX) $(MAN)/tdcs/?*.man
	$(TR2TEX)  -m $(MAN)/tdcs/?*.man > /tmp/Chap14.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap14.tex > Chap14.tex
	$(RM) /tmp/Chap14.tex

Chap15.tex : $(TR2TEX) $(MAN)/scicos/?*.man
	@echo "\section{Scicos editor}" > /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_menus.man >> /tmp/Chap15.tex
	@echo "\section{Blocks}" >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/?*_f.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scifunc_block.man >> /tmp/Chap15.tex
	@echo "\section{Data Structures}" >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_main.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_block.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_graphics.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_model.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_link.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicos_cpr.man >> /tmp/Chap15.tex
	@echo "\section{Useful Functions}" >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/standard_*.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/scicosim.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/curblock.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/getblocklabel.man >> /tmp/Chap15.tex
	$(TR2TEX)  -m $(MAN)/scicos/?*scicosvars.man >> /tmp/Chap15.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap15.tex > Chap15.tex
	$(RM) /tmp/Chap15.tex

Chap16.tex : $(TR2TEX) $(MAN)/sound/?*.man
	$(TR2TEX)  -m $(MAN)/sound/?*.man > /tmp/Chap16.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap16.tex > Chap16.tex
	$(RM) /tmp/Chap16.tex

Chap17.tex : $(TR2TEX) $(MAN)/dcd/?*.man
	$(TR2TEX)  -m $(MAN)/dcd/?*.man > /tmp/Chap17.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap17.tex > Chap17.tex
	$(RM) /tmp/Chap17.tex

Chap19.tex : $(TR2TEX) $(MAN)/tksci/?*.man
	$(TR2TEX)  -m $(MAN)/tksci/?*.man > /tmp/Chap19.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap19.tex > Chap19.tex
	$(RM) /tmp/Chap19.tex
Chap20.tex : $(TR2TEX) $(MAN)/translation/?*.man
	$(TR2TEX)  -m $(MAN)/translation/?*.man > /tmp/Chap20.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap20.tex > Chap20.tex
	$(RM) /tmp/Chap20.tex
Chap21.tex : $(TR2TEX) $(MAN)/pvm/?*.man $(MAN)/comm/?*.man 
	$(TR2TEX)  -m $(MAN)/pvm/?*.man > /tmp/Chap21.tex
	$(TR2TEX)  -m $(MAN)/comm/?*.man >> /tmp/Chap21.tex
	@sed -e 's/\\documentstyle/%\\documentstyle/' 			-e 's/\\end{document}//' -e 's/\\begin{document}//' /tmp/Chap21.tex > Chap21.tex
	$(RM) /tmp/Chap21.tex

clean::
	$(RM) *.aux *.log Chap*.tex *.dvi *.ind *.idx *.ilg  *.toc *.lot \
	Docu_pdf.* *.tmp Manual_pdf.ps Warnings *.haux *.hind *.html

distclean:: clean 
