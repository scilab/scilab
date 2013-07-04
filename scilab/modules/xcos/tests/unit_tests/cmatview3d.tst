// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- TEST WITH GRAPHIC -->

// test scope values


assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cmatview3d.zcos"));
xcos_simulate(scs_m, 4);

function assert_checkcmatview3d()
    f=gcf();
    assert_checkequal(size(f.children), [1 1])

    a=f.children(1);
    assert_checkequal(size(a.children), [1 1])

    g=a.children(1);

    assert_checkequal(a.data_bounds, [0 0 -100; 81 81 100]);

    assert_checkequal(a.x_label.text, "x");
    assert_checkequal(a.y_label.text, "y");
    assert_checkequal(a.z_label.text, "z");

    assert_checkequal(g.children, []);
    assert_checkequal(g.visible, "on");
    assert_checkequal(g.surface_mode, "on");
    assert_checkequal(g.foreground, -1);
    assert_checkequal(g.thickness, 1);
    assert_checkequal(g.mark_mode, "off");
    assert_checkequal(g.mark_style, 0);
    // is "point" as default, should not be noticeable
    // assert_checkequal(g.mark_size_unit, "tabulated");
    assert_checkequal(g.mark_size, 0);
    // should be 0 according to the documentation (-1 is invalid)
    assert_checkequal(g.mark_foreground, 0);
    // should be 0 according to the documentation (-2 is invalid)
    assert_checkequal(g.mark_background, 0);

    assert_checkequal(size(g.data.x), [1 81]);
    assert_checkequal(size(g.data.y), [1 81]);
    assert_checkequal(size(g.data.z), [81 81]);

    assert_checkequal(g.color_mode, 2);
    assert_checkequal(g.color_flag, 1);
    assert_checkequal(g.hiddencolor, 4);
    assert_checkequal(g.clip_state, "clipgrf");
    assert_checkequal(g.clip_box, []);
endfunction
assert_checkcmatview3d()

// Simulate again to check multi-simulations cases
xcos_simulate(scs_m, 4);
assert_checkcmatview3d()

