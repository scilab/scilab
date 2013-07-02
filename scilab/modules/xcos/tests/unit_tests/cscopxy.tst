// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- TEST WITH GRAPHIC -->

// test scope values 


assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cscopxy.zcos"));
xcos_simulate(scs_m, 4);

function assert_checkcscopxy()
    f=gcf();
    assert_checkequal(size(f.children), [1 1])

    a=f.children(1);
    assert_checkequal(size(a.children), [4 1])

    p1=a.children(1);

    assert_checkequal(f.figure_id, 20007);
    assert_checkequal(a.data_bounds, [-20 -30 ; 20 30]);

    assert_checkequal(a.x_label.text, "x");
    assert_checkequal(a.y_label.text, "y");

    // all polylines has the same configuration, we just need to check the first one.
    assert_checkequal(p1.polyline_style, 1);
    assert_checkequal(p1.line_mode, "off");

    assert_checkequal(p1.mark_mode, "on");
    assert_checkequal(p1.mark_style, 3);
    assert_checkequal(p1.mark_size, 1);
endfunction
assert_checkcscopxy()

// Simulate again to check multi-simulations cases
xcos_simulate(scs_m, 4);
assert_checkcscopxy();


