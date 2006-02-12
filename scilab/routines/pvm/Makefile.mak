#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/pvm.lib

OBJSC = pvm_grp.obj pvm_proc_ctrl.obj pvm_send.obj  pvm_info.obj \
        pvm_recv.obj scipvmf77.obj  varpack.obj \
        intspvm_addhosts.obj intspvm_barrier.obj intspvm_bcast.obj intspvm_bufinfo.obj  \
        intspvm_config.obj intspvm_delhosts.obj intspvm_error.obj intspvm_error_mode.obj \
        intspvm_exit.obj intspvm_f772sci.obj intspvm_getinst.obj intspvm_gettid.obj \
        intspvm_get_timer.obj intspvm_gsize.obj intspvm_halt.obj intspvm_joingroup.obj \
        intspvm_kill.obj intspvm_lvgroup.obj intspvm_mytid.obj intspvm_parent.obj \
        intspvm_probe.obj intspvm_recv.obj intspvm_recv_var.obj intspvm_reduce.obj \
        intspvm_sci2f77.obj intspvm_send.obj intspvm_send_var.obj intspvm_set_timer.obj \
        intspvm_spawn.obj intspvm_spawn_independent.obj intspvm_start.obj \
        intspvm_tasks.obj intspvm_tidtohost.obj matpvm.obj

OBJSF = mycmatptr.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(PVM_INCLUDES) -D__STDC__

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

