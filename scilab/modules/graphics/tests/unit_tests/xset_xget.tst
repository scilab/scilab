// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test xset and xget


// colormap
c_map = graycolormap(512);
xset("colormap", c_map);
out = xget("colormap");
assert_checkequal(out, c_map);

// mark size
xset("mark size", 456);
out = xget("mark size");
assert_checkequal(out, 456);

// mark
xset("mark", 4, 5);
out = xget("mark");
assert_checkequal(out, [4 5]);

// line style
xset("line style", 8);
out = xget("line style");
assert_checkequal(out, 8);

// font
out = xget("font");
assert_checkequal(out, [6 1]);
xset("font", 8, 2);
out = xget("font");
assert_checkequal(out, [8 2]);

// font size
out = xget("font size");
assert_checkequal(out, 2);

// fpf
out = xget("fpf");
assert_checkequal(out, "");

// auto clear
//out = xget("auto clear");
//assert_checkequal(out, "off");

// dashes
out = xget("dashes");
assert_checkequal(out, 8);

// hidden3d
out = xget("hidden3d");
assert_checkequal(out, 4);



// window figure
f = gcf();

out = xget("window");
assert_checkequal(out, f.figure_id);
out = xget("figure");
assert_checkequal(out, f.figure_id);

// thickness
out = xget("thickness");
assert_checkequal(out, 1);

// wdim wpdim
out = xget("wdim");
assert_checkequal(out, f.position([3, 4]));
out = xget("wpdim");
assert_checkequal(out, f.position([3, 4]));

// wpos
out = xget("wpos");
assert_checkequal(out, f.position([1, 2]));

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
assert_checkequal(out, 512);

// line mode
out = xget("line mode");
assert_checkequal(out, 1);

// pixmap
out = xget("pixmap");
assert_checkequal(out, 0);

// white
out = xget("white");
assert_checkequal(out, 514);

// wresize
out = xget("wresize");
assert_checkequal(out, 1);

// clipgrf
out = xget("clipgrf");
assert_checkequal(out, 0);

// clipoff
out = xget("clipoff");
assert_checkequal(out, 1);

// clipping => clip_box + clip_state
xset("clipping", 1,2,3,4);
out = xget("clipping");
assert_checkequal(out, [1,2,3,4]);