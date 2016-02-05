// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13438 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13438
//
// <-- Short Description -->
// drawaxis() did not return the handle of the current entity.

plot2d(1:10, 1:10);
eRef = drawaxis(x = 2:7, y = 4, dir = "u", tics = "v");
eNew = gce();
assert_checktrue(eRef == eNew);
eRef = drawaxis(x = 4, y = 2:7, dir = "r", tics = "v");
eNew = gce();
assert_checktrue(eRef == eNew);
eRef = drawaxis(x = 8, y = 2:9, dir = "r", tics = "v", val = "value: "+string(1:8));
eNew = gce();
assert_checktrue(eRef == eNew);
eRef = drawaxis(x = 3, y = 2:9, dir = "l", tics = "v", val = "value: "+string(1:8));
eNew = gce();
assert_checktrue(eRef == eNew);
