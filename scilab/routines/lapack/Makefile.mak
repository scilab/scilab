#-----------------------------
# the vc++ version is not generated
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/lapack.lib

OBJSC =

# ATLAS symbol must remain empty 
ATLAS= 
# LOCAL symbol must contain a list of local routines which are 
# also present in ATLAS  
# the following LOCAL variable is valid for ATLAS-3.2.1 

LOCAL= dgesv.obj dgetrf.obj dgetrs.obj dpotrs.obj dpotrf.obj  zgetrf.obj zgetrs.obj zpotrf.obj

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
	dgerfs.obj dlaswp.obj dorgr2.obj dormrq.obj dormr2.obj dlapmt.obj \
	dlansy.obj dgees.obj dgehrd.obj dorghr.obj dhseqr.obj dtrsen.obj dlahrd.obj dgehd2.obj \
	dlahqr.obj dlanhs.obj dlarfx.obj dtrexc.obj dtrsyl.obj dlanv2.obj dlaexc.obj dlaln2.obj \
	dlasy2.obj dladiv.obj dgeesx.obj dgesvx.obj dgeequ.obj dlaqge.obj dgecon.obj \
	dgetf2.obj dsytrf.obj dlasyf.obj dsytf2.obj dsytri.obj dlasv2.obj dtrti2.obj dlapy3.obj \
	dgelsx.obj dgetri.obj dtrtri.obj dgebrd.obj dorgbr.obj dbdsqr.obj dlabrd.obj dgebd2.obj \
	dorglq.obj dlasq1.obj dlas2.obj dorgl2.obj dlasq2.obj dlasq3.obj dlasq4.obj dormhr.obj \
	dormql.obj dgeqlf.obj  dorgql.obj dorgtr.obj dsytrd.obj dsyev.obj dsytrs.obj \
	dsycon.obj dpocon.obj dgghrd.obj dhgeqz.obj dtgevc.obj dlag2.obj dgges.obj \
	dgegs.obj dgesvd.obj dormbr.obj zlacpy.obj zgeqrf.obj zgeqpf.obj zungqr.obj zgeqr2.obj \
	zlarft.obj zlarfb.obj zlarfg.obj zung2r.obj zunm2r.obj zlarf.obj zlacgv.obj zladiv.obj \
	dgelsy.obj dormrz.obj dtrtrs.obj dtzrzf.obj dgeqp3.obj dlarzb.obj \
	dlarzt.obj dlatrz.obj dormr3.obj dlaqp2.obj dlaqps.obj dlarz.obj dgelss.obj \
	zgeqp3.obj zlaic1.obj zlaqp2.obj zlaqps.obj zunmqr.obj \
	dsysv.obj zgees.obj zgesvd.obj zgetri.obj zlange.obj zlascl.obj zlaset.obj \
	zbdsqr.obj zgebak.obj zgebal.obj zgebrd.obj zgehrd.obj zgelqf.obj zgetf2.obj zhseqr.obj \
	zlassq.obj zlaswp.obj ztrsen.obj ztrtri.obj zungbr.obj zunghr.obj zunglq.obj zunmbr.obj \
	zdrot.obj zgebd2.obj zgehd2.obj zgelq2.obj zlabrd.obj zlacon.obj zlahqr.obj \
	zlahrd.obj zlanhs.obj \
	zlarfx.obj zlasr.obj ztrexc.obj ztrsyl.obj ztrti2.obj zungl2.obj zunmlq.obj \
	dzsum1.obj izmax1.obj zlartg.obj zrot.obj zunml2.obj zgelsy.obj ztzrzf.obj \
	zunmrz.obj zlarzb.obj zlarzt.obj zlatrz.obj zunmr3.obj zlarz.obj \
	dgeev.obj zgeev.obj dtrevc.obj ztrevc.obj zlatrs.obj zgecon.obj zdrscl.obj \
	zpotf2.obj  zheev.obj zhetrd.obj zlanhe.obj zsteqr.obj \
	zungtr.obj zhetd2.obj zlatrd.obj zungql.obj zung2l.obj dggev.obj zggev.obj \
	zggbak.obj zggbal.obj zgghrd.obj zhgeqz.obj ztgevc.obj zgges.obj ztgsen.obj \
	ztgexc.obj ztgsyl.obj ztgex2.obj ztgsy2.obj zgesc2.obj zgetc2.obj zlatdf.obj \
	$(LOCAL) 

include ../../Makefile.incl.mak

CFLAGS = -D__MSC__ -DWIN32 -c -DSTRICT -D__MAKEFILEVC__ -nologo $(INCLUDES) $(DTK) $(DPVM) $(DMKL) $(USE_MT) -Z7 -W3 -Os -G5 -Gd

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
