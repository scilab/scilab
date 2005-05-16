#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = elemlib
NAM = SCI/macros/elem

MACROS = acosm.sci acosh.sci acoshm.sci asinm.sci \
	asinh.sci asinhm.sci atanh.sci atanhm.sci cosh.sci coshm.sci\
	cotg.sci coth.sci cothm.sci log10.sci sinh.sci sinhm.sci \
	tanm.sci tanh.sci tanhm.sci log2.sci\
	logm.sci  atanm.sci cosm.sci signm.sci sinm.sci sqrtm.sci\
	null.sci fix.sci smooth.sci squarewave.sci sinc.sci\
	toeplitz.sci pertrans.sci \
	modulo.sci pmodulo.sci interpln.sci \
	intl.sci intc.sci integrate.sci \
	erf.sci erfc.sci erfcx.sci binomial.sci \
	speye.sci spzeros.sci sprand.sci \
	intsplin.sci inttrap.sci GLoad.sci \
	isinf.sci isnan.sci \
	lex_sort.sci unique.sci union.sci setdiff.sci intersect.sci \
	isempty.sci  nextpow2.sci convertindex.sci vectorfind.sci \
	ind2sub.sci sub2ind.sci interp1.sci isvector.sci cell2mat.sci cellstr.sci
include ../Make.lib.mak
