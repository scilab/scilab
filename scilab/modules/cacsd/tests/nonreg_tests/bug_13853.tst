// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13853 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13853
//
// <-- Short Description -->
// plzr() thinks discrete-time systems with a numeric time step are continuous-time systems

s=poly(0,'s');
n=[1+s, 2+3*s+4*s^2, 5; 0, 1-s, s];
d=[1+3*s, 5-s^3, s+1;1+s, 1+s+s^2, 3*s-1];
h=syslin(0.1, n./d); 
plzr(h);
a=gca();
assert_checkequal(a.children(2).type, "Arc");
assert_checkequal(a.children(2).data, [-1 1 2 2 0 360]);
