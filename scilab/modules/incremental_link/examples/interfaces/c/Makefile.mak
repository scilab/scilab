SCIDIR =../../../../..
SCIDIR1 =..\..\..\..\..

#LIBRARY = name of the dll (without extension).
LIBRARY = examples

#########################################################################
#    To each .obj interface (C or Fortran) is associated one scilab function
#    CAUTION : CINTERFACES and CFUNCTIONS must have the same # of items
#########################################################################

CFUNCTIONS = ex1c ex2c ex3c ex4c ex5c ex6c ex7c ex8c ex9c ex10c ex11c ex12c	 ex13c ex14c ex15c

CINTERFACES = intex1c.obj intex2c.obj intex3c.obj \
							intex4c.obj intex5c.obj intex6c.obj intex7c.obj \
							intex8c.obj intex9c.obj intex10c.obj intex11c.obj \
							intex12c.obj intex13c.obj intex14c.obj intex15c.obj \

OTHERCOBJS = crestrc.obj as2osc.obj

################ do not edit below this line ############################

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR)/bin/LibScilab.lib"

!include $(SCIDIR1)\Makefile.incl.mak

FFLAGS = $(FC_OPTIONS) -DFORDLL -I"$(SCIDIR1)\modules\core\includes"
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/modules/core/includes"

FUNCTIONS = $(CFUNCTIONS) $(FFUNCTIONS)

OBJSF = $(FORTRANINTERFACES) $(OTHERFOBJS)

OBJSC = $(CINTERFACES) $(OTHERCOBJS)


all::  $(OTHEROBJECTS) $(LIBRARY)_gateway.c $(LIBRARY)_gateway.obj $(LIBRARY).dll $(LIBRARY).sce message

OBJS = $(OBJSF) $(OBJSC) $(OTHEROBJECTS) $(LIBRARY)_gateway.obj

CLEAROLD:
	@$(RM) $(LIBRARY).sce
	@$(RM) $(LIBRARY)_gateway.c

$(LIBRARY)_gateway.c:
	@echo "-- Generating the C function $(LIBRARY)_gateway.c";
	@echo #include "mex.h"  > $(LIBRARY)_gateway.c
	@echo /*  */ >> $(LIBRARY)_gateway.c
	@"$(SCIDIR1)\tools\tomake\Tomake" @<< @<< @<<
extern Gatefunc %s;\n
<<
$(CINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@"$(SCIDIR1)\tools\tomake\Tomake" @<< @<< @<<
extern Gatefunc C2F(%s);\n
<<
$(FORTRANINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@echo /*  */ >> $(LIBRARY)_gateway.c
	@echo static GenericTable Tab[]={  >> $(LIBRARY)_gateway.c
	@"$(SCIDIR1)\tools\tomake\Tomake" @<< @<< @<<
{(Myinterfun)sci_gateway, %s,"error msg"},\n
<<
$(CINTERFACES:.obj=)
<<
$(LIBRARY)_gateway.c
<<
	@"$(SCIDIR1)\tools\tomake\Tomake" @<< @<< @<<
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
	@"$(SCIDIR1)\tools\tomake\Tomake" @<< @<< @<<
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
	@$(RM) *.tst
  @$(RM) *.dia
	@$(RM) *.obj
	@$(RM) $(LIBRARY).sce
	@$(RM) $(LIBRARY)_gateway.c
	@$(RM) $(LIBRARY).lib
	@$(RM) *.dll
	@$(RM) *.def
	@$(RM) *.lib
	@$(RM) *.exp
	@$(RM) *.pdb

distclean:: 	clean

$(LIBRARY).dll: $(OBJS)
	@echo Creation of dll $(LIBRARY).dll and import lib from ...
	@echo $(OBJS)
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) /nologo /dll /out:"$*.dll" /implib:"$*.lib" /def:"$*.def" 

	
$(CFUNCTIONS).dotest:
	@echo exec("examples.sce",-1); > $*.tst
  @type $*.sce >> $*.tst
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e scitest('$*.tst',%t);quit;

to_do: $(CFUNCTIONS).dotest
 

tests	: to_do
