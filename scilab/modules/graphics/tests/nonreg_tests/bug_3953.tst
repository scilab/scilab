// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3953 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3953
//
// <-- Short Description -->
// delete() always modify the current object handle.
// 

x = 1:10;

// create two polylines
plot(x,x);
p1 = gce();
plot(x,x+1);
p2 = gce();

// delete the first one, the second one should still be current
delete(p1);
if (p2 <> gce()) then pause; end
