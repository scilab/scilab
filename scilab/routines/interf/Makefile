SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/interf.a

OBJSC = Interf.o ctest.o  cs2st.o getdate.o stack1.o stack2.o stack3.o \
	intlapack.o intslicot.o intarpack.o matdes.o intorand.o stcreate.o \
	intfilestat.o strelm-c.o  matdsr.o otherspecfun.o hmops.o \
	intinterp.o intsetenv.o intreadxls.o  dassl.o

OBJSF = lstelm.o lstelmi.o matelm.o matold.o lstops.o intl_e.o intl_i.o \
	matio.o intdeff.o intdiary.o intdisp.o intexec.o intexecstr.o \
	intgetf.o \
	intgetpid.o inthost.o intlib.o intprint.o intrat.o intread.o \
	intread4b.o \
	intreadb.o intwritb.o intwrite.o intwrite4b.o oldloadsave.o intfile.o \
	intgetenv.o intmgetl.o intgetio.o\
	matimp.o  matnew.o matode.o matops.o matopt.o intlsqrsolve.o \
	 matqz.o matric.o  matsys.o \
	polaut.o polelm.o polops.o strelm.o strops.o fmlelm.o \
	logic.o logelm.o xawelm.o misops.o stack0.o \
	where.o indxg.o defint.o \
	matodc.o dasrti.o \
	intg.o int2d.o int3d.o feval.o bva.o comm.o specfun.o \
	isany.o complexify.o issymmetric.o \
	followpath.o newsave.o insertfield.o v2unit.o v2cunit.o \
	hmcreate.o lapackf.o lapackf2.o fmexslicot.o fscislicot.o \
	intlinmeq.o hndlops.o


include ../../Makefile.incl

#CFLAGS = $(CC_OPTIONS) -DNODCD -DNOMETANET -DNOSCICOS -DNOSIGNAL -DNOSOUND -DNOSPARSE

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib



all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

# some versions of g77 on sparc will compile intmgetl.f
# but the code does not execute properly.
intmgetl.o: intmgetl.f ../stack.h
	$(FC) -g -c intmgetl.f -o intmgetl.o

bva.o: ../stack.h
comm.o: ../stack.h
complexify.o: ../stack.h
dasrti.o: ../stack.h
defint.o: ../stack.h
feval.o: ../stack.h
fmexslicot.o: ../stack.h
fmlelm.o: ../stack.h
followpath.o: ../stack.h
fscislicot.o: ../stack.h
hmcreate.o: ../stack.h
hndlops.o: ../stack.h
indxg.o: ../stack.h
insertfield.o: ../stack.h
int2d.o: ../stack.h
int3d.o: ../stack.h
intdeff.o: ../stack.h
intdiary.o: ../stack.h
intdisp.o: ../stack.h
intdpotrf.o: stack.h
intexec.o: ../stack.h
intexecstr.o: ../stack.h
intfile.o: ../stack.h
intg.o: ../stack.h
intgetenv.o: ../stack.h
intgetf.o: ../stack.h
intgetio.o: ../stack.h
intgetpid.o: ../stack.h
inthost.o: ../stack.h
intl_e.o: ../stack.h
intl_i.o: ../stack.h
intlib.o: ../stack.h
intlinmeq.o: ../stack.h
intlsqrsolve.o: ../stack.h
intmgetl.o: ../stack.h
intprint.o: ../stack.h
intrat.o: ../stack.h
intread.o: ../stack.h
intread4b.o: ../stack.h
intreadb.o: ../stack.h
intwritb.o: ../stack.h
intwrite.o: ../stack.h
intwrite4b.o: ../stack.h
intzpotrf.o: stack.h
isany.o: ../stack.h
issymmetric.o: ../stack.h
lapackf.o: ../stack.h
lapackf2.o: ../stack.h
logelm.o: ../stack.h
logic.o: ../stack.h
lstelm.o: ../stack.h

lstelmi.o: ../stack.h
lstops.o: ../stack.h
matelm.o: ../stack.h
matimp.o: ../stack.h
matio.o: ../stack.h
matnew.o: ../stack.h
matodc.o: ../stack.h
matode.o: ../stack.h
matold.o: ../stack.h
matops.o: ../stack.h
matopt.o: ../stack.h
matqz.o: ../stack.h
matric.o: ../stack.h
matsys.o: ../stack.h
misops.o: ../stack.h
newsave.o: ../stack.h
oldloadsave.o: ../stack.h
polaut.o: ../stack.h
polelm.o: ../stack.h
polops.o: ../stack.h
specfun.o: ../stack.h
stack0.o: ../stack.h
strelm.o: ../stack.h
strops.o: ../stack.h
v2cunit.o: ../stack.h
v2unit.o: ../stack.h
where.o: ../stack.h
xawelm.o: ../stack.h

Interf.o: Interf.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
ciargc.o: ciargc.c
cs2st.o: cs2st.c ../machine.h
ctest.o: ctest.c ../machine.h
getdate.o: getdate.c ../machine.h
intarpack.o: intarpack.c ../mex.h ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
intfilestat.o: intfilestat.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h ../sun/Sun.h
intlapack.o: intlapack.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
intorand.o: intorand.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
intslicot.o: intslicot.c ../mex.h ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
matdes.o: matdes.c ../graphics/bcg.h ../stack-c.h \
  ../graphics/CloneObjects.h ../graphics/Interaction.h ../graphics/SetProperty.h \
  ../graphics/GetProperty.h ../graphics/InitObjects.h ../graphics/DrawObjects.h \
  ../graphics/BuildObjects.h ../graphics/DestroyObjects.h \
  ../graphics/Math.h ../machine.h ../graphics/Graphics.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
  ../graphics/PloEch.h matdes.h
matdsr.o: matdsr.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h matdsr.h ../control/control.h \
  ../calelm/calelm.h
stack1.o: stack1.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h ../system/msgstore.h
stack2.o: stack2.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h ../sun/men_Sutils.h
stack3.o: stack3.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
stcreate.o: stcreate.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
strelm-c.o: strelm-c.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
structcreate.o: structcreate.c ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h

