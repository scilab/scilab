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

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/ODE/Kalman.zcos"));

CVode_val_ref = [ -0.0124518   0.0057263   0.0202561
                  -0.0056589   0.0003643   0.0040543
                   0.0139675   0.0010424  -0.0068178
                   0.0342362   0.0025550  -0.0150659
                   0.0349585   0.0022871  -0.0139099
                   0.0457677   0.0095449  -0.0057510
                   0.0438374   0.0179447   0.0110369
                   0.0399226   0.0159906   0.0084871
                   0.0466923   0.0087662  -0.0088076
                   0.0431542   0.0060647  -0.0101178
                   0.0586618   0.0186971   0.0051877
                   0.0629366   0.0288542   0.0211484
                   0.0579398   0.0109636  -0.0105544
                   0.0624743   0.0102438  -0.0139593
                   0.0654326   0.0065228  -0.0207958
                   0.0531896  -0.0026032  -0.0283963
                   0.0353228  -0.0132950  -0.0343691
                   0.0381893  -0.0053057  -0.0183788
                   0.0563168   0.0083184  -0.0028685
                   0.0363338   0.0053192   0.0038699 ];
CVode_time_ref = (0:.03:14.97)';

for i=1:4

    scs_m.props.tol(6) = i; // Solver
    try scicos_simulate(scs_m); catch disp(lasterror()); end; // CVode

    assert_checkalmostequal(res.values($-19:$, :), CVode_val_ref, [], 1d-7);
    assert_checkalmostequal(res.time, CVode_time_ref);

end
