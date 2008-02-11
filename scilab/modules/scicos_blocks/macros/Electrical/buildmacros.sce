//  Scicos
//
//  Copyright (C) INRIA - Allan Cornet <allan.cornet@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('Electricallib','SCI/modules/scicos_blocks/macros/Electrical',%f,%t);
//------------------------------------
if MSDOS then
  unix("dir /B *.mo >models");
else
  unix("ls *.mo >models");
end
if with_modelica_compiler() then 
  models=['Capacitor.mo'; 'NPN.mo';'PNP.mo';   'Resistor.mo';  'VsourceAC.mo';
          'ConstantVoltage.mo';  'ExternFunction.mo'; 'OutPort.mo'; 'VsourceDC.mo';
          'Ground.mo'; 'OutPutPort.mo';   'VVsourceAC.mo'; 'CurrentSensor.mo';
          'Inductor.mo'; 'Pin.mo'; 'VariableResistor.mo'; 'Diode.mo'; 'InPutPort.mo';
	  'PotentialSensor.mo';'VoltageSensor.mo';'SineVoltage.mo';
	  'Switch.mo';'OpAmp.mo';'NMOS.mo';'PMOS.mo';'CVS.mo';'CCS.mo';'IdealTransformer.mo';'Gyrator.mo'];
  exec("../../src/scripts/genmoc.sce");
end;
//------------------------------------
