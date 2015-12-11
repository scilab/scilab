// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13082 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13082
//
// <-- Short Description -->
// Datatip mark style was not inherited from polyline.

clf();
plot(1:1000);
e = gce();
p = e.children;
datatipSetStyle(p,2);

d1 = datatipCreate(p,400);
assert_checkequal(d1.mark_style, 7);

datatipSetStyle(p,1)
assert_checkequal(d1.mark_style, 11);
d2 = datatipCreate(p,600)
assert_checkequal(d2.mark_style, 11);

datatipSetStyle(p,2)
assert_checkequal(d1.mark_style, 7);
assert_checkequal(d2.mark_style, 7);
d3 = datatipCreate(p,800)
assert_checkequal(d3.mark_style, 7);

