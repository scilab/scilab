#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = polylib
NAM = SCI/macros/calpol

MACROS = cmndred.sci coffg.sci colcompr.sci \
	denom.sci derivat.sci determ.sci detr.sci \
	diophant.sci gcd.sci hermit.sci horner.sci \
	hrmt.sci htrianr.sci invr.sci lcm.sci \
	numer.sci pdiv.sci pol2des.sci \
	pol2str.sci polfact.sci routh_t.sci rowcompr.sci \
	sylm.sci systmat.sci pfactors.sci factors.sci lcmdiag.sci \
	inv_coeff.sci

include ../Make.lib.mak
