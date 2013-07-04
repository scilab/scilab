// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/DoPri_test.zcos"));

// Modify solver + run DDaskr + save results
scs_m.props.tol(6) = 101;       // Solver
scs_m.props.tol(1) = 1.0e-10;   // abstol
scs_m.props.tol(2) = 1.0e-10;   // reltol
ier = execstr('scicos_simulate(scs_m, ''nw'');', 'errcatch'); // Run simulation (LSodar will actually take over DDaskr)
assert_checkequal(ier, 0);
