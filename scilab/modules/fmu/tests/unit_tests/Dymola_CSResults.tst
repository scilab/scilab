//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2014 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->

// FMUs for CoSimulation from Dymola
// only outputs are checked, internal variables must not be used in connections
loadXcosLibs();
path = fmigetPath();

// check FMU ControlledTemperature
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "Dymola_CSControlledTemperature.zcos")));
scicos_simulate(scs_m,list());
// Xcos reference results from csv file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "Dymola_CSResults_ControlledTemperature.csv"), ';');
// outputs
assert_checkequal(fmuCS_Outputs.time, refResults(:,1));
assert_checkalmostequal(fmuCS_Outputs.values, refResults(:,2:4), 1e-8, []);

// check FMU BooleanNetwork1
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "Dymola_CSBooleanNetwork1.zcos")));
scicos_simulate(scs_m,list());
// Xcos reference results from csv file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "Dymola_CSResults_BooleanNetwork1.csv"), ';');
// outputs
assert_checkequal(fmuCS_Outputs.time, refResults(:,1));
assert_checkalmostequal(fmuCS_Outputs.values, refResults(:,2:10), 1e-8, []);
