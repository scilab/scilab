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

mycmatptr.obj: ../stack.h

intpvm.obj: intpvm.c ../../pvm3/include/pvm3.h ../mex.h ../stack-c.h \
	../graphics/Math.h ../machine.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h sci_pvm.h
pvm_grp.obj: pvm_grp.c ../../pvm3/include/pvm3.h ../machine.h \
	../calelm/sci_tools.h sci_pvm.h
pvm_info.obj: pvm_info.c ../../pvm3/include/pvm3.h ../machine.h \
	../stack-c.h ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h sci_pvm.h
pvm_proc_ctrl.obj: pvm_proc_ctrl.c ../../pvm3/include/pvm3.h ../machine.h \
	sci_pvm.h
pvm_recv.obj: pvm_recv.c ../../pvm3/include/pvm3.h ../machine.h \
	../stack-c.h ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
	../calelm/sci_tools.h sci_pvm.h
pvm_send.obj: pvm_send.c ../../pvm3/include/pvm3.h ../machine.h \
	../stack-c.h ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
	../calelm/sci_tools.h sci_pvm.h
sci_tools.obj: sci_tools.c ../machine.h sci_tools.h sci_pvm.h
scipvmf77.obj: scipvmf77.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h sci_pvm.h
varpack.obj: varpack.c ../machine.h ../stack-c.h ../graphics/Math.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h sci_pvm.h

