#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/tksci.lib

OBJSF = 

OBJSC  = tksci.obj tksci_SciInterf.obj \
	TK_ScilabCallback.obj \
	LAB_TK_DoOneEvent.obj \
	LAB_TK_EvalFile.obj \
	LAB_TK_EvalStr.obj \
	LAB_TK_GetVar.obj \
	LAB_TK_SetVar.obj \
	LAB_figure.obj \
	LAB_findobj.obj \
	LAB_get.obj \
	LAB_opentk.obj \
	LAB_set.obj \
	LAB_uicontrol.obj \
	LAB_close.obj \
	LAB_uimenu.obj \
	TK_uicontrol.obj \
	LAB_gcf.obj \
	gvar.obj \
	LAB_setgvar.obj LAB_getgvar.obj\
	C-LAB_Interf.obj


include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak


tksci.obj: ../machine.h



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

C-LAB_Interf.obj: C-LAB_Interf.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  ../intersci/libinter.h
LAB_TK_DoOneEvent.obj: LAB_TK_DoOneEvent.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
LAB_TK_EvalFile.obj: LAB_TK_EvalFile.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h tksci.h
LAB_TK_EvalStr.obj: LAB_TK_EvalStr.c tksci.h C-LAB_Interf.h ../machine.h \
  ../stack-def.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
LAB_TK_GetVar.obj: LAB_TK_GetVar.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
LAB_TK_SetVar.obj: LAB_TK_SetVar.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
LAB_close.obj: LAB_close.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  tksci.h TK_uicontrol.h
LAB_figure.obj: LAB_figure.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  tksci.h TK_uicontrol.h
LAB_findobj.obj: LAB_findobj.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
LAB_gcf.obj: LAB_gcf.c C-LAB_Interf.h ../machine.h ../stack-def.h tksci.h \
  TK_uicontrol.h
LAB_get.obj: LAB_get.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
LAB_getgvar.obj: LAB_getgvar.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  gvar.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
LAB_opentk.obj: LAB_opentk.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_ScilabCallback.h tksci.h
LAB_set.obj: LAB_set.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
LAB_setgvar.obj: LAB_setgvar.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  gvar.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
LAB_uicontrol.obj: LAB_uicontrol.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h TK_uicontrol.h
LAB_uimenu.obj: LAB_uimenu.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  tksci.h TK_uicontrol.h
TK_ScilabCallback.obj: TK_ScilabCallback.c ../machine.h TK_ScilabCallback.h \
  tksci.h C-LAB_Interf.h ../stack-def.h
TK_uicontrol.obj: TK_uicontrol.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
gvar.obj: gvar.c gvar.h ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h C-LAB_Interf.h
tksci.obj: tksci.c tksci.h C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_ScilabCallback.h
tksci_SciInterf.obj: tksci_SciInterf.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
