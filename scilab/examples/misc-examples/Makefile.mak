SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib
F2C="$(SCIDIR1)\bin\f2c"

include ../../Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) -DFORDLL 
CFLAGS = $(CC_OPTIONS) -DFORDLL 

all:: info

info:
	@echo "Type \"make test\" to compile all programs (in /tmp)"
	@echo "Type \"make pgm.dll\" to compile pgm program"

OBJSF = gr22.dll jac22.dll res22.dll wfex.dll wfexab.dll wjex.dll 

OBJSC = 

test : $(OBJSF) $(OBJSC)

clean::
	@del *.obj 
	@del *.c
	@del *.dll

distclean:: clean

# gr22.dll : gr22.obj 


.f.dll:
	@$(F2C) $< 
	@$(CC) $(CFLAGS) $*.c 
	@del $*.c 
	@echo Creation of dll $(DLL) and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $*.obj
	@$(LINKER) $(LINKER_FLAGS) $*.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 



##----------------------  test all the example  

EXAMPLES= dasrt1.sce dasrt2.sce dasrt3.sce dassl1.sce dassl2.sce ode1.sce ode2.sce ode3.sce odedc.sce odeoptions.sce

tests	:
	@del zlink.dia	
	@type  $(EXAMPLES) > zlink.tst 		
	"$(SCIDIR1)\bin\scilex.exe"  -e lines(-1);exec('zlink.tst');quit

clean	::
	@del zlink.dia 
	@del zlink.tst 
	@del *.dll 
	@del *.ilib 
	@del *.ilk 
	@del *.pdb 	
	@del *.def 
	@del *.exp

distclean:: clean 
	@del *.obj 





