#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/poly.lib

OBJSC =

OBJSF = bezout.obj blktit.obj\
	dimin.obj dmdsp.obj dmdspf.obj dmpad.obj dmpcnc.obj \
	dmpdsp.obj dmpext.obj dmpins.obj dmpmu.obj dmptld.obj dmptra.obj dmrdsp.obj \
	dpmul.obj dpmul1.obj dpodiv.obj dprxc.obj dpsimp.obj dwmpmu.obj fmt.obj \
	horner.obj idegre.obj impcnc.obj impext.obj impins.obj imptra.obj  \
	matra.obj mpdiag.obj mpinsp.obj mptri.obj \
	residu.obj sfact1.obj sfact2.obj strdsp.obj\
	wdmpad.obj wdmpmu.obj wmdsp.obj wmpad.obj wmpcnc.obj wmpins.obj \
	wmpmu.obj wmptld.obj wmptra.obj wpmul.obj wpmul1.obj wpodiv.obj wprxc.obj \
	mpdegr.obj dmp2pm.obj recbez.obj dmpadj.obj wmpadj.obj bezstp.obj \
	dmpcle.obj wmpcle.obj ddmpev.obj

OBJS = $(OBJSC) $(OBJSF)

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
