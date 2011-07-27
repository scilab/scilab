// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test fac3d object

// Check each default property

[X, Y] = meshgrid(-1:.1:1, -1:.1:1);
Z = X.^2 - Y.^2;
mesh(X,Y,Z);
e = gce();

assert_checkequal(e.type, "Fac3d")
assert_checkequal(e.parent.type, "Axes")
assert_checkequal(e.children, [])
assert_checkequal(e.visible, "on")
assert_checkequal(e.surface_mode, "on")
assert_checkequal(e.foreground, -1)
assert_checkequal(e.thickness, 1)
assert_checkequal(e.mark_mode, "off")
assert_checkequal(e.mark_style, 0)
assert_checkequal(e.mark_size_unit, "point")
assert_checkequal(e.mark_size, 0)
assert_checkequal(e.mark_foreground, -1)
assert_checkequal(e.mark_background, -2)
//assert_checkequal(e.data.x)
//assert_checkequal(e.data.y)
//assert_checkequal(e.data.z)
//assert_checkequal(e.data.color)
assert_checkequal(e.color_mode, 8)
assert_checkequal(e.color_flag, 0)
assert_checkequal(e.cdata_mapping, "scaled")
assert_checkequal(e.hiddencolor, 0)
assert_checkequal(e.clip_state, "clipgrf")
assert_checkequal(e.clip_box, [])
assert_checkequal(e.user_data, [])
