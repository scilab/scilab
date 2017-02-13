// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// test xset and xget

// font
out = xget("font");
assert_checkequal(out, [6 1]);

// font size
out = xget("font size");
assert_checkequal(out, 1);

// fpf
out = xget("fpf");
assert_checkequal(out, "");
xset("fpf", "%s.%s");
out = xget("fpf");
assert_checkequal(out, "%s.%s");

// auto clear
out = xget("auto clear");
assert_checkequal(out, "off");

// dashes
out = xget("dashes");
assert_checkequal(out, 1);
xset("dashes", 2);
out = xget("dashes");
assert_checkequal(out, 2);

// hidden3d
out = xget("hidden3d");
assert_checkequal(out, 4);

// window figure
f = gcf();
out = xget("window");
assert_checkequal(out, f.figure_id);
out = xget("figure");
assert_checkequal(out, f.figure_id);

// wdim wpdim
out = xget("wdim");
assert_checkequal(out, f.figure_size);
out = xget("wpdim");
assert_checkequal(out, f.figure_size);

// wpos
out = xget("wpos");
assert_checkequal(out, f.figure_position);

// viewport
out = xget("viewport");
assert_checkequal(out, f.viewport);

// background
out = xget("background");
assert_checkequal(out, -2);

// color foreground pattern
out = xget("color");
assert_checkequal(out, -1);
out = xget("foreground");
assert_checkequal(out, -1);
out = xget("pattern");
assert_checkequal(out, -1);

// lastpattern
out = xget("lastpattern");
assert_checkequal(out, 32);

// line mode
out = xget("line mode");
assert_checkequal(out, 1);

// white
out = xget("white");
assert_checkequal(out, 34);

// wresize
out = xget("wresize");
assert_checkequal(out, 1);

// clipgrf
out = xget("clipgrf");
assert_checkequal(out, 0);

// clipoff
out = xget("clipoff");
assert_checkequal(out, 1);

// XSET
ierr = execstr("xset(""fpf"");","errcatch");
assert_checkequal(ierr, 77);
