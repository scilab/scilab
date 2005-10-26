SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/rand.a

OBJSC = Rand.o mt.o kiss.o clcg2.o clcg4.o urand.o igngeom.o fsultra.o

OBJSF =  genbet.o genchi.o genexp.o genf.o gengam.o genmn.o genmul.o gennch.o gennf.o gennor.o genprm.o ignbin.o ignnbn.o ignpoi.o lennob.o phrtsd.o sdot.o setgmn.o sexpo.o sgamma.o snorm.o spofa.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

genf.o:      ../stack.h
gennf.o:      ../stack.h
setgmn.o: ../stack.h

Rand.o: Rand.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h grand.h clcg4.h \
	others_generators.h
clcg2.o: clcg2.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
clcg4.o: clcg4.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
	clcg4.h
fsultra.o: fsultra.c others_generators.h ../graphics/Math.h \
	../machine.h ../graphics/Graphics.h
igngeom.o: igngeom.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h
kiss.o: kiss.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
mt.o: mt.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h grand.h
urand.o: urand.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h


