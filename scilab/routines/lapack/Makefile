SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/lapack.a

OBJSC =

OBJSF = dgelqf.o dgels.o  dgeqrf.o dlae2.o dlaev2.o \
	dlarf.o dlarfg.o dlartg.o dlascl.o dlaset.o dlasr.o dlasrt.o \
	dlassq.o dopgtr.o dorg2l.o dorg2r.o dormqr.o dpptrf.o dspev.o \
	dspgst.o dspgv.o  dsptrd.o dsptrf.o dorgqr.o \
	dsteqr.o  lsame.o dtrcon.o dgebal.o dgebak.o \
	ilaenv.o dlamch.o dlabad.o dlange.o dormlq.o dlansp.o \
	dsterf.o dgeqr2.o dlarft.o dlarfb.o dorm2r.o dgelq2.o dlanst.o \
	dlapy2.o dlantr.o dlacon.o dlatrs.o drscl.o dorml2.o \
	slamch.o dggbal.o dggbak.o dlacpy.o dlaic1.o dgeqpf.o \
	dlatzm.o dtzrqf.o dgerqf.o dorgrq.o dgerq2.o \
	dgerfs.o dgetrs.o dlaswp.o dorgr2.o dormrq.o dormr2.o dlapmt.o \
	dlansy.o dgees.o dgehrd.o dorghr.o dhseqr.o dtrsen.o dlahrd.o dgehd2.o \
	dlahqr.o dlanhs.o dlarfx.o dtrexc.o dtrsyl.o dlanv2.o dlaexc.o dlaln2.o \
	dlasy2.o dladiv.o dgeesx.o dgesvx.o dgeequ.o dlaqge.o dgetrf.o dgecon.o \
	dgetf2.o dsytrf.o dlasyf.o dsytf2.o dsytri.o dlasv2.o dtrti2.o dlapy3.o \
	dgelsx.o dgetri.o dtrtri.o dgebrd.o dorgbr.o dbdsqr.o dlabrd.o dgebd2.o \
	dorglq.o dlasq1.o dlas2.o dorgl2.o dlasq2.o dlasq3.o dlasq4.o dormhr.o \
	dormql.o dgeqlf.o dpotrs.o dorgql.o dorgtr.o dsytrd.o dsyev.o dsytrs.o \
	dsycon.o dpocon.o dpotrf.o dgghrd.o dhgeqz.o dtgevc.o dlag2.o dgges.o \
	dgegs.o dgesvd.o dormbr.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
