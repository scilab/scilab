SCIDIR =../..
SCIDIR1 =..\..

#LIBRARY = name of the dll (without extension).
LIBRARY = examples

#########################################################################
#    To each .obj interface (C or Fortran) is associated one scilab function
#    CAUTION : CINTERFACES and CFUNCTIONS must have the same # of items
#########################################################################

CINTERFACES = intex1c.obj intex2c.obj intex3c.obj intex4c.obj intex5c.obj intex6c.obj intex7c.obj intex8c.obj intex9c.obj intex10c.obj intex11c.obj intex12c.obj intex13c.obj intex14c.obj intex15c.obj intex16c.obj intex17c.obj intex17f.obj

CFUNCTIONS = ex1c ex2c ex3c ex4c ex5c ex6c ex7c ex8c ex9c ex10c ex11c ex12c ex13c ex14c ex15c ex16c ex17c ex17f

OTHERCOBJS = pgmsc.obj pgmsf.obj as2osc.obj ex17c.obj

FORTRANINTERFACES = intex1f.obj intex2f.obj intex3f.obj intex4f.obj intex5f.obj intex6f.obj intex7f.obj intex8f.obj intex9f.obj intex10f.obj intex11f.obj intex12f.obj intex13f.obj intex14f.obj intex15f.obj intex16f.obj 

FFUNCTIONS = ex1f ex2f ex3f ex4f ex5f ex6f ex7f ex8f ex9f ex10f ex11f ex12f ex13f ex14f ex15f ex16f
 
OTHERFOBJS = as2osf.obj ex17f.obj 
################ do not edit below this line ############################

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR)/bin/LibScilab.lib"

!include $(SCIDIR1)\Makefile.incl.mak

FFLAGS = $(FC_OPTIONS) -DFORDLL -I"$(SCIDIR1)\routines"
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"

FUNCTIONS = $(CFUNCTIONS) $(FFUNCTIONS)

OBJSF = $(FORTRANINTERFACES) $(OTHERFOBJS)

OBJSC = $(CINTERFACES) $(OTHERCOBJS)


f3f.dll: f3f.obj 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	$(LINKER) $(LINKER_FLAGS) f3f.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

f3c.obj :
	$(CC) $(CFLAGS) f3c.c

f3c.dll: f3c.obj
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) f3c.obj $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

all::  CLEAROLD  $(OTHEROBJECTS) $(LIBRARY)_gateway.c $(LIBRARY)_gateway.obj $(LIBRARY).dll $(LIBRARY).sce message

OBJS = $(OBJSF) $(OBJSC) $(OTHEROBJECTS) $(LIBRARY)_gateway.obj

CLEAROLD:
	@$(RM) $(LIBRARY).sce
	@$(RM) $(LIBRARY)_gateway.c

$(LIBRARY)_gateway.c:
	@echo "-- Generating the C function $(LIBRARY)_gateway.c";
	@echo #include "mex.h"  > $(LIBRARY)_gateway.c
	@echo /*  */ >> $(LIBRARY)_gateway.c
	@"$(SCIDIR1)\macros\Tomake" @<< @<< @<<
extern Gatefunc %s;\n
<<
$(CINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@"$(SCIDIR1)\macros\Tomake" @<< @<< @<<
extern Gatefunc C2F(%s);\n
<<
$(FORTRANINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@echo /*  */ >> $(LIBRARY)_gateway.c
	@echo static GenericTable Tab[]={  >> $(LIBRARY)_gateway.c
	@"$(SCIDIR1)\macros\Tomake" @<< @<< @<<
{(Myinterfun)sci_gateway, %s,"error msg"},\n
<<
$(CINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@"$(SCIDIR1)\macros\Tomake" @<< @<< @<<
{(Myinterfun)sci_gateway, C2F(%s),"error msg"},\n
<<
$(FORTRANINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@echo 	 }; >> $(LIBRARY)_gateway.c
	@echo /*   */ >> $(LIBRARY)_gateway.c
	@echo int C2F($(LIBRARY)_gateway)() >> $(LIBRARY)_gateway.c
	@echo {  Rhs = Max(0, Rhs); >> $(LIBRARY)_gateway.c
	@echo (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F); >>  $(LIBRARY)_gateway.c
	@echo   return 0; >> $(LIBRARY)_gateway.c
	@echo } >>  $(LIBRARY)_gateway.c
	@echo /*   */ >> $(LIBRARY)_gateway.c

$(LIBRARY).sce:
	@echo -- Generating the Scilab script $(LIBRARY).sce;
	@echo scilab_functions =[... > $(LIBRARY).sce
	@"$(SCIDIR1)\macros\Tomake" @<< @<< @<<
"%s";\n
<<
$(FUNCTIONS)
<<
$(LIBRARY).sce
<<
	@echo            ]; >> $(LIBRARY).sce
	@echo files=G_make("void(Unix)","$(LIBRARY).dll"); >> $(LIBRARY).sce
	@echo addinter(files,"$(LIBRARY)_gateway",scilab_functions); >> $(LIBRARY).sce

message:
	@echo ------------------------------------------
	@echo - To load function(s)
	@echo         $(FUNCTIONS), 
	@echo - at Scilab prompt, enter:
	@echo "-->exec $(LIBRARY).sce;"
	@echo ------------------------------------------

clean	::
	@$(RM) *.obj
	@$(RM) $(LIBRARY).sce
	@$(RM) $(LIBRARY)_gateway.c
	@$(RM) $(LIBRARY).lib
	@$(RM) *.dll
	@$(RM) *.def
	@$(RM) *.ilib
	@$(RM) *.exp
	@$(RM) *.pdb

distclean:: 	clean

$(LIBRARY).dll: $(OBJS)
	@echo Creation of dll $(LIBRARY).dll and import lib from ...
	@echo $(OBJS)
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 

EXAMPLES = ex1c.sce + ex2c.sce + ex3456c.sce + ex7c.sce + ex8c.sce + ex9c.sce + ex10c.sce + ex11c.sce + ex12c.sce + ex13c.sce + ex14c.sce + ex15c.sce + ex16c.sce + ex17c.sce + ex1f.sce + ex2f.sce + ex3456f.sce + ex7f.sce + ex8f.sce + ex9f.sce + ex10f.sce + ex11f.sce + ex12f.sce + ex13f.sce + ex14f.sce + ex15f.sce + ex16f.sce + ex17f.sce

tests	:
	@del zlink.dia	
	@echo exec("examples.sce",-1); > zlink.tst
        @cat $(EXAMPLES) >> zlink.tst
	"$(SCIDIR1)\bin\scilex.exe"  -f zlink.tst 
