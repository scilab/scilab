SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/interf.a

OBJSC = Interf.o ctest.o  cs2st.o getdate.o stack1.o stack2.o stack3.o

OBJSF = lstelm.o lstelmi.o  matdes.o matdsc.o matdsr.o matelm.o\
	lstops.o intl_e.o intl_i.o \
	matio.o intdeff.o intdiary.o intdisp.o intexec.o intexecstr.o intgetf.o \
	intgetpid.o inthost.o intlib.o intprint.o intrat.o intread.o intread4b.o \
	intreadb.o intwritb.o intwrite.o intwrite4b.o oldloadsave.o intfile.o \
	intgetenv.o intmgetl.o intfilestat.o intgetio.o\
	matimp.o matlu.o matnew.o matode.o matops.o matopt.o\
	matqr.o matqz.o matric.o matsvd.o matsys.o \
	polaut.o polelm.o polops.o strelm.o strops.o fmlelm.o\
	logic.o logelm.o xawelm.o misops.o stack0.o \
	where.o indxg.o defint.o\
	matodc.o dasrti.o\
	intg.o int2d.o int3d.o feval.o bva.o comm.o specfun.o\
	isany.o \
	followpath.o newsave.o insertfield.o v2unit.o v2cunit.o \
	hmcreate.o


include ../../Makefile.incl

#CFLAGS = $(CC_OPTIONS) -DNODCD -DNOMETANET -DNOSCICOS -DNOSIGNAL -DNOSOUND -DNOSPARSE

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

bva.o: ../stack.h
comm.o: ../stack.h
coselm.o: ../stack.h
dasrti.o: ../stack.h
defint.o: ../stack.h
feval.o: ../stack.h
fmlelm.o: ../stack.h
indxg.o: ../stack.h
intclear.o: ../stack.h
intcos.o: ../stack.h
intg.o: ../stack.h
int2d.o: ../stack.h
int3d.o: ../stack.h
logelm.o: ../stack.h
logic.o: ../stack.h
lspops.o: ../stack.h
lstelm.o: ../stack.h
lstops.o: ../stack.h
matdes.o: ../stack.h ../stack-c.h
matdsc.o: ../stack.h
matdsr.o: ../stack.h
matelm.o: ../stack.h
matimp.o: ../stack.h
matio.o: ../stack.h
matlu.o: ../stack.h
matnew.o: ../stack.h
matodc.o: ../stack.h
matode.o: ../stack.h
matops.o: ../stack.h
matopt.o: ../stack.h
matqr.o: ../stack.h
matqz.o: ../stack.h
matric.o: ../stack.h
matsvd.o: ../stack.h
matsys.o: ../stack.h
metane.o: ../stack.h
misops.o: ../stack.h
polaut.o: ../stack.h
polelm.o: ../stack.h
polops.o: ../stack.h
sigelm.o: ../stack.h
specfun.o: ../stack.h
spelm.o: ../stack.h
spops.o: ../stack.h
stack.o: ../stack.h
strelm.o: ../stack.h
strops.o: ../stack.h
where.o: ../stack.h
xawelm.o: ../stack.h
stack1.o: ../callinter.h
intwhat.o: ../stack.h
intwrite4b.o: ../stack.h
intread4b.o: ../stack.h
intdeff.o : ../stack.h
intdiary.o : ../stack.h
intdisp.o : ../stack.h
intexec.o : ../stack.h
intexecstr.o : ../stack.h
intgetf.o : ../stack.h
intgetpid.o : ../stack.h
inthost.o : ../stack.h
intlib.o : ../stack.h
intprint.o : ../stack.h
intrat.o : ../stack.h
intread.o : ../stack.h
intread4b.o: ../stack.h
intreadb.o : ../stack.h
intwritb.o : ../stack.h
intwrite.o : ../stack.h
intwrite4b.o : ../stack.h
oldloadsave.o : ../stack.h
intfile.o : ../stack.h
intgetenv.o : ../stack.h
intmgetl.o: ../stack.h
stack0.o stack1.o stack2.o: ../stack.h
getdate.o: ../machine.h

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
