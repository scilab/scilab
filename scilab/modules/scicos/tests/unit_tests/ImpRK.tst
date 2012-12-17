// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/ImpRK.tst");
//  or test_run('scicos', 'ImpRK', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/RK_test.zcos"));

for i=2:4  // 'max step size' = 10^-i, precision

 // Start by updating the clock block period (sampling)
 scs_m.objs(7).model.rpar(1) = 5*(10^-i);
 scs_m.objs(8).model.rpar(1) = 5*(10^-i);

 // Modify solver and 'max step size' + run ImpRK + save results
 scs_m.props.tol(7) = 5*(10^-i); scs_m.props.tol(6) = 7;         // 'max step size' + solver
 scs_m.props.tol(2) = 1.0e-12;                                   // reltol
 try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end  // ImpRK
 rkval = res.values;   // Results
 time = res.time;      // Time

 // Modify solver and reltol + run CVode + save results
 scs_m.props.tol(6) = 1; scs_m.props.tol(2) = 1.0e-15;
 try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end
 cvval = res.values;

 // Compare results
 compa = abs(rkval-cvval);

 // Extract mean, standard deviation, maximum
 mea = mean(compa);
 [maxi, indexMaxi] = max(compa);
 stdeviation = st_deviation(compa);

 // Verifying closeness of the results
 assert_checktrue(maxi <= 10^-i);
 assert_checktrue(mea <= 10^-i);
 assert_checktrue(stdeviation <= 10^-i);

end
