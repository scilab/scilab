SCIDIR=../../..
SCIDIR1=..\..\..

include ..\..\..\Makefile.incl.mak

NAME = scshydraulicslib
NAM = SCI/macros/scicos_blocks/Hydraulics

MODELICAC=$(SCIDIR1)\bin\modelicac.exec

MACROS= Bache.sci VanneReglante.sci  vanne_draw_ports.sci \
	PerteDP.sci bache_draw_ports.sci puit_inputs.sci vanne_inputs.sci \
	PuitsP.sci bache_inputs.sci source_draw_ports.sci vanne_outputs.sci \
	SourceP.sci bache_outputs.sci source_inputs.sci

MODELS=Bache.mo PortPHQ1.mo Puits.mo  ThermoCarre.mo \
	PerteDP.mo PortPHQ2.mo Source.mo VanneReglante.mo

BINMODELS = $(MODELS:.mo=.moc)

#for Modelica extern functions
OBJS  =
LIBRARY = libHydrau
CFLAGS = $(CC_OPTIONS) -DmexFunction=mex_$*
EXTRA_LDFLAGS =
#uncomment next line if OBJECTS is not empty
#include ..\..\..\config\Makedll.incl

all  ::
	@dir /B $(MACROS) >names
	@$(SCIDIR1)\bin\scilex.exe -ns -nb -nwni -f $(SCIDIR1)\util\genlib.sce -args $(NAME) $(NAM)

all  ::
	@dir /B $(MODELS) >models
	@$(SCIDIR1)\bin\scilex.exe -ns -nb -nwni -f $(SCIDIR1)\util\genmoc.sce

# must recompile the models if modelicac changed 

$(BINMODELS) : $(MODELICAC) 


clean::

distclean::
	@del *.moc models
	@del *.bin lib names 
