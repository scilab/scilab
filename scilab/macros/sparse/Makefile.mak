#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = sparselib
NAM = SCI/macros/sparse

MACROS = pcg.sci gmres.sci

include ../Make.lib.mak
