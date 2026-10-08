// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17600 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17600
//
// <-- Short Description -->
// Files written by Scilab 5.5.2 might number the single output port of a
// SuperBlock with ordering="2". Loading such a file left an invalid port in
// the block and Scilab crashed on any access to the block model.
// issue_17600.zcos contains a SUPER_f (CONST_m 42 -> OUT_f) connected to a
// TOWS_c ; its single output port is saved with ordering="2".

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/issue_17600.zcos"));

for i = 1:size(scs_m.objs)
    o = scs_m.objs(i);
    if typeof(o) == "Block" & o.gui == "SUPER_f" then
        sb = o;
        isb = i;
    end
end

assert_checkequal(sb.model.out, -1);
assert_checkequal(sb.model.out2, -2);
assert_checkequal(sb.model.outtyp, 1);
assert_checkequal(size(sb.graphics.pout, "*"), 1);
lnk = scs_m.objs(sb.graphics.pout);
assert_checkequal(lnk.from, [isb 1 0]);

scicos_simulate(scs_m, list(), "nw");
assert_checkequal(unique(A.values), 42);
