#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/lapack.lib

OBJSC =

OBJSF = dgelqf.obj dgels.obj  dgeqrf.obj dlae2.obj dlaev2.obj \
	dlarf.obj dlarfg.obj dlartg.obj dlascl.obj dlaset.obj dlasr.obj dlasrt.obj \
	dlassq.obj dopgtr.obj dorg2l.obj dorg2r.obj dormqr.obj dpptrf.obj dspev.obj \
	dspgst.obj dspgv.obj  dsptrd.obj dsptrf.obj dorgqr.obj \
	dsteqr.obj  lsame.obj dtrcon.obj dgebal.obj dgebak.obj \
	ilaenv.obj dlamch.obj dlabad.obj dlange.obj dormlq.obj dlansp.obj \
	dsterf.obj dgeqr2.obj dlarft.obj dlarfb.obj dorm2r.obj dgelq2.obj dlanst.obj \
	dlapy2.obj dlantr.obj dlacon.obj dlatrs.obj drscl.obj dorml2.obj \
	slamch.obj dggbal.obj dggbak.obj dlacpy.obj dlaic1.obj dgeqpf.obj \
	dlatzm.obj dtzrqf.obj dgerqf.obj dorgrq.obj dgerq2.obj \
	dgerfs.obj dgetrs.obj dlaswp.obj dorgr2.obj dormrq.obj dormr2.obj dlapmt.obj \
	dlansy.obj dgees.obj dgehrd.obj dorghr.obj dhseqr.obj dtrsen.obj dlahrd.obj dgehd2.obj \
	dlahqr.obj dlanhs.obj dlarfx.obj dtrexc.obj dtrsyl.obj dlanv2.obj dlaexc.obj dlaln2.obj \
	dlasy2.obj dladiv.obj dgeesx.obj dgesvx.obj dgeequ.obj dlaqge.obj dgetrf.obj dgecon.obj \
	dgetf2.obj dsytrf.obj dlasyf.obj dsytf2.obj dsytri.obj dlasv2.obj dtrti2.obj dlapy3.obj \
	dgelsx.obj dgetri.obj dtrtri.obj dgebrd.obj dorgbr.obj dbdsqr.obj dlabrd.obj dgebd2.obj \
	dorglq.obj dlasq1.obj dlas2.obj dorgl2.obj dlasq2.obj dlasq3.obj dlasq4.obj dormhr.obj \
	dormql.obj dgeqlf.obj dpotrs.obj dorgql.obj dorgtr.obj dsytrd.obj dsyev.obj dsytrs.obj \
	dsycon.obj dpocon.obj dpotrf.obj dgghrd.obj dhgeqz.obj dtgevc.obj dlag2.obj dgges.obj \
	dgegs.obj dgesvd.obj dormbr.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
