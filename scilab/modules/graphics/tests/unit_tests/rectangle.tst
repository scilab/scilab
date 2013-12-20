// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test rectangle object

// Check each default property
xrect(-1,1,2,2)
e = gce();

// Default values
assert_checkequal(e.parent.type, "Axes");
assert_checkequal(e.children, []);
assert_checkequal(e.mark_mode, "off");
assert_checkequal(e.mark_style, 0);
assert_checkequal(e.mark_size_unit, "tabulated");
assert_checkequal(e.mark_size, 0);
assert_checkequal(e.mark_foreground, -1);
assert_checkequal(e.mark_background, -2);
assert_checkequal(e.line_mode, "on");
assert_checkequal(e.fill_mode, "off");
assert_checkequal(e.line_style, 1);
assert_checkequal(e.thickness, 1);
assert_checkequal(e.foreground, -1);
assert_checkequal(e.background, -2);
assert_checkequal(e.data, [-1 1 2 2]);
assert_checkequal(e.visible, "on");
assert_checkequal(e.clip_state, "off");
assert_checkequal(e.clip_box, []);
assert_checkequal(e.user_data, []);

// mark_mode
set(e, "mark_mode", "on");
assert_checkequal(e.mark_mode, "on");
set(e, "mark_mode", "off"); // Back to default

// mark_style
set(e, "mark_style", 1);
assert_checkequal(e.mark_style, 1);
set(e, "mark_style", 0); // Back to default

// mark_size_unit
set(e, "mark_size_unit", "point");
assert_checkequal(e.mark_size_unit, "point");
set(e, "mark_size_unit", "tabulated"); // Back to default

// mark_size
set(e, "mark_size", 1);
assert_checkequal(e.mark_size, 1);
set(e, "mark_size", 0); // Back to default

// mark_foreground
set(e, "mark_foreground", 3);
assert_checkequal(e.mark_foreground, 3);
set(e, "mark_foreground", -1); // Back to default

// mark_background;
set(e, "mark_background", 3);
assert_checkequal(e.mark_background, 3);
set(e, "mark_background", -2); // Back to default

// line_mode
set(e, "line_mode", "off");
assert_checkequal(e.line_mode, "off");
set(e, "line_mode", "on"); // Back to default

// fill_mode
set(e, "fill_mode", "on");
assert_checkequal(e.fill_mode, "on");
set(e, "fill_mode", "off"); // Back to default

// line_style
set(e, "line_style", 2);
assert_checkequal(e.line_style, 2);
set(e, "line_style", 1); // Back to default

// thickness
set(e, "thickness", 3);
assert_checkequal(e.thickness, 3);
set(e, "thickness", 1); // Back to default

// foreground
set(e, "foreground", 3);
assert_checkequal(e.foreground, 3);
set(e, "foreground", -1);  // Back to default

// background
set(e, "background", 3);
assert_checkequal(e.background, 3);
set(e, "background", -2);  // Back to default

// data
set(e, "data", [1 2 3 4]);
assert_checkequal(e.data, [1 2 3 4]);
set(e, "data", [-1 1 2 2]);

// visible
set(e, "visible", "off");
assert_checkequal(e.visible, "off");
set(e, "visible", "on"); // Back to default

// clip_state & clip_box
set(e, "clip_state", "on");
assert_checkequal(e.clip_state, "clipgrf");
assert_checkequal(e.clip_box, []);
set(e, "clip_box", [1 2 3 4]);
set(e, "clip_state", "on");
assert_checkequal(e.clip_state, "on");
assert_checkequal(e.clip_box, [1 2 3 4]);
set(e, "clip_state", "off"); // Back to default
// e.clip_box can not be reset to []

// user_data
set(e, "user_data", list());
assert_checkequal(e.user_data, list());
set(e, "user_data", []); // Back to default