SHELL = /bin/sh
SCIDIR=../

all:: Lib.exe Name.exe Gengatsce.exe Gensosce.exe Gensomex.exe Forwin.exe Tomake.exe
	Makesubdirs.bat all

distclean::
	Makesubdirs.bat distclean 

clean::
	Makesubdirs.bat clean 

include ../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) 
FFLAGS = $(FC_OPTIONS)

OBJS = Lib.obj Name.obj Gengatsce.obj Gensosce.obj Gensomex.obj Forwin.obj Tomake.obj

clean ::
	$(RM) *.obj

distclean::
	$(RM) *.obj
	$(RM) *.exe

Lib.exe	: Lib.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Lib.exe" Lib.obj 

Name.exe: Name.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Name.exe" Name.obj 

Gengatsce.exe: Gengatsce.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Gengatsce.exe" Gengatsce.obj 

Gensosce.exe: Gensosce.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Gensosce.exe" Gensosce.obj 

Gensomex.exe: Gensomex.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Gensomex.exe" Gensomex.obj 

Forwin.exe: Forwin.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Forwin.exe" Forwin.obj 

Tomake.exe: Tomake.c 
	$(CC) $(CFLAGS) $*.c 	
	$(LINKER) -SUBSYSTEM:console -OUT:"Tomake.exe" Tomake.obj 










