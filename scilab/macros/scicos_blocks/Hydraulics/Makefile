SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl

NAME = scshydraulicslib
NAM = SCI/macros/scicos_blocks/Hydraulics

MACROS= Bache.sci VanneReglante.sci  vanne_draw_ports.sci \
	PerteDP.sci bache_draw_ports.sci puit_inputs.sci vanne_inputs.sci \
	PuitsP.sci bache_inputs.sci source_draw_ports.sci vanne_outputs.sci \
	SourceP.sci bache_outputs.sci source_inputs.sci 

MODELS=Bache.mo PortPHQ1.mo Puits.mo  ThermoCarre.mo \
	PerteDP.mo PortPHQ2.mo Source.mo VanneReglante.mo 

#for Modelica extern functions 
OBJECTS  =
LIBRARY = libHydrau.a
CFLAGS = $(CC_OPTIONS) -DmexFunction=mex_$*
EXTRA_LDFLAGS =
#uncomment next line if OBJECTS is not empty
#include $(SCIDIR)/config/Makeso.incl

all  :: $(MACROS) $(MODELS)
	@ls $(MACROS) >names
	@$(SCIDIR)/bin/scilab -ns -nb -nw -f $(SCIDIR)/util/genlib.sce -args "$(NAME) $(NAM)"
	@ls $(MODELS) >models
	@$(SCIDIR)/bin/scilab -ns -nb -nw -f $(SCIDIR)/util/genmoc.sce
clean::


distclean::
	@$(RM) *.moc models
	$(RM) *.bin lib names libHydrau.*

