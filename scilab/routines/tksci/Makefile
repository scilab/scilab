SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/tksci.a

OBJSF = 

OBJSC  = tksci.o tksci_SciInterf.o \
	TK_ScilabCallback.o \
	LAB_TK_DoOneEvent.o \
	LAB_TK_EvalFile.o \
	LAB_TK_EvalStr.o \
	LAB_TK_GetVar.o \
	LAB_TK_SetVar.o \
	LAB_figure.o \
	LAB_findobj.o \
	LAB_get.o \
	LAB_opentk.o \
	LAB_set.o \
	LAB_uicontrol.o \
	LAB_close.o \
	LAB_uimenu.o \
	TK_uicontrol.o \
	LAB_gcf.o \
	gvar.o \
	LAB_setgvar.o LAB_getgvar.o\
	C-LAB_Interf.o


include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) -I. 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib


tksci.o: ../machine.h

all:: Makefile.mak

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

C-LAB_Interf.o: C-LAB_Interf.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  ../intersci/libinter.h
LAB_TK_DoOneEvent.o: LAB_TK_DoOneEvent.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
LAB_TK_EvalFile.o: LAB_TK_EvalFile.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h tksci.h
LAB_TK_EvalStr.o: LAB_TK_EvalStr.c tksci.h C-LAB_Interf.h ../machine.h \
  ../stack-def.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
LAB_TK_GetVar.o: LAB_TK_GetVar.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
LAB_TK_SetVar.o: LAB_TK_SetVar.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
LAB_close.o: LAB_close.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  tksci.h TK_uicontrol.h
LAB_figure.o: LAB_figure.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  tksci.h TK_uicontrol.h
LAB_findobj.o: LAB_findobj.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
LAB_gcf.o: LAB_gcf.c C-LAB_Interf.h ../machine.h ../stack-def.h tksci.h \
  TK_uicontrol.h
LAB_get.o: LAB_get.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
LAB_getgvar.o: LAB_getgvar.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  gvar.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
LAB_opentk.o: LAB_opentk.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_ScilabCallback.h tksci.h
LAB_set.o: LAB_set.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
LAB_setgvar.o: LAB_setgvar.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  gvar.h ../stack-c.h ../graphics/Math.h ../graphics/Graphics.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
LAB_uicontrol.o: LAB_uicontrol.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h TK_uicontrol.h
LAB_uimenu.o: LAB_uimenu.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  tksci.h TK_uicontrol.h
TK_ScilabCallback.o: TK_ScilabCallback.c ../machine.h TK_ScilabCallback.h \
  tksci.h C-LAB_Interf.h ../stack-def.h
TK_uicontrol.o: TK_uicontrol.c C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_uicontrol.h TK_ScilabCallback.h tksci.h
gvar.o: gvar.c gvar.h ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h C-LAB_Interf.h
tksci.o: tksci.c tksci.h C-LAB_Interf.h ../machine.h ../stack-def.h \
  TK_ScilabCallback.h
tksci_SciInterf.o: tksci_SciInterf.c C-LAB_Interf.h ../machine.h \
  ../stack-def.h tksci.h
