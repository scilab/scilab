#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/system.lib


OBJSC = System.obj System2.obj Calelm.obj Sun.obj Intersci.obj Blas.obj Lapack.obj \
	scicurdir.obj readline.obj msgstore.obj storeversion.obj cvstr.obj \
	inisci-c.obj diary.obj  

OBJSF = allops.obj  banier.obj\
	clause.obj comand.obj compcl.obj   \
	defmat.obj eqid.obj error.obj showstack.obj expr.obj fact.obj funs.obj \
	getch.obj  getlin.obj getnum.obj getstr.obj getsym.obj \
	getval.obj inisci.obj \
	logops.obj macro.obj mname.obj nextj.obj parse.obj findequal.obj print.obj \
	prompt.obj putid.obj  run.obj savlod.obj  stackg.obj stackgl.obj \
	ref2val.obj stackp.obj terme.obj  scirun.obj \
	majmin.obj whatln.obj \
	seteol.obj setlnb.obj skpins.obj msgs.obj prntid.obj \
	cvname.obj  compil.obj ptover.obj ptrback.obj \
	isbrk.obj  bexec.obj scilines.obj \
	mrknmd.obj mkindx.obj mklist.obj cmdstr.obj setgetmode.obj\
	typ2cod.obj israt.obj funnam.obj dspdsp.obj lspdsp.obj wspdsp.obj \
	xerbla.obj algebre.obj storeglobal.obj setippty.obj allowptr.obj copyvar.obj \
	lst2vars.obj createref.obj getfunction.obj varfunptr.obj tksynchro.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

allops.obj:  ../stack.h
bexec.obj:  ../stack.h
clause.obj:  ../stack.h
cmdstr.obj:  ../stack.h
comand.obj:  ../stack.h
compcl.obj:  ../stack.h
compil.obj:  ../stack.h
copyvar.obj:  ../stack.h
createref.obj:  ../stack.h
cvname.obj:  ../stack.h
defmat.obj:  ../stack.h
diary.obj:  ../stack.h
eqid.obj:  ../stack.h
error.obj:  ../stack.h
expr.obj:  ../stack.h
fact.obj:  ../stack.h
findequal.obj:  ../stack.h
funnam.obj:  ../stack.h
funs.obj:  ../stack.h
getch.obj:  ../stack.h
getfunction.obj:  ../stack.h
getlin.obj:  ../stack.h
getnum.obj:  ../stack.h
getstr.obj:  ../stack.h
getsym.obj:  ../stack.h
getval.obj:  ../stack.h
inisci.obj:  ../stack.h
israt.obj:  ../stack.h
logops.obj:  ../stack.h
lst2vars.obj:  ../stack.h
macro.obj:  ../stack.h
mkindx.obj:  ../stack.h
mklist.obj:  ../stack.h
mname.obj:  ../stack.h
mrknmd.obj:  ../stack.h
msgs.obj:  ../stack.h
nextj.obj:  ../stack.h
parse.obj:  ../stack.h
print.obj:  ../stack.h
prntid.obj:  ../stack.h
prompt.obj:  ../stack.h
ptover.obj:  ../stack.h
ptrback.obj:  ../stack.h
putid.obj:  ../stack.h
ref2val.obj:  ../stack.h
run.obj:  ../stack.h
savlod.obj:  ../stack.h
scilines.obj:  ../stack.h
scirun.obj:  ../stack.h ../callinter.h
seteol.obj:  ../stack.h
setgetmode.obj:  ../stack.h
setlnb.obj:  ../stack.h
showstack.obj:  ../stack.h
skpins.obj:  ../stack.h
stackg.obj:  ../stack.h
stackgl.obj:  ../stack.h
stackp.obj:  ../stack.h
storeglobal.obj:  ../stack.h
terme.obj:  ../stack.h
typ2cod.obj:  ../stack.h
varfunptr.obj:  ../stack.h
whatln.obj:  ../stack.h
xerbla.obj:  ../stack.h

Blas.obj: Blas.c ../machine.h
Calelm.obj: Calelm.c ../machine.h
Intersci.obj: Intersci.c ../machine.h
Lapack.obj: Lapack.c ../machine.h
Sun.obj: Sun.c ../machine.h
System.obj: System.c ../machine.h
System2.obj: System2.c ../machine.h
banier.obj: banier.c ../machine.h ../version.h
cvstr.obj: cvstr.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
inisci-c.obj: inisci-c.c ../machine.h ../sun/Sun.h
msgstore.obj: msgstore.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h msgstore.h
readline.obj: readline.c ../machine.h
scicurdir.obj: scicurdir.c ../machine.h
storeversion.obj: storeversion.c ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h ../version.h
tksynchro.obj: tksynchro.c ../machine.h ../tksci/tksci.h \
  ../tksci/C-LAB_Interf.h ../stack-def.h
