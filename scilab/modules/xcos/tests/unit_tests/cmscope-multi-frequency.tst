// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- TEST WITH GRAPHIC -->

// <-- NO CHECK REF -->


assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cmscope-multi-frequency.zcos"));
xcos_simulate(scs_m, 4);

function assert_checkfigure(num)
    f = gcf();
    a = f.children(num);
    p = a.children(1);

    assert_checkequal(p.mark_mode, "on");
    assert_checkequal(p.mark_style, 1);
endfunction

assert_checkfigure(1);
assert_checkfigure(2);

