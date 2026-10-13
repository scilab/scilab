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

// FMU code generation
// Test of xcos data types in fmu outputs
path = fmigetPath();
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expXcosDataTypes.zcos")));
xcos_simulate(scs_m, 4);
// compare the results of simulation
// CoSimulation
assert_checkalmostequal(sciA.values, fmuCS_A.values);
assert_checkalmostequal(sciB.values, fmuCS_B.values);
// Model Exchange
assert_checkalmostequal(sciA.values, fmuME_A.values);
assert_checkalmostequal(sciB.values, fmuME_B.values);
