# Makefile.mak used to generate dlls 
# for dem01.dem file or other files 
# This makefile can be used to create 
# dll's for files which are in other directory 
# nmake /f c:/.../demos/intro/Makefile.mak ctarget TARGET=foo SCIDIR1=c:\...\

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR1)\bin\LibScilab.lib"
F2C="$(SCIDIR1)\bin\f2c"

!include $(SCIDIR1)\Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) 
CFLAGS = $(CC_OPTIONS) -DFORDLL  -I"$(SCIDIR1)/routines/f2c" -I"$(SCIDIR1)/routines"
TARGET=FOO

all :: $(TARGET).dll

$(TARGET).dll : $(TARGET).c 
	@$(CC) $(CFLAGS) $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

distclean:: clean

clean:: 
	-@del $(TARGET).obj 
	-@del $(TARGET).dll
	-@del $(TARGET).exp 
	-@del $(TARGET).ilib 
	-@del $(TARGET).ilk 
	-@del $(TARGET).pdb
	-@del $(TARGET).def

