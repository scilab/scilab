SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl

NAME = scselectricallib
NAM = SCI/macros/scicos_blocks/Electrical

MACROS= Capacitor.sci Ground.sci NPN.sci VVsourceAC.sci \
	ConstantVoltage.sci Inductor.sci PotentialSensor.sci VariableResistor.sci \
	CurrentSensor.sci Resistor.sci VoltageSensor.sci \
	Diode.sci  VsourceAC.sci 


MODELS=Capacitor.mo Ground.mo OutPutPort.mo VariableResistor.mo \
	ConstantVoltage.mo InPutPort.mo Pin.mo VoltageSensor.mo \
	CurrentSensor.mo Inductor.mo PotentialSensor.mo VsourceAC.mo \
	Diode.mo NPN.mo Resistor.mo VsourceDC.mo \
	OutPort.mo VVsourceAC.mo 

#for Modelica extern functions
OBJECTS  =
LIBRARY = libElectrical.a
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
	$(RM) *.bin lib names 
