#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/dcd.lib

OBJSC = DcdInterf.obj ipmpar1.obj

OBJSF = algdiv.obj alngam.obj alnrel.obj apser.obj \
	basym.obj bcorr.obj betaln.obj bfrac.obj bgrat.obj bpser.obj bratio.obj brcmp1.obj brcomp.obj \
	bup.obj \
	cdfbet.obj cdfbin.obj cdfchi.obj cdfchn.obj cdff.obj cdffnc.obj cdfgam.obj cdfnbn.obj cdfnor.obj \
	cdfpoi.obj cdft.obj cumbet.obj cumbin.obj cumchi.obj cumchn.obj cumf.obj cumfnc.obj cumgam.obj \
	cumnbn.obj cumnor.obj cumpoi.obj cumt.obj \
	dbetrm.obj devlpl.obj dexpm1.obj dinvnr.obj dinvr.obj dlanor.obj dln1mx.obj dln1px.obj \
	dlnbet.obj dlngam.obj dstrem.obj dt1.obj dzror.obj \
	erf.obj erfc1.obj esum.obj exparg.obj fpser.obj \
	gam1.obj gaminv.obj gamln.obj gamln1.obj gamma.obj grat1.obj gratio.obj gsumln.obj \
	ipmpar.obj psi.obj \
	rcomp.obj rexp.obj rlog.obj rlog1.obj \
	spmpar.obj \
	stvaln.obj 


include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak


ipmpar.obj : ../stack.h
DcdInterf.obj : ../stack-c.h



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
