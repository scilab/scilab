SCIDIR=../../..
SCIDIR1=..\..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR)\bin\LibScilab.lib"
F2C="$(SCIDIR1)\bin\f2c"
TEMP=c:\windows\temp

!include $(SCIDIR1)\Makefile.incl.mak 


FFLAGS = $(FC_OPTIONS) 
CFLAGS = $(CC_OPTIONS) -DFORDLL  -I"$(SCIDIR)/routines/f2c"

OBJS = $(TEMP)\wheel.dll $(TEMP)\wheelg.dll

all:: $(OBJS) 

$(TEMP)\wheel.dll : wheel.f 
	@$(F2C) -d$(TEMP) wheel.f 
	@$(CC) $(CFLAGS) $*.c  /Fo"$*.obj"
	@del $*.c 
	@echo Creation of dll $(TEMP)\wheel.dll  and import lib 
	@$(DUMPEXTS) -o "$*.def" "wheel.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 


$(TEMP)\wheelg.dll : wheelg.f 
	@$(F2C) -d$(TEMP) wheelg.f 
	@$(CC) $(CFLAGS) $*.c  /Fo"$*.obj"
	@del $*.c 
	@echo Creation of dll $(TEMP)\wheelg.dll and import lib 
	@$(DUMPEXTS) -o "$*.def" "wheelg.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

distclean:: clean

clean:: 
	-@del $(TEMP)\*.obj 
	-@del $(TEMP)\*.dll
	-@del $(TEMP)\*.exp 
	-@del $(TEMP)\*.ilib 
	-@del $(TEMP)\*.ilk 
	-@del $(TEMP)\*.pdb
	-@del $(TEMP)\*.def
