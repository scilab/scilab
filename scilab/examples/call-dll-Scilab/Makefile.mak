# the list of files to produce the library 

include ../../Makefile.incl.mak 

OBJS = tdll.obj

all :: libtdll.dll

clean ::
	@del *.obj 
	@del libtdll.* 
	@ del *.exe 

libtdll.dll : $(OBJS) 
    $(LINKER) $(OBJS) $(GUILIBS) /nologo /subsystem:windows /dll /out:"libtdll.dll" /implib:"libtdll.lib" /def:tdll.def














	
