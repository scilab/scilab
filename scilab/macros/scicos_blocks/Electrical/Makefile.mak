#-----------------------------
SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl.mak

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
OBJS  =
LIBRARY = libElectrical
CFLAGS = $(CC_OPTIONS) -DmexFunction=mex_$*
EXTRA_LDFLAGS =
#uncomment next line if OBJECTS is not empty
#include ..\..\..\config\Makedll.incl

all  :: $(MACROS) $(MODELS)
	@dir /B $(MACROS) >names
	@$(SCIDIR1)\bin\scilex.exe -ns -nb -nwni -f $(SCIDIR1)\util\genlib.sce -args $(NAME) $(NAM)
	@dir /B $(MODELS) >models
	@$(SCIDIR1)\bin\scilex.exe -ns -nb -nwni -f $(SCIDIR1)\util\genmoc.sce
clean::

distclean::
	@del *.moc models
	@del *.bin lib names 

