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
// Test of Xcos PID controller

path = fmigetPath();

assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expXcosPIDcontroller.zcos")));
xcos_simulate(scs_m, 4);
// compare the results of simulation
// Xcos superblock PIDresults.values(:, 1)
// Xcos PID block PIDresults.values(:, 2)
// FMU CS PIDresults.values(:, 3);
// FMU ME PIDresults.values(:, 4);
assert_checkalmostequal(PIDresults.values(:,1), PIDresults.values(:,3), 10e-6);
assert_checkalmostequal(PIDresults.values(:,2), PIDresults.values(:,3), 10e-6);
assert_checkalmostequal(PIDresults.values(:,1), PIDresults.values(:,4), 10e-4);
assert_checkalmostequal(PIDresults.values(:,2), PIDresults.values(:,4), 10e-4);
