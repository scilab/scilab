#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl.mak

.SUFFIXES: .sci .bin .mo .moc $(SUFFIXES)

NAME = scselectricallib
NAM = SCI/macros/scicos_blocks/Electrical

MACROS= Capacitor.sci Ground.sci NPN.sci VVsourceAC.sci \
	ConstantVoltage.sci Inductor.sci PotentialSensor.sci VariableResistor.sci \
	CurrentSensor.sci Resistor.sci VoltageSensor.sci \
	Diode.sci  VsourceAC.sci 


MODELS=Capacitor.moc Ground.moc OutPutPort.moc VariableResistor.moc \
	ConstantVoltage.moc InPutPort.moc Pin.moc VoltageSensor.moc \
	CurrentSensor.moc Inductor.moc PotentialSensor.moc VsourceAC.moc \
	Diode.moc NPN.moc Resistor.moc VsourceDC.moc \
	OutPort.moc VVsourceAC.moc \

include ../../Make.lib.mak

.mo.moc: $(SCIDIR)/bin/modelicac
	$(SCIDIR)/bin/modelicac -c $(<) -o $(@)

all :: $(MODELS)


distclean::
	@$(RM) *.moc
