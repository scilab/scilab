SCIDIR =../..
SCIDIR1 =..\..

#LIBRARY = name of the dll (without .dll postfix)..
LIBRARY=lapackscilab

#########################################################################
#    To each .obj interface (C or Fortran) associate one scilab function
#    
CINTERFACES = intdgemm.obj intdlassq.obj intdgetrf.obj intdsyev.obj intdgeesx.obj intzgemm.obj intdgesvd.obj #intzgeesx.obj

CFUNCTIONS = dgemm dlassq dgetrf  dsyev dgeesx zgemm dgesvd #zgeesx

OTHERCOBJS = 

FORTRANINTERFACES = intfdgemm.obj inttestdgebal.obj intdgebak.obj intdgels.obj intdgeqrf.obj

FFUNCTIONS = fdgemm xxdgebal dgebak dgels dgeqrf 


OTHERFOBJS = 
################ do not edit below this line #############################
DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR1)\bin\LibScilab.lib"

!include ..\..\Makefile.incl.mak

FFLAGS = $(FC_OPTIONS) -DFORDLL -I"$(SCIDIR1)\routines"
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"

FUNCTIONS = $(CFUNCTIONS) $(FFUNCTIONS)

OBJSF = $(FORTRANINTERFACES) $(OTHERFOBJS)

OBJSC = $(CINTERFACES) $(OTHERCOBJS)

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
	@echo $(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 
	@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 
	

