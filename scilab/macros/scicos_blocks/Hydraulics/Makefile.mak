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


all  :: $(MACROS) $(MODELS)
	@dir /B $(MACROS) >names
	@$(SCIDIR1)\bin\scilex.exe -nb -nwni -f $(SCIDIR1)\util\genlib.sce -args "$(NAME) $(NAM)"
	@dir /B $(MODELS) >models
	@$(SCIDIR1)\bin\scilex.exe -nb -nwni -f $(SCIDIR1)\util\genmoc.sce
clean::

distclean::
	@del *.moc models
	@del *.bin lib names 
