SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/sound.a

OBJSC = sound.o raw.o wav.o  sox.o libst.o misc.o fileio.o soundI.o

OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

$(OBJSC):	st.h 


fileio.o: fileio.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h ../sun/Sun.h
libst.o: libst.c libst.h
misc.o: misc.c st.h wav.h ../machine.h
raw.o: raw.c st.h wav.h ../machine.h libst.h
sound.o: sound.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  st.h wav.h
soundI.o: soundI.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h sox.h wav.h
sox.o: sox.c st.h wav.h ../machine.h sox.h
sprintf_ptr.o: sprintf_ptr.c ../machine.h
wav.o: wav.c st.h wav.h ../machine.h
