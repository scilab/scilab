
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/tclsci.lib

OBJSF = 

OBJSC  = intTclDoOneEvent.obj intTclEvalFile.obj intTclEvalStr.obj \
         intTclGetVar.obj inttclsci.obj intTclSetVar.obj \
         TCL_Global.obj         

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak


tksci.obj: ../machine.h



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

