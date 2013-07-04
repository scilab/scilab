// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- TEST WITH GRAPHIC -->

// test scope values


assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cmatview.zcos"));
xcos_simulate(scs_m, 4);

function assert_cmatview()
    f=gcf();
    assert_checkequal(size(f.children), [1 1])

    a=f.children(1);
    assert_checkequal(size(a.children), [1 1])

    g=a.children(1);

    assert_checkequal(a.data_bounds, [0 0 ; 100 100]);

    assert_checkequal(a.x_label.text, "x");
    assert_checkequal(a.y_label.text, "y");

    assert_checkequal(g.children, []);
    assert_checkequal(g.visible, "on");

    assert_checkequal(size(g.data.x), [100 1]);
    assert_checkequal(size(g.data.y), [100 1]);
    assert_checkequal(size(g.data.z), [100 100]);

    assert_checkequal(g.clip_state, "clipgrf");
    assert_checkequal(g.clip_box, []);
endfunction
assert_cmatview()

// Simulate again to check multi-simulations cases
xcos_simulate(scs_m, 4);
assert_cmatview()

