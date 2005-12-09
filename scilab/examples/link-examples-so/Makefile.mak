SCIDIR=../../
SCIDIR1=..\..\

all	: message 

DIAFILES=ext1c.dia ext2c.dia ext3c.dia ext4c.dia ext5c.dia ext6c.dia \
	ext7c.dia ext8c.dia ext9c.dia ext10c.dia ext11c.dia ext12c.dia \
	ext13c.dia ext14c.dia ext1f.dia ext2f.dia  \
	ext6f.dia ext7f.dia ext8f.dia ext9f.dia ext10f.dia \
	ext11f.dia ext12f.dia ext13f.dia ext14f.dia

message:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter:;
	@echo -->exec exXX.sce; 
	@echo to execute example XX 
	@echo ------------------------------------------;
	@echo Type nmake /f Makefile.mak tests 
	@echo to run all tests 
	@echo ------------------------------------------;

tests	: $(DIAFILES) 

distclean	::
	@del *.obj 
	@del *.dia
	@del *.dll
	@del *.ilib 
	@del *.pdk
	@del *.pdb
	@del *.ilk 
	@del *.def
	@del *.exp 
	@del libext*

.SUFFIXES: .sce .dia 

.sce.dia:
	@"$(SCIDIR1)\bin\scilex.exe"  -nwni -e scitest('$*.sce',%t);quit
