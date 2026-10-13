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

// Test the FMUs from Dymola
loadXcosLibs();
path = fmigetPath();

// Dymola_Modelica_Electrical_Digital_Examples_DFFREG
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "Dymola_Modelica_Electrical_Digital_Examples_DFFREG.zcos")));
getResult = [..
    "clock.x[1]","clock.x[2]","clock.t[1]","clock.t[2]","clock.t[3]","clock.t[4]","clock.t[5]",...
     "clock.t[6]","clock.t[7]","data_0.x[1]","data_0.x[2]","data_0.t[1]",...
     "data_0.t[2]", "reset.x[1]","reset.x[2]","reset.t[1]", "reset.t[2]",...
     "reset.t[3]","data_1.t[1]","data_1.t[2]","dFFREG.tHL",...
     "dFFREG.tLH","dFFREG.delay.tHL","dFFREG.delay.tLH","dFFREG.delay.inertialDelaySensitive[1].tLH",...
     "dFFREG.delay.inertialDelaySensitive[1].tHL","dFFREG.delay.inertialDelaySensitive[1].delayTime",...
     "dFFREG.delay.inertialDelaySensitive[1].t_next","dFFREG.delay.inertialDelaySensitive[2].tLH",...
     "dFFREG.delay.inertialDelaySensitive[2].tHL","dFFREG.delay.inertialDelaySensitive[2].delayTime",...
     "dFFREG.delay.inertialDelaySensitive[2].t_next"];// output names at the context
scicos_simulate(scs_m,list());
// read the references results from file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "Dymola_Results_Electrical_Digital_Examples_DFFREG.csv"), ";");
// time
assert_checkequal(sciResults.time, refResults(:,1));
// values 
assert_checkalmostequal(sciResults.values,refResults(:,2:$),[],1e-4);

