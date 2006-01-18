SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/tclsci.lib

OBJSF = 

OBJSC  = InitTclTk.obj intClose.obj intFindObj.obj intOpenTk.obj \
	 intTclDoOneEvent.obj intTclEvalFile.obj intTclEvalStr.obj \
	 intTclGcf.obj intTclGet.obj intTclGetVar.obj inttclsci.obj \
	 intTclSet.obj intTclSetVar.obj ScilabEval.obj TclEvents.obj \
	 TclUiGet.obj TclUiSet.obj TCL_Global.obj intTclGetVersion.obj \
	 intTclUnsetVar.obj intTclExistVar.obj intTclUpVar.obj intTclDeleteInterp.obj \
	 intTclExistInterp.obj intTclCreateSlave.obj GedManagement.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak


