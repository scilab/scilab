// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test gdf()

// Check each default property
f = gdf();

if or(f.figure_position <> [200,200]) then pause, end
if or(f.figure_size <> [620,590]) then pause, end
if or(f.axes_size <> [610,460]) then pause, end
if f.auto_resize <> "on" then pause, end
if or(f.viewport <> [0,0]) then pause, end
if f.figure_name <> gettext("Graphic window number %d") then pause, end
if f.figure_id <> 0 then pause, end
if f.pixel_drawing_mode <> "copy" then pause, end
if f.anti_aliasing <> "off" then pause, end
if f.immediate_drawing <> "on" then pause, end
if f.background <> -2 then pause, end
if f.visible <> "on" then pause, end
if f.rotation_style <> "unary" then pause, end
if f.event_handler <> "" then pause, end
if f.event_handler_enable <> "off" then pause, end
if f.user_data <> [] then pause, end
if f.tag <> "" then pause, end

assert_checkequal(f.resize, "on");
assert_checkequal(f.toolbar, "figure");
assert_checkequal(f.toolbar_visible, "on");
assert_checkequal(f.menubar, "figure");
assert_checkequal(f.menubar_visible, "on");
assert_checkequal(f.infobar_visible, "on");
assert_checkequal(f.dockable, "on");
assert_checkequal(f.layout, "none");
