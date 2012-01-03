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

// clipping => clip_box + clip_state
xset("clipping", 1,2,3,4);
out = xget("clipping");
assert_checkequal(out, [1,2,3,4]);

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
