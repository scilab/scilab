#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = tdcslib
NAM = SCI/macros/tdcs

MACROS=	artest.sci  boucle.sci comric.sci  \
	exemple.sci  finit.sci  gaincom.sci gainobs.sci gpeche.sci  \
	logr.sci  lotest.sci mineInit.sci  obscont1.sci \
	obsric.sci  portrait.sci portr3d.sci recur.sci  tangent.sci \
	tdinit.sci  test_d.sci chaintest.sci bifish.sci systems.sci \
	BInit.sci

include ../Make.lib.mak
