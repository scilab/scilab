// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test plot()

// Check each default property
plot();
f = gcf();
a1 = f.children(1);
a2 = f.children(2);
c1 = a1.children(1);
c2 = a2.children(1);
p1 = c1.children;
p2 = c2.children;

// Check figure
//assert_checkequal(f.figure_position, [200,200]);
//assert_checkequal(f.figure_size, [614,568]);
//assert_checkequal(f.axes_size, [610, 460]);
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
assert_checkequal(f.info_message, "");
handMadeColorMap = [0,0,0;
0,0,1;
0,1,0;
0,1,1;
1,0,0;
1,0,1;
1,1,0;
1,1,1;
0,0,0.5647058823529411686337;
0,0,0.6901960784313725394412;
0,0,0.8156862745098039102487;
0.5294117647058823594719,0.8078431372549019773288,1;
0,0.5647058823529411686337,0;
0,0.6901960784313725394412,0;
0,0.8156862745098039102487,0;
0,0.5647058823529411686337,0.5647058823529411686337;
0,0.6901960784313725394412,0.6901960784313725394412;
0,0.8156862745098039102487,0.8156862745098039102487;
0.5647058823529411686337,0,0;
0.6901960784313725394412,0,0;
0.8156862745098039102487,0,0;
0.5647058823529411686337,0,0.5647058823529411686337;
0.6901960784313725394412,0,0.6901960784313725394412;
0.8156862745098039102487,0,0.8156862745098039102487;
0.5019607843137254832300,0.1882352941176470562112,0;
0.6274509803921568540375,0.2509803921568627416150,0;
0.7529411764705882248450,0.3764705882352941124225,0;
1,0.5019607843137254832300,0.5019607843137254832300;
1,0.6274509803921568540375,0.6274509803921568540375;
1,0.7529411764705882248450,0.7529411764705882248450;
1,0.8784313725490195956525,0.8784313725490195956525;
1,0.8431372549019607864906,0;
0,0.5,0;
0,0.75,0.75;
0.75,0,0.75;
0.75,0.75,0;
0.25,0.25,0.25];
assert_checkequal(f.color_map, handMadeColorMap);


// Check Axes 1
assert_checkequal(a1.visible, "on");
assert_checkequal(a1.axes_visible, ["on" "on" "on"]);
assert_checkequal(a1.axes_reverse, ["off" "off" "off"]);
assert_checkequal(a1.grid, [-1 -1]);
assert_checkequal(a1.grid_position, "background");
assert_checkequal(a1.x_location, "bottom");
assert_checkequal(a1.y_location, "left");
assert_checkequal(a1.auto_ticks, ["on" "on" "on"]);
//assert_checkequal(a1.x_ticks.locations, [0:0.1:1]');
//assert_checkequal(a1.y_ticks.locations, [0:0.1:1]');
//assert_checkequal(a1.z_ticks.locations, []);
assert_checkequal(a1.box, "on");
assert_checkequal(a1.filled, "on");
assert_checkequal(a1.sub_ticks, [1, 4]);
assert_checkequal(a1.font_style, 6);
assert_checkequal(a1.font_size, 1);
assert_checkequal(a1.font_color, -1);
assert_checkequal(a1.fractional_font, "off");
assert_checkequal(a1.isoview, "off");
assert_checkequal(a1.cube_scaling, "off");
assert_checkequal(a1.view, "2d");
assert_checkequal(a1.rotation_angles, [0, 270]);
assert_checkequal(a1.log_flags, "nnn");
assert_checkequal(a1.tight_limits, ["off", "off", "off"]);
assert_checkequal(a1.data_bounds, [1, -1 ; 2, 1]);
assert_checkequal(a1.zoom_box, []);
assert_checkequal(a1.margins, [0.125, 0.125, 0.125, 0.125]);
assert_checkequal(a1.axes_bounds, [0, 0.5, 1, 0.5]);
assert_checkequal(a1.auto_clear, "off");
assert_checkequal(a1.auto_scale, "on");
assert_checkequal(a1.hidden_axis_color, 4);
assert_checkequal(a1.hiddencolor, 4);
assert_checkequal(a1.line_mode, "on");
assert_checkequal(a1.line_style, 1);
assert_checkequal(a1.thickness, 1);
assert_checkequal(a1.mark_mode, "off");
assert_checkequal(a1.mark_style, 0);
assert_checkequal(a1.mark_size_unit, "tabulated");
assert_checkequal(a1.mark_size, 0);
assert_checkequal(a1.mark_foreground, -1);
assert_checkequal(a1.mark_background, -2);
assert_checkequal(a1.foreground, -1);
assert_checkequal(a1.background, -2);
assert_checkequal(a1.arc_drawing_method, "lines");
assert_checkequal(a1.clip_state, "clipgrf");
assert_checkequal(a1.clip_box, []);
assert_checkequal(a1.user_data, []);

// Check Axes 2
assert_checkequal(a2.visible, "on");
assert_checkequal(a2.axes_visible, ["on" "on" "on"]);
assert_checkequal(a2.axes_reverse, ["off" "off" "off"]);
assert_checkequal(a2.grid, [-1 -1]);
assert_checkequal(a2.grid_position, "background");
assert_checkequal(a2.x_location, "bottom");
assert_checkequal(a2.y_location, "left");
assert_checkequal(a2.auto_ticks, ["on" "on" "on"]);
//assert_checkequal(a2.x_ticks.locations, [0:0.1:1]');
//assert_checkequal(a2.y_ticks.locations, [0:0.1:1]');
//assert_checkequal(a2.z_ticks.locations, []);
assert_checkequal(a2.box, "on");
assert_checkequal(a2.filled, "on");
assert_checkequal(a2.sub_ticks, [0, 4]);
assert_checkequal(a2.font_style, 6);
assert_checkequal(a2.font_size, 1);
assert_checkequal(a2.font_color, -1);
assert_checkequal(a2.fractional_font, "off");
assert_checkequal(a2.isoview, "off");
assert_checkequal(a2.cube_scaling, "off");
assert_checkequal(a2.view, "2d");
assert_checkequal(a2.rotation_angles, [0, 270]);
assert_checkequal(a2.log_flags, "nnn");
assert_checkequal(a2.tight_limits, ["off", "off", "off"]);
assert_checkequal(a2.data_bounds, [0, -1 ; 2 * %pi, 1]);
assert_checkequal(a2.zoom_box, []);
assert_checkequal(a2.margins, [0.125, 0.125, 0.125, 0.125]);
assert_checkequal(a2.axes_bounds, [0, 0, 1, 0.5]);
assert_checkequal(a2.auto_clear, "off");
assert_checkequal(a2.auto_scale, "on");
assert_checkequal(a2.hidden_axis_color, 4);
assert_checkequal(a2.hiddencolor, 4);
assert_checkequal(a2.line_mode, "on");
assert_checkequal(a2.line_style, 1);
assert_checkequal(a2.thickness, 1);
assert_checkequal(a2.mark_mode, "off");
assert_checkequal(a2.mark_style, 0);
assert_checkequal(a2.mark_size_unit, "tabulated");
assert_checkequal(a2.mark_size, 0);
assert_checkequal(a2.mark_foreground, -1);
assert_checkequal(a2.mark_background, -2);
assert_checkequal(a2.foreground, -1);
assert_checkequal(a2.background, -2);
assert_checkequal(a2.arc_drawing_method, "lines");
assert_checkequal(a2.clip_state, "clipgrf");
assert_checkequal(a2.clip_box, []);
assert_checkequal(a2.user_data, []);

// Compound 1
assert_checkequal(c1.parent.type, "Axes");
assert_checkequal(c1.children(:).type, emptystr(1,41) + "Polyline");
assert_checkequal(c1.visible, "on");
assert_checkequal(c1.user_data, []);

// Compount 2
assert_checkequal(c2.parent.type, "Axes");
assert_checkequal(c2.children(:).type, emptystr(1,3) + "Polyline");
assert_checkequal(c2.visible, "on");
assert_checkequal(c2.user_data, []);

// Check Polylines in Compound 1
assert_checkequal(p1(:).parent.type, emptystr(1,41) + "Compound");
assert_checkequal(p1(:).children, []);
assert_checkequal(p1(:).visible, emptystr(1,41) + "on");
// Don't know how to check those data's
//assert_checkequal(p1.data, [demoData, sin(3 * demoData)]);
assert_checkequal(p1(:).closed, emptystr(1,41) + "off");
assert_checkequal(p1(:).line_mode, emptystr(1,41) + "on");
assert_checkequal(p1(:).fill_mode, emptystr(1,41) + "off");
assert_checkequal(p1(:).line_style, ones(1,41));
assert_checkequal(p1(:).thickness, ones(1,41));
assert_checkequal(p1(:).arrow_size_factor, ones(1,41));
assert_checkequal(p1(:).polyline_style, ones(1,41));
fg_colors = [36 , 35 , 34 , 5 , 33 , 2 , 37 , 36 , 35 , 34 , 5 , 33 ,...
2 , 37 , 36 , 35 , 34 , 5 , 33 , 2 , 37 , 36 , 35 , 34 ,...
5 , 33 , 2 , 37 , 36 , 35 , 34 , 5 , 33 , 2 , 37 , 36 ,...
35 , 34 , 5 , 33 , 2];
assert_checkequal(p1(:).foreground, fg_colors);
assert_checkequal(p1(:).background, -2 * ones(1,41));
assert_checkequal(p1(:).interp_color_vector, []);
assert_checkequal(p1(:).interp_color_mode, emptystr(1,41) + "off");
assert_checkequal(p1(:).mark_mode, emptystr(1,41) + "off");
assert_checkequal(p1(:).mark_style, zeros(1,41));
assert_checkequal(p1(:).mark_size_unit, emptystr(1,41) + "point");
assert_checkequal(p1(:).mark_size, zeros(1,41));
assert_checkequal(p1(:).mark_foreground, -1 * ones(1,41));
assert_checkequal(p1(:).mark_background, -2 * ones(1,41));
assert_checkequal(p1(:).x_shift, []);
assert_checkequal(p1(:).y_shift, []);
assert_checkequal(p1(:).z_shift, []);
assert_checkequal(p1(:).bar_width, zeros(1,41));
assert_checkequal(p1(:).clip_state, emptystr(1,41) + "clipgrf");
assert_checkequal(p1(:).clip_box, []);
assert_checkequal(p1(:).user_data, []);

// Check Polylines in Compound 2
demoData = [0:%pi/20:2*%pi]';
assert_checkequal(p2(:).parent.type, emptystr(1,3) + "Compound");
assert_checkequal(p2(:).children, []);
assert_checkequal(p2(:).visible, emptystr(1,3) + "on");
assert_checkequal(p2(1).data, [demoData, abs(sin(demoData))]);
assert_checkequal(p2(2).data, [demoData, cos(demoData)]);
assert_checkequal(p2(3).data, [demoData, sin(demoData)]);
assert_checkequal(p2(:).closed, emptystr(1,3) + "off");
assert_checkequal(p2(:).line_mode, ["on" , "off" , "on"]);
assert_checkequal(p2(:).fill_mode, emptystr(1,3) + "off");
assert_checkequal(p2(:).line_style, [2 , 1 , 4]);
assert_checkequal(p2(:).thickness, ones(1,3));
assert_checkequal(p2(:).arrow_size_factor, ones(1,3));
assert_checkequal(p2(:).polyline_style, ones(1,3));
assert_checkequal(p2(:).foreground, [6 , 1 , 5]);
assert_checkequal(p2(:).background, -2 * ones(1,3));
assert_checkequal(p2(:).interp_color_vector, []);
assert_checkequal(p2(:).interp_color_mode, emptystr(1,3) + "off");
assert_checkequal(p2(:).mark_mode, emptystr(1,3) + "on");
assert_checkequal(p2(:).mark_style, [9 , 1 , 9]);
assert_checkequal(p2(:).mark_size_unit, emptystr(1,3) + "point");
assert_checkequal(p2(:).mark_size, 6 * ones(1,3));
assert_checkequal(p2(:).mark_foreground, [6 , 4 , 5]);
assert_checkequal(p2(:).mark_background, -2 * ones(1,3));
assert_checkequal(p2(:).x_shift, []);
assert_checkequal(p2(:).y_shift, []);
assert_checkequal(p2(:).z_shift, []);
assert_checkequal(p2(:).bar_width, zeros(1,3));
assert_checkequal(p2(:).clip_state, emptystr(1,3) + "clipgrf");
assert_checkequal(p2(:).clip_box, []);
assert_checkequal(p2(:).user_data, []);
