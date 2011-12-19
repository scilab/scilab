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
xet("colormap", c_map);
out = xget("colormap");
assert_checkequal(out, c_map);

// clipping => clip_box + clip_state
xset("clipping", 1,2,3,4);
out = xget("clipping");
assert_checkequal(out, [1,2,3,4]);