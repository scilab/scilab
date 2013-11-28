// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12485 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12485
//
// <-- Short Description -->
// xchange returned bad values with log scale

clf();
plot2d(1:2);
x1=0;
y1=0;
[x2,y2]=xchange(x1,y1,"i2f");
[x3,y3]=xchange(x2,y2,"f2i");
assert_checkequal([x1 y1], [x3, y3]);
a=gca();
a.log_flags="lln";
[x2,y2]=xchange(x1,y1,"i2f");
[x3,y3]=xchange(x2,y2,"f2i");
assert_checkequal([x1 y1], [x3, y3]);