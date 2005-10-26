SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/dcd.a

OBJSC = DcdInterf.o ipmpar1.o

OBJSF = algdiv.o alngam.o alnrel.o apser.o \
	basym.o bcorr.o betaln.o bfrac.o bgrat.o bpser.o bratio.o brcmp1.o brcomp.o \
	bup.o \
	cdfbet.o cdfbin.o cdfchi.o cdfchn.o cdff.o cdffnc.o cdfgam.o cdfnbn.o cdfnor.o \
	cdfpoi.o cdft.o cumbet.o cumbin.o cumchi.o cumchn.o cumf.o cumfnc.o cumgam.o \
	cumnbn.o cumnor.o cumpoi.o cumt.o \
	dbetrm.o devlpl.o dinvnr.o dinvr.o \
	dstrem.o dt1.o dzror.o \
	erf.o erfc1.o esum.o exparg.o fpser.o \
	gam1.o gaminv.o gamln.o gamln1.o gamma.o grat1.o gratio.o gsumln.o \
	ipmpar.o psi.o \
	rcomp.o rexp.o rlog.o rlog1.o \
	spmpar.o stvaln.o 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

dinvr.o: ../stack.h
dlanor.o: ../stack.h
dstrem.o: ../stack.h
ipmpar.o: ../stack.h

DcdInterf.o: DcdInterf.c ../../routines/stack-c.h \
	../../routines/graphics/Math.h ../../routines/machine.h \
	../../routines/graphics/Graphics.h ../../routines/stack-def.h \
	../../routines/interf/stack1.h ../../routines/interf/stack2.h \
	../../routines/interf/stack3.h
ipmpar1.o: ipmpar1.c ../sparse/spConfig.h ../machine.h
