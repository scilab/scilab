SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/fileio.a

RIPOLE= bt-int.o bytedecoders.o logger.o ole.o olestream-unwrap.o pldstr.o ripole.o

OBJSC = sound.o raw.o wav.o  sox.o libst.o misc.o fileio.o soundI.o \
	xls.o $(RIPOLE)

OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

fileio.o: fileio.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
	../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
	../interf/stack3.h ../os_specific/Os_specific.h
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

xls.o: ../machine.h

#ripole-0.1.4 files 
bt-int.o: bt-int.c bt-int.h
logger.o: logger.c logger.h
ole.o: ole.c logger.h pldstr.h bt-int.h bytedecoders.h olestream-unwrap.h ole.h
olestream-unwrap.o : olestream-unwrap.c logger.h pldstr.h bt-int.h bytedecoders.h olestream-unwrap.h
pldstr.o:  pldstr.c logger.h pldstr.h  ole.h
ripole.o: ripole.c
