SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

.SUFFIXES: .dia .code .ps .tex $(SUFFIXES)

CHAP1 = rand1.dia  poly1.dia  poly2.dia  poly3.dia  poly4.dia  \
	Sflts2.dia Splot1.dia Splot2.dia Splot3.dia Splot4.dia \
	sysrep1.dia dscr1.dia

CHAP2 =  Sbode5.dia Sbode6.dia Sbode7.dia Sfft2.dia group1.dia  \
	group2.dia  group3.dia  fft1.dia  convol2.dia  czt2.dia  \
	czt3.dia  czt4.dia  czt5.dia  

CHAP3 = fstyp121.dia  fstyp122.dia  remez3.dia  remez4.dia  \
	remez5.dia  remez6.dia  eqfir1.dia  eqfir2.dia  

CHAP4 = Sanalog1.dia Sanalog2.dia Sanalog3.dia Sanalog5.dia \
	Sanalog6.dia Sanalog7.dia Sanalog8.dia Sanalog12.dia \
	butt.dia  cheb2.dia  iir6.dia  iir7.dia  iir8.dia  \
	iir9.dia  iir10.dia  eqiir1.dia  eqiir2.dia  eqiir3.dia  \
	Seqiir4.dia

CHAP5 = pspect1.dia  

CHAP6 = Skf1.dia Skf2.dia kf3.dia  kf4.dia  kf5.dia Swf1.dia

CHAP7 = optiir2.dia  optiir1.dia  optiir3.dia  optiir4.dia  optiir5.dia  

CHAP8 = spfact.dia simul1.dia simul3.dia

CHAP9 = wigner1.dia

all:: chapters

chapters:
	@for i in chap1 chap2 chap3 chap4 chap5 chap6 chap7 chap8 chap9 ;\
	do \
	(echo "Diary for chapter $$i"; $(MAKE) $(MFLAGS) $$i); \
	done

chap1: $(CHAP1)

chap2: $(CHAP2)

chap3: $(CHAP3)

chap4: $(CHAP4)

chap5: $(CHAP5)

chap6: $(CHAP6)

chap7: $(CHAP7)

chap8: $(CHAP8)

chap9: $(CHAP9)

.code.dia :
	@$(SCIDIR)/util/scidoc $(SCIDIR) $*.code $*.dia

clean::
	$(RM) *.dia
distclean::
	$(RM) *.dia

spfact2.dia spfact4.dia spfact3.dia spfact1.dia: spfactg.dia

spfactg.dia:
	@$(RM) spfact[1-4].dia
	@$(SCIDIR)/util/scidoc $(SCIDIR) spfact.code spfact.dia
	@ echo " " > spfactg.dia
