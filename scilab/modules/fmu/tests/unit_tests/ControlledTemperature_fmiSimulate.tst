//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->

// Simulation of the fmu Temperature sensor

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SimulationX_ControlledTemperature.fmu"));
// simulation with default values for all 'inputs/outputs' and 'start' variables
assert_checktrue(execstr("outDefault=fmiSimulate(fmu)","errcatch")==0);
// simulation with user time and debug logging
assert_checktrue(execstr("outDefault=fmiSimulate(fmu,[0,0.1,100],%t)","errcatch")==0);
// "onOffController.reference" = reference of the temperature; "temperatureSensor.T" = temperature of the sensor
assert_checkequal(outDefault.Real.result(1).name, "onOffController.reference");
assert_checktrue(and(outDefault.Real.result(1).values(:) == 50)); // reference value of the temperature 
assert_checkequal(outDefault.Real.result(2).name, "temperatureSensor.T");
// variable values of the temperature
assert_checkequal(outDefault.Real.result(2).values(1), 20); 

//plot2d(outDefault.Real.result(2).time(1:$),outDefault.Real.result(2).values(1:$-1));

// user Values for the reference temperature 
fmu.setValues.input_1_values(1,:) = 20:40;
fmu.setValues.input_1_values(1,21:31) = 40:-1:30;
fmu.setValues.input_1_values(1,32:50) = 31:3:86;
fmu.setValues.input_1_values(2,:) = 20:69;
// simulation with user values 
assert_checktrue(execstr("outDefault=fmiSimulate(fmu,[0,0.1,100],%t)","errcatch")==0);

//plot2d(outDefault.Real.result(2).time(1:$),outDefault.Real.result(2).values(1:$-1));


