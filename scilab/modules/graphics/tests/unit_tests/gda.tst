// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test gda()

// Check each default property
a = gda();

assert_checkequal(a.visible, "on");
assert_checkequal(a.axes_visible, ["off" "off" "off"]);
assert_checkequal(a.axes_reverse, ["off" "off" "off"]);
assert_checkequal(a.grid, [-1 -1]);
assert_checkequal(a.grid_position, "background");
assert_checkequal(a.x_location, "bottom");
assert_checkequal(a.y_location, "left");
assert_checkequal(a.auto_ticks, ["on" "on" "on"]);
assert_checkalmostequal(a.x_ticks.locations, [0:0.1:1]');
assert_checkalmostequal(a.y_ticks.locations, [0:0.1:1]');
assert_checkequal(a.z_ticks.locations, []);
assert_checkequal(a.box, "off");
assert_checkequal(a.filled, "on");
assert_checkequal(a.sub_ticks, [1, 1]);
assert_checkequal(a.font_style, 6);
assert_checkequal(a.font_size, 1);
assert_checkequal(a.font_color, -1);
assert_checkequal(a.fractional_font, "off");
assert_checkequal(a.isoview, "off");
assert_checkequal(a.cube_scaling, "off");
assert_checkequal(a.view, "2d");
assert_checkequal(a.rotation_angles, [0, 270]);
assert_checkequal(a.log_flags, "nnn");
assert_checkequal(a.tight_limits, ["off", "off", "off"]);
assert_checkequal(a.data_bounds, [0, 0; 1, 1]);
assert_checkequal(a.zoom_box, []);
assert_checkequal(a.margins, [0.125, 0.125, 0.125, 0.125]);
assert_checkequal(a.axes_bounds, [0, 0, 1, 1]);
assert_checkequal(a.auto_clear, "off");
assert_checkequal(a.auto_scale, "on");
assert_checkequal(a.hidden_axis_color, 4);
assert_checkequal(a.hiddencolor, 4);
assert_checkequal(a.line_mode, "on");
assert_checkequal(a.line_style, 1);
assert_checkequal(a.thickness, 1);
assert_checkequal(a.mark_mode, "off");
assert_checkequal(a.mark_style, 0);
assert_checkequal(a.mark_size_unit, "tabulated");
assert_checkequal(a.mark_size, 0);
assert_checkequal(a.mark_foreground, -1);
assert_checkequal(a.mark_background, -2);
assert_checkequal(a.foreground, -1);
assert_checkequal(a.background, -2);
assert_checkequal(a.arc_drawing_method, "lines");
assert_checkequal(a.clip_state, "off");
assert_checkequal(a.clip_box, []);
assert_checkequal(a.user_data, []);

delete(gcf());
set(gda(), "user_data", "test");
clf();
a=gca();
assert_checkequal(a.user_data, "test");