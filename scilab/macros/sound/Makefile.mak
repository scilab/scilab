#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = soundlib
NAM = SCI/macros/sound

MACROS = analyze.sci auread.sci lin2mu.sci mapsound.sci mu2lin.sci \
	playsnd.sci sound.sci soundsec.sci wavread.sci auwrite.sci \
	wavwrite.sci

include ../Make.lib.mak
