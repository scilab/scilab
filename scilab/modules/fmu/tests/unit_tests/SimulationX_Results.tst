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
//

// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->

// Test the FMUs from SimulationX
loadXcosLibs();
path = fmigetPath();

// Controlled temperarture 
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "SimulationX_ControlledTemperature.zcos")));
scicos_simulate(scs_m,list());
// read the references results from file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "SimulationX_Results_ControlledTemperature.csv"), ';');
// time
assert_checkequal(refResults(:,1), sciResults.time);
// values 
assert_checkalmostequal(refResults(:,2), sciResults.values, 1e-8, []);

// Coupled Clutches
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "SimulationX_CoupledClutches.zcos")));
scicos_simulate(scs_m,list());
// read the references results from file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "SimulationX_Results_CoupledClutches.csv"), ';');
// time
assert_checkequal(refResults(:,1), sciResults.time);
// values 
assert_checkalmostequal(refResults(:,2:$), sciResults.values, 1e-8, []);
// Engine1b
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "SimulationX_Engine1b.zcos")));
scicos_simulate(scs_m,list());
// read the references results from file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "SimulationX_Results_Engine1b.csv"), ';');
// time
assert_checkequal(refResults(:,1), sciResults.time);
// values 
assert_checkalmostequal(refResults(:,2:$), sciResults.values, 1e-8, []);
