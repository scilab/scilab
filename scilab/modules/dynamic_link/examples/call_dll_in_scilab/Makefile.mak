# the list of files to produce the library 
LINKER=link

OBJS = tdll.obj

all :: libtdll.dll

clean ::
	@del *.obj 
	@del *.dll
	@del *.lib
	@del *.exp

libtdll.dll : $(OBJS) 
    $(LINKER) $(OBJS)  /nologo /subsystem:windows /dll /out:"libtdll.dll" /implib:"libtdll.lib" /def:tdll.def
    














	
