SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/system.a

OBJSC = System.o System2.o Calelm.o Sun.o Intersci.o Blas.o Lapack.o helpap.o \
	scicurdir.o

OBJSF = allops.o  banier.o sascii.o \
	clause.o comand.o compcl.o   \
	defmat.o eqid.o error.o showstack.o expr.o fact.o funs.o \
	getch.o  getlin.o getnum.o getstr.o getsym.o \
	getval.o helpmg.o inisci.o \
	logops.o macro.o mname.o nextj.o parse.o print.o \
	prompt.o putid.o  run.o savlod.o  stackg.o stackgl.o \
	ref2val.o stackp.o terme.o xchar.o  scirun.o \
	majmin.o apropo.o whatln.o \
	seteol.o setlnb.o skpins.o msgs.o prntid.o \
	cvname.o cvstr.o compil.o ptover.o ptrback.o \
	isbrk.o scilab.o sciquit.o bexec.o scilines.o \
	mrknmd.o mkindx.o mklist.o cmdstr.o setgetmode.o\
	typ2cod.o israt.o funnam.o dspdsp.o lspdsp.o wspdsp.o \
	xerbla.o algebre.o storeglobal.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

include ../Make.lib

all:: Makefile.mak

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

include Make.banier

System.o: ../machine.h
System2.o: ../machine.h
Calelm.o: ../machine.h
Sun.o: ../machine.h
Intersci.o: ../machine.h
Blas.o: ../machine.h

allops.o: ../stack.h
apropo.o: ../stack.h
bexec.o: ../stack.h
clause.o: ../stack.h
cmdstr.o: ../stack.h
comand.o: ../stack.h
compcl.o: ../stack.h
compil.o: ../stack.h
cvname.o: ../stack.h
cvstr.o: ../stack.h
defmat.o: ../stack.h
eqid.o: ../stack.h
error.o: ../stack.h
expr.o: ../stack.h
fact.o: ../stack.h
funnam.o: ../stack.h
funs.o: ../stack.h
getch.o: ../stack.h
getlin.o: ../stack.h
getnum.o: ../stack.h
getstr.o: ../stack.h
getsym.o: ../stack.h
getval.o: ../stack.h
helpmg.o: ../stack.h
inisci.o: ../stack.h
israt.o: ../stack.h
logops.o: ../stack.h
macro.o: ../stack.h
matla0.o: ../stack.h ../machine.h
mkindx.o: ../stack.h
mklist.o: ../stack.h
mname.o: ../stack.h
mrknmd.o: ../stack.h
msgs.o: ../stack.h
msize.o: ../stack.h
nextj.o: ../stack.h
parse.o: ../stack.h
print.o: ../stack.h
prntid.o: ../stack.h
prompt.o: ../stack.h
ptover.o: ../stack.h
ptrback.o: ../stack.h
putid.o: ../stack.h
run.o: ../stack.h
sascii.o: ../stack.h
savlod.o: ../stack.h
scilines.o: ../stack.h
sciquit.o: ../stack.h
scirun.o: ../stack.h ../machine.h ../callinter.h
seteol.o: ../stack.h
setgetmode.o: ../stack.h
setlnb.o: ../stack.h
skpins.o: ../stack.h
stackg.o: ../stack.h
stackp.o: ../stack.h
terme.o: ../stack.h
typ2cod.o: ../stack.h
whatln.o: ../stack.h
xchar.o: ../stack.h
refval2.o: ../stack.h
xerbla.o: ../stack.h

