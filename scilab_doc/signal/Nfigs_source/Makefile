SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

.SUFFIXES: .code .ps .tex   $(SUFFIXES)

CHAP1 = flts1.tex flts2.tex plot1.tex plot2.tex plot3.tex  plot4.tex

CHAP2 = bode1.tex bode2.tex bode3.tex bode4.tex bode5.tex bode6.tex \
	bode7.tex group1.tex group2.tex group3.tex group4.tex \
	group5.tex group6.tex group7.tex group8.tex sample1.tex \
	sample2.tex sample3.tex sample4.tex sample5.tex intdec1.tex \
	intdec2.tex intdec3.tex intdec4.tex intdec5.tex intdec6.tex \
	intdec7.tex intdec8.tex intdec9.tex intdec10.tex fft1.tex \
	fft2.tex czt1.tex

CHAP3 = fir1.tex fir2.tex fir3.tex fir4.tex fir5.tex fir6.tex fir7.tex \
	fir8.tex fir9.tex fstyp121.tex fstyp122.tex remez1.tex \
	remez2.tex remez3.tex remez4.tex remez5.tex remez6.tex remez7.tex

CHAP4 = analog1.tex analog2.tex analog3.tex analog4.tex analog5.tex \
	analog6.tex analog7.tex analog8.tex analog9.tex analog10.tex \
	analog11.tex analog12.tex iir1.tex iir2.tex iir3.tex iir4.tex \
	iir5.tex eqiir4.tex

CHAP5 = spect1.tex spect2.tex spect3.tex spect4.tex mem1.tex mem2.tex \
	mem3.tex

CHAP6 = kf1.tex kf2.tex wf1.tex

CHAP7 = optiir1.tex optiir2.tex optiir3.tex optiir4.tex optiir5.tex \
	optfir1.tex optfir2.tex

CHAP9 = wigner1.tex

BLATEXPR = $(SCIDIR)/bin/Blatexpr

all:: chapters

chapters:
	@for i in chap1 chap2 chap3 chap4 chap5 chap6 chap7 chap9 ;\
	do \
	(echo "Figures for chapter $$i"; $(MAKE) $(MFLAGS) $$i); \
	done

chap1: $(CHAP1)

chap2: $(CHAP2)

chap3: $(CHAP3)

chap4: $(CHAP4)

chap5: $(CHAP5)

chap6: $(CHAP6)

chap7: $(CHAP7)

chap7:$(CHAP7)

chap9:$(CHAP9)

clean::
	$(RM) *.dia *.ps.n  *.ps d711.ps* *.tex 
distclean::
	$(RM) *.dia *.ps.n  *.ps d711.ps* *.tex  *.epsf

.code.tex:
	@$(SCIDIR)/util/scidoc $(SCIDIR) $*.code $*.dia
	@$(BLATEXPR) 1.0 1.0 $*.ps
	@$(RM) $*.ps

fir5.tex fir2.tex fir3.tex fir4.tex :  fir2_5.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) fir2_5.code \
		fir2_5.dia
	@$(BLATEXPR) 1.0 1.0 fir5.ps
	@$(BLATEXPR) 1.0 1.0 fir2.ps
	@$(BLATEXPR) 1.0 1.0 fir3.ps
	@$(BLATEXPR) 1.0 1.0 fir4.ps
	@$(RM) $*.ps

intdec1.tex intdec2.tex intdec3.tex intdec4.tex : intdec1_4.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) intdec1_4.code \
		intdec1_4.dia
	@$(BLATEXPR) 1.0 1.0 intdec1.ps
	@$(BLATEXPR) 1.0 1.0 intdec2.ps
	@$(BLATEXPR) 1.0 1.0 intdec3.ps
	@$(BLATEXPR) 1.0 1.0 intdec4.ps
	@$(RM) $*.ps

intdec5.tex intdec6.tex intdec7.tex intdec8.tex intdec9.tex intdec10.tex : intdec5_10.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) intdec5_10.code \
		intdec5_10.dia
	@$(BLATEXPR) 1.0 1.0 intdec5.ps
	@$(BLATEXPR) 1.0 1.0 intdec6.ps
	@$(BLATEXPR) 1.0 1.0 intdec7.ps
	@$(BLATEXPR) 1.0 1.0 intdec8.ps
	@$(BLATEXPR) 1.0 1.0 intdec9.ps
	@$(BLATEXPR) 1.0 1.0 intdec10.ps
	@$(RM) $*.ps

group1.tex group2.tex group3.tex group4.tex group5.tex :  group1_5.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) group1_5.code \
		group1_5.dia
	@$(BLATEXPR) 1.0 1.0 group1.ps
	@$(BLATEXPR) 1.0 1.0 group2.ps
	@$(BLATEXPR) 1.0 1.0 group3.ps
	@$(BLATEXPR) 1.0 1.0 group4.ps
	@$(BLATEXPR) 1.0 1.0 group5.ps
	@$(RM) $*.ps

group6.tex group7.tex group8.tex  : group6_8.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) group6_8.code \
		group6_8.dia
	@$(BLATEXPR) 1.0 1.0 group6.ps
	@$(BLATEXPR) 1.0 1.0 group7.ps
	@$(BLATEXPR) 1.0 1.0 group8.ps
	@$(RM) $*.ps

remez2.tex remez3.tex remez4.tex : remez2_4.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) remez2_4.code \
		remez2_4.dia
	@$(BLATEXPR) 1.0 1.0 remez2.ps
	@$(BLATEXPR) 1.0 1.0 remez3.ps
	@$(BLATEXPR) 1.0 1.0 remez4.ps
	@$(RM) $*.ps

remez6.tex remez7.tex remez5.tex : remez5_7.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) remez5_7.code \
		remez5_7.dia
	@$(BLATEXPR) 1.0 1.0 remez5.ps
	@$(BLATEXPR) 1.0 1.0 remez6.ps
	@$(BLATEXPR) 1.0 1.0 remez7.ps
	@$(RM) $*.ps

mem1.tex mem2.tex mem3.tex : mem1_3.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) mem1_3.code \
		mem1_3.dia
	@$(BLATEXPR) 1.0 1.0 mem1.ps
	@$(BLATEXPR) 1.0 1.0 mem2.ps
	@$(BLATEXPR) 1.0 1.0 mem3.ps
	@$(RM) $*.ps

iir2.tex iir3.tex  : iir2_3.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) iir2_3.code \
		iir2_3.dia
	@$(BLATEXPR) 1.0 1.0 iir2.ps
	@$(BLATEXPR) 1.0 1.0 iir3.ps
	@$(RM) $*.ps

spect2.tex spect3.tex spect4.tex  :  spect2_4.code
	@$(SCIDIR)/util/scidoc $(SCIDIR) spect2_4.code \
		spect2_4.dia
	@$(BLATEXPR) 1.0 1.0 spect2.ps
	@$(BLATEXPR) 1.0 1.0 spect3.ps
	@$(BLATEXPR) 1.0 1.0 spect4.ps
	@$(RM) $*.ps



