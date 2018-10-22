// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15827 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15827
//
// <-- Short Description -->
// After bode(..,"rad")
//  - datatips were still in "Hz" instead of "rad/s"
//  - In french, X axes were labelled "Fréquence" instead of "Pulsation"

s = %s;
G = (10*(s+3))/(s*(s+2)*(s^2+s+2));
sys = syslin('c', G);
f_min = .0001; f_max = 16; // Frequencies in Hz

setlanguage("fr")
clf
bode(sys, f_min, f_max, "rad");
c = gcf().children(1).children.children;
d = datatipCreate(c, 100);
assert_checktrue(grep(d.text(1), "rad/s")~=[]);
assert_checktrue(grep(gcf().children(1).x_label.text, "Pulsation")~=[]);

c = gcf().children(2).children.children;
d = datatipCreate(c, 100);
assert_checktrue(grep(d.text(1), "rad/s")~=[]);
assert_checktrue(grep(gcf().children(2).x_label.text, "Pulsation")~=[]);
