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

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/ODE/Controller.zcos"));

CVode_val_ref = [
  -0.3044468   -0.0529063   -0.1556339
  -0.3152667   -0.0434950   -0.1412728
  -0.3178104   -0.0331244   -0.1275205
  -0.3071042   -0.0127201   -0.0885981
  -0.3140531   -0.0293549   -0.1124763
  -0.3227156   -0.0419506   -0.1226843
  -0.3353218   -0.0411101   -0.1285499
  -0.3202700   -0.0083677   -0.0802106
  -0.2992298    0.0202297   -0.0450890
  -0.2463811    0.0683297    0.0322714
  -0.1970797    0.0886041    0.0663326
  -0.1349323    0.1199161    0.1213726
  -0.0965228    0.1049137    0.0971713
  -0.0428198    0.1142629    0.1258705
   0.0334498    0.1465127    0.1855820
   0.0940592    0.1375793    0.1859050
   0.1280733    0.1031344    0.1476760
   0.1468555    0.0715547    0.1117624
   0.1678585    0.0609110    0.1017769
   0.1779129    0.0480921    0.0894899 ];
CVode_time_ref = (0:.1:29.9)';

for i=1:4

    scs_m.props.tol(6) = i; // Solver
    try scicos_simulate(scs_m); catch disp(lasterror()); end; // CVode

    assert_checkalmostequal(res.values($-19:$, :), CVode_val_ref, [], 1d-4);
    assert_checkalmostequal(res.time, CVode_time_ref);

end
