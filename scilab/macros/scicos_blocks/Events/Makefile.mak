#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = scseventslib
NAM = SCI/macros/scicos_blocks/Events

MACROS= ANDBLK.sci CLKSOM_f.sci  HALT_f.sci MFCLCK_f.sci \
	ANDLOG_f.sci CLKSPLIT_f.sci EVTDLY_f.sci IFTHEL_f.sci TRASH_f.sci \
	CLKSOMV_f.sci CLOCK_f.sci EVTGEN_f.sci MCLOCK_f.sci freq_div.sci EVTVARDLY.sci

include ../../Make.lib.mak
