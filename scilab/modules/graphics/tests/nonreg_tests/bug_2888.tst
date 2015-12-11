// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2888 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2888
//
// <-- Short Description -->
// Set font_size with a big value freeze Scilab

alphabet=["a" "b" "c" "d" "e" "f" "g" ..
"h" "i" "j" "k" "l" "m" "n" ..
"o" "p" "q" "r" "s" "t" "u" ..
"v" "w" "x" "y" "z"];

s = scf();
a = gca();
a.view = "3d";
xstring(-1,-1,alphabet);
e = gce();

// very large font size
e.font_size = 100;

// rotate
for i=1:360,
    a.rotation_angles(2) = i;
end

// use fractional fonts
e.fractional_font = "on";

// rotate again
for i=1:360,
    a.rotation_angles(2) = i;
end
