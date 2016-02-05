// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13619 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13619
//
// <-- Short Description -->
// xstringl returned wrong coordinates for upper-left point.

scf();
plot([-1,1], [-1,1]);
r = xstringl(0, 0, "TEST");
assert_checktrue(r(2)<>0);