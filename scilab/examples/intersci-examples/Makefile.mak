SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib
INTERSCI="$(SCIDIR1)\bin\intersci"
F2C="$(SCIDIR1)\bin\f2c.exe"

include ../../Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) -DFORDLL  -I"$(SCIDIR1)\routines"
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR1)\routines" 

all:: info

info:
	@echo Type "nmake /f Makefile.mak dlls" to compile all programs 
	@echo Type "nmake /f Makefile.mak pgm.dll" to compile pgm program

DLLS= ex1.dll ex2.dll ex3.dll ex4.dll ex5.dll ex6.dll \
	ex7.dll ex8.dll ex9.dll ex10.dll ex11.dll ex12.dll \
	ex13.dll ex14.dll ex15.dll ex16.dll

dlls : $(DLLS) 

ex1.dll : ex1fi.obj ex1c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**   $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex1fi.obj : ex1fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex2.dll : ex2fi.obj ex2f.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex2fi.obj : ex2fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex3.dll : ex3fi.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex3fi.obj : ex3fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex4.dll : ex4fi.obj ex4f.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex4fi.obj : ex4fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex5.dll : ex5fi.obj ex5f.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex5fi.obj : ex5fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 


ex6.dll : ex6fi.obj ex6c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex6fi.obj : ex6fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex7.dll : ex7fi.obj ex7f.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex7fi.obj : ex7fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex8.dll : ex8fi.obj ex8c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex8fi.obj : ex8fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex9.dll : ex9fi.obj ex9f.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex9fi.obj : ex9fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 


ex10.dll : ex10fi.obj ex10f.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex10fi.obj : ex10fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 


ex11.dll : ex11fi.obj ex11c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex11fi.obj : ex11fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex12.dll : ex12fi.obj ex12c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex12fi.obj : ex12fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex13.dll : ex13fi.obj ex13c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex13fi.obj : ex13fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex14.dll : ex14fi.obj ex14c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex14fi.obj : ex14fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

ex15.dll : ex15fi.obj ex15c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex15fi.obj : ex15fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 


ex16.dll : ex16fi.obj ex16c.obj 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

ex16fi.obj : ex16fi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

clean::
	

distclean	::
	@del *.obj 
	@del *.dll
	@del *.ilib 
	@del *.pdk
	@del *.pdb
	@del *.ilk 
	@del *.def
	@del *.exp 

#------------------------------------------------------------

OBJS=ex1c.obj ex2f.obj ex4f.obj ex5f.obj ex6c.obj ex7f.obj ex8c.obj \
	ex9f.obj ex10f.obj ex11c.obj ex12c.obj ex13c.obj ex14c.obj  \
	ex15c.obj ex16c.obj

zallfi.dll : zallfi.obj $(OBJS) 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $**  $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

zallfi.obj : zallfi.desc
	@$(INTERSCI) $* 
	@$(F2C)  $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 

zallfi.desc : 
	@del zallfi.desc
	copy *.desc  zallfi.desc	

tests	: zallfi.desc zallfi.obj 
	"$(SCIDIR1)\bin\scilex.exe"  -f zall.sce

distclean:: clean

clean	::
	@del zallfi
	@del zallfi.sce 
	@del zallfi.f 
	@del zallfi.obj 
	@del zallfi.dia

