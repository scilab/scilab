#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = texmacslib
NAM = SCI/macros/texmacs

MACROS = %b_p.sci %c_p.sci %i_p.sci %l_p.sci %p_p.sci \
	%r_p.sci %s_p.sci addmenus.sci  plotout.sci \
	texout.sci 

include ../Make.lib.mak
