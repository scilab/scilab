SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/system.a


OBJSC = System.o System2.o Calelm.o Sun.o Intersci.o Blas.o Lapack.o \
	scicurdir.o readline.o msgstore.o storeversion.o cvstr.o \
	inisci-c.o diary.o  

OBJSF = allops.o  banier.o\
	clause.o comand.o compcl.o   \
	defmat.o eqid.o error.o showstack.o expr.o fact.o funs.o \
	getch.o  getlin.o getnum.o getstr.o getsym.o \
	getval.o inisci.o \
	logops.o macro.o mname.o nextj.o parse.o findequal.o print.o \
	prompt.o putid.o  run.o savlod.o  stackg.o stackgl.o \
	ref2val.o stackp.o terme.o  scirun.o \
	majmin.o whatln.o \
	seteol.o setlnb.o skpins.o msgs.o prntid.o \
	cvname.o  compil.o ptover.o ptrback.o \
	isbrk.o  bexec.o scilines.o \
	mrknmd.o mkindx.o mklist.o cmdstr.o setgetmode.o\
	typ2cod.o israt.o funnam.o dspdsp.o lspdsp.o wspdsp.o \
	xerbla.o algebre.o storeglobal.o setippty.o allowptr.o copyvar.o \
	lst2vars.o createref.o getfunction.o varfunptr.o tksynchro.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

allops.o:  ../stack.h
bexec.o:  ../stack.h
clause.o:  ../stack.h
cmdstr.o:  ../stack.h
comand.o:  ../stack.h
compcl.o:  ../stack.h
compil.o:  ../stack.h
copyvar.o:  ../stack.h
createref.o:  ../stack.h
cvname.o:  ../stack.h
defmat.o:  ../stack.h
diary.o:  ../stack.h
eqid.o:  ../stack.h
error.o:  ../stack.h
expr.o:  ../stack.h
fact.o:  ../stack.h
findequal.o:  ../stack.h
funnam.o:  ../stack.h
funs.o:  ../stack.h
getch.o:  ../stack.h
getfunction.o:  ../stack.h
getlin.o:  ../stack.h
getnum.o:  ../stack.h
getstr.o:  ../stack.h
getsym.o:  ../stack.h
getval.o:  ../stack.h
inisci.o:  ../stack.h
israt.o:  ../stack.h
logops.o:  ../stack.h
lst2vars.o:  ../stack.h
macro.o:  ../stack.h
mkindx.o:  ../stack.h
mklist.o:  ../stack.h
mname.o:  ../stack.h
mrknmd.o:  ../stack.h
msgs.o:  ../stack.h
nextj.o:  ../stack.h
parse.o:  ../stack.h
print.o:  ../stack.h
prntid.o:  ../stack.h
prompt.o:  ../stack.h
ptover.o:  ../stack.h
ptrback.o:  ../stack.h
putid.o:  ../stack.h
ref2val.o:  ../stack.h
run.o:  ../stack.h
savlod.o:  ../stack.h
scilines.o:  ../stack.h
scirun.o:  ../stack.h ../callinter.h
seteol.o:  ../stack.h
setgetmode.o:  ../stack.h
setlnb.o:  ../stack.h
showstack.o:  ../stack.h
skpins.o:  ../stack.h
stackg.o:  ../stack.h
stackgl.o:  ../stack.h
stackp.o:  ../stack.h
storeglobal.o:  ../stack.h
terme.o:  ../stack.h
typ2cod.o:  ../stack.h
varfunptr.o:  ../stack.h
whatln.o:  ../stack.h
xerbla.o:  ../stack.h

Blas.o: Blas.c ../machine.h
Calelm.o: Calelm.c ../machine.h
Intersci.o: Intersci.c ../machine.h
Lapack.o: Lapack.c ../machine.h
Sun.o: Sun.c ../machine.h
System.o: System.c ../machine.h
System2.o: System2.c ../machine.h
banier.o: banier.c ../machine.h ../version.h
cvstr.o: cvstr.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
inisci-c.o: inisci-c.c ../machine.h ../sun/Sun.h
msgstore.o: msgstore.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h msgstore.h
readline.o: readline.c ../machine.h
scicurdir.o: scicurdir.c ../machine.h
storeversion.o: storeversion.c ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h ../version.h
tksynchro.o: tksynchro.c ../machine.h ../tksci/tksci.h \
  ../tksci/C-LAB_Interf.h ../stack-def.h
