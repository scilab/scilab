SHELL = /bin/sh

SCIDIR = ../..

LIBRARY = examples.a

OTHERLIBS = 

#########################################################################
#    To each .o interface (C or Fortran) is associated one scilab function
#    CAUTION : CINTERFACES and CFUNCTIONS must have the same # of items
#########################################################################

CINTERFACES = intex1c.o intex2c.o intex3c.o intex4c.o intex5c.o intex6c.o intex7c.o intex8c.o intex9c.o intex10c.o intex11c.o intex12c.o intex13c.o intex14c.o intex15c.o intex16c.o intex17c.o intex17f.o intex18c.o

CFUNCTIONS = ex1c ex2c ex3c ex4c ex5c ex6c ex7c ex8c ex9c ex10c ex11c ex12c ex13c ex14c ex15c ex16c ex17c ex17f ex18c

OTHERCOBJS = pgmsc.o pgmsf.o as2osc.o ex17c.o 

FORTRANINTERFACES = intex1f.o intex2f.o intex3f.o intex4f.o intex5f.o intex6f.o intex7f.o intex8f.o intex9f.o intex10f.o intex11f.o intex12f.o intex13f.o intex14f.o intex15f.o intex16f.o 

FFUNCTIONS = ex1f ex2f ex3f ex4f ex5f ex6f ex7f ex8f ex9f ex10f ex11f ex12f ex13f ex14f ex15f ex16f

OTHERFOBJS = as2osf.o ex17f.o 

################ do not edit below this line ############################

GATEWAY = $(LIBRARY:.a=)
STARTUP = $(LIBRARY:.a=)

#CFUNCTIONS = $(CINTERFACES:.o=) 
#Functions and interfaces share the same name

include $(SCIDIR)/Makefile.incl

CFLAGS = $(CC_OPTIONS) $(CC_PICFLAGS) -I$(SCIDIR)/routines
FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS) -I$(SCIDIR)/routines

FUNCTIONS = $(CFUNCTIONS) $(FFUNCTIONS)

OBJSF = $(FORTRANINTERFACES) $(OTHERFOBJS)

OBJSC = $(CINTERFACES) $(OTHERCOBJS)

all:: all_nomes  message

all_nomes : $(LIBRARY) $(GATEWAY)_gateway.c $(GATEWAY)_gateway.o $(STARTUP).sce 

include $(SCIDIR)/routines/Make.lib

rmold:
	$(RM) $(STARTUP).sce $(GATEWAY)_gateway.c 

$(GATEWAY)_gateway.c: Makefile 
	@$(RM) $(GATEWAY)_gateway.c 
	@echo "-- Generating the C function $(GATEWAY)_gateway.c";
	@echo "#include \"mex.h\" " > $(GATEWAY)_gateway.c;
	@echo " " >> $(GATEWAY)_gateway.c;
	@echo "extern Gatefunc %s;\n" > format;
	@echo $(CINTERFACES:.o=) >> args;
	@echo $(GATEWAY)_gateway.c > file;
	@$(SCIDIR)/macros/Tomake Xformat Xargs Xfile;
	@$(RM) format args file;
	@echo "extern Gatefunc C2F(%s);\n" > format;
	@echo $(FORTRANINTERFACES:.o=) >> args;
	@echo $(GATEWAY)_gateway.c > file;
	@$(SCIDIR)/macros/Tomake Xformat Xargs Xfile;
	@$(RM) format args file;
	@echo " " >> $(GATEWAY)_gateway.c;
	@echo "static GenericTable Tab[]={"  >> $(GATEWAY)_gateway.c;
	@echo "{(Myinterfun)sci_gateway, %s,\"error msg\"},\n" > format;
	@echo $(CINTERFACES:.o=) > args;
	@echo $(GATEWAY)_gateway.c > file;
	@$(SCIDIR)/macros/Tomake  Xformat Xargs Xfile;
	@$(RM) format args file;
	@echo "{(Myinterfun)sci_gateway, C2F(%s),\"error msg\"},\n" > format;
	@echo $(FORTRANINTERFACES:.o=) > args;
	@echo $(GATEWAY)_gateway.c > file;
	@$(SCIDIR)/macros/Tomake  Xformat Xargs Xfile;
	@$(RM) format args file;
	@echo "	 };" >> $(GATEWAY)_gateway.c;
	@echo " " >> $(GATEWAY)_gateway.c;
	@echo "int C2F($(GATEWAY)_gateway)()" >> $(GATEWAY)_gateway.c;
	@echo "{  Rhs = Max(0, Rhs);" >> $(GATEWAY)_gateway.c;
	@echo "(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);" >>  $(GATEWAY)_gateway.c;
	@echo "  return 0;" >> $(GATEWAY)_gateway.c;
	@echo "}" >>  $(GATEWAY)_gateway.c;
	@echo " " >> $(GATEWAY)_gateway.c;

$(STARTUP).sce: Makefile 
	@echo "-- Generating the Scilab script $(STARTUP).sce";
	@echo "scilab_functions =[..." > $(STARTUP).sce;
	@echo "\"%s\";\n" > format;
	@echo $(FUNCTIONS) > args;
	@echo $(STARTUP).sce > file; 
	@$(SCIDIR)/macros/Tomake Xformat Xargs Xfile;
	@$(RM) format args file;
	@echo "           ];" >> $(STARTUP).sce;
	@echo "auxiliary=\"$(OTHERLIBS)\";"  >> $(STARTUP).sce;
	@echo "files=G_make([\"$(GATEWAY)_gateway.o\",\"$(LIBRARY)\", auxiliary],\"void(Win)\");"  >>$(STARTUP).sce;
	@echo "addinter(files,\"$(GATEWAY)_gateway\",scilab_functions);" >> $(STARTUP).sce;

message:
	@echo "------------------------------------------";
	@echo "To load $(FUNCTIONS)";
	@echo "        functions, at Scilab prompt, enter:";
	@echo "-->exec $(STARTUP).sce";
	@echo "------------------------------------------";

clean	::
	@$(RM) *.o *.obj *~ so_locations
	@$(RM) $(GATEWAY)_gateway.c $(STARTUP).sce
	@$(RM) *.dia
	@$(RM) $(GATEWAY).tst
	@$(RM) format file args

distclean	:: clean 
	@$(RM) *.dll


EXAMPLES = ex1c.sce ex2c.sce ex3456c.sce ex7c.sce ex8c.sce ex9c.sce ex10c.sce \
	ex11c.sce ex12c.sce ex13c.sce ex14c.sce ex15c.sce ex16c.sce ex17c.sce \
	ex1f.sce ex2f.sce ex3456f.sce ex7f.sce ex8f.sce ex9f.sce ex10f.sce \
	ex11f.sce ex12f.sce ex13f.sce ex14f.sce ex15f.sce ex16f.sce ex17f.sce

tests	: $(GATEWAY).dia

$(GATEWAY).dia	: $(GATEWAY).tst all_nomes 
	@$(SCIDIR)/util/scidem $(SCIDIR) $(GATEWAY).tst $(GATEWAY).dia

$(GATEWAY).tst : Makefile 
	@echo "exec(\"$(GATEWAY).sce\",-1);" > $(GATEWAY).tst
	@cat $(EXAMPLES) >> $(GATEWAY).tst 		


