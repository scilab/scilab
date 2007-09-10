#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/interf.lib

OBJSC = Interf.obj ctest.obj  cs2st.obj getdate.obj stack1.obj stack2.obj stack3.obj \
	intlapack.obj intslicot.obj intarpack.obj matdes.obj intorand.obj stcreate.obj \
	intfilestat.obj strelm-c.obj  matdsr.obj otherspecfun.obj hmops.obj \
	intinterp.obj intsetenv.obj intreadxls.obj dassl.obj intqld.obj intgetmemory.obj\
	intUImenu.obj intgraphics.obj intgget.obj intgset.obj intshowalluimenushandles.obj\
	IsEqualVar.obj

OBJSF = lstelm.obj lstelmi.obj matelm.obj matold.obj lstops.obj intl_e.obj intl_i.obj \
	matio.obj intdeff.obj intdiary.obj intdisp.obj intexec.obj intexecstr.obj \
	intgetf.obj \
	intgetpid.obj inthost.obj intlib.obj intprint.obj intrat.obj intread.obj \
	intread4b.obj \
	intreadb.obj intwritb.obj intwrite.obj intwrite4b.obj oldloadsave.obj intfile.obj \
	intgetenv.obj intmgetl.obj intgetio.obj\
	matimp.obj  matnew.obj matode.obj matops.obj matopt.obj intlsqrsolve.obj \
	matqz.obj matric.obj  matsys.obj \
	polaut.obj polelm.obj polops.obj strelm.obj strops.obj fmlelm.obj \
	logic.obj logelm.obj xawelm.obj misops.obj stack0.obj \
	where.obj indxg.obj defint.obj \
	matodc.obj dasrti.obj \
	intg.obj int2d.obj int3d.obj feval.obj bva.obj comm.obj specfun.obj \
	isany.obj complexify.obj issymmetric.obj \
	followpath.obj newsave.obj insertfield.obj v2unit.obj v2cunit.obj \
	hmcreate.obj lapackf.obj lapackf2.obj fmexslicot.obj fscislicot.obj \
	intlinmeq.obj hndlops.obj


include ../../Makefile.incl.mak

#CFLAGS = $(CC_OPTIONS) -DNODCD -DNOMETANET -DNOSCICOS -DNOSIGNAL -DNOSOUND -DNOSPARSE

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

bva.obj: ../stack.h
comm.obj: ../stack.h
complexify.obj: ../stack.h
dasrti.obj: ../stack.h
defint.obj: ../stack.h
feval.obj: ../stack.h
fmexslicot.obj: ../stack.h
fmlelm.obj: ../stack.h
followpath.obj: ../stack.h
fscislicot.obj: ../stack.h
hmcreate.obj: ../stack.h
hndlops.obj: ../stack.h
indxg.obj: ../stack.h
insertfield.obj: ../stack.h
int2d.obj: ../stack.h
int3d.obj: ../stack.h
intdeff.obj: ../stack.h
intdiary.obj: ../stack.h
intdisp.obj: ../stack.h
intdpotrf.obj: stack.h
intexec.obj: ../stack.h
intexecstr.obj: ../stack.h
intfile.obj: ../stack.h
intg.obj: ../stack.h
intgetenv.obj: ../stack.h
intgetf.obj: ../stack.h
intgetio.obj: ../stack.h
intgetpid.obj: ../stack.h
inthost.obj: ../stack.h
intl_e.obj: ../stack.h
intl_i.obj: ../stack.h
intlib.obj: ../stack.h
intlinmeq.obj: ../stack.h
intlsqrsolve.obj: ../stack.h
intmgetl.obj: ../stack.h
intprint.obj: ../stack.h
intrat.obj: ../stack.h
intread.obj: ../stack.h
intread4b.obj: ../stack.h
intreadb.obj: ../stack.h
intwritb.obj: ../stack.h
intwrite.obj: ../stack.h
intwrite4b.obj: ../stack.h
intzpotrf.obj: stack.h
isany.obj: ../stack.h
issymmetric.obj: ../stack.h
lapackf.obj: ../stack.h
lapackf2.obj: ../stack.h
logelm.obj: ../stack.h
logic.obj: ../stack.h
lstelm.obj: ../stack.h

lstelmi.obj: ../stack.h
lstops.obj: ../stack.h
matelm.obj: ../stack.h
matimp.obj: ../stack.h
matio.obj: ../stack.h
matnew.obj: ../stack.h
matodc.obj: ../stack.h
matode.obj: ../stack.h
matold.obj: ../stack.h
matops.obj: ../stack.h
matopt.obj: ../stack.h
matqz.obj: ../stack.h
matric.obj: ../stack.h
matsys.obj: ../stack.h
misops.obj: ../stack.h
newsave.obj: ../stack.h
oldloadsave.obj: ../stack.h
polaut.obj: ../stack.h
polelm.obj: ../stack.h
polops.obj: ../stack.h
specfun.obj: ../stack.h
stack0.obj: ../stack.h
strelm.obj: ../stack.h
strops.obj: ../stack.h
v2cunit.obj: ../stack.h
v2unit.obj: ../stack.h
where.obj: ../stack.h
xawelm.obj: ../stack.h

Interf.obj: Interf.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
ciargc.obj: ciargc.c
cs2st.obj: cs2st.c ../machine.h
ctest.obj: ctest.c ../machine.h
getdate.obj: getdate.c ../machine.h
intarpack.obj: intarpack.c ../mex.h ../stack-c.h ../graphics/Math.h \
	../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
intfilestat.obj: intfilestat.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h ../os_specific/Os_specific.h
intlapack.obj: intlapack.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
intorand.obj: intorand.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
intslicot.obj: intslicot.c ../mex.h ../stack-c.h ../graphics/Math.h \
	../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
matdes.obj: matdes.c ../graphics/bcg.h ../stack-c.h \
	../graphics/CloneObjects.h ../graphics/Interaction.h ../graphics/SetProperty.h \
	../graphics/GetProperty.h ../graphics/InitObjects.h ../graphics/DrawObjects.h \
	../graphics/BuildObjects.h ../graphics/DestroyObjects.h \
	../graphics/Math.h ../machine.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
	../graphics/PloEch.h matdes.h
matdsr.obj: matdsr.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h matdsr.h ../control/control.h \
	../calelm/calelm.h
stack1.obj: stack1.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h ../system/msgstore.h
stack2.obj: stack2.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h ../os_specific/men_Sutils.h
stack3.obj: stack3.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
stcreate.obj: stcreate.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
strelm-c.obj: strelm-c.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
structcreate.obj: structcreate.c ../stack-c.h ../graphics/Math.h \
	../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
IsEqualVar.obj: ../stack.h IsEqualVar.h

