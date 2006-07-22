SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/tclsci.a

OBJSF = 

OBJSC  = InitTclTk.o sci_close.o sci_findobj.o sci_opentk.o \
	 sci_TCL_DoOneEvent.o sci_TCL_EvalFile.o sci_TCL_EvalStr.o \
	 sci_TCL_gcf.o sci_get.o sci_TCL_GetVar.o gw_tclsci.o \
	 sci_set.o sci_TCL_SetVar.o ScilabEval.o TclEvents.o \
	 TclUiGet.o TclUiSet.o TCL_Global.o sci_TCL_GetVersion.o \
	 sci_TCL_UnsetVar.o sci_TCL_ExistVar.o sci_TCL_UpVar.o sci_TCL_DeleteInterp.o \
	 sci_TCL_ExistInterp.o sci_TCL_CreateSlave.o GedManagement.o sci_TCL_ExistArray.o \
	 tksynchro.o setenvctcl.o


include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib


