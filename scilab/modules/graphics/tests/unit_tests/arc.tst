// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test arc object

// Check each default property
xarc(-1,1,2,2,0,90*64);
e = gce();

assert_checkequal(e.parent.type, "Axes")
assert_checkequal(e.children, [])
assert_checkequal(e.thickness, 1)
assert_checkequal(e.line_style, 1)
assert_checkequal(e.line_mode, "on")
assert_checkequal(e.fill_mode, "off")
assert_checkequal(e.foreground, -1)
assert_checkequal(e.background, -2)
assert_checkequal(e.data, [-1 1 2 2 0 90])
assert_checkequal(e.visible, "on")
assert_checkequal(e.arc_drawing_method, "lines")
assert_checkequal(e.clip_state, "off")
assert_checkequal(e.clip_box, [])
assert_checkequal(e.user_data, [])
