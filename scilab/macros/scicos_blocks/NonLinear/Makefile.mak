#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = scsnonlinearlib
NAM = SCI/macros/scicos_blocks/NonLinear

MACROS= ABSBLK_f.sci ABS_VALUE.sci COSBLK_f.sci   DLRADAPT_f.sci  \
	EXPBLK_f.sci FSV_f.sci INTRP2BLK_f.sci INTRPLBLK_f.sci INVBLK_f.sci \
	LOGBLK_f.sci LOOKUP_f.sci MAXMIN.sci MAX_f.sci MIN_f.sci POWBLK_f.sci \
	PRODUCT.sci PROD_f.sci QUANT_f.sci   SATURATION.sci SAT_f.sci SIGNUM.sci \
	SINBLK_f.sci TANBLK_f.sci TrigFun.sci
                   
 

include ../../Make.lib.mak
