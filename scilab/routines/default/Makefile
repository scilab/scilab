# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh

SCIDIR=../..
include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

EXAMPLES= Ex-colnew.o Ex-corr.o  Ex-feval.o  Ex-fsolve.o  Ex-impl.o  \
	Ex-intg.o \
	Ex-ode-more.o  Ex-ode.o  Ex-odedc.o  Ex-optim.o  Ex-schur.o  \
	Ex-fort.o \
	Ex-dasrt.o  Ex-dassl.o  Ex-fbutn.o Ex-int2d.o Ex-int3d.o

OBJS =  FTables.o $(EXAMPLES) mainsci.o matusr.o matus2.o Funtab.o  \
	msgstxt.o scimem.o callinterf.o mexlib.o

all:: $(OBJS)

FTables.h : Flist 
	./FCreate

distclean::
	$(RM) *.o

#=============================== dependencies 

Ex-fort.o Ex-ode.o Ex-odedc.o msgstxt.o matus2.o matusr.o userf2.o : ../stack.h
$(EXAMPLES) : ../stack.h 

#=======================generated with gcc======================

callinterf.o: callinterf.c ../machine.h ../sun/addinter.h callinterf.h
FTables.o: FTables.c FTables0.h ../machine.h FTables.h ../stack-c.h \
  ../graphics/Math.h ../graphics/Graphics.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h ../sun/link.h
Funtab.o: Funtab.c ../machine.h fundef
mexlib.o: mexlib.c ../mex.h ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
scimem.o: scimem.c ../machine.h
