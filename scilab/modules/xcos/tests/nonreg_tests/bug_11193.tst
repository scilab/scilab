// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Alexandre HERISSE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 11193 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11193
//
// <-- Short Description -->
// Lincos did not honor context values.

loadXcosLibs();

// First test with B = 3.0;
B = 3.0;
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_11193.zcos"));

// looking for the Superblock to linearize
for i=1:length(scs_m.objs)
    if typeof(scs_m.objs(i))=="Block" & scs_m.objs(i).gui=="SUPER_f" then
        scs_m = scs_m.objs(i).model.rpar;
        break;
    end
end

sys = lincos(scs_m);
assert_checkequal(sys.A, []);
assert_checkequal(sys.B, []);
assert_checkequal(sys.C, []);
assert_checkequal(sys.D, 3.0);

// Second test with B = 7.0
B = 7.0;
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_11193.zcos"));

// looking for the Superblock to linearize
for i=1:length(scs_m.objs)
    if typeof(scs_m.objs(i))=="Block" & scs_m.objs(i).gui=="SUPER_f" then
        scs_m = scs_m.objs(i).model.rpar;
        break;
    end
end

sys = lincos(scs_m);
assert_checkequal(sys.A, []);
assert_checkequal(sys.B, []);
assert_checkequal(sys.C, []);
assert_checkequal(sys.D, 7.0);


