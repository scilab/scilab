SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib

include ../../Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) -DFORDLL 
CFLAGS = $(CC_OPTIONS) -DFORDLL 

all:: info

info:
	@echo "Type \"nmake /f Makefile.mak test\" to compile all programs (in /tmp)"
	@echo "Type \"nmake /f Makefile.mak pgm.obj\" to compile pgm program"


OBJSF = ext1f.dll ext2f.dll ext3f.dll ext4f.dll ext5f.dll ext6f.dll \
	ext7f.dll ext8f.dll ext9f.dll ext10f.dll ext11f.dll ext12f.dll \
	ext13f.dll ext14f.dll

OBJSC = ext1c.dll ext2c.dll ext3c.dll ext4c.dll ext5c.dll ext6c.dll \
	ext7c.dll ext8c.dll ext9c.dll ext10c.dll ext11c.dll ext12c.dll \
	ext13c.dll ext14c.dll

test : $(OBJSF) $(OBJSC)

.c.dll : 
	@$(CC) $(CFLAGS) $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 


!IF "$(USE_F2C)" == "YES"
.f.dll	:
	@echo ----------- Compile file $*.f (using f2c) -------------
	@"$(SCIDIR1)\bin\f2c.exe" $*.f 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 
!ELSE 
.f.dll	:
	@echo -----------Compile file $*.f  (using $(FC)) -------------
	@$(FC) $(FFLAGS) $<
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 
!ENDIF 


distclean:: clean

clean	::
	@del zlink.dia 
	@del zlink.tst

distclean::
	@del zlink.dia 
	@del zlink.tst 
	@del *.exp 
	@del *.ilib 
	@del *.ilk 
	@del *.def 
	@del *.pdb 
	@del *.obj
	@del *.dll

tests	:  zlink.dia 

EXAMPLES=ext1c.sce ext1f.sce ext2c.sce ext2f.sce ext3c.sce \
	ext3f.sce ext4c.sce ext4f.sce ext5c.sce ext5f.sce ext6c.sce \
	ext6f.sce ext7c.sce ext7f.sce ext8c.sce ext8f.sce ext9c.sce ext9f.sce \
	ext10c.sce ext10f.sce ext11c.sce ext11f.sce ext12c.sce 	ext12f.sce \
	ext13c.sce ext13f.sce

zlink.dia	: $(EXAMPLES)
	type *.sce > zlink.tst 		
	"$(SCIDIR1)\bin\scilex.exe"  -e lines(-1);exec('zlink.tst');quit;

clean	::
	@del zlink.dia 
	@del zlink.tst


