#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/rand.lib

OBJSC = Rand.obj mt.obj kiss.obj clcg2.obj clcg4.obj urand.obj igngeom.obj fsultra.obj

OBJSF =  genbet.obj genchi.obj genexp.obj genf.obj gengam.obj genmn.obj genmul.obj gennch.obj gennf.obj gennor.obj genprm.obj ignbin.obj ignnbn.obj ignpoi.obj lennob.obj phrtsd.obj sdot.obj setgmn.obj sexpo.obj sgamma.obj snorm.obj spofa.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

genf.obj:      ../stack.h
gennf.obj:      ../stack.h
setgmn.obj: ../stack.h

Rand.obj: Rand.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h grand.h clcg4.h \
	others_generators.h
clcg2.obj: clcg2.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
clcg4.obj: clcg4.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
	clcg4.h
fsultra.obj: fsultra.c others_generators.h ../graphics/Math.h \
	../machine.h ../graphics/Graphics.h
igngeom.obj: igngeom.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
kiss.obj: kiss.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
mt.obj: mt.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h grand.h
urand.obj: urand.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h


