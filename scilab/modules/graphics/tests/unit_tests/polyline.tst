// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test polyline object

// Check each default property
x=sin(2*%pi*(0:4)/5);
y=cos(2*%pi*(0:4)/5);
xpoly(x,y);

e = gce();

assert_checkequal(e.parent.type, "Axes")
assert_checkequal(e.children, [])
assert_checkequal(e.visible, "on")
assert_checkequal(e.data, [x; y]')
assert_checkequal(e.closed, "off")
assert_checkequal(e.line_mode, "on")
assert_checkequal(e.fill_mode, "off")
assert_checkequal(e.line_style, 1)
assert_checkequal(e.thickness, 1)
assert_checkequal(e.arrow_size_factor, 1)
assert_checkequal(e.polyline_style, 1)
assert_checkequal(e.foreground, -1)
assert_checkequal(e.background, -2)
assert_checkequal(e.interp_color_vector, [])
assert_checkequal(e.interp_color_mode, "off")
assert_checkequal(e.mark_mode, "off")
assert_checkequal(e.mark_style, 0)
assert_checkequal(e.mark_size_unit, "tabulated")
assert_checkequal(e.mark_size, 0)
assert_checkequal(e.mark_foreground, -1)
assert_checkequal(e.mark_background, -2)
assert_checkequal(e.x_shift, [])
assert_checkequal(e.y_shift, [])
assert_checkequal(e.z_shift, [])
assert_checkequal(e.bar_width, 0)
assert_checkequal(e.clip_state, "off")
assert_checkequal(e.clip_box, [])
assert_checkequal(e.user_data, [])
