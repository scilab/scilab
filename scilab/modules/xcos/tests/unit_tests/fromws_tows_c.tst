// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
//
// Test Diagram with fromws after end behavior
// Input random values
//

assert_checktrue(importXcosDiagram(SCI+"/modules/xcos/tests/unit_tests/fromws_tows_c.zcos"));

// V_in size feat simulation final time and asked values
V_in = struct("time", (0:0.5:5000)', "values", rand(10001, 1));
scicos_simulate(scs_m, list());
assert_checkequal(V_in.time,   V_out_zero.time);
assert_checkequal(V_in.values, V_out_zero.values);
assert_checkequal(V_in.time,   V_out_hold.time);
assert_checkequal(V_in.values, V_out_hold.values);
assert_checkequal(V_in.time,   V_out_repeat.time);
assert_checkequal(V_in.values, V_out_repeat.values);

// V_in is shorter than simulation time
V_in = struct("time", (0:0.5:1000)', "values", rand(2001, 1));
scicos_simulate(scs_m, list());
assert_checkequal(V_in.time,   V_out_zero.time(1:2001));
assert_checkequal(V_in.values, V_out_zero.values(1:2001));
assert_checkequal(V_out_zero.values(2002:10001), zeros(10001 - 2002 + 1, 1));
assert_checkequal(V_in.time,   V_out_hold.time(1:2001));
assert_checkequal(V_in.values, V_out_hold.values(1:2001));
assert_checkequal(V_out_hold.values(2002:10001), ones(10001 - 2002 + 1, 1).*V_in.values($));
assert_checkequal(V_in.time,   V_out_repeat.time(1:2001));
assert_checkequal(V_in.values, V_out_repeat.values(1:2001));
//assert_checkequal(V_in.values, V_out_repeat.values(2002:4002));
