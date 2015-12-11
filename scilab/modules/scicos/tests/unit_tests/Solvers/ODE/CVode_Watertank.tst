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

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/ODE/Watertank.zcos"));

CVode_val_ref = [
    7.8198758    9.6795397
    7.8460626    9.6628927
    7.8715566    9.6488678
    7.8963773    9.6375731
    7.9205414    9.6287258
    7.9440674    9.6219139
    7.9669711    9.6174424
    7.9892697    9.6150375
    8.0109781    9.6146991
    8.0321113    9.6160474
    8.052689     9.619105
    8.0727243    9.6238066
    8.0922301    9.6298068
    8.1112209    9.6370943
    8.1297115    9.6456041
    8.1477135    9.6551443
    8.1652409    9.6656234
    8.1823051    9.6768902
    8.198919     9.6888469
    8.2150942    9.7013865 ];
CVode_time_ref = (0.1:.1:10)';

for i=1:4

    scs_m.props.tol(6) = i; // Solver
    try scicos_simulate(scs_m); catch disp(lasterror()); end; // CVode

    assert_checkalmostequal(res.values($-19:$, :), CVode_val_ref, 1d-3);
    assert_checkalmostequal(res.time, CVode_time_ref);

end
