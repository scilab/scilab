SCIDIR=../../..
SCIDIR1=..\..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR)\bin\LibScilab.lib"
F2C=$(SCIDIR1)\bin\f2c
TEMP=c:\windows\temp

!include $(SCIDIR1)\Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) 
CFLAGS = $(CC_OPTIONS) -DFORDLL  -I"$(SCIDIR)/routines/f2c"

OBJS = $(TEMP)\ener.dll $(TEMP)\npend.dll $(TEMP)\np.dll

all:: $(OBJS) 

$(TEMP)\ener.dll : ener.f 
	@$(F2C) -d$(TEMP) ener.f 
	@$(CC) $(CFLAGS) $*.c /Fo"$*.obj"
	@del $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "ener.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 


$(TEMP)\npend.dll : npend.f 
	@$(F2C) -d$(TEMP) npend.f 
	@$(CC) $(CFLAGS) $*.c /Fo"$*.obj"
	@del $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "npend.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

$(TEMP)\np.dll : np.f 
	@$(F2C) -d$(TEMP) np.f 
	@$(CC) $(CFLAGS) $*.c /Fo"$*.obj"
	@del $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "np.dll" $*.obj
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


