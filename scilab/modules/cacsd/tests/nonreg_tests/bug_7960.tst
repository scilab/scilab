// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 7960 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7960
//
// <-- Short Description -->
// plzr can not produce a pole zero plot of many simple transfer functions

z=poly(0,'z');
Num=0.64-1.5054092*z+z*z;
Den=0.970225-1.9652966*z+z*z;
HZ=Num/Den;
zr_ref=roots(Num);
pl_ref=roots(Den);

plzr(HZ);
a=gca();
// Check poles
pl=a.children(3);
assert_checkequal(pl.type, "Polyline");
assert_checkalmostequal(pl.data(1), real(pl_ref(1)));
assert_checkalmostequal(pl.data(2), real(pl_ref(2)));
assert_checkalmostequal(pl.data(3), imag(pl_ref(1)));
assert_checkalmostequal(pl.data(4), imag(pl_ref(2)));

// Check zeros
zr=a.children(4);
assert_checkequal(zr.type, "Polyline");
assert_checkalmostequal(zr.data(1), real(zr_ref(1)));
assert_checkalmostequal(zr.data(2), real(zr_ref(2)));
assert_checkalmostequal(zr.data(3), imag(zr_ref(1)));
assert_checkalmostequal(zr.data(4), imag(zr_ref(2)));
close;
