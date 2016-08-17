// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- XCOS TEST -->
//

ilib_verbose(0); //to remove ilib_* traces

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/DAE/Hydraulics.zcos"));

// Modify solver + run DDaskr + save results
scs_m.props.tol(6) = 102;       // Solver
scicos_simulate(scs_m);   // DDaskr
ddaskrval = res.values;         // Results

// Modify solver + run IDA + save results
scs_m.props.tol(6) = 100;      // Solver
scicos_simulate(scs_m);  // IDA
idaval = res.values;           // Results

// Compare results
compa = abs(ddaskrval-idaval);

// Extract mean, standard deviation, maximum
mea = mean(compa);
[maxi, indexMaxi] = max(compa);
stdeviation = stdev(compa);

// Verifying closeness of the results
assert_checktrue(maxi <= 2d-6);
assert_checktrue(mea <= 2d-6);
assert_checktrue(stdeviation <= 2d-6);
