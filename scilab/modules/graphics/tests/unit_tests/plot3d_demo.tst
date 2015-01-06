// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test plot3d()

// Check each default property
plot3d();
f = gcf();
a = gca();
e = a.children(1);

// Check figure
//assert_checkequal(f.figure_position, [200,200]);
//assert_checkequal(f.figure_size, [614,568]);
//assert_checkequal(f.axes_size, [610,460]);
assert_checkequal(f.auto_resize, "on");
assert_checkequal(f.viewport, [0,0]);
assert_checkequal(f.figure_name, gettext("Graphic window number %d"));
assert_checkequal(f.figure_id, 0);
assert_checkequal(f.pixel_drawing_mode, "copy");
assert_checkequal(f.anti_aliasing, "off");
assert_checkequal(f.immediate_drawing, "on");
assert_checkequal(f.background, -2);
assert_checkequal(f.visible, "on");
assert_checkequal(f.rotation_style, "unary");
assert_checkequal(f.event_handler, "");
assert_checkequal(f.event_handler_enable, "off");
assert_checkequal(f.user_data, []);
assert_checkequal(f.tag, "");

// Check Axes
assert_checkequal(a.visible, "on");
assert_checkequal(a.axes_visible, ["on" "on" "on"]);
assert_checkequal(a.axes_reverse, ["off" "off" "off"]);
assert_checkequal(a.grid, [-1 -1, -1]);
assert_checkequal(a.grid_position, "background");
assert_checkequal(a.x_location, "bottom");
assert_checkequal(a.y_location, "left");
assert_checkequal(a.auto_ticks, ["on" "on" "on"]);
//assert_checkequal(a.x_ticks.locations, [0:0.1:1]');
//assert_checkequal(a.y_ticks.locations, [0:0.1:1]');
//assert_checkequal(a.z_ticks.locations, []);
assert_checkequal(a.box, "on");
assert_checkequal(a.filled, "on");
assert_checkequal(a.sub_ticks, [1, 1, 0]);
assert_checkequal(a.font_style, 6);
assert_checkequal(a.font_size, 1);
assert_checkequal(a.font_color, -1);
assert_checkequal(a.fractional_font, "off");
assert_checkequal(a.isoview, "off");
assert_checkequal(a.cube_scaling, "off");
assert_checkequal(a.view, "3d");
assert_checkequal(a.rotation_angles, [70, 70]);
assert_checkequal(a.log_flags, "nnn");
assert_checkequal(a.tight_limits, ["off", "off", "off"]);
assert_checkequal(a.data_bounds, [-%pi, -%pi, -1; %pi, %pi, 1]);
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

// Check Plot3d Object
assert_checkequal(e.parent.type, "Axes");
assert_checkequal(e.children, []);
assert_checkequal(e.visible, "on");
assert_checkequal(e.surface_mode, "on");
assert_checkequal(e.foreground, -1);
assert_checkequal(e.thickness, 1);
assert_checkequal(e.mark_mode, "off");
assert_checkequal(e.mark_style, 0);
assert_checkequal(e.mark_size_unit, "tabulated");
assert_checkequal(e.mark_size, 0);
assert_checkequal(e.mark_foreground, -1);
assert_checkequal(e.mark_background, -2);
demoData = %pi * [-1:0.05:1]';
assert_checkequal(e.data.x, demoData);
assert_checkequal(e.data.y, demoData);
assert_checkequal(e.data.z, sin(demoData)*cos(demoData)');
assert_checkequal(e.color_mode, 2);
assert_checkequal(e.color_flag, 1);
assert_checkequal(e.hiddencolor, 4);
assert_checkequal(e.clip_state, "clipgrf");
assert_checkequal(e.clip_box, []);
assert_checkequal(e.user_data, []);
