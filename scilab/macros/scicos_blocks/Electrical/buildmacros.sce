//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
MSDOS=(getos()=='Windows');
SCI=getenv('SCI'); 
TMPDIR=getenv('TMPDIR');
//------------------------------------
genlib('scselectricallib','SCI/macros/scicos_blocks/Electrical');
//------------------------------------
//if MSDOS then
//  unix("dir /B *.mo >models");
//else
//  unix("ls *.mo >models");
//end
if with_ocaml() then 
  models=['Capacitor.mo'; 'NPN.mo';'PNP.mo';   'Resistor.mo';  'VsourceAC.mo';
          'ConstantVoltage.mo';  'ExternFunction.mo'; 'OutPort.mo'; 'VsourceDC.mo';
          'Ground.mo'; 'OutPutPort.mo';   'VVsourceAC.mo'; 'CurrentSensor.mo';
          'Inductor.mo'; 'Pin.mo'; 'VariableResistor.mo'; 'Diode.mo'; 'InPutPort.mo';
	  'PotentialSensor.mo';'VoltageSensor.mo';'SineVoltage.mo';
	  'Switch.mo';'OpAmp.mo';'NMOS.mo';'PMOS.mo';'CVS.mo';'CCS.mo';'IdealTransformer.mo';'Gyrator.mo'];
  exec("../../../util/genmoc.sce");
end;
//------------------------------------
