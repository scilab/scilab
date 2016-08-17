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

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/ODE/Kalman.zcos"));

// Modify solver + run IDA
scs_m.props.tol(6) = 100;       // Solver
ier = execstr("xcos_simulate(scs_m, 4);", "errcatch"); // Run simulation (LSodar will actually take over)
assert_checkequal(ier, 0);
ier = execstr("scicos_simulate(scs_m);", "errcatch");
assert_checkequal(ier, 0);

// Modify solver + run DDaskr Newton
scs_m.props.tol(6) = 101;       // Solver
ier = execstr("xcos_simulate(scs_m, 4);", "errcatch"); // Run simulation (LSodar will actually take over)
assert_checkequal(ier, 0);
ier = execstr("scicos_simulate(scs_m);", "errcatch");
assert_checkequal(ier, 0);

// Modify solver + run DDaskr GMRes
scs_m.props.tol(6) = 102;       // Solver
ier = execstr("xcos_simulate(scs_m, 4);", "errcatch"); // Run simulation (LSodar will actually take over)
assert_checkequal(ier, 0);
ier = execstr("scicos_simulate(scs_m);", "errcatch");
assert_checkequal(ier, 0);
