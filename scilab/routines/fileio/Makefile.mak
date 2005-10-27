#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/fileio.lib

RIPOLE= bt-int.obj bytedecoders.obj logger.obj ole.obj olestream-unwrap.obj pldstr.obj ripole.obj

OBJSC = sound.obj raw.obj wav.obj  sox.obj libst.obj misc.obj fileio.obj soundI.obj \
	xls.obj $(RIPOLE)

OBJSF = 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

fileio.obj: fileio.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
	../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
	../interf/stack3.h ../os_specific/Os_specific.h
libst.obj: libst.c libst.h
misc.obj: misc.c st.h wav.h ../machine.h
raw.obj: raw.c st.h wav.h ../machine.h libst.h
sound.obj: sound.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
	st.h wav.h
soundI.obj: soundI.c ../stack-c.h ../graphics/Math.h ../machine.h \
	../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
	../interf/stack2.h ../interf/stack3.h sox.h wav.h
sox.obj: sox.c st.h wav.h ../machine.h sox.h
sprintf_ptr.obj: sprintf_ptr.c ../machine.h
wav.obj: wav.c st.h wav.h ../machine.h

xls.obj: ../machine.h

#ripole-0.1.4 files 
bt-int.obj: bt-int.c bt-int.h
logger.obj: logger.c logger.h
ole.obj: ole.c logger.h pldstr.h bt-int.h bytedecoders.h olestream-unwrap.h ole.h
olestream-unwrap.obj : olestream-unwrap.c logger.h pldstr.h bt-int.h bytedecoders.h olestream-unwrap.h
pldstr.obj:  pldstr.c logger.h pldstr.h  ole.h
ripole.obj: ripole.c
