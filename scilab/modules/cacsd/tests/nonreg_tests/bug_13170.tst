// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13170 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13170
//
// <-- Short Description -->
// plzr plotted wrong legends.

s = %s;
G1 = syslin("c", 1,   s^3+s^2+5*s+2);
G2 = syslin("c", s+3, s^3+s^2+5*s+2);

plzr(G1);
a = gce();
assert_checkequal(a.text, _("Poles"));

plzr(G2);
b = gce();
assert_checkequal(b.text, [_("Poles"); _("Zeros")]);
