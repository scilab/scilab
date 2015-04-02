// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13437 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13437
//
// <-- Short Description -->
// axis drawn by drawxis use different font

plot();
a = gca();
axis = drawaxis(y=-1, x=1:.1:2, dir="u");
assert_checkequal(a.font_size, axis.labels_font_size);
assert_checkequal(a.font_style, axis.labels_font_style);
assert_checkequal(a.font_color, axis.labels_font_color);