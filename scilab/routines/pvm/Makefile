SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/pvm.a

OBJSC = pvm_grp.o pvm_proc_ctrl.o pvm_send.o  pvm_info.o \
	pvm_recv.o scipvmf77.o  varpack.o intpvm.o

OBJSF = mycmatptr.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)  -I${PVMINCLUDE}

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

mycmatptr.o: ../stack.h

intpvm.o: intpvm.c ../../pvm3/include/pvm3.h ../mex.h ../stack-c.h \
	../graphics/Math.h ../machine.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h sci_pvm.h
pvm_grp.o: pvm_grp.c ../../pvm3/include/pvm3.h ../machine.h \
	../calelm/sci_tools.h sci_pvm.h
pvm_info.o: pvm_info.c ../../pvm3/include/pvm3.h ../machine.h \
	../stack-c.h ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h sci_pvm.h
pvm_proc_ctrl.o: pvm_proc_ctrl.c ../../pvm3/include/pvm3.h ../machine.h \
	sci_pvm.h
pvm_recv.o: pvm_recv.c ../../pvm3/include/pvm3.h ../machine.h \
	../stack-c.h ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
	../calelm/sci_tools.h sci_pvm.h
pvm_send.o: pvm_send.c ../../pvm3/include/pvm3.h ../machine.h \
	../stack-c.h ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
	../calelm/sci_tools.h sci_pvm.h
sci_tools.o: sci_tools.c ../machine.h sci_tools.h sci_pvm.h
scipvmf77.o: scipvmf77.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h sci_pvm.h
varpack.o: varpack.c ../machine.h ../stack-c.h ../graphics/Math.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h sci_pvm.h

