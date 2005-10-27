SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

EXAMPLES= Ex-colnew.obj Ex-corr.obj  Ex-feval.obj  Ex-fsolve.obj  Ex-impl.obj  Ex-intg.obj \
	Ex-ode-more.obj  Ex-ode.obj  Ex-odedc.obj  Ex-optim.obj  Ex-schur.obj  Ex-fort.obj \
	Ex-dasrt.obj  Ex-dassl.obj  Ex-fbutn.obj  Ex-int2d.obj  Ex-int3d.obj \
	Ex-lsqrsolve.obj

OBJS =  FTables.obj $(EXAMPLES) mainsci.obj matusr.obj matus2.obj Funtab.obj  msgstxt.obj \
	scimem.obj  callinterf.obj mexlib.obj

# jpc win32 
OBJS =  FTables.obj $(EXAMPLES) mainwin95.obj matusr.obj matus2.obj Funtab.obj  msgstxt.obj \
	scimem.obj callinterf.obj mexlib.obj

all:: $(OBJS)

FTables.obj : FTables.h 

FTables.h : Flist 

Funtab.obj : fundef 

$(EXAMPLES) : ../stack.h 

clean::
	-del *.obj

distclean::
	-del *.obj

# symbols 

# A partial def file (without headers) for the default objects 
# to build a scilex.def 

DEF=../../libs/default.def 

all:: $(DEF)

$(DEF) : $(OBJS) 
	@echo Creation of $(DEF)
	@..\..\bin\dumpexts.exe -o $(DEF) -n scilex.dll $(OBJS)

matus2.obj matusr.obj msgstxt.obj user2.obj :   ../stack.h
callinterf.obj: callinterf.c ../machine.h ../os_specific/addinter.h callinterf.h
FTables.obj: FTables.c FTables0.h ../machine.h FTables.h ../stack-c.h \
	../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
	../interf/stack1.h ../interf/stack2.h ../interf/stack3.h ../os_specific/link.h
Funtab.obj: Funtab.c ../machine.h fundef ../tclsci/fundef.tclsci
mexlib.obj: mexlib.c ../mex.h ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
scimem.obj: scimem.c ../machine.h
