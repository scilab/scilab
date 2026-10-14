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

// Test of numerical results of Xcos PID controller. Comparison with FMU checker
// results.

path = fmigetPath();

assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expXcosPIDnumerical.zcos")));

xcosSolver = [ ...
    "***Lsodar***"
    "***CVode BDF Newton***"
    "***CVode BDF Functional***"
    "***CVode ADAMS Newton***"
    "***CVode ADAMS Functional***"
    "***Dormand-Prince***"
    "***Runge-Kutta***"];

// load comparison function
exec(fullfile(path, "tests", "unit_tests", "check_results.sce"), -1);

// read FMU checker results
outME = csvRead(fullfile(path, "tests", "unit_tests", "expXcosPIDMEnumericalFMUcheckOut.csv"), ";");
outCS = csvRead(fullfile(path, "tests", "unit_tests", "expXcosPIDCSnumericalFMUcheckOut.csv"), ";");

// delete headers
outME = outME(3:$,:);
outCS = outCS(2:$,:);

// FMUchecker time
assert_checkequal(outME(:,1), outCS(:,1));
outputs = struct();
outputs.time = outME(:,1);

// FMUchecker values
outputs.values = [outME(:,2), outCS(:,2)];

// compare Xcos superblock results with FMUchecker results
// PIDrefs == Xcos superblock results
// outputs == FMUchecker results for Xcos FMUs 
disp("//*********Xcos superblock and FMUchecker*********//");
outName = ["super_MEFMUchecker"; "super_CSFMUchecker"];
for i=1:7
    scs_m.props.tol(6) = i-1;
    xcos_simulate(scs_m, 4);
    check_results(PIDrefs, outputs, outName, xcosSolver(i));
end

// compare FMUchecker results with Xcos import FMU
disp("//*********FMUchecker and Xcos import FMU*********//");
outName = ["xcosME_MEFMUchecker"; "xcosCS_CSFMUchecker"];
outRefs = struct();
outRefs.time = outputs.time;
// ME FMUchecker
outRefs.values = outputs.values(:,1);
for i=1:7
    scs_m.props.tol(6) = i-1;
    xcos_simulate(scs_m, 4);
    check_results(outRefs, PIDresults, outName(1), xcosSolver(i));
end
// CS FMUchecker
outRefs.values = outputs.values(:,2);
for i=1:7
    scs_m.props.tol(6) = i-1;
    xcos_simulate(scs_m, 4);
    PIDresults.values = PIDresults.values(:,2);
    check_results(outRefs, PIDresults, outName(2), xcosSolver(i));
end

// compare Xcos superblock results with Xcos import FMU
disp("//*********Xcos superblock and Xcos import FMU*********//");
outName = ["xcosME_superXcos"; "xcosCS_superXcos"];
for i=1:7
    scs_m.props.tol(6) = i-1;
    xcos_simulate(scs_m, 4);
    check_results(PIDrefs, PIDresults, outName, xcosSolver(i));
end
