SCIDIR=../../..
SCIDIR1=..\..\..

!include $(SCIDIR1)\Makefile.incl

CFLAGS = $(CC_OPTIONS) $(CC_PICFLAGS) -I"$(SCIDIR)/routines"
FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS) -I"$(SCIDIR1)\routines"

################# List of source mexfiles ######################
CMEXSOURCES = 

FORTRANMEXSOURCES = fmexfunction1.f fmexfunction2.f

################# List of associated Scilab functions  ##########
#               (= by default the name of the mexfunction)      #
FCTS = $(CMEXSOURCES:.c=) $(FORTRANMEXSOURCES:.f=)

################## List of routines used by the mexfiles ########
OTHERCSOURCES = 

OTHERFORTRANSOURCES = 

################## Name of startup file (default = "startup.sce") #

STARTUP = startup

################## Name of gateway file (default = "generic_gateway.c #

GENERIC = generic
########## Do not edit below this line #############

MEXOBJS = $(CMEXSOURCES:.c=.obj) $(FORTRANMEXSOURCES:.f=.obj)

OTHEROBJS = $(OTHERCSOURCES:.c=.obj) $(OTHERFORTRANSOURCES:.f=.obj)

OTHERSOURCES = $(OTHERCSOURCES) $(OTHERFORTRANSOURCES)

MEXSOURCES = $(CMEXSOURCES) $(FORTRANMEXSOURCES)

OBJS = $(MEXOBJS) $(OTHEROBJS) $(GENERIC)_gateway.obj

all	::  rmold $(GENERIC)_gateway.c $(OBJS) $(STARTUP).sce message $(GENERIC).dll

clean	::
	$(RM) *.obj so_locations $(STARTUP).sce $(GENERIC)_gateway.c

distclean ::
	$(RM) *.obj so_locations $(STARTUP).sce $(GENERIC)_gateway.c

$(GENERIC)_gateway.c $(STARTUP).sce:
	@echo "-- Generating the C function $(GENERIC)_gateway.c";
	@echo "-- ... and the Scilab script $(STARTUP).sce";
	@echo $(FCTS) > tmp_arg1;
	@echo $(OTHEROBJS) > tmp_arg2;
	@echo $(MEXSOURCES) > tmp_arg3; 
	@echo $(STARTUP) $(GENERIC) > tmp_arg4;
	@$(SCIDIR)/macros/Gengatsce @<< @<< @<< @<<
$(FCTS)
<<
$(OTHEROBJS)
<<
$(MEXSOURCES)
<<
$(STARTUP) $(GENERIC)
<<

message:
	@echo "------------------------------------------";
	@echo "To load $(FCTS)";
	@echo "        functions, at Scilab prompt, enter:";
	@echo "-->exec $(STARTUP).sce";
	@echo "------------------------------------------";

rmold:
	@$(RM) $(STARTUP).sce $(GENERIC)_gateway.c

$(GENERIC).dll: $(OBJS)
	@echo Creation of dll $*.dll and import lib 
	@$(DUMPEXTS) -o "$*.def" "$*.dll" $**
	@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:"$*.dll" /implib:"$*.ilib" /def:"$*.def" 
