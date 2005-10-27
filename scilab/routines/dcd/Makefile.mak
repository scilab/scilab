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
	dbetrm.obj devlpl.obj dinvnr.obj dinvr.obj \
	dstrem.obj dt1.obj dzror.obj \
	erf.obj erfc1.obj esum.obj exparg.obj fpser.obj \
	gam1.obj gaminv.obj gamln.obj gamln1.obj gamma.obj grat1.obj gratio.obj gsumln.obj \
	ipmpar.obj psi.obj \
	rcomp.obj rexp.obj rlog.obj rlog1.obj \
	spmpar.obj stvaln.obj 
#dlngam.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

dinvr.obj: ../stack.h
dlanor.obj: ../stack.h
dstrem.obj: ../stack.h
ipmpar.obj: ../stack.h

DcdInterf.obj: DcdInterf.c ../../routines/stack-c.h \
	../../routines/graphics/Math.h ../../routines/machine.h \
	../../routines/graphics/Graphics.h ../../routines/stack-def.h \
	../../routines/interf/stack1.h ../../routines/interf/stack2.h \
	../../routines/interf/stack3.h
ipmpar1.obj: ipmpar1.c ../sparse/spConfig.h ../machine.h
