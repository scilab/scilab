SHELL = /bin/sh
SCIDIR=..
XMLINT=xmlint
LINKER=link
CC=CL

FILE1 = stdAfx
FILE2 = xmlint

OBJSC1 = $(FILE1).obj
OBJSC2 = $(FILE2).obj

CFLAGS = /O2 /G5 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /FD /EHsc /ML /Yc"stdafx.h" /Fp"$(XMLINT).pch" /Fo"" /Fd"" /W3 /c /TP
LIBS = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
GUIFLAGS=/SUBSYSTEM:CONSOLE /MACHINE:X86 
LINKFLAGS=/INCREMENTAL:NO /NOLOGO /PDB:".\Release/$(XMLINT).pdb"
#=================== dumpexts.exe ===========================

all:: $(XMLINT).exe

$(XMLINT).exe 	:
	$(CC) $(CFLAGS) $(FILE1).cpp
  $(CC) $(CFLAGS) $(FILE2).cpp
	$(LINKER) /OUT:".\$(XMLINT).exe" $(LINKFLAGS) $(GUIFLAGS) $(LIBS) "$(OBJSC1)" "$(OBJSC2)"

clean::
  @del $(XMLINT).exe 
  @del *.tli /Q
  @del *.obj /Q
  @del *.pch /Q
  @del *.idb /Q
  @del *.tlh /Q
distclean::
  @del *.tli /Q
  @del *.obj /Q
  @del *.pch /Q
  @del *.idb /Q
  @del *.tlh /Q
  
  


