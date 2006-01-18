SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/tclsci.a

OBJSF = 

OBJSC  = InitTclTk.o intClose.o intFindObj.o intOpenTk.o \
	 intTclDoOneEvent.o intTclEvalFile.o intTclEvalStr.o \
	 intTclGcf.o intTclGet.o intTclGetVar.o inttclsci.o \
	 intTclSet.o intTclSetVar.o ScilabEval.o TclEvents.o \
	 TclUiGet.o TclUiSet.o TCL_Global.o intTclGetVersion.o \
	 intTclUnsetVar.o intTclExistVar.o intTclUpVar.o intTclDeleteInterp.o \
	 intTclExistInterp.o intTclCreateSlave.o GedManagement.o


include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib


