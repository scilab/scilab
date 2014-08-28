// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13359 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13359
//
// <-- Short Description -->
// Nyquist datatip were not displaying negative frequencies properly
//
// <-- TEST WITH GRAPHIC -->

s = poly(0, 's');
h = syslin('c', 1 / (s + 1));
nyquist(h);

ax = gca();
pl = ax.children(1).children(2);
d1 = datatipCreate(pl, 200);
txt_datatip = d1.text;
assert_checkequal(strindex(txt_datatip(2), "-"), 1);
