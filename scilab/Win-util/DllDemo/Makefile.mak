# the list of files to produce the library 

include ../../Makefile.incl.mak 

OBJS = tdll.obj

all :: libtdll.dll

clean ::
	@del *.obj 
	@del libtdll.* 
	@ del *.exe 

distclean :: clean 

libtdll.dll : $(OBJS) 
    $(LINKER) $(OBJS) $(GUILIBS) /nologo /subsystem:windows /dll /out:"libtdll.dll" /implib:"libtdll.lib" /def:tdll.def


#=================== test1 ===========================

all:: test1.exe

test1.exe 	: test1.obj
	@echo Creation of $*.exe 
	@$(LINKER) -SUBSYSTEM:console -OUT:"test1.exe" test1.obj \
	libtdll.lib 

#=================== test2 ===========================

all:: test2.exe

test2.exe 	: test2.obj
	@echo Creation of $*.exe 
	@$(LINKER) -SUBSYSTEM:console -OUT:"test2.exe" test2.obj 












	
