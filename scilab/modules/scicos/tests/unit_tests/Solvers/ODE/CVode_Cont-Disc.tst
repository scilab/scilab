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

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/ODE/Cont-Disc.zcos"));

CVode_val_ref = [
    0.0526307    0.0281557  -0.0448704
    0.0532495    0.0310114  -0.0395186
    0.0515776    0.0292653  -0.0411181
    0.0541249    0.0359859  -0.0302313
    0.0557393    0.0408484  -0.0222704
    0.0553117    0.0416374  -0.0205659
    0.0532631    0.0391278  -0.0237235
    0.0527059    0.0395825  -0.0224774
    0.0484859    0.0326594  -0.0322248
    0.0478463    0.0328263  -0.0311995
    0.0480311    0.0346173  -0.0277562
    0.0482951    0.0365765  -0.0241936
    0.0499827    0.0413830  -0.0164582
    0.0482402    0.0393498  -0.0191464
    0.0449862    0.0341952  -0.0262754
    0.0456246    0.0367935  -0.0217585
    0.0442013    0.0352645  -0.0235108
    0.0418061    0.0317424  -0.0281739
    0.0423243    0.0340336  -0.0241589
    0.0416363    0.0338569  -0.0237435 ];
CVode_time_ref = (17.2:.1:29.9)';

for i=1:4

    scs_m.props.tol(6) = i; // Solver
    try scicos_simulate(scs_m); catch disp(lasterror()); end; // CVode

    assert_checkalmostequal(res.values($-19:$, :), CVode_val_ref, 1d-4);
    assert_checkalmostequal(res.time, CVode_time_ref);

end
