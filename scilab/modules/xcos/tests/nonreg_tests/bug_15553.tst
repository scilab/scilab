// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Paul Bignier
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15553 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15553
//
// <-- Short Description -->
// TOWS_c block failed when simulation time = 0
//

assert_checktrue(importXcosDiagram("SCI/modules/xcos/examples/sinks_pal/TOWS_c_Example.zcos"));
scs_m.props.tf = 0;
clear A;
xcos_simulate(scs_m, 4);
assert_checktrue(isdef("A"));
assert_checkequal(A, struct("values", [], "time", []));
clear A;
scicos_simulate(scs_m);
assert_checktrue(isdef("A"));
assert_checkequal(A, struct("values", [], "time", []));

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/tows_c.zcos"));
scs_m.props.tf = 0;
clear A intV;
xcos_simulate(scs_m, 4);
assert_checktrue(isdef("A"));
assert_checkequal(A, struct("values", [], "time", []));
assert_checktrue(isdef("intV"));
assert_checkequal(intV, struct("values", [], "time", []));
clear A intV;
scicos_simulate(scs_m);
assert_checktrue(isdef("A"));
assert_checkequal(A, struct("values", [], "time", []));
assert_checktrue(isdef("intV"));
assert_checkequal(intV, struct("values", [], "time", []));
