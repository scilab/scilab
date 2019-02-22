// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15965 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15965
//
// <-- Short Description -->
// plot(x,y) did not support y as encoded integers

x = (1:20)/20;
y = uint16(100*rand(1,20));
plot(x, y);
curve = gce().children.data;
assert_checkequal(curve, [x' double(y')]);
