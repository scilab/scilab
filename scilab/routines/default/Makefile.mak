SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

EXAMPLES= Ex-colnew.obj Ex-corr.obj  Ex-feval.obj  Ex-fsolve.obj  Ex-impl.obj  Ex-intg.obj \
	Ex-ode-more.obj  Ex-ode.obj  Ex-odedc.obj  Ex-optim.obj  Ex-schur.obj  Ex-fort.obj \
	Ex-dasrt.obj  Ex-dassl.obj  Ex-fbutn.obj  Ex-int2d.obj  Ex-int3d.obj


OBJS =  FTables.obj $(EXAMPLES) mainsci.obj matusr.obj matus2.obj Funtab.obj  msgstxt.obj \
	scimem.obj  callinterf.obj mexlib.obj

# jpc win32 
OBJS =  FTables.obj $(EXAMPLES) mainwin95.obj matusr.obj matus2.obj Funtab.obj  msgstxt.obj \
	scimem.obj callinterf.obj mexlib.obj

all:: $(OBJS)

FTables.obj : FTables.h 

FTables.h : Flist 
	./FCreate

Funtab.obj : fundef 

$(EXAMPLES) : ../stack.h 

distclean::
	del *.obj

# symbols 

# A partial def file (without headers) for the default objects 
# to build a scilex.def 

DEF=../../libs/default.def 

all:: $(DEF)

$(DEF) : $(OBJS) 
	@echo Creation of $(DEF)
	@..\..\bin\dumpexts.exe -o $(DEF) -n scilex.dll $(OBJS)


matus2.obj matusr.obj msgstxt.obj user2.obj :   ../stack.h
callinterf.obj : ../machine.h  callinterf.h
