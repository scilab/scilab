#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/pvm.lib

OBJSC = pvm_grp.obj pvm_proc_ctrl.obj pvm_send.obj  pvm_info.obj \
	pvm_recv.obj scipvmf77.obj  varpack.obj intpvm.obj

OBJSF = mycmatptr.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(PVM_INCLUDES) -D__STDC__

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

intpvm.obj: ../stack.h
lpack.obj:../stack-c.h ../machine.h
pvm_grp.obj: ../calelm/sci_tools.h ../machine.h
pvm_info.obj: ../machine.h
pvm_proc_ctrl.obj: ../machine.h
pvm_recv.obj:../stack.h ../calelm/sci_tools.h ../machine.h
pvm_send.obj:../stack.h ../calelm/sci_tools.h ../machine.h




