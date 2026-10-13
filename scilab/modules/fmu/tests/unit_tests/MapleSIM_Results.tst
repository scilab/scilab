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

// Test the FMUs from MapleSIM
loadXcosLibs();
path = fmigetPath();

// Coupled Clutches 
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "MapleSIM_CoupledClutches_Windows.zcos")));
getResult = ["CC_r(0)","CC_r(1)","CC_r(2)","CC_r(3)","CC_r(4)","CC_r(5)","CC_r(6)","CC_r(7)","CC_r(8)","CC_r(9)","CC_r(10)"]; // output names at the context
scicos_simulate(scs_m,list());
// read the references results from file
refResults = csvRead(fullfile(path, "tests", "unit_tests", "MapleSIM_Results_CoupledClutches.csv"), ';');
// time
assert_checkequal(refResults(:,1), sciResults.time);
// values 
assert_checkalmostequal(refResults(:,2:$), sciResults.values, 1e-8, []);
