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

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

intpvm.o: ../stack.h
lpack.o:../stack-c.h ../machine.h
pvm_grp.o: ../calelm/sci_tools.h ../machine.h
pvm_info.o: ../machine.h
pvm_proc_ctrl.o: ../machine.h
pvm_recv.o:../stack.h ../calelm/sci_tools.h ../machine.h
pvm_send.o:../stack.h ../calelm/sci_tools.h ../machine.h




