#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/control.lib

ARL2= arl2.obj arl2a.obj phi.obj \
	outl2.obj jacl2.obj feq.obj optml2.obj deg1l2.obj degl2.obj storl2.obj calsca.obj \
	hessl2.obj tild.obj mzdivq.obj domout.obj front.obj  dzdivq.obj \
	scapol.obj onface.obj watfac.obj modul.obj lq.obj rootgp.obj 

OBJSC =

OBJSF =  bezous.obj cerr.obj coef.obj dclmat.obj dexpm1.obj \
	dfrmg.obj dhetr.obj expan.obj find.obj folhp.obj fout.obj \
	invtpl.obj irow1.obj irow2.obj lrow2.obj lybad.obj \
	lybsc.obj lydsr.obj pade.obj polmc.obj \
	proj2.obj qzk.obj ricd.obj rilac.obj ssxmc.obj sszer.obj  \
	sybad.obj sydsr.obj syhsc.obj wcerr.obj wclmat.obj \
	wdegre.obj wesidu.obj wexpm1.obj wpade.obj rtitr.obj $(ARL2) \
	dgbfa.obj dgbsl.obj dgeco.obj dgedi.obj dgefa.obj dgesl.obj dlslv.obj \
	dpofa.obj dqrdc.obj dqrsl.obj dqrsm.obj drref.obj dsvdc.obj giv.obj hhdml.obj \
	shrslv.obj wgeco.obj wgedi.obj wgefa.obj wgesl.obj wlslv.obj wpofa.obj wqrdc.obj \
	wqrsl.obj wrref.obj wsvdc.obj ereduc.obj \
	balanc.obj balbak.obj bdiag.obj calcsc.obj cbal.obj comqr3.obj \
	corth.obj cortr.obj dsubsp.obj exch.obj exchqz.obj fxshfr.obj hqror2.obj htribk.obj \
	htridi.obj imtql3.obj inva.obj newest.obj nextk.obj orthes.obj ortran.obj \
	qhesz.obj qitz.obj quad.obj quadit.obj quadsd.obj qvalz.obj qvecz.obj \
	realit.obj scaleg.obj reduc2.obj \
	rpoly.obj split.obj tql2.obj tred2.obj wbalin.obj wbdiag.obj wexchn.obj \
	wshrsl.obj fstair.obj riccpack.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

# according to compiler, do not optimize the following files

#hqror2.obj: hqror2.f
#	$(FC)  -c hqror2.f -o hqror2.obj
#
#comqr3.obj: comqr3.f
#	$(FC)  -c comqr3.f -o comqr3.obj
#
#tql2.obj: tql2.f
#	$(FC)  -c tql2.f -o tql2.obj
#
#imtql3.obj: imtql3.f
#	$(FC)  -c imtql3.f -o imtql3.obj
#
#dsvdc.obj: dsvdc.f
#	$(FC)  -c dsvdc.f -o dsvdc.obj
#
#wsvdc.obj: wsvdc.f
#	$(FC)  -c wsvdc.f -o wsvdc.obj
#
#pade.obj: pade.f
#	$(FC)  -c pade.f -o pade.obj
