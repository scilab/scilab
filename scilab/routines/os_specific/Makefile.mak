# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/os_specific.lib

OBJSF = bashos.obj basin.obj basout.obj \
	clunit.obj ctrlc.obj dbasin.obj  \
	fgetarg.obj getpro.obj inibrk.obj sigbas.obj GetExceptionCode.obj

OBJSC  = getenvc.obj link.obj systemc.obj  csignal.obj getpidc.obj timer.obj \
	flags.obj men_Sutils.obj addinter.obj tmpdir.obj cluni0.obj \
	inffic.obj isanan.obj texmacs.obj getarg.obj setenvc.obj getmemory.obj \
	sci_mem_alloc.obj win_mem_alloc.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

include ../Make.lib.mak

bashos.obj: ../stack.h
basin.obj: ../stack.h
basout.obj: ../stack.h
clunit.obj: ../stack.h
dbasin.obj: ../stack.h
sigbas.obj: ../stack.h
sync.obj: ../stack.h

getenvc.obj: ../machine.h
link.obj: ../machine.h link_linux.c link_SYSV.c link_std.c link_W95.c
systemc.obj: ../machine.h
csignal.obj: ../machine.h
getpidc.obj: ../machine.h
timer.obj: ../machine.h
addinter.obj : addinter.h 
