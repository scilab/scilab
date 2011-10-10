// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test xstring

xstring(0.5,0.5,["$\overbrace{Scilab}$" "n''est ";"pas" "$\underbrace{Matlab}$"]);

e = gce();

assert_checkequal(e.type, "Text")
assert_checkequal(e.parent.type, "Axes")
assert_checkequal(e.children, [])
assert_checkequal(e.text, ["$\overbrace{Scilab}$","n''est";"pas","$\underbrace{Matlab}$"])
assert_checkequal(e.alignment, "left")
assert_checkequal(e.data, [0.5 0.5])
assert_checkequal(e.box, "off")
assert_checkequal(e.line_mode, "on")
assert_checkequal(e.fill_mode, "off")
assert_checkequal(e.text_box, [0 0])
assert_checkequal(e.text_box_mode, "off")
assert_checkequal(e.font_foreground, -1)
assert_checkequal(e.foreground, -1)
assert_checkequal(e.background, -2)
assert_checkequal(e.font_style, 6)
assert_checkequal(e.font_size, 1)
assert_checkequal(e.fractional_font, "off")
assert_checkequal(e.auto_dimensionning, "on")
assert_checkequal(e.font_angle, 0)
assert_checkequal(e.clip_state, "off")
assert_checkequal(e.clip_box, [])
assert_checkequal(e.user_data, [])

