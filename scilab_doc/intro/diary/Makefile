SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

.SUFFIXES: .code .dia $(SUFFIXES)

TESTS = d1p1.dia d1p2.dia d1p2bis.dia \
	d1p3.dia d1p4.dia d1p5.dia d1p6.dia d1p7.dia \
	d1p8.dia d1p9.dia d1p10.dia d1p11.dia d1p12.dia d1p13.dia \
	d1p14.dia d1p15.dia d1p16.dia d1p17.dia d1p18.dia d1p19.dia \
	d2.10.dia d2.10a.dia d2.11.dia d2.11a.dia d2.12.dia \
	d2.14.dia d2.15.dia d2.5.dia d2.6.dia d2.7.dia d2.8.dia d2.9.dia\
	d2.9a.dia d3.1.dia d3.10.dia d3.11.dia d3.12.dia d3.2.dia \
	d3.3.dia d3.4.dia d3.5.dia d3.6.dia d3.7.dia d3.8.dia d3.9.dia \
	d4.1.dia d4.2.dia d4.3.dia d4.4.dia d4.5.dia d4.6.dia d4.7.dia \
	d4.8.dia d5.1.dia d5.2.dia d5.3.dia d5.4.dia d5.5.dia d5.6.dia \
	d6.1.dia d1p1.dia d6.2.dia da.1.dia da.2.dia \
	da.3.dia connect.dia demo1.dia mulplot.dia simple.dia \
	syslin.dia extract.dia list.dia

all:: $(TESTS)

clean::
	$(RM) $(TESTS) foo.c foo.o

distclean::
	$(RM) $(TESTS) foo.c foo.o fig*.ps

.code.dia :
	@$(RM) x.dat
	@if [ $* != d4.7 -a $* != d1.15 ]; then \
		$(SCIDIR)/util/scidoc $(SCIDIR) $*.code $*.dia ;\
	else 	$(SCIDIR)/util/scidoc $(SCIDIR) $*.code $*.dia 1 ; \
	fi;
#	@echo "\begin{verbatim}" > begv
	@cat begv $*.dia > prov
#	@echo "\end{verbatim}" > endv
	@cat prov endv > $*.dia
	@$(RM) prov
#	@$(RM) begv endv
