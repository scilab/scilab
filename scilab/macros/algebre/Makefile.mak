#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = alglib
NAM = SCI/macros/algebre

MACROS = coff.sci colcomp.sci companion.sci rref.sci cond.sci pinv.sci sva.sci\
	rank.sci givens.sci im_inv.sci randpencil.sci \
	proj.sci rowcomp.sci pen2ea.sci kroneck.sci quaskro.sci \
	spaninter.sci spanplus.sci sqroot.sci trace.sci kernel.sci \
	fullrf.sci fullrfk.sci projspec.sci penlaur.sci glever.sci \
	rowshuff.sci polar.sci orth.sci pencan.sci linsolve.sci \
	pbig.sci psmall.sci range.sci nlev.sci spantwo.sci aff2ab.sci\
	householder.sci chfact.sci chsolve.sci genmarkov.sci classmarkov.sci \
	eigenmarkov.sci norm.sci\
	gspec.sci gschur.sci  diff.sci
include ../Make.lib.mak
