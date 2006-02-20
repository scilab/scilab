SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/pvm.a

OBJSC = pvm_grp.o pvm_proc_ctrl.o pvm_send.o  pvm_info.o \
        pvm_recv.o scipvmf77.o  varpack.o \
        intspvm_addhosts.o intspvm_barrier.o intspvm_bcast.o intspvm_bufinfo.o  \
        intspvm_config.o intspvm_delhosts.o intspvm_error.o intspvm_error_mode.o \
        intspvm_exit.o intspvm_f772sci.o intspvm_getinst.o intspvm_gettid.o \
        intspvm_get_timer.o intspvm_gsize.o intspvm_halt.o intspvm_joingroup.o \
        intspvm_kill.o intspvm_lvgroup.o intspvm_mytid.o intspvm_parent.o \
        intspvm_probe.o intspvm_recv.o intspvm_recv_var.o intspvm_reduce.o \
        intspvm_sci2f77.o intspvm_send.o intspvm_send_var.o intspvm_set_timer.o \
        intspvm_spawn.o intspvm_spawn_independent.o intspvm_start.o \
        intspvm_tasks.o intspvm_tidtohost.o matpvm.o	

OBJSF = mycmatptr.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)  -I${PVMINCLUDE}

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

mycmatptr.o: ../stack.h

matpvm.o: matpvm.c ../../pvm3/include/pvm3.h ../mex.h ../stack-c.h \
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

