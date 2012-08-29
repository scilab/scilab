// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 9032 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9032
//
// <-- Short Description -->
// The rectangular contour of 'Text' object doesn't fit for huge font size.

xstring(.2, .1, ["Hello"]);
e=gce();
e.box="on"; e.line_mode="on"; e.fill_mode="on";
e.background = 15;
e.font_size = 16;

// The text must fill the rectangle.
