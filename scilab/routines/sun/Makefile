# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..

LIBRARY = $(SCIDIR)/libs/sun.a

OBJSF = bashos.o basin.o basout.o \
	clunit.o ctrlc.o dbasin.o  \
	fgetarg.o getpro.o inibrk.o sigbas.o 

OBJSC  = getenvc.o link.o systemc.o zzledt.o  csignal.o getpidc.o timer.o \
	flags.o men_Sutils.o addinter.o h_help_data.o tmpdir.o cluni0.o \
	inffic.o isanan.o  

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

bashos.o: ../stack.h
basin.o: ../stack.h
basout.o: ../stack.h
clunit.o: ../stack.h
dbasin.o: ../stack.h
sigbas.o: ../stack.h
sync.o: ../stack.h

getenvc.o: ../machine.h
link.o: ../machine.h link_linux.c link_SYSV.c link_std.c link_W95.c
systemc.o: ../machine.h
zzledt.o: ../machine.h
csignal.o: ../machine.h
getpidc.o: ../machine.h
timer.o: ../machine.h
addinter.o : addinter.h 
isanan.o:	../machine.h
