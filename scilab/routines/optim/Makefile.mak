#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/optim.lib

OBJSC = sp.obj 

OBJSF =	ajour.obj bfgsd.obj calbx.obj calmaj.obj fmulb1.obj fmuls1.obj fpq2.obj \
	fremf1.obj gcbd.obj gcp.obj  majysa.obj majour.obj majz.obj \
	n1fc1.obj n1fc1a.obj ffinf1.obj fprf2.obj fremf2.obj n1fc1o.obj frdf1.obj \
	fuclid.obj nlis2.obj \
	n1gc2.obj n1gc2a.obj n1gc2b.obj n1qn1.obj n1qn1a.obj \
	proj.obj qnbd.obj rednor.obj relvar.obj rlbd.obj satur.obj shanph.obj optr03.obj \
	optr01.obj zgcbd.obj zqnbd.obj n1qn2.obj n1qn2a.obj strang.obj plcbas.obj \
	anfm01.obj anfm02.obj anfm03.obj anfm04.obj anfm05.obj anfm06.obj \
	anrs01.obj anrs02.obj \
	aux003.obj auxo01.obj opvf03.obj tol03.obj desr03.obj dimp03.obj nvkt03.obj dnrm0.obj \
	pasr03.obj dipvtf.obj icse.obj icse0.obj icse1.obj icse2.obj zthz.obj icscof.obj \
	icsei.obj icsec2.obj \
	dogleg.obj enorm.obj fdjac1.obj fdjac2.obj hybrd.obj hybrd1.obj hybrj.obj hybrj1.obj \
	dpmpar.obj lmder.obj lmdif.obj lmpar.obj qrsolv.obj \
	qform.obj qrfac.obj r1mpyq.obj r1updt.obj\
	fmc11z.obj fajc1.obj fretc1.obj fmani1.obj fcomp1.obj \
	fmlag1.obj nlis0.obj fcube.obj fmc11a.obj fmc11b.obj fmc11e.obj \
	n1qn3.obj n1qn3a.obj ddd2.obj dcube.obj ctonb.obj ctcab.obj ql0001.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

sp.obj: spd.h ../machine.h
